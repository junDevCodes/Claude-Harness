# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Monorepo Structure

This is a **monorepo** containing both backend (FastAPI) and frontend (Next.js) applications:

- `backend/` - Python FastAPI backend with PostgreSQL
- `frontend/` - Next.js 15 frontend with TypeScript and Tailwind CSS

## Backend Development

### Prerequisites

- Python 3.12.3 (exact version, see `backend/pyproject.toml`)
- Docker & Docker Compose

### Setup

```bash
cd backend
uv venv
source .venv/bin/activate
uv pip install -e .
uv pip install -e .[dev]  # Install dev dependencies (black, isort, mypy, ruff)
```

### Running the Backend

```bash
# Development server (note: module is backend.main not app.main)
cd backend
uvicorn backend.main:app --reload --port 28080

# Docker Compose (production-like environment)
cd backend
docker-compose up
```

### Code Quality

```bash
cd backend
black .                           # Format code
isort . --profile black          # Sort imports
ruff check --fix .               # Lint with auto-fix
mypy .                           # Type checking
pre-commit run --all-files       # Run all pre-commit hooks
```

### Backend Architecture

**Framework:** FastAPI with async/await pattern using SQLModel + SQLAlchemy

**Database Layer:**

- **Read/Write Separation**: Separate database connections for read and write operations
- Event loop-based caching of engines and sessionmakers in `backend/db/orm.py`
- Session factories:
  - `get_write_session()` / `get_write_session_dependency()` for write operations
  - `get_read_session()` / `get_read_session_dependency()` for read operations
- PostgreSQL with asyncpg driver, SSL required for connections

**Domain-Driven Design:**

- Business logic organized in `backend/domain/{entity}/` directories
- Each domain contains: `model.py` (SQLModel), `service.py` (business logic), `repository.py` (data access)
- Domains: `user`, `auth`, `artist`, `artwork`, `admin`, `curai`, `exhibition`, `message`, `notification`, `subscription`, `shared`

**API Structure:**

- Versioned endpoints under `/api/v1/` prefix
- Routers in `backend/api/v1/routers/`: `auth.py`, `artist.py`, `artwork.py`, `admin.py`, `curai.py`, `exhibition.py`, `message.py`, `notification.py`, `health.py`
- DTOs in `backend/dtos/` for request/response validation
- Main app creation in `backend/main.py` via `create_application()`

**Configuration:**

- Settings in `backend/core/config.py` using Pydantic BaseSettings
- Environment variables required: database credentials (read/write), JWT config
- CORS configured for local development and production domains (qwarty.net)

**Deployment:**

- Docker image: `206404754787.dkr.ecr.ap-northeast-2.amazonaws.com/qwarty-backend:latest`
- Deployed to AWS ECS cluster `qwarty-backend-cluster`
- Auto-deployment on push to `main` branch via GitHub Actions

## Frontend Development

### Prerequisites

- Node.js 20+
- pnpm package manager

### Setup

```bash
cd frontend
pnpm install
```

### Running the Frontend

```bash
cd frontend
pnpm dev          # Development server with Turbopack (http://localhost:3000)
pnpm build        # Production build with Turbopack
pnpm start        # Start production server
pnpm lint         # Run ESLint
```

### Frontend Architecture

**Framework:** Next.js 15 (App Router) with React 19, TypeScript, Tailwind CSS 4

**Project Structure:**

- `src/app/` - Next.js App Router pages and API routes
  - Route groups: `/login`, `/sign-up`, `/artists`, `/artist`, `/account`, `/admin`, `/agent`, `/explore`, `/messages`, `/search`
  - API routes: `/api/upload` (S3 file upload)
- `src/components/` - Reusable React components organized by feature
- `src/lib/` - Core utilities and configurations
  - `api.ts` - API client for backend communication
  - `serverAuth.ts` - Server-side authentication utilities
  - `s3Upload.ts` - AWS S3 upload utilities with compression
  - `emailAuth.ts` - Email authentication utilities
  - `firebase.ts` - Firebase configuration
  - `theme.ts` - MUI theme configuration
- `src/hooks/` - Custom React hooks
- `src/providers/` - React context providers
- `src/utils/` - Utility functions
- `src/types/` - TypeScript type definitions
- `src/const/` - Application constants
- `src/interfaces/` - TypeScript interfaces
- `src/locales/` - Internationalization (i18n) with next-intl

**Key Technologies:**

- **Styling:** Tailwind CSS 4, MUI Material (components), Emotion (CSS-in-JS)
- **State Management:** React hooks and context providers
- **Authentication:** JWT tokens with server-side validation
- **File Upload:** AWS S3 with client-side compression
- **Internationalization:** next-intl for i18n support
- **UI Components:** MUI Material, Lucide React icons

**Configuration:**

- `next.config.ts` - Next.js config with remote image patterns (AWS S3), Turbopack enabled
- `tailwind.config.ts` - Tailwind CSS 4 configuration
- `eslint.config.mjs` - ESLint configuration with TypeScript support
- Environment variables required: API endpoint, Firebase config, AWS credentials, Kakao OAuth

## Development Workflow

### Environment Files

Backend and frontend both require `.env` files:

- `backend/.env` - Database credentials (read/write), JWT config
- `frontend/.env` - API endpoint, Firebase, AWS S3, Kakao OAuth credentials

### Git Workflow

- Main branch: `main` (protected, auto-deploys backend to AWS ECS)
- Create feature branches for development
- Backend deployment triggers automatically on push to `main` via `.github/workflows/deploy-real.yaml`
  - Builds Docker image and pushes to ECR: `206404754787.dkr.ecr.ap-northeast-2.amazonaws.com/qwarty-backend:latest`
  - Deploys to ECS service: `prod-apne2-qwarty-backend-svc` in cluster `qwarty-backend-cluster`
  - Task definition: `backend/prod-apne2-qwarty-backend-task-def.json`

### Key Design Patterns

**Backend:**

- Domain-Driven Design with clear separation of concerns
- Repository pattern for data access
- DTO pattern for API contracts
- Dependency injection via FastAPI's `Depends()`
- Async/await throughout with proper session management

**Frontend:**

- Server-side rendering (SSR) with App Router
- Client/Server component separation
- Server actions for API calls
- Image optimization with S3 upload
- Responsive design with Tailwind CSSs

### Important Notes

- **Backend module path:** Use `backend.main:app` not `app.main:app` when running uvicorn
- **Database sessions:** Always use appropriate read/write session factory from `backend/db/orm.py`
  - Use `get_write_session_dependency()` for FastAPI endpoints that modify data
  - Use `get_read_session_dependency()` for FastAPI endpoints that only read data
- **Frontend API calls:** Centralized in `src/lib/api.ts`
- **Image uploads:** S3 upload uses presigned POST URLs for direct client-side upload
  - Flow: Client → Backend (`POST /api/v1/upload/presigned-url`) → Backend generates presigned POST → Client uploads directly to S3
  - Backend: `backend/utils/s3.py` - `generate_presigned_post()` creates presigned POST with fields and conditions (max 50MB)
  - Frontend: `src/lib/s3Upload.ts` - Handles compression (browser-image-compression), presigned URL request, and S3 upload
  - Images are compressed client-side to WebP format before upload (optional, depending on function used)
  - Supports thumbnail generation: uploads both original and compressed thumbnail in parallel
- **Authentication:** JWT-based, server-side validation in `src/lib/serverAuth.ts`
- **Pre-commit hooks:** Backend uses black, isort, ruff, and other checks via `.pre-commit-config.yaml`

## Testing & Performance Monitoring

### Browser Testing with Chrome DevTools MCP

**IMPORTANT:** Always use **chrome-devtools MCP** for frontend browser testing and performance measurement. Do NOT manually start the dev server with `pnpm dev` for testing.

**Available MCP Tools:**

- `mcp__chrome-devtools__navigate_page` - Navigate to URL
- `mcp__chrome-devtools__take_snapshot` - Take page snapshot (structure)
- `mcp__chrome-devtools__take_screenshot` - Take screenshot
- `mcp__chrome-devtools__click` - Click elements
- `mcp__chrome-devtools__fill` - Fill form inputs
- `mcp__chrome-devtools__list_console_messages` - Check console errors
- `mcp__chrome-devtools__list_network_requests` - Monitor API calls
- `mcp__chrome-devtools__performance_start_trace` - Start performance recording
- `mcp__chrome-devtools__performance_stop_trace` - Stop and analyze performance

### Performance Testing Workflow

**Step 1: Start Dev Server in Background**
If chrome-devtools MCP is alreay running, kill it first.
using chrome-devtools MCP, start dev server in background

**Step 2: Run Browser Tests with chrome-devtools MCP**

```typescript
// Example test flow:
1. Navigate to page: mcp__chrome-devtools__navigate_page({ url: "http://localhost:3000/ko" })
2. Take snapshot: mcp__chrome-devtools__take_snapshot({ verbose: false })
3. Check console: mcp__chrome-devtools__list_console_messages()
4. Start performance trace: mcp__chrome-devtools__performance_start_trace({ reload: true, autoStop: true })
5. Analyze results: Review LCP, FCP, TTI, CLS metrics
6. Take screenshot: mcp__chrome-devtools__take_screenshot({ fullPage: true })
```

**Step 3: Measure Core Web Vitals**

- **LCP (Largest Contentful Paint):** Target <2000ms
- **FCP (First Contentful Paint):** Target <1000ms
- **CLS (Cumulative Layout Shift):** Target <0.1
- **TTI (Time to Interactive):** Target <2500ms
- **TBT (Total Blocking Time):** Target <300ms

### Test Documentation

**Test Plans and Reports:**

- `frontend/tests/browser/` - Browser test documentation
- `frontend/tests/browser/test-reports/` - Test execution reports
- `frontend/docs/performance-baseline.md` - Performance baseline metrics
- `frontend/docs/PERFORMANCE-DASHBOARD.md` - Performance dashboard

**Test Coverage:**

- Home page tests (8 tests)
- Artists page tests (10 tests)
- SearchBar component tests (20 tests)
- Total: 38 automated test cases

### Lighthouse CI (Automated Performance Regression Testing)

**Configuration:** `.lighthouserc.js` in frontend directory
**GitHub Actions:** `.github/workflows/lighthouse-ci.yaml`

**Manual Lighthouse CI Run:**

```bash
cd frontend
pnpm build
npx lhci autorun
```

### Example: Testing Home Page

```bash
# 1. Ensure backend is running
cd backend
uvicorn backend.main:app --reload --port 28080

# 2. Start frontend dev server
cd frontend
pnpm dev

# 3. Use chrome-devtools MCP tools to:
# - Navigate to http://localhost:3000/ko
# - Take snapshot to verify structure
# - Check console messages (expect 0 errors)
# - Start performance trace with reload
# - Review Core Web Vitals
# - Take screenshots for documentation
# - Compare with baseline (frontend/docs/performance-baseline.md)
```

## AI Agent System (Curai)

**Framework:** Pydantic AI with streaming SSE responses

**Architecture:**

- Thread-based conversations with message history persistence
- Agentic search using pre-defined DB tools

---

## Agent 작업 문서 체계

모든 Claude Agents는 작업 시 `docs/` 디렉토리의 **4문서 체계**를 기준으로 작업한다.

### 4문서 역할

| 문서 | 역할 |
|---|---|
| `docs/plan.md` | 전체 완성 계획 및 기획을 큰 단위로 관리하는 **작업 기획서** |
| `docs/task.md` | plan.md에서 할당된 **바로 다음 진행할 기능 단위** 작업 상세 실행 계획서.<br>복수 기능 동시 기재 금지. 범위 초과 시 plan.md 항목 분리 후 재작성. |
| `docs/history.md` | 전체 작업 이력을 누적 기록하는 **영구 맥락 문서**.<br>현재 task.md 작업이 왜 이루어지는지 맥락 파악용. plan/task 완료 시 요약 흡수. |
| `docs/checklist.md` | task.md 작업에서 점검할 완료 기준을 정리한 **작업 확인서** |

### task.md 범위 기준

- checklist.md 미완료 항목이 15개를 초과하면 기능 범위 과다로 판단한다
- 기능이 복수일 경우 plan.md에서 항목을 먼저 분리한 뒤 task.md를 재작성한다

### 작업 시작 시 (필수)

```
1. docs/history.md 읽기  → 이전 맥락 파악
2. docs/task.md 읽기     → 현재 작업 범위 확인
3. docs/checklist.md 읽기 → 완료 기준 확인
```

### 작업 완료 시 (필수)

```
1. docs/checklist.md 최종 검증 항목 체크
2. docs/history.md 업데이트 (완료 내용 + 다음 세션 맥락)  ← history 먼저
3. docs/task.md 완료 항목 표시
4. docs/plan.md 해당 Phase 항목 체크
⚠️ checklist 전부 완료 시: task.md는 다음 기능으로 교체, checklist.md도 동시 교체
```

### 4문서 생명주기

| 문서 | 성격 | 완료 시 처리 |
|---|---|---|
| `plan.md` | 스프린트/마일스톤 계획 | 전체 완료([x]) → history.md 요약 기록 → 새 sprint로 교체 |
| `task.md` | 현재 기능 단위 작업 | checklist 전부 [x] → history.md 요약 기록 → 다음 기능으로 교체 |
| `checklist.md` | task.md 검증용 | task.md와 동시 교체 |
| `history.md` | 영구 이력 | 초기화 없음. plan/task 완료 내용 모두 흡수. |

⚠️ task.md 또는 plan.md 교체 전 반드시 history.md에 완료 내용을 기록해야 한다.

### 문서 업데이트 시점

| 문서 | 업데이트 시점 |
|---|---|
| `plan.md` | Phase 완료 시, 새 Phase 시작 시, 큰 방향 변경 시 |
| `task.md` | 새 Task 시작 시, Task 완료 시 |
| `history.md` | 세션 종료 전, 컨텍스트 컴팩션 전, Task 완료 시 |
| `checklist.md` | Task 시작 시(기준 작성), 진행 중(체크), 완료 시(최종 검증) |

### 이 레포의 특성 (중요)

- `backend/`와 `frontend/`는 **실행 가능한 앱이 아니라 Claude가 참고하는 베이스 코드**다.
- Skills, Agents, Hooks는 새 프로젝트에 복사하여 사용하는 **자산 라이브러리**다.
- 새 기술스택 베이스 코드는 `base/[stack]/` 디렉토리에 추가한다.
- 전체 운영 계획은 `base_code_plan.md`를 참고한다.

### base/[stack]/ 작업 시 문서 규칙

`base/[stack]/` 베이스 코드를 작업할 때는 해당 스택 디렉토리 안에 독립적인 4문서를 작성한다.

```
base/[stack]/
├── docs/
│   ├── plan.md       ← 해당 스택 베이스 코드 구성 계획
│   ├── task.md       ← 세부 작업 항목 및 진행 상태
│   ├── history.md    ← 작업 맥락 및 주요 결정 사항
│   └── checklist.md  ← 완료 기준 체크리스트
```

- 루트 `docs/`는 **하네스 전체 관리용** — 베이스 코드 작업 중 수정하지 않는다.
- `base/[stack]/docs/`는 **해당 스택 작업 전용** — 세션별 독립 작업 시 충돌 없이 병렬 진행 가능.
- 작업 시작 시 `base/[stack]/docs/history.md` → `task.md` → `checklist.md` 순서로 읽는다.

### 병렬 세션 작업 규칙 (공유 파일 충돌 방지)

Phase 1-B에서 여러 스택을 병렬 세션으로 작업할 때 반드시 준수한다.

**격리 원칙:**
- 각 세션은 담당 `base/[stack]/` 디렉토리만 수정한다.
- 루트 `docs/` (plan.md, task.md, history.md, checklist.md)는 **수정 금지**.
- 루트 `CLAUDE.md`, `CLAUDE_INTEGRATION_GUIDE.md`, `base_code_plan.md`는 **수정 금지**.
- 루트 문서 일괄 업데이트는 모든 병렬 세션 완료 후 대표 세션 하나가 진행한다.

**각 세션의 책임 범위:**
```
담당 세션  →  base/[stack]/
                ├── docs/plan.md       ← 직접 작성
                ├── docs/task.md       ← 직접 작성
                ├── docs/history.md    ← 직접 작성
                ├── docs/checklist.md  ← 직접 작성
                └── ... (스택 소스 코드)
```

**충돌이 발생하는 패턴 (금지):**
- 다른 세션의 `base/[otherstack]/` 수정
- 루트 `docs/` 파일 편집
- `CLAUDE.md`, `CLAUDE_INTEGRATION_GUIDE.md` 편집

---

### Skills 병렬 세션 작업 규칙 (Phase 2-B / 2-C)

Phase 2에서 여러 스킬을 병렬 세션으로 작업할 때 반드시 준수한다.

**격리 원칙:**
- 각 세션은 담당 `.claude/skills/[skill-name]/` 디렉토리만 생성·수정한다.
- `skill-rules.json`은 **수정 금지** — 모든 병렬 세션 완료 후 Phase 2-D 일괄 세션에서만 수정.
- `.claude/skills/README.md`는 **수정 금지** — Phase 2-D 일괄 세션 전용.
- 루트 `docs/` (plan.md, task.md, history.md, checklist.md)는 **수정 금지**.
- `CLAUDE.md`, `base_code_plan.md`는 **수정 금지**.
- 품질 점검(Phase 2-C) 중 `skill-rules.json` 수정이 필요한 이슈 발견 시 → 스킬 디렉토리 내 `QUALITY_NOTES.md`에 메모만 남기고 Phase 2-D에서 일괄 반영.

**각 세션의 책임 범위:**
```
담당 세션  →  .claude/skills/[skill-name]/
                ├── SKILL.md          ← 직접 작성/수정 (500줄 이하)
                └── resources/        ← 직접 작성 (필요 시)
                    └── [topic].md
```

**Phase 2-D 일괄 세션 전용 파일 (병렬 세션 수정 금지):**
```
.claude/skills/skill-rules.json   ← 신규 스킬 트리거 규칙 추가
.claude/skills/README.md          ← 전체 스킬 목록 갱신
docs/plan.md / task.md / history.md / checklist.md
base_code_plan.md
CLAUDE.md
```

**충돌이 발생하는 패턴 (금지):**
- `skill-rules.json`에 직접 스킬 항목 추가
- 다른 세션의 `.claude/skills/[other-skill]/` 수정
- 루트 `docs/` 파일 편집

---

### Agents 병렬 세션 작업 규칙 (Phase 3-A)

Phase 3에서 여러 에이전트를 병렬 세션으로 작업할 때 반드시 준수한다.

**격리 원칙:**
- 각 세션은 담당 `.claude/agents/[agent-name].md` 파일만 생성·수정한다.
- `.claude/agents/README.md`는 **수정 금지** — 모든 병렬 세션 완료 후 Phase 3-B 일괄 세션에서만 수정.
- 루트 `docs/` (plan.md, task.md, history.md, checklist.md)는 **수정 금지**.
- `CLAUDE.md`, `base_code_plan.md`는 **수정 금지**.

**각 세션의 책임 범위:**
```
담당 세션  →  .claude/agents/[agent-name].md  ← 직접 작성/수정
```

**Phase 3-B 일괄 세션 전용 파일 (병렬 세션 수정 금지):**
```
.claude/agents/README.md          ← 전체 에이전트 목록 갱신
docs/plan.md / task.md / history.md / checklist.md
base_code_plan.md
CLAUDE.md
```

**충돌이 발생하는 패턴 (금지):**
- 다른 세션의 `.claude/agents/[other-agent].md` 수정
- `.claude/agents/README.md` 편집
- 루트 `docs/` 파일 편집

---

### 스킬 완성 기준 (Definition of Done — Phase 2-B)

`.claude/skills/[skill-name]/`에 생성된 스킬이 완성으로 인정되려면 아래 기준을 모두 충족해야 한다.

**필수 항목:**
- [ ] `SKILL.md` 존재 및 **500줄 이하**
- [ ] YAML frontmatter 포함 (`name`, `description`, `triggers` 최소 3개 필드)
- [ ] 핵심 패턴 3개 이상 + 각 패턴에 코드 예시
- [ ] 트리거 키워드 5개 이상 명시
- [ ] 안티패턴 섹션 포함
- [ ] `skill-rules.json` 등록은 Phase 2-D에서 일괄 처리 (개별 세션에서 미처리)

**선택 항목:**
- `resources/` 디렉토리 — SKILL.md 500줄 초과 시 상세 내용 분리

---

### 에이전트 완성 기준 (Definition of Done — Phase 3-A)

`.claude/agents/[agent-name].md`에 생성된 에이전트가 완성으로 인정되려면 아래 기준을 모두 충족해야 한다.

**필수 항목:**
- [ ] 역할 및 목적 명시 (1-2문장)
- [ ] 실행 단계 순서 명시 (번호 목록)
- [ ] 사용 가능 도구 목록 (Read, Write, Edit, Bash 등)
- [ ] 출력 형식 정의 (결과물 예시 포함)
- [ ] 실행 예시 1개 이상

---

### 베이스 코드 완성 기준 (Definition of Done)

`base/[stack]/`에 생성된 베이스 코드가 완성으로 인정되려면 아래 기준을 모두 충족해야 한다.

**공통 필수 항목 (전체 스택):**
- [ ] `base/[stack]/docs/plan.md` 존재 및 내용 채워져 있음
- [ ] `base/[stack]/docs/task.md` 존재 및 내용 채워져 있음
- [ ] `base/[stack]/docs/history.md` 존재 및 내용 채워져 있음
- [ ] `base/[stack]/docs/checklist.md` 존재 및 내용 채워져 있음
- [ ] `README.md` — 프로젝트 구조, 실행 방법, 커스터마이징 포인트 포함

**웹 백엔드 스택 (fastapi / express / django / nestjs / spring-boot):**
- [ ] JWT 인증 구현 (access token + refresh token)
- [ ] 기본 CRUD 예시 1개 이상 (User 엔티티 권장)
- [ ] `.env.example` — 전체 환경변수 목록 포함
- [ ] `Dockerfile` + `docker-compose.yaml` 포함
- [ ] 테스트 템플릿 1개 이상 (단위 or 통합 테스트)

**모바일 스택 (react-native):**
- [ ] JWT 인증 구현 (Secure Storage 연동)
- [ ] 기본 화면 예시 1개 이상
- [ ] `.env.example` — 전체 환경변수 목록 포함
- [ ] 테스트 템플릿 1개 이상

**임베디드 스택 (c-embedded / cpp-embedded):**
- [ ] HAL 추상화 레이어 구현 (gpio, uart 등)
- [ ] `CMakeLists.txt` 또는 `Makefile` 포함 (빌드 가능 구조)
- [ ] 테스트 템플릿 1개 이상 (Unity or Google Test)

**검증 방법:**
- 웹/모바일: `README.md`의 실행 방법을 따라 서버/앱이 기동되어야 함
- 임베디드: `cmake .. && make` 또는 `make` 명령으로 빌드 오류 없이 컴파일되어야 함
