# Harness 커맨드 레퍼런스

> Advanced Harness의 모든 슬래시 커맨드, 스크립트, 훅 시스템의 상세 레퍼런스.
> 신규 사용자는 `HARNESS_USAGE_GUIDE.md`를 먼저 읽으세요.

---

## Quick Reference

| 도구 | 유형 | 역할 | 사용 시점 |
|---|---|---|---|
| `scripts/harness-init.sh` | 쉘 스크립트 | 신규 프로젝트 초기화 | 프로젝트 첫 시작 |
| `/harness-init` | 슬래시 커맨드 | Claude 세션 내 초기화 | 프로젝트 첫 시작 (Claude 세션) |
| `/harness-apply` | 슬래시 커맨드 | 신규/기존 자동 판별 → 하네스 적용 | 초기화 검증 또는 기존 프로젝트 적용 |
| `/harness-docs-update` | 슬래시 커맨드 | 4문서 체계 업데이트 | 세션 종료 전 |
| `/harness-plan-sync` | 슬래시 커맨드 | plan.md → 다음 Phase task.md 자동 생성 | 새 Phase 시작 시 |
| `/harness-changelog` | 슬래시 커맨드 | 변경 자산 분석 → CHANGELOG.md 항목 생성 | 자산 릴리즈 시 |
| `/harness-backport` | 슬래시 커맨드 | 실 프로젝트 패턴 → 하네스 역전파 | 검증된 패턴 발견 시 |
| `/dev-docs` | 슬래시 커맨드 | 전략적 구현 계획 + 작업 분해 | 새 기능 기획 시 |
| `/dev-docs-update` | 슬래시 커맨드 | 컨텍스트 컴팩션 전 docs/ 업데이트 | 컨텍스트 한계 접근 시 |

---

## 1. 초기화 도구

### `scripts/harness-init.sh`

**개요:** Claude 없이 터미널에서 단독 실행 가능한 대화형 초기화 스크립트.
스택 선택 → `.claude/` 복사 → 베이스 코드 복사 → `docs/` stub → `CLAUDE.md` 생성까지 자동화.

**사용법:**

```bash
# 기본 (대화형 스택 메뉴)
bash scripts/harness-init.sh

# 옵션 지정
bash scripts/harness-init.sh --stack 1 --target ~/my-project

# 도움말
bash scripts/harness-init.sh --help
```

**옵션:**

| 옵션 | 설명 |
|---|---|
| `--help`, `-h` | 도움말 출력 |
| `--stack N` | 스택 번호 직접 지정 (0~10) |
| `--target DIR` | 대상 디렉토리 직접 지정 |

**스택 목록 (0~10):**

| 번호 | 스택 | 생성 디렉토리 |
|---|---|---|
| 1 | FastAPI + Next.js | `backend/` + `frontend/` |
| 2 | NestJS + Next.js | `backend/` + `frontend/` |
| 3 | Django + Next.js | `backend/` + `frontend/` |
| 4 | Express + Next.js | `backend/` + `frontend/` |
| 5 | Spring Boot + Next.js | `backend/` + `frontend/` |
| 6 | FastAPI only | `backend/` |
| 7 | Next.js only | `frontend/` |
| 8 | React Native | `mobile/` |
| 9 | C Embedded | `firmware/` |
| 10 | C++ Embedded | `firmware/` |
| 0 | 스킬/에이전트만 | 베이스 코드 없음 |

**실행 예시:**

```bash
# FastAPI + Next.js 풀스택 프로젝트 초기화
bash scripts/harness-init.sh --stack 1 --target ~/workspace/my-app
```

**생성 결과물:**

```
my-app/
├── .claude/           ← Skills(24) + Agents(16) + Commands(9) + Hooks + settings.json
├── backend/           ← base/fastapi/ 복사본 (스택 1 기준)
├── frontend/          ← base/nextjs/ 복사본 (스택 1 기준)
├── docs/
│   ├── plan.md        ← stub (Phase 1 템플릿)
│   ├── task.md        ← stub
│   ├── history.md     ← stub
│   └── checklist.md   ← stub
└── CLAUDE.md          ← 스택 기반 템플릿 (수정 필요)
```

---

### `/harness-init` (Claude 세션)

**개요:** `harness-init.sh`의 Claude 세션 내 대화형 버전.
터미널 직접 실행이 어려운 환경이나 대화형 안내가 필요할 때 사용.

**실행 흐름 요약:**

1. 스택 목록 제시 → 선택 요청
2. 현재 디렉토리 탐지 (`.claude/` 존재 시 `/harness-apply` 권장 후 중단)
3. 하네스 경로 확인
4. 복사 명령어 순서대로 출력
5. pathPattern 권장값 제시
6. `CLAUDE.md` 초기화 지시
7. 완료 체크리스트 출력

**사용 예시:**

```
/harness-init
/harness-init fastapi-nextjs
/harness-init react-native
```

> 기존 프로젝트(`.claude/` 존재)에는 `/harness-apply` 사용 권장.

---

## 2. 적용 도구

### `/harness-apply`

**개요:** 현재 프로젝트 구조를 분석하여 신규/기존 모드를 자동 판별 후 충돌 없이 하네스 자산을 적용.

**신규/기존 판별 기준:**

| 조건 | 모드 | 동작 |
|---|---|---|
| `.claude/` 없음 | 신규 모드 | 전체 복사 후 설정 가이드 |
| `.claude/` 있음 | 기존 모드 | 충돌 분석 → 병합 전략 제시 |

**기존 모드 주요 기능:**

- `settings.json` 충돌 분석 → 훅 항목만 추가 (기존 내용 보존)
- `skill-rules.json` 충돌 분석 → 새 스킬 규칙만 추가
- `pathPattern` 소스 경로 자동 탐지 → 권장 설정 제시
- 최종 검증 체크리스트 자동 출력

**사용 예시:**

```
/harness-apply
/harness-apply ~/harness/advanced-harness-window
```

---

### `/harness-docs-update`

**개요:** 이 하네스 프로젝트 전용 4문서 체계 업데이트 커맨드.
세션 종료 전 또는 docs-update-reminder 훅 알림 시 실행.

**실행 흐름 (6단계):**

```
Step 0. task.md 완료 항목 → plan.md [ ] → [x] 자동 동기화
Step 1. docs/checklist.md 최종 검증 항목 체크
Step 2. docs/task.md 완료 항목 표시
Step 3. docs/history.md 업데이트 (완료 내용 + 다음 세션 맥락)
Step 4. docs/plan.md 해당 Phase 항목 체크 + Phase 완료 시 ✅ 추가
Step 5. 완료 확인 메시지 출력
```

**실행 시점:**

- 세션 종료 전 (Stop 훅 리마인더 수신 시)
- 컨텍스트 컴팩션 직전
- 주요 Task 완료 시

---

## 3. 계획 관리 도구

### `/harness-plan-sync`

**개요:** `docs/plan.md`를 분석하여 다음 미완료 Phase의 실행 계획을 자동으로 `task.md`와 `checklist.md`에 생성.

**실행 흐름:**

```
1. docs/plan.md 전체 읽기
2. 미완료 Phase 탐색 (✅ 없고 [ ] 항목 1개 이상인 첫 번째 Phase)
3. [ ] 항목 전체 추출
4. 각 항목별 상세 분석 (산출물, 의존성, 기술 요소)
5. docs/task.md 덮어쓰기 (새 Phase 상세 계획)
6. docs/checklist.md 업데이트 (DoD 기준)
7. 완료 안내 + 시작 명령어 제안
```

**사용 예시:**

```
/harness-plan-sync          ← 자동으로 다음 미완료 Phase 탐색
/harness-plan-sync Phase 5  ← 특정 Phase 고정
```

---

### `/dev-docs`

**개요:** 현재 프로젝트 컨텍스트를 분석하여 전략적 구현 계획과 구조적 작업 분해를 생성.
`plan.md` + `task.md` 초안을 한 번에 작성할 때 유용.

---

### `/dev-docs-update`

**개요:** 컨텍스트 컴팩션 전 현재 작업 상태를 `docs/` 4문서에 반영.
`/harness-docs-update`의 범용 버전 (하네스 전용 규칙 미적용).

---

## 4. 버전 관리 도구

### `/harness-changelog`

**개요:** 이번 세션 변경 자산을 분석하여 `CHANGELOG.md`에 항목을 자동 생성하고, 릴리즈 여부를 확인합니다.

**버전 번호 결정 기준:**

```
major (X+1.0.0): 하네스 디렉토리 구조 전면 개편
minor (X.Y+1.0): 새 자산 추가 (스킬/에이전트/커맨드/베이스 스택/훅)
patch (X.Y.Z+1): 기존 자산 수정만 (오탐 수정, 문서 업데이트, 역전파)
```

**버전 결정 우선순위:** `major > minor > patch`
같은 세션에 신규 추가 + 수정이 함께 있으면 → `minor`

**실행 흐름 (6단계):**

```
Step 1. CHANGELOG.md 현재 최신 버전 읽기
Step 2. 변경 자산 분류 (Added/Changed/Fixed)
Step 3. 버전 번호 결정
Step 4. [Unreleased] 섹션에 항목 추가
Step 5. 릴리즈 여부 확인 (Y → 버전 릴리즈, n → Unreleased 유지)
Step 6. base_code_plan.md 버전 동기화 (릴리즈 시)
```

**CHANGELOG 항목 형식:**

```markdown
## [Unreleased]

### Added
- Phase X-Y: `.claude/skills/[name]/SKILL.md` — 스킬명 신규 추가

### Changed
- Backport: `base/[stack]/[파일]` — [패턴명] 역전파

### Fixed
- Phase X-Y: `skill-rules.json` — [스킬명] 오탐 수정 (키워드 → 수정 내용)
```

**사용 예시:**

```
/harness-changelog            ← 자동 버전 결정
/harness-changelog v2.0.0     ← 버전 직접 지정
```

---

## 5. 역전파 도구

### `/harness-backport`

**개요:** 실 프로젝트에서 발견한 재사용 가능한 패턴을 `base/[stack]/` 및 관련 `Skill`에 체계적으로 반영.

**역전파 O/X 기준:**

| 역전파 O (권장) | 역전파 X (제외) |
|---|---|
| 재사용 가능한 공통 패턴 | 비즈니스 로직 |
| 새로운 아키텍처 패턴 | 환경 특화 설정 |
| 성능 최적화 기법 | 개인정보/인증정보 |
| 보안 강화 패턴 | 미검증 실험적 코드 |
| 라이브러리 업그레이드 대응 패턴 | 프로젝트 특화 도메인명 |

**실행 흐름 (5단계):**

```
Step 1. 역전파 패턴 파악 (인수 없으면 질문)
Step 2. 역전파 O/X 기준 체크 + 해당 스택 확인
Step 3. base/[stack]/ 수정 범위 + Skill 문서화 범위 확정
Step 4-1. base/[stack]/ 파일 수정 (범용화: 도메인명 → User/Item)
Step 4-2. SKILL.md 패턴 섹션 추가
Step 4-3. CHANGELOG.md [Unreleased]에 역전파 항목 추가
Step 4-4. base_code_plan.md 역전파 이력 갱신
Step 5. 완료 파일 목록 출력 + skill-rules.json 키워드 추가 여부 확인
```

**역전파 수정 원칙:**

- 비즈니스 도메인명 → 범용명 변환 (`Artist` → `User`, `Concert` → `Item`)
- 환경 특화 설정 제거 (하드코딩 URL, 특정 계정 정보)
- 실행 가능한 상태 유지 (import 경로, 의존성 맞춤)
- 역전파 출처 주석 추가

```python
# 역전파 — [패턴명]
# 출처: [실 프로젝트명] YYYY-MM-DD
# 목적: [패턴의 목적 설명]
```

**사용 예시:**

```
/harness-backport FastAPI JWT 슬라이딩 윈도우 갱신 패턴
/harness-backport NestJS @Cacheable Redis 캐싱 전략
/harness-backport 웹훅 이벤트 멱등성 보장 패턴 (Express + FastAPI 공통)
```

> 역전파 전 실 프로젝트에서 3회 이상 사용 또는 성능/보안 효과 측정 권장.
> 역전파 후 `/harness-changelog`로 버전 이력 반드시 기록.

---

## 6. 훅 시스템

Claude Code `settings.json`에 등록된 자동화 훅 목록.

### `skill-activation-prompt` (UserPromptSubmit)

**역할:** 프롬프트 제출 시 `skill-rules.json` 기반으로 관련 스킬을 자동 제안.
**파일:** `.claude/hooks/skill-activation-prompt.{ts,sh}`
**추천:** ⭐ 필수

### `session-start-docs-loader` (UserPromptSubmit)

**역할:** 세션 첫 프롬프트 시 4문서 읽기 지시를 Claude 컨텍스트에 자동 주입.
- 세션별 캐시 파일 (`.claude/.session-cache/[session_id].loaded`) 확인 후 1회만 실행
- `docs/history.md`의 현재 Phase 정보도 함께 주입

**파일:** `.claude/hooks/session-start-docs-loader.{ts,sh}`
**추천:** ⭐ 필수

### `docs-update-reminder` (Stop)

**역할:** 세션 종료 시 `docs/history.md` 미갱신 여부를 확인하고 리마인더 출력.
- `.loaded` 파일 mtime과 `history.md` mtime을 비교
- `history.md`가 최신이면 조용히 종료

**파일:** `.claude/hooks/docs-update-reminder.{ts,sh}`
**비활성화:** `SKIP_DOCS_REMINDER=1` 환경변수 설정
**추천:** 권장

### `pr-review-trigger` (PostToolUse/Bash)

**역할:** `gh pr create` 명령 감지 시 exit 2로 코드 리뷰 권고 컨텍스트를 Claude에 주입.
- GitHub Actions `pr-code-review.yaml`과 연동하면 PR 댓글 자동 작성

**파일:** `.claude/hooks/pr-review-trigger.{ts,sh}`
**비활성화:** `SKIP_PR_REVIEW=1` 환경변수 설정
**리뷰 저장:** `docs/reviews/[YYYY-MM-DD]-[branch]-review.md`
**추천:** 선택 (gh 사용 시)

### `post-tool-use-tracker` (PostToolUse/Edit|MultiEdit|Write)

**역할:** 파일 변경 시 변경 목록을 추적하여 컨텍스트에 기록.
**파일:** `.claude/hooks/post-tool-use-tracker.sh`
**추천:** 권장

### `tsc-check` (Stop)

**역할:** 세션 종료 시 TypeScript 컴파일 오류 체크.
**파일:** `.claude/hooks/tsc-check.sh`
**추천:** 선택 (TypeScript 멀티 서비스 시)

---

### 훅 등록 확인 방법

```bash
# settings.json 훅 등록 확인
cat .claude/settings.json | python3 -m json.tool

# 등록된 훅 목록 확인
grep -A2 '"command"' .claude/settings.json
```

**settings.json 핵심 구조:**

```json
{
  "hooks": {
    "UserPromptSubmit": [
      { "hooks": [{ "type": "command", "command": "...skill-activation-prompt.sh" }] },
      { "hooks": [{ "type": "command", "command": "...session-start-docs-loader.sh" }] }
    ],
    "PostToolUse": [
      { "matcher": "Edit|MultiEdit|Write", "hooks": [{ "type": "command", "command": "...post-tool-use-tracker.sh" }] },
      { "matcher": "Bash", "hooks": [{ "type": "command", "command": "...pr-review-trigger.sh" }] }
    ],
    "Stop": [
      { "hooks": [{ "type": "command", "command": "...tsc-check.sh" }] },
      { "hooks": [{ "type": "command", "command": "...trigger-build-resolver.sh" }] },
      { "hooks": [{ "type": "command", "command": "...docs-update-reminder.sh" }] }
    ]
  }
}
```

---

## 7. 일반 운영 워크플로우

### 신규 프로젝트 → 역전파 → 버전 릴리즈 사이클

```
┌─────────────────────────────────────────────────────────┐
│  1. 신규 프로젝트 시작                                     │
│     bash scripts/harness-init.sh --stack 1 --target ~/app │
│     → .claude/ + base/ + docs/ + CLAUDE.md 자동 구성      │
└──────────────────────┬──────────────────────────────────┘
                       ↓
┌─────────────────────────────────────────────────────────┐
│  2. 기능 개발                                             │
│     Claude Code 세션 → 4문서 기반 작업                     │
│     session-start-docs-loader 훅: 세션 시작 시 4문서 주입  │
│     docs-update-reminder 훅: 세션 종료 시 리마인더         │
└──────────────────────┬──────────────────────────────────┘
                       ↓
┌─────────────────────────────────────────────────────────┐
│  3. 패턴 역전파 (유용한 패턴 발견 시)                       │
│     /harness-backport [패턴 설명]                         │
│     → base/[stack]/ + SKILL.md 업데이트                   │
└──────────────────────┬──────────────────────────────────┘
                       ↓
┌─────────────────────────────────────────────────────────┐
│  4. 버전 릴리즈                                           │
│     /harness-changelog                                   │
│     → CHANGELOG.md [Unreleased] → [vX.Y.Z] 릴리즈        │
└─────────────────────────────────────────────────────────┘
```

---

### 4문서 체계 운영 사이클

```
세션 시작
  → session-start-docs-loader: docs/ 3문서 자동 읽기 주입

작업 중
  → /harness-plan-sync: plan.md → 다음 Phase task.md 자동 생성 (Phase 시작 시)

작업 종료
  → /harness-docs-update: 4문서 체계 업데이트 (6단계)
  → docs-update-reminder: history.md 미갱신 시 리마인더
```

**4문서 업데이트 시점:**

| 문서 | 업데이트 시점 |
|---|---|
| `plan.md` | Phase 완료 시, 새 Phase 시작 시, 큰 방향 변경 시 |
| `task.md` | 새 Task 시작 시, Task 완료 시 |
| `history.md` | **세션 종료 전** (필수), 컨텍스트 컴팩션 전, Task 완료 시 |
| `checklist.md` | Task 시작 시 (기준 작성), 진행 중 (체크), 완료 시 (최종 검증) |

---

*Last updated: 2026-03-09*
