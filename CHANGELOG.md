# CHANGELOG

Advanced Harness 자산 버전 이력. [Keep a Changelog](https://keepachangelog.com/ko/) 형식을 준수한다.

## 버전 명명 규칙

```
v[major].[minor].[patch] — YYYY-MM-DD

major : 하네스 구조 전면 개편 (디렉토리 구조 변경 등)
minor : 새 자산 추가 (새 스킬/에이전트/커맨드/스택)
patch : 기존 자산 수정/오탐 수정/문서 업데이트
```

---

## [Unreleased]

---

## [v1.5.0] — 2026-03-09

### Added
- Phase 5-B: `scripts/harness-init.sh` — 터미널 단독 실행형 대화형 초기화 스크립트 (스택 11가지)
- Phase 5-B: `.claude/commands/harness-init.md` — Claude 세션 내 대화형 초기화 커맨드
- Phase 5-B: `CHANGELOG.md` — 하네스 전체 자산 버전 이력 관리
- Phase 5-B: `.claude/commands/harness-changelog.md` — CHANGELOG 항목 자동 생성 커맨드
- Phase 5-B: `.claude/commands/harness-backport.md` — 실 프로젝트 패턴 역전파 워크플로우 커맨드

---

## [v1.4.0] — 2026-03-09

### Added
- Phase 5-A: `HARNESS_USAGE_GUIDE.md` — 신규/기존 프로젝트 통합 사용자 가이드
- Phase 5-A: `.claude/commands/harness-apply.md` — 신규/기존 모드 자동 판별 + 병합 안내 커맨드

### Changed
- Phase 5-A: `CLAUDE_INTEGRATION_GUIDE.md` — 기존 프로젝트 병합 패턴 섹션 추가
- Phase 5-A: `base_code_plan.md` — Commands 5→6개, 사용자 가이드 문서 섹션 추가

---

## [v1.3.0] — 2026-03-09

### Added
- Phase 4-D: `.claude/commands/harness-plan-sync.md` — plan.md 분석 → task.md 자동 생성 커맨드

### Changed
- Phase 4-D: `.claude/commands/harness-docs-update.md` — Step 0 추가 (task.md → plan.md 자동 동기화)
- Phase 4-D: `base_code_plan.md` — Commands 4→5개

---

## [v1.2.0] — 2026-03-09

### Added
- Phase 4-C: `.claude/hooks/pr-review-trigger.{ts,sh}` — Bash PostToolUse 훅, `gh pr create` 감지 → exit 2 컨텍스트 주입
- Phase 4-C: `.github/workflows/pr-code-review.yaml` — PR 체크리스트 댓글 자동 작성

### Changed
- Phase 4-C: `.claude/agents/code-architecture-reviewer.md` — 범용화 (하드코딩 스택 제거)
- Phase 4-C: `.claude/settings.json` — PostToolUse Bash 매처 블록 추가

---

## [v1.1.0] — 2026-03-09

### Added
- Phase 4-A: `.claude/hooks/session-start-docs-loader.{ts,sh}` — UserPromptSubmit 훅, 세션 첫 프롬프트 시 4문서 자동 컨텍스트 주입
- Phase 4-A: `.claude/hooks/docs-update-reminder.{ts,sh}` — Stop 훅, 세션 종료 시 history.md 미갱신이면 리마인더
- Phase 4-A: `.claude/commands/harness-docs-update.md` — 4문서 업데이트 5단계 지시 커맨드

### Fixed
- Phase 4-B: `skill-rules.json` v1.3 오탐 5건 수정
  - `fastapi-backend-guidelines`: `async`, `await` 키워드 제거
  - `database-guidelines`: `migration` → `database migration`, `schema migration` 구체화
  - `skill-developer`: `hook system` → `claude hook system`
  - `pytest-backend-testing`: 일반 키워드 6개 → `python test`, `pytest mock` 등 구체화
  - `error-tracking`: `monitoring` → `error monitoring`

---

## [v1.0.0] — 2026-03-09

### Added
- Phase 1: 베이스 코드 9개 스택
  - `base/fastapi/` — FastAPI + DDD + SQLModel + JWT + S3
  - `base/nextjs/` — Next.js 15 + AuthProvider + shadcn/ui
  - `base/express/` — TypeScript + Prisma + JWT HttpOnly 쿠키
  - `base/django/` — DRF + SimpleJWT + AbstractBaseUser
  - `base/nestjs/` — Passport JWT + Repository 패턴 + E2E 11케이스
  - `base/spring-boot/` — Security JWT Filter + JPA + GlobalExceptionHandler
  - `base/react-native/` — Expo SDK 52 + Zustand v5 + SecureStore + 401 Queue
  - `base/c-embedded/` — HAL Ops Table + FreeRTOS + 정적 Ring Buffer + Unity 25테스트
  - `base/cpp-embedded/` — 순수 가상 HAL + RAII + Google Test 43테스트
- Phase 2: Skills 24개 (`skill-rules.json` v1.2 전체 등록)
  - 신규 10개: express, django, nestjs, spring-boot, react-native, embedded-c, embedded-cpp, docker, aws, database
  - 기존 14개: fastapi, nextjs, pdf, docx, pptx, mermaid, brand, ppt-brand, web-design, frontend-design, error-tracking, vercel-react, pytest, skill-developer
- Phase 3: Agents 16개
  - 플래너: planner
  - 아키텍처: code-architecture-reviewer
  - 자동화: db-migration-agent, api-spec-generator, test-coverage-agent
  - 성능/보안: performance-auditor, security-auditor
  - 기타: auto-error-resolver, statusline-setup 등
- Phase 4-A: 4문서 자동화 훅 2개, `/harness-docs-update` 커맨드
