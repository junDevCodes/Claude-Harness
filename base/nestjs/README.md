# NestJS Base Starter

NestJS 10 + TypeScript + Prisma + JWT 인증 + Docker 기반의 **범용 백엔드 스타터 킷**.

새 프로젝트 시작 시 이 디렉토리를 복사하여 즉시 개발을 시작할 수 있습니다.

---

## 기술 스택

| 레이어 | 기술 |
|---|---|
| Framework | NestJS 10 |
| Language | TypeScript 5 |
| ORM | Prisma 5 |
| Database | PostgreSQL 16 |
| Authentication | JWT (access 15m + refresh 7d) |
| Validation | class-validator + class-transformer |
| API Docs | Swagger (개발 환경 자동 생성) |
| Container | Docker + Docker Compose |

---

## 프로젝트 구조

```
src/
├── auth/                    # JWT 인증 모듈
│   ├── dto/                 # RegisterDto, LoginDto
│   ├── guards/              # JwtAuthGuard, JwtRefreshGuard
│   ├── strategies/          # JwtStrategy, JwtRefreshStrategy
│   ├── auth.controller.ts   # POST /auth/register, login, refresh, logout; GET /auth/me
│   ├── auth.module.ts
│   └── auth.service.ts
├── user/                    # User 도메인 (CRUD 예시)
│   ├── dto/                 # UpdateUserDto
│   ├── user.controller.ts   # GET/PATCH/DELETE /users/me
│   ├── user.module.ts
│   ├── user.repository.ts   # DB 접근 레이어
│   └── user.service.ts
├── prisma/                  # PrismaModule (Global)
│   ├── prisma.module.ts
│   └── prisma.service.ts
├── common/                  # 공통 유틸리티
│   ├── decorators/
│   │   └── current-user.decorator.ts   # @CurrentUser()
│   ├── filters/
│   │   └── http-exception.filter.ts    # 전역 예외 처리
│   └── interceptors/
│       └── transform.interceptor.ts    # 응답 래핑 { success, data, timestamp }
├── app.module.ts
└── main.ts                  # Bootstrap, Swagger, ValidationPipe
prisma/
└── schema.prisma            # User 모델
test/
└── auth.e2e-spec.ts         # E2E 테스트 (register → login → me → refresh → logout)
```

---

## 빠른 시작

### 1. 환경변수 설정

```bash
cp .env.example .env
# .env 파일을 열어 DATABASE_URL, JWT_SECRET 등 설정
```

### 2. 의존성 설치

```bash
pnpm install
```

### 3. DB 마이그레이션

```bash
# PostgreSQL이 실행 중인 상태에서
pnpm db:migrate
# 또는 (첫 실행 시 마이그레이션 이름 입력)
npx prisma migrate dev --name init
```

### 4. 개발 서버 실행

```bash
pnpm start:dev
```

서버 기동 후:
- API: `http://localhost:3000/api/v1`
- Swagger: `http://localhost:3000/api`

---

## Docker로 실행

```bash
# DB + 앱 전체 기동
docker-compose up -d

# 로그 확인
docker-compose logs -f app

# 종료
docker-compose down
```

---

## API 엔드포인트

### Auth

| Method | Endpoint | 설명 | 인증 |
|---|---|---|---|
| POST | `/api/v1/auth/register` | 회원가입 | - |
| POST | `/api/v1/auth/login` | 로그인 | - |
| POST | `/api/v1/auth/refresh` | 토큰 갱신 | refresh token |
| POST | `/api/v1/auth/logout` | 로그아웃 | access token |
| GET | `/api/v1/auth/me` | 현재 사용자 | access token |

### Users

| Method | Endpoint | 설명 | 인증 |
|---|---|---|---|
| GET | `/api/v1/users/me` | 내 프로필 조회 | access token |
| PATCH | `/api/v1/users/me` | 프로필 수정 | access token |
| DELETE | `/api/v1/users/me` | 계정 삭제 | access token |

### 인증 흐름

```
POST /auth/register  →  { user, accessToken, refreshToken }
POST /auth/login     →  { user, accessToken, refreshToken }
POST /auth/refresh   →  { accessToken, refreshToken }  (Authorization: Bearer <refreshToken>)
POST /auth/logout    →  204 No Content
```

### 응답 형식

**성공:**
```json
{
  "success": true,
  "data": { ... },
  "timestamp": "2026-03-02T00:00:00.000Z"
}
```

**에러:**
```json
{
  "success": false,
  "statusCode": 401,
  "message": "Invalid credentials",
  "path": "/api/v1/auth/login",
  "timestamp": "2026-03-02T00:00:00.000Z"
}
```

---

## 테스트

```bash
# 단위 테스트
pnpm test

# E2E 테스트 (PostgreSQL DB 필요)
pnpm test:e2e

# 커버리지
pnpm test:cov
```

---

## 커스터마이징 포인트

### 1. 새 도메인 모듈 추가

```bash
# NestJS CLI로 모듈/서비스/컨트롤러 생성
npx nest g module post
npx nest g service post
npx nest g controller post
```

그 후 `src/app.module.ts`에 `PostModule` 등록.

### 2. Prisma 스키마 확장

`prisma/schema.prisma`에 모델 추가:
```prisma
model Post {
  id        String   @id @default(cuid())
  title     String
  content   String
  authorId  String
  author    User     @relation(fields: [authorId], references: [id])
  createdAt DateTime @default(now())
  updatedAt DateTime @updatedAt

  @@map("posts")
}
```

그 후:
```bash
pnpm db:migrate
pnpm db:generate
```

### 3. 환경변수

| 변수 | 설명 | 기본값 |
|---|---|---|
| `PORT` | 서버 포트 | `3000` |
| `DATABASE_URL` | PostgreSQL 연결 URL | — |
| `JWT_SECRET` | access token 서명 키 | — |
| `JWT_EXPIRES_IN` | access token 만료 | `15m` |
| `JWT_REFRESH_SECRET` | refresh token 서명 키 | — |
| `JWT_REFRESH_EXPIRES_IN` | refresh token 만료 | `7d` |
| `CORS_ORIGINS` | CORS 허용 도메인 (콤마 구분) | `http://localhost:3000` |

### 4. Guard 적용

```typescript
// 특정 라우트에만 적용
@Get('protected')
@UseGuards(JwtAuthGuard)
async protectedRoute(@CurrentUser() user: JwtPayload) {
  return user;
}

// 컨트롤러 전체에 적용
@UseGuards(JwtAuthGuard)
@Controller('items')
export class ItemController { ... }
```

---

## 아키텍처 원칙

- **Repository 패턴**: Service는 비즈니스 로직만, Repository는 DB 접근만 담당
- **전역 ValidationPipe**: DTO 자동 검증 (whitelist, transform)
- **전역 TransformInterceptor**: 모든 성공 응답을 `{ success, data, timestamp }` 형식으로 통일
- **전역 HttpExceptionFilter**: 모든 에러 응답을 표준 형식으로 통일
- **ConfigModule (isGlobal)**: `.env` 값을 어디서나 `ConfigService`로 주입 가능
- **PrismaModule (Global)**: 한 번 등록으로 전체 모듈에서 `PrismaService` 주입 가능
