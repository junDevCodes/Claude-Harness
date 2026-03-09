# Task — base/nestjs/ 세부 작업 계획서

> **문서 역할:** `plan.md`에서 정의한 목표를 달성하기 위한 세부 실행 계획.
> 완료된 항목은 체크하고, 완료 시 `history.md`로 이동한다.

---

## Phase 1 — 프로젝트 기반 설정 ✅

| 파일 | 내용 | 상태 |
|---|---|---|
| `package.json` | NestJS 10, Prisma, JWT, bcrypt, class-validator, Swagger | ✅ |
| `tsconfig.json` | strict, decorator, path alias | ✅ |
| `nest-cli.json` | sourceRoot: src | ✅ |
| `.env.example` | DATABASE_URL, JWT_SECRET, PORT 등 전체 변수 | ✅ |
| `.gitignore` | node_modules, dist, .env, prisma/generated | ✅ |

## Phase 2 — Prisma 스키마 ✅

| 파일 | 내용 | 상태 |
|---|---|---|
| `prisma/schema.prisma` | User 모델 (id, email, password, username, hashedRefreshToken, createdAt, updatedAt) | ✅ |

---

## Phase 3 — 공통 레이어 (common/) ✅

| 파일 | 내용 | 상태 |
|---|---|---|
| `src/prisma/prisma.module.ts` | Global PrismaModule | ✅ |
| `src/prisma/prisma.service.ts` | PrismaClient extends, onModuleInit/Destroy | ✅ |
| `src/common/filters/http-exception.filter.ts` | 전역 예외 처리 (status, message, timestamp, path) | ✅ |
| `src/common/interceptors/transform.interceptor.ts` | 응답 래핑 { success, data, timestamp } | ✅ |
| `src/common/decorators/current-user.decorator.ts` | @CurrentUser() 파라미터 데코레이터 | ✅ |

---

## Phase 4 — Auth 모듈 ✅

| 파일 | 내용 | 상태 |
|---|---|---|
| `src/auth/dto/register.dto.ts` | email, password, username (class-validator) | ✅ |
| `src/auth/dto/login.dto.ts` | email, password | ✅ |
| `src/auth/strategies/jwt.strategy.ts` | access token Passport 전략 | ✅ |
| `src/auth/strategies/jwt-refresh.strategy.ts` | refresh token Passport 전략 (Bearer) | ✅ |
| `src/auth/guards/jwt-auth.guard.ts` | @UseGuards에서 사용할 access guard | ✅ |
| `src/auth/guards/jwt-refresh.guard.ts` | refresh endpoint용 guard | ✅ |
| `src/auth/auth.service.ts` | register, login, refresh, logout, getMe | ✅ |
| `src/auth/auth.controller.ts` | POST /auth/register, login, refresh, logout; GET /auth/me | ✅ |
| `src/auth/auth.module.ts` | PassportModule, JwtModule, 전략/가드 등록 | ✅ |

---

## Phase 5 — User 도메인 (CRUD 예시) ✅

| 파일 | 내용 | 상태 |
|---|---|---|
| `src/user/dto/create-user.dto.ts` | 내부용 (auth.service에서 호출) | ✅ |
| `src/user/dto/update-user.dto.ts` | username, hashedRefreshToken | ✅ |
| `src/user/user.repository.ts` | findById, findByEmail, create, update, delete | ✅ |
| `src/user/user.service.ts` | 비즈니스 로직 (repository 호출) | ✅ |
| `src/user/user.controller.ts` | GET /users/me, PATCH /users/me, DELETE /users/me | ✅ |
| `src/user/user.module.ts` | UserModule (exports UserService, UserRepository) | ✅ |

---

## Phase 6 — 앱 루트 ✅

| 파일 | 내용 | 상태 |
|---|---|---|
| `src/app.module.ts` | ConfigModule, PrismaModule, AuthModule, UserModule 등록 | ✅ |
| `src/main.ts` | Bootstrap, Swagger, ValidationPipe, 전역 필터/인터셉터, CORS | ✅ |

---

## Phase 7 — 인프라 & 문서 ✅

| 파일 | 내용 | 상태 |
|---|---|---|
| `Dockerfile` | 멀티 스테이지 (node:20-alpine builder → runner) | ✅ |
| `docker-compose.yaml` | postgres + nestjs-app 서비스 | ✅ |
| `README.md` | 구조, 실행 방법, 커스터마이징 포인트 | ✅ |

---

## Phase 8 — 테스트 ✅

| 파일 | 내용 | 상태 |
|---|---|---|
| `test/auth.e2e-spec.ts` | register → login → me → refresh → logout e2e 흐름 (11케이스) | ✅ |
| `test/jest-e2e.json` | e2e 테스트 Jest 설정 | ✅ |

---

*Created: 2026-03-02*
*Completed: 2026-03-02 (전체 구현 완료)*
