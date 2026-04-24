---
name: jira-workflow-guidelines
description: Atlassian Jira (Cloud) 워크플로우 가이드라인 — Epic/Task 생성, 상태 전이, 브랜치 생성 컨벤션 검증, 시안 사전 승인 정책. 모든 write/update/delete 작업은 시안 제시 후 사용자 명시 OK 대기. 프로젝트별 커스터마이징(PROJECT_KEY / ASSIGNEE / COMPONENT / customfield) 가이드 포함. Jira CLI/MCP/REST 4-tier fallback 설계.
triggers:
  - jira
  - Jira
  - 지라
  - 에픽
  - epic
  - task
  - sprint
  - 스프린트
  - "story point"
  - 스토리포인트
  - "jira workflow"
  - "이슈 생성"
  - "epic 생성"
---

# Jira Workflow Guidelines

Atlassian Jira Cloud 기반 이슈 관리 워크플로우 표준. 실 프로젝트(Here:O) 패턴 기반으로 일반화.

> **출처:** `S14P31A206/.claude/skills/jira/SKILL.md` (2026-04-24 역전파)
> **Agent 연계:** `jira-workflow-expert` 에이전트가 이 가이드라인의 정책을 실행 — CLI 계약 기반 4-tier fallback

---

## 🔧 Pipeline(공통) vs Config(프로젝트별) 분리

**핵심 원칙:** 파이프라인 로직은 모든 프로젝트에서 동일. 다른 것은 **설정값**뿐.

| 구분 | 내용 | 프로젝트마다 다름? | 관리 방식 |
|---|---|---|---|
| **Pipeline (공통)** | Epic→Task 생성 → 컨벤션 검증 → 시안 제시 → 승인 대기 → API 호출 → 브랜치 생성 → 상태 전이 → 완료 보고 | ❌ 동일 (skill 본문 정의) | 이 SKILL.md |
| **Config (프로젝트별)** | PROJECT_KEY, JIRA_HOST, ASSIGNEE_ACCOUNT_ID, COMPONENT, custom_field IDs, domain, base_branch, valid_types, transitions map | ✅ **항상 다름** | `.claude/skills/jira/config.yaml` 또는 `lib.py` 상수 |
| **Data (팀별)** | ai_tasks.yaml (Epic/Task 목록), epic_keys.json / task_keys.json (생성 키 캐시) | ✅ 팀이 작성·갱신 | `.claude/skills/jira/` 내부 |
| **Secrets (개인)** | JIRA_USER_EMAIL, JIRA_ACCESS_TOKEN | ✅ 개인별 | `.env` (gitignore 필수) |

**시사점:** 에이전트는 **Config/Data/Secrets가 프로젝트에 존재하는지 감지**해야 하며, 없으면 **Bootstrap 와이자드**로 자동 생성 안내해야 한다 (§Bootstrap 참조).

---

## 🚀 Bootstrap — 첫 실행 초기 세팅 (Agent 필수 동작)

**트리거 조건:** 아래 중 하나라도 부재:
- `.claude/skills/jira/config.yaml` **또는** `.claude/skills/jira/lib.py` (둘 다 없음)
- `.env`에 `JIRA_USER_EMAIL` / `JIRA_ACCESS_TOKEN` 누락
- `ai_tasks.yaml` 부재 (Epic/Task 관리용 — 선택)

**에이전트 동작:** 위 감지 시 사용자에게 요청 처리 보류 + Bootstrap 와이자드 실행 제안.

### Step 1 — 환경변수 (Secrets) 검증

```bash
# 에이전트 점검
test -n "$JIRA_USER_EMAIL" && test -n "$JIRA_ACCESS_TOKEN" || echo "missing"
```

부재 시:
```
❌ JIRA_USER_EMAIL / JIRA_ACCESS_TOKEN 누락.
.env 파일에 아래 추가 후 다시 시도하세요:
  JIRA_USER_EMAIL=you@example.com
  JIRA_ACCESS_TOKEN=<https://id.atlassian.com/manage-profile/security/api-tokens 발급>
```

### Step 2 — Jira 인스턴스 식별

사용자 질문:
```
Q1. Jira Cloud URL? (예: https://mycompany.atlassian.net)
Q2. PROJECT_KEY? (예: PROJ, MYAPP, S14P31A206)
```

검증:
```bash
curl -s -u "$JIRA_USER_EMAIL:$JIRA_ACCESS_TOKEN" \
  "$JIRA_HOST/rest/api/3/project/$PROJECT_KEY" | jq '.key, .name'
```

### Step 3 — 본인 Account ID 자동 조회

```bash
curl -s -u "$JIRA_USER_EMAIL:$JIRA_ACCESS_TOKEN" \
  "$JIRA_HOST/rest/api/3/myself" | jq '.accountId, .displayName'
```
→ `ASSIGNEE_ACCOUNT_ID` 자동 획득.

### Step 4 — Custom Field ID 자동 매핑

```bash
curl -s -u "$JIRA_USER_EMAIL:$JIRA_ACCESS_TOKEN" \
  "$JIRA_HOST/rest/api/3/field" | \
  jq '.[] | select(.name | test("Epic Name|Epic Link|Sprint|Story Points")) | {name, id}'
```

출력 예시를 사용자에게 보여주고 확인:
```
발견된 custom field:
  - Epic Name  → customfield_10011
  - Epic Link  → customfield_10014
  - Sprint     → customfield_10020
  - Story Points → customfield_10031
매핑 이대로 저장할까요? (Y/N)
```

### Step 5 — Component 선택

```bash
curl -s -u "$JIRA_USER_EMAIL:$JIRA_ACCESS_TOKEN" \
  "$JIRA_HOST/rest/api/3/project/$PROJECT_KEY/components" | \
  jq '.[] | {id, name}'
```

사용자에게 목록 제시 → 선택 → `COMPONENT_ID` / `COMPONENT_NAME` 확정.

### Step 6 — 프로젝트 컨벤션 확인

사용자 질문 (각 항목 기본값 제시 후 Y/수정):
```
Q3. domain (브랜치·라벨 prefix)? [default: backend]
Q4. base_branch? [default: main]
Q5. valid_types 커스터마이징? [default: feat/fix/refactor/perf/docs/style/test/chore/env/plan/design]
Q6. transitions id (todo / in_progress / done)?
    → curl {JIRA_HOST}/rest/api/3/issue/<sample-key>/transitions 로 조회 추천
```

### Step 7 — `config.yaml` 생성 + 시험 호출

에이전트가 `.claude/skills/jira/config.yaml` 작성 + 즉시 시험:
```bash
python .claude/skills/jira/lib.py list --jql "project = $PROJECT_KEY"
# 또는 jira-cli/MCP 사용
```

성공 시 Bootstrap 완료, 원래 요청 이어서 처리.

---

## 핵심 정책

### 1. 시안 사전 승인 정책 (Draft-First Policy)

**모든 write/update/delete API 호출 전에 사용자에게 시안 제시 + 명시적 OK 대기.**

| 작업 | 시안 필요? | 이유 |
|---|---|---|
| Epic 생성 | ✅ 필수 | 1회성 + 프로젝트 구조 영향 |
| Task 생성 | ✅ 필수 | 컨벤션 위반 시 정정 불가 |
| 상태 전이 | ✅ 필수 | 스프린트 보고에 영향 |
| 브랜치 생성 | ✅ 필수 | 로컬이라도 재작업 비용 있음 |
| `status` 조회 | ❌ 즉시 실행 | read-only |
| `list` 조회 | ❌ 즉시 실행 | read-only |
| Epic 진척률 | ❌ 즉시 실행 | read-only |

**시안 형식:** payload 테이블 + 영향 범위 + 컨벤션 매핑 + "OK?" 질문

**예외:** 사용자가 "한 번에 진행" 또는 "일괄 승인" 명시 시 개별 승인 생략 가능

### 2. 컨벤션 검증 (Convention-First Policy)

시안 제시 **전에** 컨벤션 위반을 자동 검증하고, 위반 시 시안 제시를 **중단**하여 yaml/input 수정을 요청한다.

---

## 컨벤션 체크리스트

프로젝트마다 구체값은 다르지만 **검증 항목 7개는 공통**이다.

| # | 항목 | 규칙 (일반) | 검증 시점 |
|---|---|---|---|
| 1 | **Epic summary** | `[기능명] 큰 단위 목표` 형식 | epic-create |
| 2 | **Task summary** | `type/구체적 작업 (동사형)` | task-create |
| 3 | **Type 허용 set** | `plan / design / env / feat / perf / fix / refactor / docs / style / test / chore` (프로젝트별 확장 가능) | task-create |
| 4 | **Component** | 프로젝트별 고정값 (예: "AI", "Frontend") | epic-create, task-create |
| 5 | **Story Points** | 피보나치 `1 / 2 / 3 / 5 / 8` — 8 지양, 시간 X 복잡도/노력/위험 종합 | task-create |
| 6 | **Assignee** | 본인 `accountId` (프로젝트별 고정) | 모든 create |
| 7 | **Sprint** | active sprint 자동 매핑 (`current` / `next` / `+1` / `+2`) | task-create |
| 8 | **Branch name** | `{type}/{domain}/{short-key}` — 소문자 / 하이픈 / 한글 X / Jira key 포함 | branch |
| 9 | **Labels** | `{domain} / epic-{N} / task-{NN} / type-{X}` | task-create |

### 컨벤션 위반 시 동작

```
❌ 시안 제시 금지 → 위반 항목 리스트로 보고 → yaml/input 수정 안내 → 재시도 요청
```

---

## 8개 명령 시그니처 (Agent 계약)

### Read-only (시안 생략, 즉시 실행)

| 명령 | 입력 | 출력 (JSON) |
|---|---|---|
| `status <key>` | issue key | `{key, summary, status, assignee, labels, components}` |
| `list [--jql]` | JQL (옵션) | `{jql, count, issues: [...]}` 기본: `assignee = currentUser() AND status != Done` |
| `epic-status` | — | `{epics: {eid: {done, total, in_progress}}}` |

### Write (시안 승인 필수)

| 명령 | 입력 | 시안 제시 항목 |
|---|---|---|
| `epics-init` | ai_tasks.yaml epics | 각 Epic의 summary / Component / Epic Name / labels / assignee (N건 테이블) |
| `task <task-id>` | task_id (예: 1-1, 2-3) | summary / Epic Link / Component / SP / Sprint / Labels / 예상 브랜치명 |
| `branch <jira-key>` | issue key | git 명령어 시퀀스 + 브랜치명 + 컨벤션 검증 결과 |
| `start <jira-key>` | issue key | "진행 중 전이" 확인 문구 |
| `done <jira-key>` | issue key | "완료 전이" 확인 문구 |

---

## 시안 템플릿

### Task 생성 시안

```
Task <id> 생성 시안:
─────────────────────────
summary    : <yaml의 summary>
Epic Link  : <epic_key> (<epic_name>)
Component  : <COMPONENT>
SP         : <sp> (피보나치 준수)
Sprint     : <sprint_id> (<resolved spec>)
Labels     : <domain>, epic-N, task-NN, type-X
Assignee   : 본인
Branch (다음 단계): <type>/<domain>/<short>-<PROJECT_KEY>-NNN
─────────────────────────
컨벤션 검증: ✅ 모두 통과 (또는 ⚠️ 위반 N건: <리스트>)
─────────────────────────
실행하시겠습니까? (OK / NO / 수정 요청)
```

### Epic 일괄 생성 시안

```
Epic N개 일괄 생성 시안:

| # | summary | Epic Name | Component |
|---|---|---|---|
| 1 | [기능명] ... | <NAME>-1 | <COMPONENT> |
...

영향: temp/jira_epic_keys.json 새로 생성, Jira에 N건 추가
재실행 가능 여부: 이미 키 있으면 --force 필요

실행하시겠습니까? (OK / NO)
```

### 브랜치 생성 시안

```
브랜치 생성 시안:

명령어 시퀀스:
  $ git checkout <base_branch>        # 프로젝트 기본 브랜치
  $ git pull origin <base_branch>
  $ git checkout -b <branch>

브랜치명: <type>/<domain>/<short>-<PROJECT_KEY>-NNN
컨벤션: ✅ 소문자 / 하이픈 / 한글 X / Jira key 포함

영향: 로컬에 새 브랜치 생성, 원격 영향 X (push 별도)

실행하시겠습니까? (OK / NO)
```

---

## 프로젝트별 커스터마이징 가이드

이 skill을 다른 프로젝트에 적용할 때 프로젝트별로 다르게 설정해야 하는 항목들:

### 필수 커스터마이징 (환경변수 또는 config)

| 항목 | Placeholder | 예시 값 (Here:O) |
|---|---|---|
| `PROJECT_KEY` | `{PROJECT_KEY}` | `S14P31A206` |
| `JIRA_HOST` | `{JIRA_HOST}` | `https://ssafy.atlassian.net` |
| `ASSIGNEE_ACCOUNT_ID` | `{ASSIGNEE_ACCOUNT_ID}` | `712020:811e3f0a-...` |
| `COMPONENT_ID` | `{COMPONENT_ID}` | `16416` |
| `COMPONENT_NAME` | `{COMPONENT_NAME}` | `AI` |
| `DOMAIN_PREFIX` (브랜치·라벨) | `{DOMAIN}` | `ai-module`, `ai` |
| `BASE_BRANCH` | `{BASE_BRANCH}` | `dev`, `main`, `develop` |

### Custom Field ID (Jira Cloud 인스턴스별)

| 필드 | 공통 패턴 | 확인 방법 |
|---|---|---|
| Epic Name | `customfield_10011` (자주 사용) | `GET /rest/api/3/field` 또는 인스턴스 관리자 확인 |
| Epic Link | `customfield_10014` | 동일 |
| Sprint | `customfield_10020` | 동일 |
| Story Points | `customfield_10031` | 동일 |

⚠️ **인스턴스마다 custom field ID가 다를 수 있다**. 새 프로젝트 적용 시 `GET /rest/api/3/field` 로 확인 필수.

### 커스터마이징 배치 방법 (2가지 패턴)

**Pattern A: `lib.py` 기반 (실 프로젝트 패턴)**

프로젝트 루트에 `.claude/skills/jira/lib.py` 배치 + CLI 계약 준수 → `jira-workflow-expert` 에이전트가 자동 위임.

**필요 파일:**
```
.claude/skills/jira/
├── SKILL.md        (프로젝트 고유 설정 포함)
├── lib.py          (API 로직 + argparse CLI)
├── ai_tasks.yaml   (Epic/Task 정의)
└── README.md       (팀용 가이드)
```

**lib.py CLI 필수 subcommand:**
- `task-create --task-id X [--sprint S]` → `{key, url, summary, sp, branch_suggestion}`
- `epic-create [--force]` → `{epics: {eid: {key, url}}}`
- `transition --key K --to {todo|in_progress|done}` → `{ok, key, new_status}`
- `status --key K` → `{key, summary, status, assignee, ...}`
- `list [--jql J]` → `{jql, count, issues: [...]}`

**Pattern B: Config YAML 기반 (경량 패턴)**

`lib.py` 없이 `.claude/skills/jira/config.yaml`만으로 운영 → `jira-workflow-expert` 에이전트가 jira-cli/acli/MCP/REST로 직접 호출.

**config.yaml 템플릿 (Bootstrap Step 7에서 생성):**

```yaml
# .claude/skills/jira/config.yaml
# Generated by jira-workflow-expert bootstrap on <date>
# 파이프라인은 skill 본문이 정의, 이 파일은 프로젝트별 값만 보관.

# ─── Step 2: 인스턴스 식별 ───
project_key: MYPROJ              # Jira 프로젝트 키
jira_host: https://mycompany.atlassian.net

# ─── Step 3: 본인 Account ID (myself API로 자동 획득) ───
assignee_account_id: "712020:xxx-xxx-xxx"

# ─── Step 4: Custom Field (field API로 자동 매핑) ───
custom_fields:
  epic_name: customfield_10011   # "Epic Name"
  epic_link: customfield_10014   # "Epic Link"
  sprint: customfield_10020
  story_points: customfield_10031

# ─── Step 5: Component (project/{key}/components API) ───
component:
  id: "10000"
  name: Backend

# ─── Step 6: 프로젝트 컨벤션 ───
domain: backend                  # 브랜치·라벨 prefix
base_branch: main                # 브랜치 생성 시 베이스 (main/dev/develop)

valid_types:                     # Task summary prefix 허용 set
  - feat
  - fix
  - refactor
  - perf
  - docs
  - style
  - test
  - chore
  - env
  - plan
  - design

valid_sp: [1, 2, 3, 5, 8]        # 피보나치 (8 지양 권장)

labels_pattern:                  # Task 라벨 자동 생성 규칙
  - "{domain}"
  - "epic-{epic_id}"
  - "task-{task_id}"
  - "type-{type}"

transitions:                     # GET /issue/<key>/transitions 로 조회
  todo: "11"
  in_progress: "21"
  done: "31"

branch_pattern: "{type}/{domain}/{short}-{project_key}-{issue_number}"
#   예: feat/backend/login-MYPROJ-209
```

---

## 환경변수 / 시크릿

| 변수 | 용도 | 누락 시 |
|---|---|---|
| `JIRA_USER_EMAIL` | Basic Auth 사용자 식별 | 실행 중단 + 설정 안내 |
| `JIRA_ACCESS_TOKEN` | API 토큰 (https://id.atlassian.com/manage-profile/security/api-tokens) | 실행 중단 + 설정 안내 |
| `CLAUDE_PROJECT_DIR` | 프로젝트 루트 (선택) | `cwd()` fallback |

**보안 규칙:**
- `echo $JIRA_ACCESS_TOKEN` 등 평문 노출 금지
- 토큰을 skill/agent 소스 코드에 하드코딩 금지 — `.env` 또는 환경변수만 사용
- Error 로그에 토큰이 포함되지 않는지 확인

---

## 에러·실패 처리

| 상황 | 처리 |
|---|---|
| 컨벤션 위반 | 시안 제시 단계에서 위반 항목 표시 + 실행 중단. yaml/input 수정 후 재시도 안내 |
| API 401 | 토큰 만료. `.env`의 `JIRA_ACCESS_TOKEN` 갱신 안내 |
| API 403 | 프로젝트 권한 부족. 관리자에게 role 확인 안내 |
| API 4xx (기타) | 응답 body 첫 500자 표시 + payload 재검토 안내 |
| Epic 미생성 상태에서 task | "`epics-init` 먼저 실행" 안내 |
| 이미 생성된 task 재호출 | 기존 key 표시 + `--force` 플래그 안내 |
| sprint 조회 실패 | agile 보드 미설정 가능성 — Sprint 필드 생략하고 수동 할당 안내 |

---

## 사용 예시

### Task on-demand 생성 흐름

```
사용자: "/jira task 1-1"
Claude:
  1. yaml 로드 + 컨벤션 검증 PASS
  2. 시안 제시 (Task 생성 템플릿)
  3. "실행하시겠습니까? (OK / NO)"
사용자: "OK"
Claude:
  4. API 호출 → {PROJECT_KEY}-209 생성
  5. 결과 보고 + 다음 명령 안내:
     - /jira branch {PROJECT_KEY}-209
     - /jira start {PROJECT_KEY}-209
```

### 컨벤션 위반 흐름

```
사용자: "/jira task 1-5"
Claude:
  1. yaml 로드
  2. 컨벤션 검증:
     ⚠️ 위반 2건:
       - SP '13' invalid (피보나치만, 8 지양)
       - type 'improvement' invalid (허용: [plan, design, env, ...])
  3. 시안 제시 중단 → yaml 수정 요청
```

---

## 안티패턴

| ❌ 안티패턴 | ✅ 권장 |
|---|---|
| 시안 생략하고 바로 create API 호출 | 반드시 시안 제시 → 승인 대기 |
| PROJECT_KEY 하드코딩 | config.yaml 또는 환경변수 |
| 8 SP로 Task 자주 생성 | 피보나치 ≤5 권장, 8은 분해 검토 |
| `list` 등 read-only도 시안 제시 | 즉시 실행 |
| 브랜치명에 한글 포함 | 소문자 + 하이픈 + 영문만 |
| 컨벤션 위반 무시하고 강행 | 위반 시 실행 중단 + yaml 수정 요청 |
| 단일 커밋에 여러 Jira key 연결 | 1 commit = 1 Jira key 권장 |

---

## 외부 참조

- Jira Cloud REST API: https://developer.atlassian.com/cloud/jira/platform/rest/v3/
- Custom field 확인: `GET /rest/api/3/field`
- Agile Sprint API: https://developer.atlassian.com/cloud/jira/software/rest/
- ADF (Atlassian Document Format): https://developer.atlassian.com/cloud/jira/platform/apis/document/

---

## 변경 이력

| 일자 | 변경 |
|---|---|
| 2026-04-24 | 하네스 v1 — Here:O 프로젝트의 `jira` skill에서 역전파, 일반화 |
