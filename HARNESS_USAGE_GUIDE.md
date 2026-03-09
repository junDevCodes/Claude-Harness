# Harness 사용 가이드

> **Advanced Harness**를 내 프로젝트에 적용하는 완전한 가이드.
> 신규 프로젝트와 기존 프로젝트 모두 동일한 Claude Code AI 지원 환경을 구성할 수 있습니다.

---

## Quick Start

### 경로 A: 신규 프로젝트 시작

```bash
# 1. 초기화 스크립트 실행 (스택 선택 → .claude/ + base/ + docs/ 자동 복사)
bash /path/to/advanced-harness-window/scripts/harness-init.sh

# 또는 Claude Code 세션에서 실행
/harness-init

# 2. Claude Code에서 /harness-apply 실행 → 설정 검증
```

### 경로 B: 기존 프로젝트에 적용

```bash
# 1. Claude Code에서 /harness-apply 실행
#    → 기존 설정 분석 + 충돌 없는 병합 전략 제시

# 2. 제시된 병합 전략에 따라 파일 병합

# 3. /harness-docs-update 실행 → 4문서 체계 초기화
```

> **핵심 원칙:** 기존 프로젝트에는 `.claude/`를 **복사하지 않고 병합**합니다. 기존 설정을 보존하면서 하네스 자산을 추가합니다.

---

## 1부: 신규 프로젝트에 하네스 적용

### Step 1: harness-init.sh로 초기화 (권장)

`scripts/harness-init.sh`를 실행하면 **스택 선택 → `.claude/` 복사 → 베이스 코드 복사 → `docs/` 초기화 → `CLAUDE.md` 생성**이 한 번에 완료됩니다.

```bash
# 기본 실행 (대화형 메뉴)
bash /path/to/advanced-harness-window/scripts/harness-init.sh

# 스택과 대상 디렉토리를 직접 지정
bash /path/to/advanced-harness-window/scripts/harness-init.sh --stack 1 --target ~/my-project

# 도움말
bash /path/to/advanced-harness-window/scripts/harness-init.sh --help
```

실행하면 아래 스택 메뉴가 출력됩니다:

```
스택을 선택하세요:
  풀스택
  [1]  FastAPI + Next.js    (Python 백엔드 + React 프론트엔드)
  [2]  NestJS + Next.js     (TypeScript 풀스택)
  [3]  Django + Next.js     (Python DRF + React 프론트엔드)
  [4]  Express + Next.js    (Node.js 백엔드 + React 프론트엔드)
  [5]  Spring Boot + Next.js (Java 백엔드 + React 프론트엔드)

  단독 스택
  [6]  FastAPI only          (Python 백엔드만)
  [7]  Next.js only          (React 프론트엔드만)
  [8]  React Native          (Expo 모바일 앱)
  [9]  C Embedded            (임베디드 펌웨어 — FreeRTOS/HAL)
  [10] C++ Embedded          (임베디드 펌웨어 — RAII/HAL)

  자산만
  [0]  스킬/에이전트만       (.claude/ 복사, 베이스 코드 제외)
```

**자동으로 처리되는 작업:**

| 단계 | 내용 |
|---|---|
| `.claude/` 복사 | Skills, Agents, Hooks, Commands 전체 복사 |
| 훅 권한 설정 | `chmod +x .claude/hooks/*.sh` |
| 훅 의존성 설치 | `cd .claude/hooks && npm install` |
| 베이스 코드 복사 | 스택에 따라 `backend/`, `frontend/`, `mobile/`, `firmware/` 생성 |
| `docs/` 4문서 | `plan.md` / `task.md` / `history.md` / `checklist.md` stub 생성 |
| `CLAUDE.md` | 스택 기반 템플릿 자동 생성 |

> **Claude 세션에서 실행하려면:** `/harness-init` 커맨드를 사용하세요.
> `harness-init.sh` 실행 후 바로 Step 3으로 이동합니다.

---

**대안 방법 (수동 복사):**

```bash
# .claude/ 전체 복사 (Skills, Agents, Hooks, Commands)
cp -r /path/to/advanced-harness-window/.claude /path/to/my-project/
chmod +x /path/to/my-project/.claude/hooks/*.sh

# 베이스 코드 복사 (스택 선택)
cp -r /path/to/advanced-harness-window/base/fastapi  /path/to/my-project/backend/   # FastAPI
cp -r /path/to/advanced-harness-window/base/nextjs   /path/to/my-project/frontend/  # Next.js
cp -r /path/to/advanced-harness-window/base/nestjs   /path/to/my-project/backend/   # NestJS
cp -r /path/to/advanced-harness-window/base/react-native /path/to/my-project/mobile/ # React Native

# docs/ 4문서 디렉토리 생성
mkdir -p /path/to/my-project/docs
```

### Step 3: pathPattern 설정 (핵심)

`skill-rules.json`의 `pathPatterns`를 내 프로젝트 구조에 맞게 수정합니다.

**내 프로젝트 구조 파악:**
```bash
# 주요 소스 경로 확인
find . -name "*.ts" -o -name "*.py" | grep -v node_modules | grep -v .venv | head -20
```

**pathPattern 수정 예시:**

```json
// .claude/skills/skill-rules.json

// FastAPI 프로젝트 (backend/ 아래에 코드가 있는 경우)
"pathPatterns": ["backend/**/*.py", "backend/domain/**/*.py"]

// Next.js 프로젝트 (frontend/src/ 아래에 코드가 있는 경우)
"pathPatterns": ["frontend/src/**/*.tsx", "frontend/src/**/*.ts"]

// 단순 구조 (src/ 바로 아래)
"pathPatterns": ["src/**/*.ts", "src/**/*.tsx"]
```

### Step 4: CLAUDE.md 초기화

프로젝트 루트에 `CLAUDE.md` 파일을 생성합니다:

```markdown
# CLAUDE.md

## 프로젝트 개요
[프로젝트 설명]

## 기술 스택
- Backend: [스택]
- Frontend: [스택]

## Agent 작업 문서 체계

모든 Claude Agents는 작업 시 `docs/` 디렉토리의 4문서 체계를 기준으로 작업한다.

| 문서 | 역할 |
|---|---|
| `docs/plan.md` | 전체 완성 계획 기획서 |
| `docs/task.md` | 현재 작업 상세 실행 계획서 |
| `docs/history.md` | 이전 작업 맥락 문서 |
| `docs/checklist.md` | 완료 기준 확인서 |

### 작업 시작 시 (필수)
1. docs/history.md 읽기
2. docs/task.md 읽기
3. docs/checklist.md 읽기
```

### Step 5: docs/ 4문서 초기화

```bash
mkdir -p docs
```

Claude Code에서:
```
/harness-docs-update
```

또는 수동으로 `docs/plan.md`, `docs/task.md`, `docs/history.md`, `docs/checklist.md` 생성.

### Step 6: 검증

```bash
# 훅 실행 권한 확인
ls -la .claude/hooks/*.sh  # -rwxr-xr-x 이어야 함

# skill-rules.json 문법 확인
cat .claude/skills/skill-rules.json | python3 -m json.tool > /dev/null && echo "✅ JSON 유효"

# settings.json 문법 확인
cat .claude/settings.json | python3 -m json.tool > /dev/null && echo "✅ JSON 유효"

# 4문서 확인
ls docs/  # plan.md task.md history.md checklist.md
```

---

## 2부: 기존 프로젝트에 하네스 적용

> 기존 프로젝트는 이미 `.claude/`가 있을 수 있습니다.
> **절대 덮어쓰지 않고**, 병합 방식으로 진행합니다.

### Step 1: 현재 상태 점검

```bash
# .claude/ 구조 확인
ls -la .claude/
ls -la .claude/skills/ 2>/dev/null || echo "skills 없음"
ls -la .claude/hooks/ 2>/dev/null || echo "hooks 없음"
ls -la .claude/agents/ 2>/dev/null || echo "agents 없음"

# 기존 settings.json 확인
cat .claude/settings.json 2>/dev/null || echo "settings.json 없음"

# 기존 skill-rules.json 확인
cat .claude/skills/skill-rules.json 2>/dev/null || echo "skill-rules.json 없음"
```

### Step 2: Skills 병합

```bash
# 기존 skills/ 디렉토리가 없으면 전체 복사
cp -r /path/to/harness/.claude/skills /path/to/my-project/.claude/

# 기존 skills/ 디렉토리가 있으면 새 스킬만 추가 (기존 건드리지 않음)
# 하네스 스킬 목록 확인
ls /path/to/harness/.claude/skills/
# 내 프로젝트에 없는 스킬만 복사
cp -r /path/to/harness/.claude/skills/fastapi-backend-guidelines .claude/skills/
cp -r /path/to/harness/.claude/skills/nextjs-frontend-guidelines .claude/skills/
# ... 필요한 스킬만 선택적으로 복사
```

### Step 3: skill-rules.json 병합

**기존 `skill-rules.json`이 없는 경우:**
```bash
cp /path/to/harness/.claude/skills/skill-rules.json .claude/skills/
```

**기존 `skill-rules.json`이 있는 경우 — 수동 병합:**

1. 하네스의 `skill-rules.json` 열기
2. 내 프로젝트의 `skill-rules.json` 열기
3. 하네스에서 필요한 스킬 항목을 복사하여 내 파일에 추가

```json
// 내 .claude/skills/skill-rules.json에 항목 추가하는 방식
{
  "skills": {
    // ... 기존 스킬 유지 ...

    // 하네스에서 추가할 스킬들
    "fastapi-backend-guidelines": { ... },
    "nextjs-frontend-guidelines": { ... }
  }
}
```

**⚠️ 중복 스킬 주의:** 같은 이름의 스킬이 있으면 기존 설정을 유지하거나 신중히 병합합니다.

### Step 4: pathPattern을 내 프로젝트 구조에 맞게 재설정

**이것이 가장 중요한 단계입니다.** 하네스의 기본 pathPattern은 `base/[stack]/` 경로를 가리키므로, 내 프로젝트 구조에 맞게 반드시 수정해야 합니다.

```bash
# 내 프로젝트의 Python 파일 위치 확인
find . -name "*.py" -not -path "./.venv/*" -not -path "./node_modules/*" | head -10

# 내 프로젝트의 TypeScript 파일 위치 확인
find . -name "*.ts" -not -path "./node_modules/*" | head -10
```

**pathPattern 수정 예시 — 일반적인 구조:**

```json
// 상황 1: backend/, frontend/ 분리된 경우
"fastapi-backend-guidelines": {
  "fileTriggers": {
    "pathPatterns": ["backend/**/*.py", "app/**/*.py"]
  }
},
"nextjs-frontend-guidelines": {
  "fileTriggers": {
    "pathPatterns": ["frontend/src/**/*.tsx", "src/**/*.tsx"]
  }
}

// 상황 2: 모노레포 (packages/ 구조)
"fastapi-backend-guidelines": {
  "fileTriggers": {
    "pathPatterns": ["packages/api/**/*.py", "apps/backend/**/*.py"]
  }
}

// 상황 3: 단일 서비스 (src/ 바로)
"fastapi-backend-guidelines": {
  "fileTriggers": {
    "pathPatterns": ["src/**/*.py"]
  }
}
```

### Step 5: settings.json 병합

**기존 `settings.json`이 없는 경우:**
```bash
cp /path/to/harness/.claude/settings.json .claude/settings.json
# 이후 내 프로젝트에 맞게 조정
```

**기존 `settings.json`이 있는 경우 — 훅 항목만 추가:**

```json
// 기존 settings.json에 하네스 훅 추가 (기존 내용 보존)
{
  // ... 기존 내용 유지 ...
  "hooks": {
    "UserPromptSubmit": [
      // ... 기존 훅 유지 ...
      {
        "hooks": [{
          "type": "command",
          "command": "$CLAUDE_PROJECT_DIR/.claude/hooks/skill-activation-prompt.sh"
        }]
      },
      {
        "hooks": [{
          "type": "command",
          "command": "$CLAUDE_PROJECT_DIR/.claude/hooks/session-start-docs-loader.sh"
        }]
      }
    ],
    "PostToolUse": [
      // ... 기존 훅 유지 ...
      {
        "matcher": "Edit|MultiEdit|Write",
        "hooks": [{
          "type": "command",
          "command": "$CLAUDE_PROJECT_DIR/.claude/hooks/post-tool-use-tracker.sh"
        }]
      }
    ],
    "Stop": [
      // ... 기존 훅 유지 ...
      {
        "hooks": [{
          "type": "command",
          "command": "$CLAUDE_PROJECT_DIR/.claude/hooks/docs-update-reminder.sh"
        }]
      }
    ]
  }
}
```

> **주의:** 기존 훅 배열에 새 항목을 **추가**합니다. 기존 항목을 삭제하거나 교체하지 마세요.

### Step 6: Agents 복사

Agents는 독립적으로 동작하므로 그냥 복사합니다:

```bash
# 전체 복사 (기존 agents/ 없는 경우)
cp -r /path/to/harness/.claude/agents .claude/

# 선택적 복사 (기존 agents/ 있는 경우)
cp /path/to/harness/.claude/agents/db-migration-agent.md .claude/agents/
cp /path/to/harness/.claude/agents/api-spec-generator.md .claude/agents/
# ... 필요한 에이전트만 선택
```

### Step 7: Hooks 복사

```bash
# hooks/ 디렉토리가 없는 경우
cp -r /path/to/harness/.claude/hooks .claude/

# hooks/ 디렉토리가 있는 경우 — 필요한 훅만 추가
cp /path/to/harness/.claude/hooks/skill-activation-prompt.{ts,sh} .claude/hooks/
cp /path/to/harness/.claude/hooks/session-start-docs-loader.{ts,sh} .claude/hooks/
cp /path/to/harness/.claude/hooks/docs-update-reminder.{ts,sh} .claude/hooks/

# 실행 권한 부여
chmod +x .claude/hooks/*.sh

# 의존성 설치 (TypeScript 훅)
cd .claude/hooks && npm install
```

### Step 8: CLAUDE.md에 4문서 체계 추가

기존 `CLAUDE.md`에 아래 섹션을 **추가**합니다 (기존 내용 유지):

```markdown
---

## Agent 작업 문서 체계

모든 Claude Agents는 작업 시 `docs/` 디렉토리의 4문서 체계를 기준으로 작업한다.

| 문서 | 역할 |
|---|---|
| `docs/plan.md` | 전체 완성 계획 기획서 |
| `docs/task.md` | 현재 작업 상세 실행 계획서 |
| `docs/history.md` | 이전 작업 맥락 문서 |
| `docs/checklist.md` | 완료 기준 확인서 |

### 작업 시작 시 (필수)
1. docs/history.md 읽기  → 이전 맥락 파악
2. docs/task.md 읽기     → 현재 작업 범위 확인
3. docs/checklist.md 읽기 → 완료 기준 확인
```

### Step 9: docs/ 4문서 초기화

기존 코드를 기반으로 초기 4문서를 작성합니다. Claude Code에서:

```
현재 프로젝트의 기존 코드를 분석해서 docs/ 4문서를 초기화해줘.
- docs/plan.md: 앞으로의 개발 계획
- docs/task.md: 현재 진행 중인 작업
- docs/history.md: 지금까지 작업한 내용 요약
- docs/checklist.md: 현재 작업의 완료 기준
```

### Step 10: 검증

```bash
# 훅 실행 권한 확인
ls -la .claude/hooks/*.sh

# JSON 파일 문법 확인
cat .claude/settings.json | python3 -m json.tool > /dev/null && echo "✅ settings.json 유효"
cat .claude/skills/skill-rules.json | python3 -m json.tool > /dev/null && echo "✅ skill-rules.json 유효"

# 추가된 스킬 확인
ls .claude/skills/

# 4문서 확인
ls docs/  # plan.md task.md history.md checklist.md 모두 존재해야 함
```

---

## 3부: 자산 선택 가이드

### 스택별 권장 Skills 조합

| 기술 스택 | 필수 스킬 | 권장 추가 |
|---|---|---|
| FastAPI | `fastapi-backend-guidelines`, `pytest-backend-testing` | `database-guidelines`, `docker-guidelines` |
| Next.js 15 | `nextjs-frontend-guidelines` | `vercel-react-best-practices`, `frontend-design` |
| Express.js | `express-backend-guidelines` | `database-guidelines`, `docker-guidelines` |
| NestJS | `nestjs-backend-guidelines` | `database-guidelines`, `docker-guidelines` |
| Django | `django-backend-guidelines` | `database-guidelines`, `docker-guidelines` |
| Spring Boot | `spring-boot-guidelines` | `database-guidelines`, `docker-guidelines` |
| React Native | `react-native-guidelines` | `frontend-design` |
| AWS 배포 | `aws-guidelines` | `docker-guidelines` |
| 임베디드 C | `embedded-c-guidelines` | — |
| 임베디드 C++ | `embedded-cpp-guidelines` | — |

**공통 추천 (모든 프로젝트):**
- `error-tracking` — Sentry 에러 추적
- `skill-developer` — 프로젝트 맞춤 스킬 생성

### 필수 훅 vs 선택 훅

| 훅 | 유형 | 설명 | 추천 |
|---|---|---|---|
| `skill-activation-prompt` | UserPromptSubmit | 스킬 자동 제안 | ⭐ 필수 |
| `session-start-docs-loader` | UserPromptSubmit | 세션 시작 시 4문서 자동 주입 | ⭐ 필수 |
| `docs-update-reminder` | Stop | 세션 종료 시 history.md 업데이트 리마인더 | 권장 |
| `post-tool-use-tracker` | PostToolUse | 파일 변경 추적 | 권장 |
| `pr-review-trigger` | PostToolUse/Bash | PR 생성 시 코드 리뷰 권고 | 선택 (gh 사용 시) |
| `tsc-check` | Stop | TypeScript 컴파일 오류 체크 | 선택 (TS 멀티 서비스 시) |

### 권장 Agents

| 상황 | 권장 에이전트 |
|---|---|
| DB 스키마 변경 | `db-migration-agent` |
| API 문서 자동화 | `api-spec-generator` |
| 테스트 커버리지 개선 | `test-coverage-agent` |
| 성능 점검 | `performance-auditor` |
| 보안 점검 | `security-auditor` |
| 코드 아키텍처 리뷰 | `code-architecture-reviewer` |

---

## 4부: 검증 체크리스트

### 신규 프로젝트 검증

- [ ] `.claude/` 디렉토리 구조 정상
- [ ] `.claude/hooks/*.sh` 실행 권한 (`-rwxr-xr-x`)
- [ ] `.claude/skills/skill-rules.json` JSON 유효
- [ ] `.claude/settings.json` JSON 유효
- [ ] `skill-rules.json`의 `pathPatterns`가 내 프로젝트 경로 반영
- [ ] Claude Code 새 세션 시작 → 4문서 로드 메시지 출력 확인
- [ ] 스킬 활성화 테스트: 관련 프롬프트 입력 → 스킬 제안 확인
- [ ] `docs/` 4문서 4개 모두 존재

### 기존 프로젝트 검증

- [ ] 기존 `settings.json` 설정 유지되었는지 확인
- [ ] 기존 스킬/에이전트 파일 그대로 존재하는지 확인
- [ ] 새로 추가된 스킬 정상 작동 테스트
- [ ] `skill-rules.json` 문법 오류 없음
- [ ] 기존 훅과 새 훅 모두 동작 확인
- [ ] `docs/` 4문서 초기화 완료
- [ ] `CLAUDE.md` 4문서 체계 섹션 포함

---

## 5부: 트러블슈팅

### 문제 1: 훅이 실행되지 않음

**증상:** `UserPromptSubmit` 후 아무 출력 없음

**해결:**
```bash
# 실행 권한 확인
ls -la .claude/hooks/skill-activation-prompt.sh
# -rwxr-xr-x 아니면:
chmod +x .claude/hooks/*.sh

# settings.json 훅 경로 확인
grep "command" .claude/settings.json
# $CLAUDE_PROJECT_DIR 변수가 올바르게 사용되고 있는지 확인
```

### 문제 2: 스킬이 활성화되지 않음

**증상:** 관련 프롬프트 입력해도 스킬 제안 없음

**해결:**
```bash
# skill-rules.json 문법 확인
cat .claude/skills/skill-rules.json | python3 -m json.tool

# 스킬 디렉토리 확인
ls .claude/skills/[스킬명]/

# pathPattern 확인 — 내 파일 경로와 일치하는지
# 예: "backend/**/*.py"가 실제 파일 경로 backend/app/main.py와 매칭되는지
```

### 문제 3: TypeScript 훅 실행 오류

**증상:** `npx tsx` 오류 발생

**해결:**
```bash
# 의존성 확인
ls .claude/hooks/node_modules/

# 없으면 설치
cd .claude/hooks && npm install

# tsx 직접 테스트
npx tsx .claude/hooks/skill-activation-prompt.ts
```

### 문제 4: settings.json 병합 후 JSON 오류

**증상:** Claude Code 시작 시 settings.json 파싱 오류

**해결:**
```bash
# JSON 문법 검사
cat .claude/settings.json | python3 -m json.tool

# 오류 위치 확인 후 직접 수정
# 흔한 실수: 배열 마지막 항목 후 쉼표, 중괄호 불일치
```

### 문제 5: skill-rules.json 중복 스킬 오류

**증상:** 같은 스킬 이름이 두 번 등록됨

**해결:**
```bash
# 중복 확인
cat .claude/skills/skill-rules.json | python3 -c "
import json, sys
data = json.load(sys.stdin)
skills = list(data.get('skills', {}).keys())
dupes = [s for s in skills if skills.count(s) > 1]
print('중복:', dupes)
"
# 중복 항목 하나를 삭제 (기존 설정 유지 권장)
```

### 문제 6: 4문서 로더 훅이 항상 실행됨 (캐시 미작동)

**증상:** 매번 세션 시작 시 4문서 로드 메시지 출력

**해결:**
```bash
# 세션 캐시 디렉토리 확인
ls .claude/.session-cache/

# .gitignore에 *.loaded 패턴 있는지 확인
grep "loaded" .gitignore

# 없으면 추가
echo ".claude/.session-cache/*.loaded" >> .gitignore
```

---

## 6부: 하네스 운영 도구

> Phase 5-B에서 추가된 운영 도구 모음.
> 신규 프로젝트 초기화, 자산 버전 관리, 실 프로젝트 패턴 역전파를 지원합니다.

### 버전 관리 — `/harness-changelog`

이번 세션에서 변경된 하네스 자산을 분석하여 `CHANGELOG.md` 항목을 자동 생성합니다.

**버전 번호 결정 기준:**

| 변경 유형 | 버전 변경 |
|---|---|
| 새 스킬 / 에이전트 / 커맨드 / 베이스 스택 추가 | `minor` (X.Y+1.0) |
| 기존 자산 수정 / 오탐 수정 / 문서 업데이트 | `patch` (X.Y.Z+1) |
| `.claude/` 또는 `base/` 전체 구조 변경 | `major` (X+1.0.0) |

```bash
# Claude Code 세션에서 실행
/harness-changelog

# 버전 직접 지정
/harness-changelog v2.0.0
```

**실행 흐름:** CHANGELOG.md 읽기 → 변경 자산 분류 → 버전 결정 → [Unreleased] 항목 추가 → 릴리즈 확인 → base_code_plan.md 버전 동기화

---

### 역전파 워크플로우 — `/harness-backport`

실 프로젝트에서 발견한 유용한 패턴을 `base/[stack]/` 및 관련 Skill에 체계적으로 반영합니다.

**역전파 O/X 기준:**

| 역전파 O (권장) | 역전파 X (제외) |
|---|---|
| 재사용 가능한 공통 패턴 (JWT, 페이지네이션 등) | 비즈니스 로직 |
| 새로운 아키텍처 패턴 | 환경 특화 설정 |
| 성능 최적화 기법 | 개인정보/인증정보 |
| 보안 강화 패턴 | 미검증 실험적 코드 |
| 라이브러리 업그레이드 대응 패턴 | 프로젝트 특화 도메인명 |

```bash
# 실행 예시
/harness-backport FastAPI JWT 슬라이딩 윈도우 갱신 패턴 역전파
/harness-backport NestJS Redis 캐싱 전략 역전파
```

**실행 흐름 (5단계):**

1. 역전파 O/X 기준 판별
2. 해당 스택 + 복수 스택 적용 가능 여부 확인
3. `base/[stack]/` 수정 범위 확정 + Skill 문서화 범위 확정
4. `base/[stack]/` 파일 수정 → `SKILL.md` 패턴 섹션 추가 → `CHANGELOG.md` 항목 추가 → `base_code_plan.md` 이력 갱신
5. 완료 파일 목록 출력

> **역전파 후:** `/harness-changelog`로 버전 이력 반드시 기록

---

### 신규 프로젝트 초기화 — `scripts/harness-init.sh`

Claude 없이 터미널에서 단독 실행 가능한 초기화 스크립트.
상세 내용은 [1부: 신규 프로젝트에 하네스 적용](#1부-신규-프로젝트에-하네스-적용) 참조.

```bash
bash scripts/harness-init.sh --help
```

---

### 일반 운영 흐름

신규 프로젝트 생성 → 기능 개발 → 역전파 → 버전 릴리즈 사이클:

```
1. 신규 프로젝트 시작
   bash scripts/harness-init.sh  (또는 /harness-init)
   → .claude/ + base/ + docs/ 자동 구성

2. 기능 개발
   → 실 프로젝트에서 유용한 패턴 발견 시 메모

3. 패턴 역전파 (해당 시)
   /harness-backport [패턴 설명]
   → base/[stack]/ + SKILL.md 업데이트

4. 버전 릴리즈
   /harness-changelog
   → CHANGELOG.md [Unreleased] → [vX.Y.Z] 릴리즈
```

> **전체 커맨드 레퍼런스:** `HARNESS_COMMANDS_REFERENCE.md` 참조

---

## 부록: 하네스 구조 요약

```
advanced-harness-window/
├── .claude/
│   ├── skills/        ← 24개 스킬 (스택별 개발 가이드라인)
│   ├── agents/        ← 16개 에이전트 (자동화 서브태스크)
│   ├── commands/      ← 9개 슬래시 커맨드
│   ├── hooks/         ← 자동화 훅 (훅 3종 6개 파일)
│   └── settings.json  ← 훅 등록 설정
├── base/              ← 9개 스택 베이스 코드 (신규 프로젝트용)
│   ├── fastapi/       ← Python FastAPI + SQLModel + JWT + S3
│   ├── nextjs/        ← Next.js 15 + App Router + JWT + shadcn/ui
│   ├── express/       ← Express.js + TypeScript + Prisma + JWT
│   ├── django/        ← Django 5 + DRF + SimpleJWT + PostgreSQL
│   ├── nestjs/        ← NestJS + Prisma + JWT Guards
│   ├── spring-boot/   ← Spring Boot 3 + Java 21 + Security + JPA
│   ├── react-native/  ← Expo SDK 52 + Zustand + SecureStore
│   ├── c-embedded/    ← C99 + CMake + HAL + FreeRTOS
│   └── cpp-embedded/  ← C++17 + CMake + HAL 클래스 + Google Test
└── docs/              ← 하네스 자체 관리 문서 (참고용)
```

---

*Last updated: 2026-03-09*
