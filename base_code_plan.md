# Advanced Harness — Base Code Plan

**현재 버전: v1.7.0** (2026-03-15)

## 비전

이 레포지토리는 모든 개발 프로젝트의 **공통 출발점**이다.
새 프로젝트를 시작할 때 기술 스택, Skills, Agents, Hooks을 조합하여 즉시 Claude Code 기반의 고생산성 개발 환경을 구성하는 **자산 라이브러리**로 운영한다.

---

## 자산 구조

```
advanced-harness-window/
├── .claude/
│   ├── skills/          ← Claude 지식 모듈 라이브러리 (24개)
│   ├── agents/          ← 전문 서브에이전트 라이브러리 (29개)
│   ├── commands/        ← 슬래시 커맨드 라이브러리 (11개)
│   └── hooks/           ← 자동화 훅 라이브러리
├── base/                ← 기술스택별 베이스 코드 (9개 스택)
│   ├── fastapi/         ← FastAPI + SQLModel + JWT + S3 + Docker
│   ├── nextjs/          ← Next.js 15 + App Router + JWT + shadcn/ui
│   ├── express/         ← Express.js + TypeScript + Prisma + JWT + S3 + Docker
│   ├── django/          ← Django 5 + DRF + SimpleJWT + PostgreSQL + S3 + Docker
│   ├── nestjs/          ← NestJS + TypeScript + Prisma + JWT Guards + Docker
│   ├── spring-boot/     ← Spring Boot 3 + Java 21 + Security + JPA + Docker
│   ├── react-native/    ← Expo SDK 52 + TypeScript + Expo Router + Zustand + SecureStore
│   ├── c-embedded/      ← C99 + CMake + HAL 추상화 + FreeRTOS 태스크 패턴
│   └── cpp-embedded/    ← C++17 + CMake + HAL 클래스 + RAII + Google Test
├── docs/                ← 프로젝트 관리 문서 (4문서 체계)
└── base_code_plan.md    ← 이 문서
```

---

## 현재 보유 자산 현황

### Skills (24개)

| 카테고리 | 스킬 | 트리거 방식 |
|---|---|---|
| **웹 백엔드** | `fastapi-backend-guidelines` | FastAPI, SQLModel, async |
| **웹 백엔드** | `express-backend-guidelines` | express, express.js, node backend |
| **웹 백엔드** | `django-backend-guidelines` | django, drf, SimpleJWT |
| **웹 백엔드** | `nestjs-backend-guidelines` | nestjs, nest.js, nestjs guard |
| **웹 백엔드** | `spring-boot-guidelines` | spring boot, jpa, spring security |
| **웹 백엔드** | `pytest-backend-testing` | pytest, 테스트, coverage |
| **웹 백엔드** | `error-tracking` | Sentry, 에러 핸들링 |
| **프론트엔드** | `nextjs-frontend-guidelines` | Next.js, React, App Router |
| **프론트엔드** | `vercel-react-best-practices` | React 성능, 최적화 |
| **프론트엔드** | `web-design-guidelines` | UI 리뷰, 접근성 감사 |
| **프론트엔드** | `frontend-design` | UI 컴포넌트, 페이지 제작 |
| **모바일** | `react-native-guidelines` | react native, expo, expo sdk |
| **임베디드** | `embedded-c-guidelines` | embedded c, freertos, hal gpio |
| **임베디드** | `embedded-cpp-guidelines` | embedded cpp, hal interface, raii |
| **인프라** | `docker-guidelines` | docker, dockerfile, docker-compose |
| **인프라** | `aws-guidelines` | aws, s3, ecs, ecr, iam |
| **인프라** | `database-guidelines` | database, ERD, index, migration |
| **문서/다이어그램** | `mermaid` | 다이어그램, 플로우차트 |
| **문서/다이어그램** | `pdf` | PDF 생성/편집 |
| **문서/다이어그램** | `pptx` | 프레젠테이션 제작 |
| **문서/다이어그램** | `docx` | Word 문서 제작 |
| **브랜드** | `brand-guidelines` | Anthropic 브랜드 스타일 |
| **브랜드** | `ppt-brand-guidelines` | VRL PPT 브랜드 |
| **메타** | `skill-developer` | 스킬 생성/관리 |

### Agents (29개)

| 카테고리 | 에이전트 | 역할 |
|---|---|---|
| **오케스트레이션** | `pipeline-orchestrator` | 멀티 에이전트 파이프라인 조율 마스터 오케스트레이터 *(Phase 6-A)* |
| **계획** | `planner` | 구현 계획 수립 |
| **계획** | `plan-reviewer` | 계획 검토 및 피드백 |
| **계획** | `refactor-planner` | 리팩토링 전략 수립 |
| **코드 품질** | `code-architecture-reviewer` | 아키텍처 리뷰 |
| **코드 품질** | `code-refactor-master` | 코드 개선 실행 |
| **품질 검증** | `reality-checker` | 품질 게이트 — 기본값 NEEDS WORK, 증거 기반 PASS 판정 *(Phase 6-A)* |
| **품질 검증** | `evidence-collector` | 테스트 증거 수집·정리·문서화 *(Phase 6-A)* |
| **품질 검증** | `accessibility-auditor` | WCAG 2.2 / ARIA 접근성 점검, Critical/High/Medium/Low 분류 *(Phase 6-A)* |
| **보안** | `security-auditor` | OWASP Top 10 인증/인가 취약점 점검 |
| **보안** | `security-engineer` | 보안 취약점 패치 구현, 인증·인가 보안 기능 신규 개발 *(Phase 6-B)* |
| **성능** | `performance-auditor` | Core Web Vitals + BE API 응답시간 점검, P1/P2/P3 권고 |
| **성능** | `performance-benchmarker` | API/DB/FE 기준값 측정, 부하 테스트, 회귀 감지 *(Phase 6-B)* |
| **테스트** | `test-coverage-agent` | 커버리지 측정 및 미커버 테스트 자동 작성 |
| **테스트** | `api-tester` | 실행 중인 서버 대상 HTTP API E2E 테스트 (외부 블랙박스) *(Phase 6-B)* |
| **CI/CD & DevOps** | `devops-automator` | GitHub Actions 워크플로우, Dockerfile, ECR/ECS 배포 파이프라인 생성 *(Phase 6-A)* |
| **프로토타이핑** | `rapid-prototyper` | PoC/MVP 빠른 구현 — "일단 돌아가는 것" 우선 원칙 *(Phase 6-A)* |
| **데이터베이스** | `db-migration-agent` | DB 마이그레이션 자동화 (Alembic/Prisma/Django/Flyway) |
| **문서화** | `documentation-architect` | 문서 구조 설계 |
| **문서화** | `api-spec-generator` | OpenAPI 3.0 스펙 자동 생성 |
| **문서화** | `technical-writer` | README/API 가이드/ADR/CONTRIBUTING 기술 문서 자동 생성 *(Phase 6-A)* |
| **UI/UX 설계** | `ui-designer` | 디자인 토큰 + CVA 컴포넌트 + shadcn/ui 기반 UI 구현 *(Phase 6-B)* |
| **UI/UX 설계** | `ux-researcher` | 사용자 리서치, 페르소나 정의, UX 개선 권고, ui-designer 인계 *(Phase 6-B)* |
| **제품 관리** | `product-manager` | PRD 작성, RICE/MoSCoW 우선순위, planner 인계 *(Phase 6-B)* |
| **자동화** | `auto-error-resolver` | TypeScript 컴파일 에러 자동 수정 |
| **자동화** | `frontend-error-fixer` | 프론트엔드 에러 자동 수정 |
| **인증** | `auth-route-tester` | 인증 라우트 테스트 |
| **인증** | `auth-route-debugger` | 인증 라우트 디버깅 |
| **리서치** | `web-research-specialist` | 웹 리서치 |

### Slash Commands (11개)

| 커맨드 | 역할 |
|---|---|
| `/dev-docs` | 전략적 구현 계획 + 작업 분해 |
| `/dev-docs-update` | 컨텍스트 컴팩션 전 docs/ 업데이트 |
| `/route-research-for-testing` | 편집 라우트 맵핑 + 테스트 실행 |
| `/harness-docs-update` | 이 하네스 프로젝트 전용 4문서 업데이트 |
| `/harness-plan-sync` | plan.md → 다음 미완료 Phase 탐색 → task.md + checklist.md 자동 생성 |
| `/harness-apply` | 현재 프로젝트 분석 → 신규/기존 모드 판별 → 충돌 없는 하네스 자산 적용 |
| `/harness-init` | 새 프로젝트에 하네스 자산 초기화 (스택 선택 → .claude/ + base/ + docs/ 복사) |
| `/harness-changelog` | 이번 세션 변경 자산 분석 → CHANGELOG.md 항목 자동 생성 + 버전 결정 |
| `/harness-backport` | 실 프로젝트 패턴 → 하네스 base/ + Skill 역전파 워크플로우 |
| `/quality-gate` | checklist.md DoD 항목 증거 수집 → PASS / NEEDS WORK 자동 판정 *(Phase 6-C)* |
| `/dev-qa-loop` | 구현 → quality-gate → NEEDS WORK 수정 루프 (최대 3회) *(Phase 6-C)* |

### 사용자 가이드 문서

| 문서 | 역할 |
|---|---|
| `HARNESS_USAGE_GUIDE.md` | 신규/기존 프로젝트 하네스 적용 통합 가이드 (사용자용) |
| `HARNESS_COMMANDS_REFERENCE.md` | 전체 커맨드·스크립트·훅 상세 레퍼런스 (11개 커맨드 + 훅 6종) |
| `CLAUDE_INTEGRATION_GUIDE.md` | Claude Code 하네스 통합 자동화 지침 (Claude용) |

### 베이스 코드 (9개 스택 전체 완성)

| 기술스택 | 경로 | 완성일 | 핵심 특징 |
|---|---|---|---|
| FastAPI | `base/fastapi/` | 2026-03-01 | DDD, 읽기/쓰기 DB 분리, JWT, S3 presigned POST |
| Next.js 15 | `base/nextjs/` | 2026-03-01 | App Router, SSR, HttpOnly JWT, shadcn/ui 11종 |
| Express.js | `base/express/` | 2026-03-01 | TypeScript, Prisma ORM, JWT HttpOnly 쿠키, S3 |
| Django 5 | `base/django/` | 2026-03-01 | DRF, SimpleJWT, 커스텀 User, settings/ 환경 분리 |
| NestJS | `base/nestjs/` | 2026-03-02 | Prisma, Passport JWT, @CurrentUser(), 글로벌 응답 포맷 |
| Spring Boot 3 | `base/spring-boot/` | 2026-03-02 | Java 21, Security, JWT Filter, JPA, Gradle Kotlin DSL |
| React Native | `base/react-native/` | 2026-03-02 | Expo SDK 52, Expo Router, Zustand, SecureStore, Axios |
| C Embedded | `base/c-embedded/` | 2026-03-01 | C99, HAL Ops Table, FreeRTOS 패턴, Unity 테스트 25/25 |
| C++ Embedded | `base/cpp-embedded/` | 2026-03-02 | C++17, RAII, 순수 가상 HAL, Google Test 43/43 |

---

## 새 프로젝트에 적용하는 방법

### Step 1 — 필요한 자산 선택

```
✅ 기술스택 결정 → 해당 base/ 코드 복사
✅ 필요한 Skills 확인 → skill-rules.json에서 불필요한 항목 제거
✅ 필요한 Agents 확인 → 불필요한 에이전트 파일 제거
✅ MCP 서버 확인 → .mcp.json 수정
```

### Step 2 — 프로젝트 초기화

```bash
# 1. .claude/ 디렉토리 복사 (Skills, Agents, Hooks, Commands)
cp -r advanced-harness-window/.claude ./new-project/.claude

# 2. 베이스 코드 복사 (원하는 스택 선택)
# 웹 풀스택 예시
cp -r advanced-harness-window/base/fastapi ./new-project/backend
cp -r advanced-harness-window/base/nextjs ./new-project/frontend

# 모바일 예시
cp -r advanced-harness-window/base/react-native ./new-project/mobile

# 임베디드 예시
cp -r advanced-harness-window/base/cpp-embedded ./new-project/firmware

# 3. CLAUDE.md 업데이트 (프로젝트 아키텍처 설명)
# 4. skill-rules.json 정제 (불필요한 스킬 규칙 제거)
```

### Step 3 — 4문서 체계 초기화

프로젝트 시작 시 아래 4개 문서를 `docs/`에 생성:

```
docs/plan.md      ← 전체 기획서 (큰 단위 작업 관리)
docs/task.md      ← 현재 할당된 작업 상세 계획서
docs/history.md   ← 진행된 작업 내용 + 다음 태스크 맥락
docs/checklist.md ← 현재 태스크 점검 체크리스트
```

---

## 역전파 프로세스 (실 프로젝트 → base/)

실 프로젝트 개발 중 발견한 좋은 패턴을 `base/[stack]/`에 반영하는 절차.

### 언제 역전파하는가

- 새로운 범용 유틸리티 패턴 발견 (예: 재시도 로직, 페이지네이션 헬퍼)
- 보안/성능 개선 패턴 확립 (예: 더 나은 JWT 갱신 전략)
- 실 운영에서 검증된 에러 핸들링 패턴
- 테스트 커버리지 개선 패턴

### 역전파 절차

```
1. 패턴 식별
   실 프로젝트에서 3회 이상 반복 사용되거나
   명확한 이점이 검증된 코드 패턴을 식별

2. 범용화
   비즈니스 로직 제거 → 추상화 → 범용 예시로 변환
   특정 도메인명(예: Artist, Order) → 범용명(예: User, Item)

3. base/[stack]/ 반영
   대상 스택 디렉토리에 패턴 적용
   base/[stack]/docs/history.md에 변경 이력 기록

4. 스킬/에이전트 연계 (해당 시)
   패턴이 스킬 가이드라인 수준이면 해당 스킬 문서 업데이트
   패턴이 자동화 가능하면 새 에이전트 추가 검토

5. 루트 docs/ 업데이트
   base_code_plan.md 베이스 코드 섹션 업데이트
   docs/history.md에 역전파 이력 기록
```

---

## 확장 로드맵

### Phase 2 — Skills 라이브러리 확충 (✅ 완료 — 2026-03-08)

24개 스킬 전체 완성. skill-rules.json v1.2로 전체 등록 완료.

### Phase 3 — Agents 라이브러리 확충 ✅ 완료 (29개)

Phase 3(16개) + Phase 6-A TIER 1(7개) + Phase 6-B TIER 2(6개) = 총 29개 완성.

### Phase 4 — 자동화 강화

- `taskops auto run`: plan.md → task.md → 구현 → 검증 전 과정 자동화
- CI/CD 훅 확충: PR 생성 시 자동 코드 리뷰 에이전트 실행
- 스킬 자동 매칭 정확도 개선 (skill-rules.json 정제)

### Phase 5 — 멀티 프로젝트 운영

- 프로젝트 생성 CLI 스크립트 (원하는 스택 선택 → 자산 자동 복사)
- 자산 버전 관리 체계 (changelog 자동화)
- 새 패턴 역전파 워크플로우 (실 프로젝트 → base/ 반영)

---

## 자산 관리 원칙

1. **베이스 코드는 범용성을 유지한다** — 특정 프로젝트의 비즈니스 로직을 포함하지 않는다.
2. **스킬은 독립적이어야 한다** — 각 스킬은 다른 스킬에 의존하지 않는다.
3. **500줄 룰을 지킨다** — SKILL.md는 500줄 이하, 상세 내용은 resources/에 분리.
4. **베이스 코드에 변경이 생기면 이 레포에 반영한다** — 새 프로젝트에서 좋은 패턴을 발견하면 base/로 역전파한다.
5. **4문서 체계를 모든 작업에 적용한다** — Claude Agents는 항상 docs/ 4문서를 기준으로 작업한다.
6. **테스트 코드를 자산에 포함한다** — 베이스 코드에는 테스트 템플릿도 포함한다.

---

## 자산 버전 관리

- 이 레포는 `main` 브랜치가 항상 **안정된 자산 상태**를 유지한다.
- 새 스킬/에이전트/베이스 코드 추가 시 feature 브랜치에서 작업 후 PR로 병합한다.
- 각 자산에는 최초 작성일과 마지막 업데이트 날짜를 주석으로 명시한다.

---

*Last updated: 2026-03-15 (v1.7.0 — 병렬 세션 분석 기능 추가, 4문서 축약 정리)*
