# Plan — base/express/ 구성 계획

> **문서 역할:** `base/express/` 베이스 코드의 전체 구성 목표와 방향.

---

## 목표

Express.js + TypeScript 기반 백엔드 프로젝트를 시작할 때 즉시 사용 가능한 **범용 스타터 킷**.

비즈니스 도메인 로직 없이 인프라 패턴만 포함하며, 도메인을 추가하면 프로덕션 수준 REST API를 빠르게 구성할 수 있다.

---

## 핵심 구성 요소

| 구성 요소 | 구현 방식 | 상태 |
|---|---|---|
| DDD 구조 | domain/user + domain/shared | ✅ |
| Prisma ORM | PostgreSQL + 마이그레이션 | ✅ |
| JWT 인증 | access token (15m) + refresh token (30d) | ✅ |
| Refresh Token Rotation | 재사용 공격 방지 | ✅ |
| User CRUD API | GET/POST/PATCH/DELETE /api/v1/users | ✅ |
| Auth API | /auth/sign-up, /auth/login, /auth/refresh, /auth/me, /auth/logout | ✅ |
| 에러 핸들러 | AppError 계층, Express 에러 미들웨어 | ✅ |
| S3 업로드 유틸리티 | presigned POST/GET + delete (AWS SDK v3) | ✅ |
| 테스트 템플릿 | tests/auth.test.ts (Jest + supertest) | ✅ |
| Dockerfile | node:20-alpine + multi-stage | ✅ |
| docker-compose.yaml | postgres + api 서비스 | ✅ |
| .env.example | 전체 환경변수 목록 | ✅ |
| README.md | 구조 + 실행 방법 + 커스터마이징 | ✅ |

---

## 기술 스택

- **Runtime**: Node.js 20 LTS
- **Framework**: Express.js 4.x
- **Language**: TypeScript 5.x
- **ORM**: Prisma 5.x (PostgreSQL)
- **Auth**: jsonwebtoken (HS256)
- **Password**: bcryptjs
- **Validation**: Zod
- **ID**: ULID (`ulidx`)
- **S3**: AWS SDK v3 (`@aws-sdk/client-s3`, `@aws-sdk/s3-presigned-post`, `@aws-sdk/s3-request-presigner`)
- **Logger**: Pino
- **Test**: Jest + supertest + ts-jest
- **Container**: Docker + Docker Compose

---

## 디렉토리 구조

```
base/express/
├── src/
│   ├── api/
│   │   └── v1/
│   │       └── routes/    ← auth.routes.ts, user.routes.ts, index.ts
│   ├── config/            ← config.ts (Zod env validation)
│   ├── db/                ← prisma.ts (PrismaClient 싱글톤)
│   ├── domain/
│   │   ├── shared/        ← base.repository.ts (Generic CRUD)
│   │   └── user/          ← user.model.ts, user.repository.ts, user.service.ts, auth.service.ts
│   ├── dtos/              ← auth.dto.ts, user.dto.ts (Zod 스키마)
│   ├── errors/            ← app.error.ts, error.types.ts
│   ├── middleware/        ← error.handler.ts, auth.middleware.ts, validate.ts
│   └── utils/             ← logger.ts, password.ts, s3.ts, jwt.ts
├── prisma/
│   └── schema.prisma
├── tests/
│   ├── auth.test.ts
│   └── helpers/           ← test.app.ts
├── docs/                  ← 4문서 (plan/task/history/checklist)
├── .env.example
├── .gitignore
├── Dockerfile
├── docker-compose.yaml
├── package.json
├── tsconfig.json
└── README.md
```

---

## API 엔드포인트

### Auth

| Method | Path | Description | Auth |
|---|---|---|---|
| POST | /api/v1/auth/sign-up | 회원가입 | ✗ |
| POST | /api/v1/auth/login | 로그인 | ✗ |
| POST | /api/v1/auth/refresh | 토큰 갱신 | ✗ |
| GET | /api/v1/auth/me | 내 정보 | ✓ |
| DELETE | /api/v1/auth/logout | 로그아웃 | ✓ |

### Users

| Method | Path | Description | Auth |
|---|---|---|---|
| GET | /api/v1/users | 유저 목록 | ✓ |
| GET | /api/v1/users/:id | 유저 상세 | ✓ |
| PATCH | /api/v1/users/:id | 유저 수정 | ✓ |
| DELETE | /api/v1/users/:id | 유저 삭제 | ✓ (ADMIN) |

### Health

| Method | Path | Description |
|---|---|---|
| GET | /health | 서버 상태 |

---

## 설계 원칙

1. **범용성** — 비즈니스 도메인 로직 없음, 인프라 패턴만 포함
2. **DDD 구조** — domain 레이어에 비즈니스 로직 집중
3. **Repository 패턴** — DB 접근을 domain에서 분리
4. **DTO 계층** — Zod 스키마로 입출력 검증
5. **에러 계층** — AppError 기반 일관된 에러 응답

---

**완료: 2026-03-01 — Definition of Done 충족** ✅

---

## Phase 2 — `express-backend-guidelines` 스킬 생성 (현재)

> **목적:** `base/express/`를 참조해 `express-backend-guidelines` 스킬 작성.
> **격리 원칙:** `.claude/skills/express-backend-guidelines/` 디렉토리만 생성·수정.

### 스킬 핵심 내용

| 패턴 | 내용 |
|---|---|
| DDD 구조 | Router → Service → Repository 3-Layer |
| Prisma ORM | Generic BaseRepository + 페이지네이션 |
| JWT 인증 | access(15m) + refresh(30d) + Rotation |
| AppError 계층 | 팩토리 메서드 + Express 에러 미들웨어 |
| Zod 검증 | DTO 스키마 + validate 미들웨어 |
| S3 유틸리티 | presigned POST/GET + delete |

### 스킬 완성 기준 (Definition of Done)

- [ ] `SKILL.md` 500줄 이하
- [ ] YAML frontmatter (`name`, `description`, `triggers` 5개 이상)
- [ ] 핵심 패턴 3개 이상 + 코드 예시
- [ ] 안티패턴 섹션 포함

---

*Last updated: 2026-03-05 (Phase 2 스킬 생성 세션 시작)*
