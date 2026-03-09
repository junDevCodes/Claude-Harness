# Advanced Harness

Claude Code 기반 개발 프로젝트를 위한 **공통 자산 라이브러리**.
Skills, Agents, Hooks, Commands, Base Code를 새 프로젝트에 복사하여 즉시 고생산성 AI 지원 환경을 구성할 수 있습니다.

> **사용 가이드:** `HARNESS_USAGE_GUIDE.md` | **커맨드 레퍼런스:** `HARNESS_COMMANDS_REFERENCE.md`

---

## 자산 현황

| 자산 유형 | 수량 | 비고 |
|---|---|---|
| **Skills** | 24개 | skill-rules.json v1.3 전원 등록 |
| **Agents** | 16개 | Phase 3 완료 |
| **Slash Commands** | 9개 | Phase 5-B 완료 |
| **Hooks** | 6종 | 4문서 자동화 + CI/CD 연동 |
| **Base Stacks** | 9개 | FastAPI/Next.js/Express/Django/NestJS/Spring Boot/React Native/C/C++ |

---

## 빠른 시작

### 신규 프로젝트

```bash
bash /path/to/advanced-harness-window/scripts/harness-init.sh
```

### 기존 프로젝트에 적용

Claude Code 세션에서:
```
/harness-apply
```

---

## 구조

```
advanced-harness-window/
├── .claude/
│   ├── skills/        ← 24개 스킬 (스택별 개발 가이드라인)
│   ├── agents/        ← 16개 에이전트 (자동화 서브태스크)
│   ├── commands/      ← 9개 슬래시 커맨드
│   ├── hooks/         ← 자동화 훅 6종
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
├── CHANGELOG.md                  ← 자산 버전 이력 (현재 v1.5.0)
└── base_code_plan.md             ← 전체 자산 운영 계획
```

---

## Skills (24개)

| 카테고리 | 스킬 |
|---|---|
| 웹 백엔드 | `fastapi-backend-guidelines`, `express-backend-guidelines`, `django-backend-guidelines`, `nestjs-backend-guidelines`, `spring-boot-guidelines` |
| 프론트엔드 | `nextjs-frontend-guidelines`, `vercel-react-best-practices`, `frontend-design`, `web-design-guidelines` |
| 모바일 | `react-native-guidelines` |
| 임베디드 | `embedded-c-guidelines`, `embedded-cpp-guidelines` |
| 인프라 | `docker-guidelines`, `aws-guidelines`, `database-guidelines` |
| 테스팅 | `pytest-backend-testing` |
| 문서/도구 | `pdf`, `docx`, `pptx`, `mermaid`, `brand-guidelines`, `ppt-brand-guidelines`, `error-tracking`, `skill-developer` |

---

## Agents (16개)

| 에이전트 | 역할 |
|---|---|
| `db-migration-agent` | Alembic/Prisma/Django 마이그레이션 자동화 |
| `api-spec-generator` | OpenAPI 3.0 YAML 스펙 자동 생성 |
| `test-coverage-agent` | 테스트 커버리지 분석 및 미커버 코드 테스트 작성 |
| `performance-auditor` | Core Web Vitals + 백엔드 API 성능 점검 |
| `security-auditor` | OWASP Top 10 기준 인증/인가 취약점 점검 |
| `code-architecture-reviewer` | 코드 품질, 아키텍처 일관성 리뷰 |
| `auto-error-resolver` | TypeScript 컴파일 에러 자동 수정 |
| `planner` | 구조화된 개발 계획 생성 |
| `plan-reviewer` | 구현 전 개발 계획 리뷰 |
| `refactor-planner` | 단계별 리팩토링 전략 수립 |
| `code-refactor-master` | 종합 리팩토링 실행 |
| `documentation-architect` | 개발 문서, API 문서 생성 |
| `auth-route-debugger` | 인증 관련 이슈(401/403, JWT) 디버깅 |
| `auth-route-tester` | 인증된 API 라우트 테스트 |
| `frontend-error-fixer` | 빌드타임/런타임 프론트엔드 에러 수정 |
| `web-research-specialist` | 기술 이슈 리서치 |

---

## Slash Commands (9개)

| 커맨드 | 용도 |
|---|---|
| `/harness-init` | 신규 프로젝트 초기화 (Claude 세션 내) |
| `/harness-apply` | 신규/기존 프로젝트 자동 판별 → 하네스 적용 |
| `/harness-docs-update` | 4문서 체계 업데이트 |
| `/harness-plan-sync` | plan.md → 다음 Phase task.md 자동 생성 |
| `/harness-changelog` | 변경 자산 분석 → CHANGELOG.md 항목 생성 |
| `/harness-backport` | 실 프로젝트 패턴 → 하네스 역전파 |
| `/dev-docs` | 전략적 구현 계획 + 작업 분해 생성 |
| `/dev-docs-update` | 컨텍스트 컴팩션 전 docs/ 업데이트 |
| `/route-research-for-testing` | 편집된 라우트 감지 → 스모크 테스트 |

---

## 현재 적용 중인 프로젝트

`backend/`, `frontend/`는 하네스가 적용된 실제 서비스 코드입니다 (참고용).
새 프로젝트 시작 시에는 `base/[stack]/`을 사용하세요.
