# Plan — base/nestjs/ 베이스 코드 구성 계획

> **문서 역할:** `base/nestjs/` NestJS 베이스 코드의 전체 구성 방향과 목표를 관리하는 기획서.
> 세부 작업은 `task.md`, 진행 맥락은 `history.md`, 점검은 `checklist.md`에서 관리한다.

---

## 목표

새 백엔드 프로젝트 시작 시 즉시 포크하여 사용할 수 있는
**범용 NestJS 백엔드 스타터 킷**을 구성한다.

특정 비즈니스 로직은 배제하고, 공통 아키텍처 패턴과 인증 흐름만 포함한다.

---

## 기술 스택

| 레이어 | 기술 |
|---|---|
| **Framework** | NestJS 10+ (TypeScript) |
| **ORM** | Prisma 5 |
| **Database** | PostgreSQL 16 |
| **Authentication** | JWT (access token 15m + refresh token 7d) |
| **Password** | bcrypt |
| **Validation** | class-validator + class-transformer |
| **Config** | @nestjs/config (.env 기반) |
| **API Docs** | Swagger (@nestjs/swagger) |
| **Testing** | Jest (unit) + Supertest (e2e) |
| **Container** | Docker + Docker Compose |

---

## 아키텍처 방향

### Module 구조 (Feature-based)

```
src/
├── app.module.ts            # Root Module
├── main.ts                  # Bootstrap + Swagger
├── auth/                    # 인증 모듈 (JWT Guards, Strategies)
├── user/                    # User 도메인 (CRUD 예시)
├── prisma/                  # Prisma 서비스 (글로벌)
└── common/                  # 공통 유틸리티
    ├── filters/             # 전역 예외 필터
    ├── interceptors/        # 응답 변환 인터셉터
    └── decorators/          # 커스텀 데코레이터
```

### 인증 흐름

```
POST /auth/register  →  bcrypt hash  →  DB 저장
POST /auth/login     →  검증  →  access(15m) + refresh(7d) 발급
POST /auth/refresh   →  refresh 검증  →  access 재발급
POST /auth/logout    →  DB refresh token 무효화
GET  /auth/me        →  @UseGuards(JwtAuthGuard)  →  현재 사용자 반환
```

### Guard 체계

```
JwtAuthGuard        ← access token 검증 (Passport JWT)
JwtRefreshGuard     ← refresh token 검증 (별도 Strategy)
```

### Repository 패턴

```
Controller  →  Service  →  Repository  →  Prisma
```

---

## 파일 트리 (전체 목표)

```
base/nestjs/
├── src/
│   ├── auth/
│   │   ├── dto/
│   │   │   ├── login.dto.ts
│   │   │   └── register.dto.ts
│   │   ├── guards/
│   │   │   ├── jwt-auth.guard.ts
│   │   │   └── jwt-refresh.guard.ts
│   │   ├── strategies/
│   │   │   ├── jwt.strategy.ts
│   │   │   └── jwt-refresh.strategy.ts
│   │   ├── auth.controller.ts
│   │   ├── auth.module.ts
│   │   └── auth.service.ts
│   ├── user/
│   │   ├── dto/
│   │   │   ├── create-user.dto.ts
│   │   │   └── update-user.dto.ts
│   │   ├── user.controller.ts
│   │   ├── user.module.ts
│   │   ├── user.repository.ts
│   │   └── user.service.ts
│   ├── prisma/
│   │   ├── prisma.module.ts
│   │   └── prisma.service.ts
│   ├── common/
│   │   ├── decorators/
│   │   │   └── current-user.decorator.ts
│   │   ├── filters/
│   │   │   └── http-exception.filter.ts
│   │   └── interceptors/
│   │       └── transform.interceptor.ts
│   ├── app.module.ts
│   └── main.ts
├── prisma/
│   └── schema.prisma
├── test/
│   ├── auth.e2e-spec.ts
│   └── jest-e2e.json
├── docs/
│   ├── plan.md
│   ├── task.md
│   ├── history.md
│   └── checklist.md
├── .env.example
├── .gitignore
├── Dockerfile
├── docker-compose.yaml
├── nest-cli.json
├── package.json
├── README.md
└── tsconfig.json
```

---

## 커스터마이징 포인트

새 프로젝트 포크 시 수정할 위치:

| 항목 | 위치 |
|---|---|
| 앱 이름/포트 | `main.ts`, `.env` |
| DB 연결 | `.env` (DATABASE_URL) |
| JWT Secret | `.env` (JWT_SECRET, JWT_REFRESH_SECRET) |
| 새 도메인 추가 | `src/[domain]/` 모듈 생성 후 `app.module.ts` 등록 |
| Prisma 스키마 | `prisma/schema.prisma` 에 모델 추가 |
| CORS 허용 도메인 | `main.ts` cors 설정 |

---

## 완성 기준 (Definition of Done)

루트 `docs/plan.md` Phase 1-B 기준 적용:

- [ ] `docs/` 4문서 작성 완료
- [ ] JWT 인증 (access + refresh)
- [ ] User CRUD 예시
- [ ] `.env.example`
- [ ] `Dockerfile` + `docker-compose.yaml`
- [ ] 테스트 템플릿 1개 이상
- [ ] `README.md`

---

*Created: 2026-03-02*
