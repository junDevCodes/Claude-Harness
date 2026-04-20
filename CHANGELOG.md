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

## [v1.9.0] — 2026-04-20

### Added
- **Phase 14 Opus 4.7 자산 최적화** — 전 에이전트·스킬을 Opus 4.7 특성에 맞춰 정밀 최적화.
- `docs/phase-14-diagnosis.md` — Phase 14-A 진단 리포트 신규 생성 (자산 현황 갭 분석 + Model Tier 매트릭스 + 실행 계획).
- `.claude/agents/README.md` — **Opus 4.7 Operating Principles** 섹션 신규 추가 (Model Tier 배정표, Extended Thinking 활용 지침, 병렬 tool calls 원칙, 출력 형식 표준).

### Changed
- **Agents 33개에 `model:` 필드 전수 명시** (이전: 명시 6 / 미명시 27):
  - `opus` (11): pipeline-orchestrator, reality-checker, security-auditor, security-engineer, code-architecture-reviewer(sonnet→opus 승격), code-refactor-master, planner, plan-reviewer, refactor-planner, ml-evaluator, ml-engineer
  - `sonnet` (17): data-analyst, data-pipeline-architect, product-manager, ux-researcher, ui-designer, devops-automator, rapid-prototyper, api-spec-generator, api-tester, test-coverage-agent, accessibility-auditor, performance-auditor, performance-benchmarker, db-migration-agent, technical-writer, documentation-architect(inherit→sonnet), evidence-collector
  - `haiku` (5): auto-error-resolver, frontend-error-fixer, auth-route-debugger, auth-route-tester(sonnet→haiku 강등), web-research-specialist(sonnet→haiku 강등)
- **Agents tools YAGNI 3건**: `plan-reviewer`(WebSearch/WebFetch 제거), `frontend-error-fixer`(Grep 추가), `auto-error-resolver`(Glob/Grep 추가).
- **Skills 5개 description 간결화** (라우터 매칭 정확도 향상): `docx`(904자→280자), `pdf`(500자→85자), `skill-developer`(500자→280자), `embedded-c-guidelines`(500자→290자), `spring-boot-guidelines`(500자→180자).
- `.claude/skills/skill-rules.json` — v1.5 → **v1.6** (구조 안정화, enforcement=block은 Next.js 1개 유지).
- `base_code_plan.md` — 버전 v1.8.1 → **v1.9.0**, Last updated 갱신.

### Decided (토의 결과)
- **Model Tier 배정 기준**: 3-tier 전수 명시 (비용·속도·정확도 최적 매칭).
- **500줄 룰**: 엄격 유지 + 포화 스킬은 `resources/` 분리 (Anthropic progressive disclosure 원칙).
- **Block Enforcement 범위**: 현행 유지 (Next.js 1개). 다른 영역 오탐률 실측 결과 block 필요성 없음.

### Deferred (Phase 14-B2 — 후속 세션)
- 500줄 포화 4개 스킬 `resources/` 분리: `react-native-guidelines`, `pptx`, `express-backend-guidelines`, `django-backend-guidelines`.
- 이유: 리팩토링 공수 대비 당면 이익 제한적. `/harness-plan-sync` 재가동 시 병렬 세션으로 처리.

### Context
- 발단: 사용자 "opus 4.7 기준으로 스킬 및 에이전트 최적화 하려고 하는데, 계획 한번 세워줘" (2026-04-20).
- 진단: Phase 14-A에서 자산 현황 갭 분석 → Model Tier 매트릭스 확정.
- 실행: 이 세션에서 14-A/B/C/D/E를 단일 흐름으로 완료.

---

## [v1.8.1] — 2026-04-20

### Fixed
- 정합성 동기화 패치 — Phase 13 commit (5cada71)에서 일부 메타 문서 갱신 누락된 항목 일괄 동기화.
- `base_code_plan.md`: 버전 표기 v1.7.0 → v1.8.0, "Skills (26개)" → "(30개)", "Agents (29개)" → "(33개)" 라벨 수정 + 표에 데이터/AI 4+4행 추가, Phase 3 완료 표기 (29개) → (33개), Last updated 갱신.
- `.claude/agents/README.md`: "Agent Quick Reference (29)" → "(33)" + 표에 데이터/AI 4행 추가 (data-analyst, data-pipeline-architect, ml-engineer, ml-evaluator).
- `docs/history.md`: 자산 현황 표 (26→30, 29→33, v1.4→v1.5, v1.7.0→v1.8.0), 현재 상태 요약 갱신, Phase 13 완료 섹션 + 정합성 동기화 섹션 추가.
- `docs/plan.md`: Phase 12-B 체크박스 표시, Phase 13 섹션 신설, 자산 현황 대시보드 갱신.
- `docs/task.md` / `docs/checklist.md`: Phase 12-B 진행 중 → Phase 13 완료 + 정합성 동기화 완료 → 다음 작업 대기 상태로 교체.

### Context
- 발견 시점: 2026-04-20 사용자가 "현재 보유중인 스킬 및 에이전트 상세 재검증" 요청 시 메타 문서 정합성 점검 중 발견.
- 자산 본체(SKILL.md 30개, agent .md 33개, skill-rules.json v1.5)는 모두 DoD PASS — 메타 카운트만 동기화.

---

## [v1.8.0] — 2026-04-16

### Added
- Phase 13 (데이터/AI 트랙): 신규 4개 스킬 추가 — `data-analysis-guidelines` (pandas/polars EDA, 결측/이상치, 기술통계, 상관 분석, 시각화), `data-pipeline-guidelines` (Batch/Stream 선택, Airflow/Prefect/Dagster 비교, 메달리온 Bronze/Silver/Gold, Great Expectations, idempotency, feature store), `ml-training-guidelines` (sklearn/LightGBM/PyTorch 단계별 매트릭스, MLflow 실험 추적, Optuna 튜닝, GroupKFold 세션 누수 방지, 재현성·Model Card), `ml-evaluation-guidelines` (분류/회귀/시계열 지표, holdout vs CV 전략, threshold 최적화, McNemar/5x2 CV 유의성 검정, baseline 회귀 감지, calibration).
- Phase 13: 신규 4개 에이전트 추가 — `data-analyst` (EDA 수행 → 인사이트 리포트 생성), `data-pipeline-architect` (Mermaid 아키텍처 + DAG 스캐폴드 + GE suite), `ml-engineer` (학습 스크립트 + MLflow + FastAPI 추론 엔드포인트 + Model Card), `ml-evaluator` (holdout 평가 + 누수 검증 + baseline 회귀 감지 + 통계 유의성 검정).
- Phase 13: `skill-rules.json` v1.5 — 4개 신규 스킬 트리거 규칙 등록 (pathPatterns 범용화: `ml/**`, `backend/pipelines/**`, `notebooks/**`, `base/ml/**`). 전체 등록 스킬: 26→30개.
- Phase 13: `.claude/agents/README.md` — "📊 데이터 / AI" 카테고리 섹션 신규 추가 (4개 에이전트 상세 설명 포함). Available Agents 29→33개.
- Phase 13: `.claude/skills/README.md` — "데이터 / AI 스킬 (4개)" 카테고리 섹션 신규 추가. Available Skills 26→30개.

### Changed
- Phase 13: `README.md`, `HARNESS_USAGE_GUIDE.md`, `base_code_plan.md` — 자산 수 업데이트 (스킬 26→30개, 에이전트 29→33개).

### Context
- 출처: HERE:O (S14P31A206) 프로젝트에서 CSI 신호처리 → 데이터 파이프라인 → ML 모델 학습 → 평가 전주기 필요성 발견. 5개 스킬 + 4개 에이전트 중 범용 가능한 4+4를 하네스로 역전파.
- 프로젝트 전용 `csi-signal-processing-guidelines`는 S14P31A206에만 유지 (WiFi CSI 특화 - 범용성 낮음).

---

## [v1.7.0] — 2026-03-15

### Added
- Phase 12-A: `/harness-plan-sync` — Step 4.5 "병렬 세션 분석" 로직 신규 삽입. Phase 내 `[ ]` 항목들의 수정 범위를 분석하여 독립/순차/일괄 세션 자동 그룹핑.
- Phase 12-A: `/harness-plan-sync` — task.md 출력 템플릿에 "세션 전략" 섹션 추가. 병렬 세션 테이블(세션 색상 + 담당 항목 + 수정 범위 + 격리 규칙) 자동 생성.
- Phase 12-A: `/harness-plan-sync` — 단일 세션 판정 기준 추가. 항목 3개 이하 / 동일 디렉토리 / 순차 의존성 시 병렬 생략.
- Phase 12-A: `planner.md` — Step 4-1 "Parallel Session Analysis" 선택 단계 추가. 격리 경계 그룹핑 + 일괄 세션 마지막 배치 원칙.

### Changed
- Phase 12-A: 4문서 축약 정리 — `docs/history.md` 1000줄→175줄 압축, `docs/plan.md` 완료 Phase 축약 테이블화.

---

## [v1.6.2] — 2026-03-11

### Fixed
- Phase 10-A: `post-tool-use-tracker.sh` — `jq` 의존성 → `node` 기반 JSON 파싱으로 교체. Windows/MINGW64 환경에서 `jq` 미설치 시 `tool_name`/`file_path`/`session_id` 모두 빈 값 → 파일 추적 전체 미작동 → `error-handling-reminder` 기능 무력화 결함 수정. `MultiEdit` 도구의 `edits[0].file_path` 배열 구조 분기 추가.

### Changed
- Phase 10-B: `error-handling-reminder.sh` — `|| exit 0` 추가. tsx 실패 시 Stop 훅 non-zero 반환 방지 (다른 sh 래퍼 패턴과 통일).
- Phase 10-B: `docs-update-reminder.sh` — 동일 `|| exit 0` 패턴 적용.
- Phase 10-B: `planner.md` — Step 4의 구식 3-파일 템플릿(`[task-name]-plan.md`, `[task-name]-context.md`, `[task-name]-tasks.md`) 섹션 전체 제거. 4문서 체계(`docs/plan.md`, `docs/task.md`)와 충돌 해소.
- Phase 10-B: `.claude/settings.json` — `permissions.allow`에서 `"Bash:*"` 제거. `harness-init.sh`로 신규 프로젝트 복사 시 모든 bash 명령 자동 승인되는 보안 리스크 해소.
- Phase 10-C: `.claude/settings.json` — `enabledMcpjsonServers`에서 `mysql`, `sequential-thinking` 제거 (`.mcp.json` 미정의 서버). `playwright`만 유지하여 목록 일치.
- Phase 10-C: `.mcp.json` — `.gitignore`에 추가 (빈 API 키 git 추적 → 실키 커밋 위험 방지). `.mcp.json.example` 템플릿 신규 생성.
- Phase 10-C: `.github/workflows/pr-code-review.yaml` — `tj-actions/changed-files@v44` → SHA 커밋 핀 (`090894a3c32fd56f3e9a9b85c2d1ab17da84d6c6`). 공급망 공격 이력 대응.
- Phase 10-C: `.github/workflows/pr-code-review.yaml` — 워크플로우 레벨 `permissions: read-all` 추가.
- Phase 10-C: `session-start-docs-loader.ts` — task.md 섹션 검증 regex `/^##\s*(작업\s*개요|Task|Phase|개요)/m` → `/^##\s+\S/m` 완화. 한국어 Phase명 오탐 해소.
- Phase 10-C: `error-handling-reminder.ts` — `line.split(':')` → `indexOf`/`lastIndexOf`/`slice` 기반 파싱으로 교체. Windows 절대 경로(`D:/workspace/...`) 포함 시 `filePath` 잘못 분리되던 문제 수정.
- Phase 10-C: `skill-activation-prompt.ts` — `new RegExp(pattern, 'i')` → 모듈 레벨 `regexCache` Map + `getRegex()` 헬퍼로 교체. `intentPatterns` 매 프롬프트마다 정규식 재컴파일 방지.
- Phase 10-C: `tsc-check.sh` — while 루프 내 `cd "$repo_path"` 단독 사용 → `$(cd "$repo_path" && ...)` 서브쉘 패턴으로 교체. 루프 내 작업 디렉토리 부작용 격리.

---

## [v1.6.1] — 2026-03-11

### Fixed
- Phase 8-A: `base_code_plan.md` — 버전 표기 `v1.5.0` → `v1.6.0` 수정 (CHANGELOG 최신 버전과 불일치)
- Phase 8-A: `tsc-check.sh` — Stop 훅 → PostToolUse(`Edit|MultiEdit|Write`) 훅 이동. `tool_name` 없는 Stop 페이로드로 TSC 체크 전체 무효화되던 근본 결함 수정. 하드코딩 서비스 목록 제거 → `tsconfig.json` 동적 탐지. `eval` 제거 → `npx tsc --noEmit` 직접 호출.
- Phase 8-A: `trigger-build-resolver.sh` — 전면 재작성. 매 Stop마다 `/tmp/claude-hook-debug.log` 덤프하던 디버그 코드 제거. 하드코딩 서비스 배열 제거. `claude --agent` 잘못된 CLI 제거. `git status --porcelain` 기반 범용 TS/JS 변경 감지로 교체.
- Phase 8-A: `session-start-docs-loader.ts` — catch 블록 `process.exit(1)` → `process.exit(0)`. UserPromptSubmit 실패 시 Claude 프롬프트 차단 방지.
- Phase 8-B: `session-start-docs-loader.sh` — `set -e` 제거 + `|| exit 0` 추가. npx/tsx 실패 시 UserPromptSubmit 차단 방지.
- Phase 8-B: `skill-activation-prompt.sh` — 동일 패턴 적용 (`set -e` 제거 + `|| exit 0`).
- Phase 8-B: `post-tool-use-tracker.sh` — `set -e` 제거. 보조 명령 실패 시 PostToolUse 훅 비정상 종료 방지.
- Phase 8-B: `error-handling-reminder.ts` — 버그 4개 수정: ① 경로 `$HOME/.claude/` → `$CLAUDE_PROJECT_DIR/.claude/` ② `split('\t')` → `split(':')` 파싱 포맷 불일치 ③ `file.path` → `file.filePath` 필드명 불일치 ④ `async function main()` → `function main()`.
- Phase 8-C: 에이전트 10개 YAML frontmatter `tools:` 필드 추가 (`auth-route-debugger`, `auth-route-tester`, `code-architecture-reviewer`, `code-refactor-master`, `documentation-architect`, `frontend-error-fixer`, `plan-reviewer`, `planner`, `refactor-planner`, `web-research-specialist`). CLAUDE.md 에이전트 DoD 필수 항목 충족.
- Phase 8-C: `planner.md` — `dev/active/` 경로 참조 5곳 → `docs/plan.md` + `docs/task.md` 전면 교체 (하네스 4문서 체계 정렬).
- Phase 8-C: `auth-route-debugger.md` — 크리덴셜 하드코딩 범용화: `testuser`/`testpassword` → `$TEST_USER`/`$TEST_PASSWORD`, `yourRealm` → `[your-realm]`, `your-app-client` → `[your-client-id]`, PM2 서비스명 범용화.
- Phase 8-D: `skill-activation-prompt.ts` — `process.exit(1)` 2곳 → `process.exit(0)`. UserPromptSubmit 훅 catch에서 프롬프트 차단 위험 제거.
- Phase 8-E: `tsc-check.sh` — `jq` 의존성 → `node` 기반 JSON 파싱으로 교체. Windows/MINGW64 환경에서 `jq` 미설치 시 TSC 체크 전혀 실행 안 되던 결함 수정.

### Changed
- Phase 8-B: `error-handling-reminder.ts` + `.sh` — 4개 버그 수정 완료 후 Stop 훅 신규 등록 (순서: `trigger-build-resolver.sh` → `error-handling-reminder.sh` → `docs-update-reminder.sh`).
- Phase 8-B: `stop-build-check-enhanced.sh` — 삭제. 4중 결함(구 tsc-cache 의존 + eval + set -e + tsc-check.sh 기능 중복) 확인 후 제거.
- Phase 8-B: `session-start-docs-loader.ts` — TTL 7일 자동 정리 로직 추가. `.session-cache/*.loaded` 파일 7일 초과 시 자동 삭제.
- Phase 8-B: `.mjs` 파일 4개 삭제 (`post-tool-use-tracker.mjs`, `skill-activation-prompt.mjs`, `trigger-build-resolver.mjs`, `tsc-check.mjs`). settings.json 미참조 dead code 정리.
- Phase 8-D: `docs-update-reminder.ts`, `pr-review-trigger.ts` — `async function main()` → `function main()`, `main().catch(...)` → `main()`. await 없는 불필요한 async 제거.
- Phase 8-D: `.claude/hooks/pnpm-lock.yaml` — 삭제. `package-lock.json`과의 이중 lock 파일 정리.
- Phase 8-D: `.claude/settings.json` — `enableAllProjectMcpServers: true` → `false`. `enabledMcpjsonServers` 명시 목록(mysql/sequential-thinking/playwright)과의 모순 해소.
- Phase 8-D: `.github/workflows/pr-code-review.yaml` — `createComment` 단독 → `listComments` + botComment 탐색 + `updateComment`/`createComment` 분기. PR synchronize 시 중복 댓글 누적 방지.

---

## [v1.6.0] — 2026-03-10

### Added
- Phase 6-A (TIER 1 에이전트 7개):
  - `.claude/agents/pipeline-orchestrator.md` — 멀티 에이전트 파이프라인 조율 마스터 오케스트레이터 (5단계 DAG)
  - `.claude/agents/reality-checker.md` — 품질 게이트 (기본값 NEEDS WORK, 증거 기반 PASS 판정, DoD 1:1 연동)
  - `.claude/agents/accessibility-auditor.md` — WCAG 2.2 / ARIA 접근성 점검, Critical/High/Medium/Low 심각도 분류
  - `.claude/agents/evidence-collector.md` — 테스트 증거(로그/빌드/API응답/스크린샷) 수집·정리·문서화
  - `.claude/agents/devops-automator.md` — GitHub Actions 워크플로우, Dockerfile 최적화, ECR/ECS 배포 파이프라인 자동 생성
  - `.claude/agents/rapid-prototyper.md` — PoC/MVP 빠른 구현 ("일단 돌아가는 것" 우선, code-refactor-master 인계)
  - `.claude/agents/technical-writer.md` — README/API 가이드/ADR/CONTRIBUTING 기술 문서 자동 생성
- Phase 6-B (TIER 2 에이전트 6개):
  - `.claude/agents/security-engineer.md` — security-auditor 진단 기반 보안 코드 구현 및 패치 (진단/구현 역할 분리)
  - `.claude/agents/performance-benchmarker.md` — API/DB/FE 기준값 측정, Locust 부하 테스트, 회귀 감지
  - `.claude/agents/api-tester.md` — 실행 중인 서버 대상 HTTP API E2E 테스트 (외부 블랙박스 관점, Newman/Pact)
  - `.claude/agents/ui-designer.md` — 디자인 토큰 3계층 + CVA 컴포넌트 + Storybook + shadcn/ui 기반 UI 구현
  - `.claude/agents/ux-researcher.md` — 사용자 리서치, 페르소나 정의, As-Is/To-Be 흐름 분석, ui-designer 인계
  - `.claude/agents/product-manager.md` — PRD 작성, RICE/MoSCoW 우선순위 결정, OKR 연계, planner 인계
- Phase 6-C (커맨드 2개):
  - `.claude/commands/quality-gate.md` — checklist.md DoD 항목 기반 증거 수집(Glob/Grep/Bash) → PASS/NEEDS WORK 자동 판정 (Pessimistic Default)
  - `.claude/commands/dev-qa-loop.md` — 구현 → quality-gate → NEEDS WORK 수정 루프 (최대 3회, [Loop N/3] 카운터, 에스컬레이션)
- Phase 6-C: `docs/history.md` — 에이전트 핸드오프 포맷 가이드 섹션 추가 (6개 필드 템플릿 + ux-researcher → ui-designer 예시)

### Changed
- Phase 6-D: `.claude/agents/README.md` — Available Agents 16→29개, 16개 카테고리로 전면 재편 (오케스트레이션/품질검증/UI/UX설계/제품관리 신규 카테고리 포함)
- Phase 6-D: `base_code_plan.md` — Agents 섹션 16→29개 갱신, Commands 섹션 9→11개 갱신, 확장 로드맵 Phase 3 완료 처리

### Removed
- Phase 6-D: `agency-agents-main/` — agency-agents 분석 완료 후 삭제 (흡수한 에이전트 13개 전원 .claude/agents/에 완성)

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
