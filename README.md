# Advanced Harness

Claude Code 기반 개발 프로젝트를 위한 **공통 자산 라이브러리**.
Skills, Agents, Hooks, Commands, Base Code를 새 프로젝트에 복사하여 즉시 고생산성 AI 지원 환경을 구성할 수 있습니다.

> **사용 가이드:** `HARNESS_USAGE_GUIDE.md` | **커맨드 레퍼런스:** `HARNESS_COMMANDS_REFERENCE.md`

---

## 자산 현황

| 자산 유형 | 수량 | 비고 |
|---|---|---|
| **Skills** | 30개 | `skill-rules.json` v1.6 전원 등록 |
| **Agents** | 33개 | model tier 전수 명시 (opus 11 / sonnet 17 / haiku 5) |
| **Slash Commands** | 11개 | `/quality-gate`, `/dev-qa-loop` 포함 |
| **Hooks** | 3종 (8 스크립트) | 4문서 자동화 2 + CI/CD 1 + 보조 5 |
| **Base Stacks** | 9개 | FastAPI / Next.js / Express / Django / NestJS / Spring Boot / React Native / C / C++ |

---

## 빠른 시작

> **핵심:** 하네스 레포는 별도 경로에 두고, 이 디렉토리에서 Claude Code를 실행합니다.
> 프로젝트 안에 넣는 것이 아닙니다.

### 3단계 워크플로우

```bash
# Step 1. 하네스 레포에서 Claude Code 실행
cd /path/to/advanced-harness-window
claude

# Step 2. 대상 프로젝트에 적용 (신규/기존 자동 판별)
/harness-apply /path/to/my-project

# Step 3. 적용 완료 후 대상 프로젝트에서 개발 시작
cd /path/to/my-project
claude
```

### 신규 프로젝트 (스크립트로 한 번에)

```bash
bash /path/to/advanced-harness-window/scripts/harness-init.sh
```

---

## 구조

```
advanced-harness-window/
├── .claude/
│   ├── skills/        ← 30개 스킬 (스택별 가이드라인 + 데이터/AI)
│   ├── agents/        ← 33개 에이전트 (model tier 3-tier 명시)
│   ├── commands/      ← 11개 슬래시 커맨드
│   ├── hooks/         ← 8 자동화 훅 스크립트 (3종 분류)
│   └── settings.json  ← 훅 등록 설정
├── base/              ← 9개 스택 베이스 코드
│   ├── fastapi/       ← Python FastAPI + SQLModel + JWT + S3
│   ├── nextjs/        ← Next.js 15 + App Router + JWT + shadcn/ui
│   ├── express/       ← Express.js + TypeScript + Prisma + JWT
│   ├── django/        ← Django 5 + DRF + SimpleJWT + PostgreSQL
│   ├── nestjs/        ← NestJS + Prisma + JWT Guards
│   ├── spring-boot/   ← Spring Boot 3 + Java 21 + Security + JPA
│   ├── react-native/  ← Expo SDK 52 + Zustand + SecureStore
│   ├── c-embedded/    ← C99 + CMake + HAL + FreeRTOS
│   └── cpp-embedded/  ← C++17 + CMake + HAL 클래스 + Google Test
├── docs/              ← 하네스 4문서 체계 (plan/task/history/checklist)
├── scripts/
│   └── harness-init.sh           ← 신규 프로젝트 초기화 스크립트
├── HARNESS_USAGE_GUIDE.md        ← 신규/기존 프로젝트 적용 가이드
├── HARNESS_COMMANDS_REFERENCE.md ← 커맨드 + 훅 전체 레퍼런스
├── CHANGELOG.md                  ← 자산 버전 이력 (현재 v1.9.1)
└── base_code_plan.md             ← 전체 자산 운영 계획
```

---

## Skills (30개)

| 카테고리 | 스킬 |
|---|---|
| 웹 백엔드 | `fastapi-backend-guidelines`, `express-backend-guidelines`, `django-backend-guidelines`, `nestjs-backend-guidelines`, `spring-boot-guidelines` |
| 백엔드 테스팅 | `pytest-backend-testing` |
| 프론트엔드 | `nextjs-frontend-guidelines`, `react-vite-pwa-guidelines`, `threejs-realtime-guidelines`, `vercel-react-best-practices`, `frontend-design`, `web-design-guidelines` |
| 모바일 | `react-native-guidelines` |
| 임베디드 | `embedded-c-guidelines`, `embedded-cpp-guidelines` |
| 인프라 | `docker-guidelines`, `aws-guidelines`, `database-guidelines` |
| 문서/도구 | `pdf`, `docx`, `pptx`, `mermaid`, `brand-guidelines`, `ppt-brand-guidelines` |
| 관측 | `error-tracking` |
| 데이터/AI | `data-analysis-guidelines`, `data-pipeline-guidelines`, `ml-training-guidelines`, `ml-evaluation-guidelines` |
| 메타 | `skill-developer` |

---

## Agents (33개) — Model Tier 전수 명시

| 카테고리 | 에이전트 (tier) |
|---|---|
| 오케스트레이션 | `pipeline-orchestrator` (opus) |
| 계획 | `planner` (opus), `plan-reviewer` (opus), `refactor-planner` (opus) |
| 코드 품질 | `code-architecture-reviewer` (opus), `code-refactor-master` (opus) |
| 품질 검증 | `reality-checker` (opus), `evidence-collector` (sonnet), `accessibility-auditor` (sonnet) |
| 보안 | `security-auditor` (opus), `security-engineer` (opus) |
| 성능 | `performance-auditor` (sonnet), `performance-benchmarker` (sonnet) |
| 테스트 | `test-coverage-agent` (sonnet), `api-tester` (sonnet) |
| CI/CD | `devops-automator` (sonnet) |
| 프로토타이핑 | `rapid-prototyper` (sonnet) |
| 데이터베이스 | `db-migration-agent` (sonnet) |
| 문서화 | `documentation-architect` (sonnet), `api-spec-generator` (sonnet), `technical-writer` (sonnet) |
| UI/UX | `ui-designer` (sonnet), `ux-researcher` (sonnet) |
| 제품 관리 | `product-manager` (sonnet) |
| 자동화 | `auto-error-resolver` (haiku), `frontend-error-fixer` (haiku) |
| 인증 | `auth-route-tester` (haiku), `auth-route-debugger` (haiku) |
| 리서치 | `web-research-specialist` (haiku) |
| 데이터/AI | `data-analyst` (sonnet), `data-pipeline-architect` (sonnet), `ml-engineer` (opus), `ml-evaluator` (opus) |

> Model tier 배정 근거 및 Extended Thinking / 병렬 tool calls 원칙은 `.claude/agents/README.md` **Opus 4.7 Operating Principles** 섹션 참고.

---

## Slash Commands (11개)

| 커맨드 | 용도 |
|---|---|
| `/harness-init` | 신규 프로젝트 초기화 (Claude 세션 내) |
| `/harness-apply` | 신규/기존 프로젝트 자동 판별 → 하네스 적용 |
| `/harness-docs-update` | 4문서 체계 업데이트 |
| `/harness-plan-sync` | plan.md → 다음 Phase task.md 자동 생성 (병렬 세션 전략 포함) |
| `/harness-changelog` | 변경 자산 분석 → CHANGELOG.md 항목 생성 |
| `/harness-backport` | 실 프로젝트 패턴 → 하네스 역전파 |
| `/quality-gate` | `docs/checklist.md` DoD 증거 기반 PASS / NEEDS WORK 자동 판정 |
| `/dev-qa-loop` | 구현 → quality-gate → NEEDS WORK 수정 사이클 (최대 3회) |
| `/dev-docs` | 전략적 구현 계획 + 작업 분해 생성 |
| `/dev-docs-update` | 컨텍스트 컴팩션 전 docs/ 업데이트 |
| `/route-research-for-testing` | 편집된 라우트 감지 → 스모크 테스트 |

---

## Hooks (3종, 8 스크립트)

| 분류 | 훅 스크립트 | 역할 |
|---|---|---|
| **4문서 자동화** | `session-start-docs-loader.{sh,ts}` | UserPromptSubmit — 세션 시작 시 4문서 로드 지시 |
| **4문서 자동화** | `docs-update-reminder.{sh,ts}` | Stop — 4문서 업데이트 알림 |
| **CI/CD 연동** | `pr-review-trigger.{sh,ts}` | Stop — PR 생성 감지 시 코드 리뷰 트리거 |
| (보조) | `skill-activation-prompt.{sh,ts}` | UserPromptSubmit — 프롬프트 기반 스킬 활성화 |
| (보조) | `error-handling-reminder.{sh,ts}` | Stop — 에러 핸들링 알림 |
| (보조) | `post-tool-use-tracker.sh` | PostToolUse — 편집 파일 추적 |
| (보조) | `tsc-check.sh` | PostToolUse — TypeScript 컴파일 체크 |
| (보조) | `trigger-build-resolver.sh` | Stop — 빌드 트리거 |

> 훅 설정·디버깅은 `.claude/hooks/README.md` / `.claude/hooks/CONFIG.md` 참고.

---

## 현재 적용 중인 프로젝트

`backend/`, `frontend/`는 하네스가 적용된 실제 서비스 코드입니다 (참고용).
새 프로젝트 시작 시에는 `base/[stack]/`을 사용하세요.
