# AI IDE Skills 설정 가이드

> **advanced-harness** 프로젝트의 AI 코딩 어시스턴트별 Skills/Rules 설정 방법
>
> Repository: https://github.com/chacha95/advanced-harness

---

## 목차

1. [개요: 4대 AI IDE 비교](#1-개요-4대-ai-ide-비교)
2. [Claude Code (Anthropic CLI)](#2-claude-code-anthropic-cli)
3. [Cursor](#3-cursor)
4. [OpenAI Codex CLI](#4-openai-codex-cli)
5. [Google Antigravity](#5-google-antigravity)
6. [이 프로젝트에 적용하기](#6-이-프로젝트에-적용하기)
7. [.gitignore 설정](#7-gitignore-설정)

---

## 1. 개요: 4대 AI IDE 비교

| 항목 | Claude Code | Cursor | Codex CLI | Antigravity |
|------|------------|--------|-----------|-------------|
| **제작사** | Anthropic | Cursor Inc. | OpenAI | Google |
| **모델** | Claude (Opus/Sonnet/Haiku) | GPT-4o / Claude 등 선택 | GPT-4o / o3 | Gemini 3 |
| **설정 파일** | `CLAUDE.md` | `.cursor/rules/*.mdc` | `AGENTS.md` | `.agent/rules/*.md` |
| **스킬/확장** | `.claude/skills/<name>/SKILL.md` | N/A (Rules로 대체) | N/A (AGENTS.md 하나로) | `.agent/skills/<name>/SKILL.md` |
| **글로벌 설정** | `~/.claude/CLAUDE.md` | Cursor Settings > Rules | `~/.codex/AGENTS.md` | `~/.gemini/GEMINI.md` |
| **파일 형식** | Markdown + `@import` | Markdown + YAML frontmatter (`.mdc`) | Plain Markdown | Markdown + YAML frontmatter |
| **경로 스코핑** | `paths:` frontmatter | `globs:` frontmatter | 디렉토리 근접성 | Workspace 단위 |
| **모노레포 지원** | 중첩 `.claude/` 자동 탐색 | 중첩 `.cursor/rules/` | 중첩 `AGENTS.md` 자동 워크 | Workspace 스코프 |
| **외부 파일 참조** | `@path/to/file` 구문 | `@file` 참조 | 불가 | 불가 |
| **로컬 오버라이드** | `CLAUDE.local.md` | N/A | `AGENTS.override.md` | N/A |

---

## 2. Claude Code (Anthropic CLI)

### 2.1 아키텍처

Claude Code는 **Memory (CLAUDE.md)** + **Skills (SKILL.md)** + **Agents** 3계층 시스템을 사용합니다.

```
우선순위 (높음 → 낮음):
┌─────────────────────────────┐
│ Enterprise Policy           │  /Library/Application Support/ClaudeCode/CLAUDE.md
├─────────────────────────────┤
│ User Global                 │  ~/.claude/CLAUDE.md, ~/.claude/rules/*.md
├─────────────────────────────┤
│ Project (팀 공유)            │  ./CLAUDE.md, ./.claude/CLAUDE.md, ./.claude/rules/*.md
├─────────────────────────────┤
│ Local Only (개인)            │  ./CLAUDE.local.md (.gitignore 자동 추가)
├─────────────────────────────┤
│ Auto Memory (AI 자동 작성)   │  ~/.claude/projects/<project>/memory/MEMORY.md
└─────────────────────────────┘
```

### 2.2 CLAUDE.md 작성법

프로젝트 루트에 `CLAUDE.md`를 생성합니다. `@import` 구문으로 외부 파일을 참조할 수 있습니다.

```markdown
# CLAUDE.md

프로젝트 개요는 @README.md 참조.

## Build & Test
- `pnpm build`로 빌드
- `pnpm test`로 테스트 실행

## 코드 컨벤션
- TypeScript strict mode 사용
- 기존 네이밍 패턴 준수

## Git
@docs/git-instructions.md
```

**Import 규칙:**
- 상대 경로로 해석 (importing 파일 기준)
- 최대 5단계 중첩 가능
- 코드 펜스 내부의 `@`는 무시됨

### 2.3 경로 스코프 Rules (.claude/rules/)

특정 파일 패턴에만 적용되는 규칙을 작성할 수 있습니다:

```markdown
<!-- .claude/rules/backend-api.md -->
---
paths:
  - "backend/api/**/*.py"
  - "backend/domain/**/*.py"
---

# Backend API Rules

- 모든 엔드포인트에 입력 검증 포함
- 표준 에러 응답 형식 사용: `{ "detail": str, "code": int }`
- FastAPI `Depends()` 로 의존성 주입
```

```markdown
<!-- .claude/rules/frontend-components.md -->
---
paths:
  - "frontend/src/components/**/*.tsx"
  - "frontend/src/app/**/*.tsx"
---

# Frontend Component Rules

- Server/Client 컴포넌트 명확히 분리
- 'use client' 디렉티브 필요시 최상단에 명시
- Tailwind CSS 4 유틸리티 클래스 우선 사용
```

### 2.4 Skills 생성법

Skills는 슬래시 명령(`/skill-name`) 또는 AI 자동 트리거로 활성화되는 기능 확장입니다.

**디렉토리 구조:**
```
.claude/skills/
├── my-skill/
│   ├── SKILL.md          # 필수: 지시사항 + YAML frontmatter
│   ├── template.md       # 선택: 템플릿
│   ├── examples/         # 선택: 예시
│   └── scripts/          # 선택: 실행 스크립트
```

**SKILL.md 형식:**
```markdown
---
name: my-skill
description: "스킬 설명. AI가 자동 활성화 판단에 사용합니다."
disable-model-invocation: false    # true면 사용자만 직접 호출 가능
user-invocable: true               # false면 /메뉴에 안 보임
allowed-tools: Read, Grep, Bash    # 승인 없이 사용 가능한 도구
context: fork                      # fork면 격리된 서브에이전트에서 실행
agent: Explore                     # 서브에이전트 타입
model: sonnet                      # 모델 오버라이드
argument-hint: "[issue-number]"    # 자동완성 힌트
---

# 스킬 지시사항

$ARGUMENTS 로 사용자 인자를 받을 수 있습니다.
$ARGUMENTS[0] 또는 $0 으로 첫 번째 인자 접근.

동적 컨텍스트 주입: !`git branch --show-current`
```

**주요 Frontmatter 필드:**

| 필드 | 필수 | 설명 |
|------|------|------|
| `name` | 아니오 | 슬래시 명령 이름 (기본값: 디렉토리명) |
| `description` | 권장 | 스킬 용도. AI 자동 활성화 판단에 사용 |
| `disable-model-invocation` | 아니오 | `true`면 AI 자동 트리거 차단 |
| `user-invocable` | 아니오 | `false`면 `/` 메뉴에서 숨김 |
| `allowed-tools` | 아니오 | 승인 없이 사용 가능한 도구 목록 |
| `context` | 아니오 | `fork`로 격리 실행 |
| `agent` | 아니오 | 서브에이전트 타입: `Explore`, `Plan`, `general-purpose` |
| `model` | 아니오 | 모델 오버라이드 |

### 2.5 Agents 설정

Agents는 `.claude/agents/` 디렉토리에 정의합니다. 복잡한 서브태스크를 자율적으로 처리합니다.

```
.claude/agents/
├── my-agent.md       # 에이전트 정의 파일
```

에이전트 파일은 Task tool의 `subagent_type` 파라미터에서 참조됩니다.

### 2.6 이 프로젝트의 현재 구성

```
advanced-harness/
├── CLAUDE.md                          # 프로젝트 메인 설정 (11KB)
├── .claude/
│   ├── settings.json                  # Claude Code 설정
│   ├── skills/                        # 15개 스킬
│   │   ├── brand-guidelines/
│   │   ├── docx/
│   │   ├── error-tracking/
│   │   ├── fastapi-backend-guidelines/
│   │   ├── frontend-design/
│   │   ├── mermaid/
│   │   ├── nextjs-frontend-guidelines/
│   │   ├── pdf/
│   │   ├── ppt-brand-guidelines/
│   │   ├── pptx/
│   │   ├── pytest-backend-testing/
│   │   ├── skill-developer/
│   │   ├── vercel-react-best-practices/
│   │   └── web-design-guidelines/
│   ├── agents/                        # 12개 에이전트
│   ├── commands/                      # 3개 커맨드
│   └── hooks/                         # Git/pre-commit 훅
```

---

## 3. Cursor

### 3.1 아키텍처

Cursor는 `.cursor/rules/` 디렉토리의 `.mdc` 파일(Markdown + YAML frontmatter)로 규칙을 관리합니다. 레거시 `.cursorrules` 파일도 지원하지만 deprecated입니다.

```
우선순위:
┌─────────────────────────────┐
│ Global Rules                │  Cursor Settings > General > Rules for AI
├─────────────────────────────┤
│ Project Rules               │  .cursor/rules/*.mdc (Git 커밋)
├─────────────────────────────┤
│ Subdirectory Rules          │  src/.cursor/rules/*.mdc (해당 폴더에만 적용)
└─────────────────────────────┘
```

### 3.2 Rules 활성화 타입

| 타입 | Frontmatter | 동작 |
|------|-------------|------|
| **Always** | `alwaysApply: true` | 모든 대화에 무조건 로드 |
| **Auto Attached** | `globs: src/**/*.ts` | 매칭 파일 참조 시 자동 로드 |
| **Agent Requested** | `description: ...` (globs 없음) | AI가 description 보고 판단 |
| **Manual** | globs 없음, `alwaysApply: false` | `@rule-name`으로 수동 호출만 |

### 3.3 .mdc 파일 형식

```markdown
---
description: FastAPI 백엔드 개발 가이드라인
globs: backend/**/*.py
alwaysApply: false
---

# FastAPI Backend Rules

## 코드 스타일
- PEP 8 준수
- 모든 public 함수에 type hints 필수
- black 기본 line length: 88

## 테스트
- pytest + fixtures 사용
- 신규 코드 80%+ 커버리지 목표

<cursor_good_example>
async def get_user(user_id: int, session: AsyncSession = Depends(get_read_session_dependency)) -> User:
    return await session.get(User, user_id)
</cursor_good_example>

<cursor_bad_example>
def getUser(id):
    return db.query(User).filter(User.id == id).first()
</cursor_bad_example>
```

**주요 Frontmatter 필드:**

| 필드 | 타입 | 설명 |
|------|------|------|
| `description` | string | AI가 규칙 적용 시점 판단에 사용 |
| `globs` | string 또는 list | Auto Attached 모드용 파일 패턴 |
| `alwaysApply` | boolean | 모든 대화에 강제 로드 |

> `globs`와 `description`은 동시에 사용하지 않는 것이 권장됩니다 (다른 매칭 전략).

### 3.4 모노레포 구성 예시

```
repo-root/
├── .cursor/
│   └── rules/
│       ├── 01-workspace.mdc         # alwaysApply: true - 전체 모노레포 규칙
│       ├── 02-architecture.mdc      # alwaysApply: true - 레포 구조 개요
│       └── 03-git.mdc              # Agent Requested - Git 컨벤션
├── backend/
│   └── .cursor/
│       └── rules/
│           ├── fastapi.mdc          # globs: **/*.py
│           └── database.mdc         # globs: **/*.sql
└── frontend/
    └── .cursor/
        └── rules/
            ├── nextjs.mdc           # globs: **/*.tsx
            └── styles.mdc           # globs: **/*.css
```

> 파일명에 번호 접두사(`01_`, `02_`)를 붙여 로드 순서를 제어할 수 있습니다.

### 3.5 외부 파일 참조

```markdown
@file ../tsconfig.json
@file ../package.json
@file docs/architecture.md
```

### 3.6 `.cursorrules`에서 마이그레이션

1. `.cursor/rules/index.mdc` 생성 (`alwaysApply: true`)
2. 기존 `.cursorrules` 내용 붙여넣기
3. `.cursorrules` 삭제
4. 주제별로 파일 분리 (권장)

---

## 4. OpenAI Codex CLI

### 4.1 아키텍처

Codex CLI는 `AGENTS.md` 파일 하나로 모든 설정을 관리합니다. Git 루트부터 현재 디렉토리까지 워크(walk)하며 발견된 모든 `AGENTS.md`를 순서대로 병합합니다.

```
탐색 순서 (위→아래, 아래가 오버라이드):
┌─────────────────────────────┐
│ Global                      │  ~/.codex/AGENTS.override.md → ~/.codex/AGENTS.md
├─────────────────────────────┤
│ Git Root                    │  <git-root>/AGENTS.override.md → AGENTS.md
├─────────────────────────────┤
│ Subdirectory Walk           │  각 디렉토리마다 override.md → AGENTS.md
├─────────────────────────────┤
│ Current Directory           │  ./AGENTS.override.md → ./AGENTS.md
└─────────────────────────────┘
```

### 4.2 AGENTS.md 작성법

Plain Markdown. YAML frontmatter 없음. 자유 형식이지만 기능적 섹션으로 구분 권장:

```markdown
# AGENTS.md

## Working Agreements
- 모든 JavaScript 파일 수정 후 `npm test` 실행
- `pnpm` 사용 (`npm` 아님)

## Repository Conventions
- PR 전 `npm run lint` 실행
- Public 유틸리티 변경 시 `docs/`에 문서화
- 시크릿 커밋 금지

## Backend Rules
- FastAPI + SQLModel 사용
- 비동기 패턴 필수 (async/await)
- 읽기/쓰기 세션 분리
```

### 4.3 핵심 규칙

- 디렉토리당 **1개 파일만** 포함
- 전체 병합 크기 제한: **32 KiB** (기본값)
- `AGENTS.override.md`가 같은 디렉토리의 `AGENTS.md`보다 우선
- 빈 파일은 무시됨

### 4.4 설정 파일 (`~/.codex/config.toml`)

```toml
# 커스텀 폴백 파일명 지정
project_doc_fallback_filenames = ["TEAM_GUIDE.md", ".agents.md"]

# 병합 크기 제한 상향
project_doc_max_bytes = 65536
```

### 4.5 모노레포 구성 예시

Codex CLI는 디렉토리 워크 방식으로 모노레포를 자연스럽게 지원합니다:

```
repo/
├── AGENTS.md                    # 레포 전체: pnpm 사용, Git 컨벤션
├── backend/
│   └── AGENTS.md                # 백엔드: pytest, FastAPI 패턴
├── frontend/
│   └── AGENTS.md                # 프론트: React Testing Library, Tailwind
└── services/
    └── payments/
        └── AGENTS.override.md   # 결제: 보안 특별 요구사항 (오버라이드)
```

### 4.6 검증 방법

```bash
# 현재 활성 지시사항 확인
codex --ask-for-approval never "Summarize current instructions"

# 특정 서브디렉토리에서 확인
codex --cd services/payments --ask-for-approval never "Show active instruction files"
```

---

## 5. Google Antigravity

### 5.1 아키텍처

Antigravity는 **Rules** (상시 적용), **Workflows** (사용자 트리거 매크로), **Skills** (AI 트리거 기능팩) 3가지 개념을 분리합니다.

```
┌─────────────────────────────┐
│ Global Rules                │  ~/.gemini/GEMINI.md
├─────────────────────────────┤
│ Global Skills               │  ~/.gemini/antigravity/skills/<name>/SKILL.md
├─────────────────────────────┤
│ Global Workflows            │  ~/.gemini/antigravity/global_workflows/<name>.md
├─────────────────────────────┤
│ Workspace Rules             │  .agent/rules/<name>.md
├─────────────────────────────┤
│ Workspace Skills            │  .agent/skills/<name>/SKILL.md
├─────────────────────────────┤
│ Workspace Workflows         │  .agent/workflows/<name>.md
└─────────────────────────────┘
```

### 5.2 개념 구분

| 개념 | 성격 | 트리거 |
|------|------|--------|
| **Rules** | 시스템 지시사항 (상시 ON) | 자동 (항상 적용) |
| **Skills** | 기능팩 (필요시 로드) | AI가 description 매칭으로 자동 활성화 |
| **Workflows** | 슬래시 명령 매크로 | 사용자가 `/workflow-name`으로 수동 호출 |

### 5.3 Rules 작성법

```markdown
<!-- .agent/rules/coding-standards.md -->
* PEP 8 스타일 가이드 준수
* 모든 함수에 docstring 작성
* 상속보다 컴포지션 우선
```

### 5.4 Skills 작성법

```markdown
<!-- .agent/skills/fastapi-guide/SKILL.md -->
---
name: fastapi-guide
description: FastAPI 백엔드 개발 시 DDD 패턴, 레포지토리 패턴, 비동기 패턴 적용 가이드
---

# FastAPI 개발 가이드

## Use this skill when
- FastAPI 라우터 생성
- SQLModel 모델 정의
- 서비스/레포지토리 레이어 작성

## Do not use this skill when
- 프론트엔드 코드 작업
- 인프라/배포 설정

## Instructions
1. Domain-Driven Design 구조 준수
2. 읽기/쓰기 세션 분리
3. Depends()로 의존성 주입
```

### 5.5 Workflows 작성법

```markdown
<!-- .agent/workflows/deploy-check.md -->
---
name: deploy-check
description: 배포 전 체크리스트 확인
---

1. `cd backend && ruff check --fix .`
2. `cd backend && mypy .`
3. `cd frontend && pnpm lint`
4. `cd frontend && pnpm build`
5. 모든 체크 통과 시 "배포 준비 완료" 출력
```

### 5.6 Skills 디렉토리 구조

```
.agent/skills/
└── my-skill/
    ├── SKILL.md          # 필수: YAML frontmatter + 지시사항
    ├── scripts/          # 선택: Python, Bash, Node 스크립트
    ├── references/       # 선택: 문서, 템플릿
    └── assets/           # 선택: 정적 파일
```

### 5.7 모노레포 참고

Antigravity는 중첩 `.agent/` 디렉토리를 공식 지원하지 않습니다. Workspace 단위로 하나의 `.agent/` 디렉토리를 사용합니다. 모노레포에서는 Skill 본문에 글로브 패턴이나 조건문을 활용하는 것이 커뮤니티 워크어라운드입니다.

### 5.8 UI에서 접근

IDE에서: `...` (우측 상단) > Customizations > Rules / Workflows / Skills

---

## 6. 이 프로젝트에 적용하기

### 6.1 Claude Code (이미 설정 완료)

이 프로젝트는 이미 Claude Code 설정이 완료되어 있습니다:

```bash
# 클론 후 바로 사용 가능
git clone https://github.com/chacha95/advanced-harness.git
cd advanced-harness
claude  # Claude Code 실행 - CLAUDE.md 자동 로드
```

**포함된 구성요소:**
- `CLAUDE.md` - 프로젝트 메인 설정 (모노레포 구조, 빌드, 아키텍처 전체 기술)
- 15개 Skills - `fastapi-backend-guidelines`, `nextjs-frontend-guidelines` 등
- 12개 Agents - `auth-route-tester`, `code-architecture-reviewer` 등
- 3개 Commands - `/dev-docs`, `/dev-docs-update`, `/route-research-for-testing`

### 6.2 Cursor 설정하기

Claude Code의 기존 설정을 Cursor 형식으로 변환합니다:

**Step 1: 디렉토리 생성**
```bash
mkdir -p .cursor/rules
mkdir -p backend/.cursor/rules
mkdir -p frontend/.cursor/rules
```

**Step 2: 루트 규칙 생성 (.cursor/rules/01-workspace.mdc)**
```markdown
---
description: advanced-harness 모노레포 전체 규칙
alwaysApply: true
---

# Monorepo Structure

- `backend/` - Python FastAPI backend with PostgreSQL
- `frontend/` - Next.js 15 frontend with TypeScript and Tailwind CSS

## Git Workflow
- Feature branch에서 작업, main 직접 커밋 금지
- main push 시 AWS ECS 자동 배포 트리거

## General Rules
- 기존 프로젝트 패턴/네이밍 준수
- TypeScript strict mode, Python type hints 필수
```

**Step 3: 백엔드 규칙 생성 (backend/.cursor/rules/fastapi.mdc)**
```markdown
---
description: FastAPI 백엔드 개발 가이드
globs: "**/*.py"
---

# FastAPI Backend Rules

## Architecture
- Domain-Driven Design: `backend/domain/{entity}/`
- 각 도메인: model.py, service.py, repository.py
- Versioned endpoints: `/api/v1/` prefix

## Database
- Read/Write 세션 분리 (`get_read_session` / `get_write_session`)
- SQLModel + asyncpg, SSL 필수
- FastAPI `Depends()` 로 의존성 주입

## Code Quality
- `black .` → `isort . --profile black` → `ruff check --fix .` → `mypy .`

<cursor_good_example>
@router.get("/users/{user_id}")
async def get_user(
    user_id: int,
    session: AsyncSession = Depends(get_read_session_dependency)
) -> UserResponse:
    repo = UserRepository(session)
    return await repo.get_by_id(user_id)
</cursor_good_example>
```

**Step 4: 프론트엔드 규칙 생성 (frontend/.cursor/rules/nextjs.mdc)**
```markdown
---
description: Next.js 15 프론트엔드 개발 가이드
globs: "**/*.{tsx,ts}"
---

# Next.js Frontend Rules

## Architecture
- App Router (src/app/) + Server/Client Components 분리
- 'use client' 필요시 명시
- API 호출은 `src/lib/api.ts` 중앙화

## Styling
- Tailwind CSS 4 유틸리티 클래스 우선
- MUI Material 컴포넌트 병행 사용

## Auth
- JWT 기반, 서버사이드 검증 (`src/lib/serverAuth.ts`)

## i18n
- next-intl 사용, `src/locales/` 에서 관리

<cursor_good_example>
'use client';
import { useTranslations } from 'next-intl';

export default function UserProfile({ userId }: { userId: string }) {
  const t = useTranslations('profile');
  return <h1 className="text-2xl font-bold">{t('title')}</h1>;
}
</cursor_good_example>
```

### 6.3 OpenAI Codex CLI 설정하기

**Step 1: 루트 AGENTS.md 생성**
```bash
# 프로젝트 루트에 생성
cat > AGENTS.md << 'EOF'
# Advanced Harness - Codex Instructions

## Monorepo Structure
- `backend/` - Python 3.12 FastAPI backend with PostgreSQL (asyncpg)
- `frontend/` - Next.js 15 (App Router), React 19, TypeScript, Tailwind CSS 4

## Working Agreements
- 기존 프로젝트 패턴과 네이밍 컨벤션 준수
- Feature branch에서 작업, main 직접 커밋 금지
- 코드 품질 도구: Backend(black, isort, ruff, mypy), Frontend(eslint)

## Backend (Python/FastAPI)
- Module path: `backend.main:app` (not `app.main:app`)
- DDD 구조: `backend/domain/{entity}/` → model.py, service.py, repository.py
- DB Read/Write 세션 분리: `get_read_session_dependency()` / `get_write_session_dependency()`
- API endpoints: `/api/v1/` prefix
- Run: `uvicorn backend.main:app --reload --port 28080`

## Frontend (Next.js 15)
- App Router + Server/Client Components 분리
- API 호출 중앙화: `src/lib/api.ts`
- JWT 인증: `src/lib/serverAuth.ts`
- S3 업로드: presigned POST URL 방식
- Run: `pnpm dev` (Turbopack)
EOF
```

**Step 2: 서브디렉토리별 AGENTS.md (선택)**
```bash
# backend/AGENTS.md
cat > backend/AGENTS.md << 'EOF'
## Backend Specific Rules
- `uv` 패키지 매니저 사용
- 모든 엔드포인트 async/await 필수
- DTOs: `backend/dtos/` 에서 request/response 검증
- Config: `backend/core/config.py` (Pydantic BaseSettings)
- pre-commit hooks: black, isort, ruff
EOF

# frontend/AGENTS.md
cat > frontend/AGENTS.md << 'EOF'
## Frontend Specific Rules
- `pnpm` 패키지 매니저 사용
- Tailwind CSS 4 + MUI Material 병행
- next-intl로 i18n 관리 (src/locales/)
- 이미지: S3 + browser-image-compression → WebP 변환
- Components: `src/components/` 기능별 구성
EOF
```

### 6.4 Google Antigravity 설정하기

**Step 1: 디렉토리 생성**
```bash
mkdir -p .agent/rules
mkdir -p .agent/skills/fastapi-guide
mkdir -p .agent/skills/nextjs-guide
mkdir -p .agent/workflows
```

**Step 2: Rules 생성 (.agent/rules/workspace.md)**
```markdown
* 이 프로젝트는 FastAPI 백엔드 + Next.js 15 프론트엔드 모노레포입니다
* backend/ 에서는 Python 3.12, FastAPI, SQLModel, asyncpg를 사용합니다
* frontend/ 에서는 Next.js 15 (App Router), React 19, TypeScript, Tailwind CSS 4를 사용합니다
* 기존 패턴과 네이밍 컨벤션을 준수하세요
* Backend module path: backend.main:app
* DB 읽기/쓰기 세션 분리 필수
* Feature branch에서 작업, main 직접 커밋 금지
```

**Step 3: Skills 생성**
```markdown
<!-- .agent/skills/fastapi-guide/SKILL.md -->
---
name: fastapi-guide
description: FastAPI 백엔드 코드 작성 시 DDD 패턴, 레포지토리 패턴, async/await 가이드
---

# FastAPI Backend Guide

## Use this skill when
- backend/ 디렉토리 내 Python 코드 작성
- API 엔드포인트, 서비스, 레포지토리 생성
- 데이터베이스 모델 정의

## Instructions
- Domain: backend/domain/{entity}/ 구조 (model.py, service.py, repository.py)
- Router: backend/api/v1/routers/ 에 위치
- DTOs: backend/dtos/ 에서 request/response 검증
- 세션: get_read_session_dependency() / get_write_session_dependency() 사용
- Config: backend/core/config.py (Pydantic BaseSettings)
```

```markdown
<!-- .agent/skills/nextjs-guide/SKILL.md -->
---
name: nextjs-guide
description: Next.js 15 프론트엔드 코드 작성 시 App Router, 컴포넌트, 스타일링 가이드
---

# Next.js Frontend Guide

## Use this skill when
- frontend/ 디렉토리 내 TypeScript/React 코드 작성
- 페이지, 컴포넌트, API 라우트 생성
- 스타일링, 인증, i18n 작업

## Instructions
- App Router: src/app/ 에 페이지 구성
- Server/Client 컴포넌트 명확히 분리
- API 호출: src/lib/api.ts 중앙화
- 인증: JWT + src/lib/serverAuth.ts
- 스타일: Tailwind CSS 4 우선, MUI 병행
- i18n: next-intl (src/locales/)
```

**Step 4: Workflows 생성 (선택)**
```markdown
<!-- .agent/workflows/quality-check.md -->
---
name: quality-check
description: 전체 코드 품질 체크 실행
---

1. `cd backend && black . && isort . --profile black && ruff check --fix . && mypy .`
2. `cd frontend && pnpm lint && pnpm build`
3. 결과 요약 출력
```

---

## 7. .gitignore 설정

각 IDE의 설정 파일 중 버전 관리할 것과 제외할 것을 구분합니다:

```gitignore
# === AI IDE 설정: 커밋 대상 ===
# Claude Code (프로젝트 설정)
# CLAUDE.md              → 커밋
# .claude/skills/        → 커밋
# .claude/agents/        → 커밋
# .claude/commands/      → 커밋

# Cursor (프로젝트 규칙)
# .cursor/rules/         → 커밋

# Codex CLI
# AGENTS.md              → 커밋

# Antigravity
# .agent/                → 커밋

# === AI IDE 설정: 제외 대상 ===
# Claude Code (개인/로컬 설정)
CLAUDE.local.md
.claude/settings.local.json

# Cursor (에디터 상태)
.cursor/*.json
!.cursor/rules/

# Codex CLI (오버라이드)
# AGENTS.override.md     → 개인 오버라이드면 제외, 팀 오버라이드면 커밋
```

프로젝트의 `.gitignore`에 추가할 내용:

```gitignore
# AI IDE - Local only
CLAUDE.local.md
.claude/settings.local.json
AGENTS.override.md
```

---

## 부록: CLAUDE.md ↔ 타 IDE 변환 치트시트

| CLAUDE.md 패턴 | Cursor 변환 | Codex 변환 | Antigravity 변환 |
|---|---|---|---|
| `CLAUDE.md` (루트) | `.cursor/rules/01-workspace.mdc` (`alwaysApply: true`) | `AGENTS.md` (루트) | `.agent/rules/workspace.md` |
| `.claude/rules/backend.md` (`paths: backend/**`) | `backend/.cursor/rules/fastapi.mdc` (`globs: **/*.py`) | `backend/AGENTS.md` | `.agent/skills/fastapi/SKILL.md` |
| `.claude/skills/X/SKILL.md` | `.cursor/rules/X.mdc` (`description:` 기반) | `AGENTS.md` 섹션으로 통합 | `.agent/skills/X/SKILL.md` |
| `CLAUDE.local.md` | Global Rules (Settings) | `~/.codex/AGENTS.md` 또는 `AGENTS.override.md` | `~/.gemini/GEMINI.md` |
| `@import` 구문 | `@file` 참조 | 지원 안 함 | 지원 안 함 |

---

## 부록: 빠른 설정 스크립트

아래 스크립트를 실행하면 4개 IDE 설정 파일을 한번에 생성합니다:

```bash
#!/bin/bash
# setup-ai-ide.sh - 모든 AI IDE 설정 파일 생성

REPO_ROOT="$(git rev-parse --show-toplevel)"

echo "=== Cursor 설정 생성 ==="
mkdir -p "$REPO_ROOT/.cursor/rules"
mkdir -p "$REPO_ROOT/backend/.cursor/rules"
mkdir -p "$REPO_ROOT/frontend/.cursor/rules"
echo "✓ .cursor/rules/ 디렉토리 생성 완료"
echo "→ .cursor/rules/*.mdc 파일을 수동으로 작성하세요 (위 가이드 참조)"

echo ""
echo "=== Codex CLI 설정 생성 ==="
if [ ! -f "$REPO_ROOT/AGENTS.md" ]; then
    echo "→ AGENTS.md 파일을 수동으로 작성하세요 (위 가이드 참조)"
else
    echo "✓ AGENTS.md 이미 존재"
fi

echo ""
echo "=== Antigravity 설정 생성 ==="
mkdir -p "$REPO_ROOT/.agent/rules"
mkdir -p "$REPO_ROOT/.agent/skills"
mkdir -p "$REPO_ROOT/.agent/workflows"
echo "✓ .agent/ 디렉토리 생성 완료"
echo "→ .agent/rules/*.md, .agent/skills/*/SKILL.md 파일을 수동으로 작성하세요"

echo ""
echo "=== Claude Code ==="
if [ -f "$REPO_ROOT/CLAUDE.md" ]; then
    echo "✓ CLAUDE.md 이미 설정 완료"
    echo "✓ .claude/skills/ $(ls "$REPO_ROOT/.claude/skills/" 2>/dev/null | wc -l | tr -d ' ')개 스킬"
else
    echo "→ CLAUDE.md 파일을 수동으로 작성하세요"
fi

echo ""
echo "=== .gitignore 업데이트 ==="
if ! grep -q "CLAUDE.local.md" "$REPO_ROOT/.gitignore" 2>/dev/null; then
    cat >> "$REPO_ROOT/.gitignore" << 'GITIGNORE'

# AI IDE - Local only
CLAUDE.local.md
.claude/settings.local.json
AGENTS.override.md
GITIGNORE
    echo "✓ .gitignore에 AI IDE 제외 항목 추가"
else
    echo "✓ .gitignore 이미 설정됨"
fi

echo ""
echo "완료! 각 IDE별 설정 파일 작성은 docs/AI-IDE-SKILLS-SETUP-GUIDE.md 참조"
```

---

*Last updated: 2026-02-18*
