# base/express — Express.js + TypeScript Starter Kit

Express.js + TypeScript + Prisma 기반 백엔드 프로젝트를 즉시 시작할 수 있는 **범용 스타터 킷**.

비즈니스 도메인 로직 없이 인프라 패턴만 포함하며, 새 도메인을 추가하면 프로덕션 수준 REST API를 빠르게 구성할 수 있다.

---

## 기술 스택

| 항목 | 기술 |
|---|---|
| Runtime | Node.js 20 LTS |
| Framework | Express.js 4.x |
| Language | TypeScript 5.x |
| ORM | Prisma 5.x (PostgreSQL) |
| Auth | jsonwebtoken (HS256) |
| Validation | Zod |
| Logger | Pino |
| Test | Jest + supertest |
| Container | Docker + Docker Compose |

---

## 디렉토리 구조

```
src/
├── api/
│   └── v1/
│       └── routes/
│           ├── auth.routes.ts   ← 인증 라우터
│           ├── user.routes.ts   ← 유저 CRUD 라우터
│           └── index.ts         ← v1 라우터 통합
├── config/
│   └── config.ts                ← Zod env 검증
├── db/
│   └── prisma.ts                ← PrismaClient 싱글톤
├── domain/
│   ├── shared/
│   │   └── base.repository.ts   ← Generic CRUD 인터페이스
│   └── user/
│       ├── user.model.ts        ← User 타입 정의
│       ├── user.repository.ts   ← Prisma 기반 User CRUD
│       ├── user.service.ts      ← User 비즈니스 로직
│       └── auth.service.ts      ← JWT 인증 비즈니스 로직
├── dtos/
│   ├── auth.dto.ts              ← 회원가입/로그인 Zod 스키마
│   └── user.dto.ts              ← 유저 수정 Zod 스키마
├── errors/
│   ├── app.error.ts             ← AppError 클래스
│   └── error.types.ts           ← 에러 코드 enum
├── middleware/
│   ├── auth.middleware.ts       ← JWT 검증 미들웨어
│   ├── error.handler.ts         ← Express 에러 핸들러
│   └── validate.ts              ← Zod 요청 검증 미들웨어
└── utils/
    ├── jwt.ts                   ← 토큰 생성/검증
    ├── logger.ts                ← Pino 로거
    ├── password.ts              ← bcrypt hash/compare
    └── s3.ts                    ← AWS S3 presigned URL
```

---

## Quick Start

### 1. 환경 설정

```bash
cp .env.example .env
# .env 파일의 환경변수 값 설정
```

### 2. 의존성 설치 & DB 마이그레이션

```bash
npm install
npx prisma migrate dev --name init
```

### 3. 개발 서버 실행

```bash
npm run dev
# http://localhost:3000
```

### 4. Docker Compose로 실행

```bash
# .env 파일에 JWT_ACCESS_SECRET, JWT_REFRESH_SECRET 설정 필수
docker-compose up --build
```

---

## API 엔드포인트

### Auth

```
POST   /api/v1/auth/sign-up     회원가입
POST   /api/v1/auth/login       로그인
POST   /api/v1/auth/refresh     토큰 갱신
GET    /api/v1/auth/me          내 정보 (인증 필요)
DELETE /api/v1/auth/logout      로그아웃 (인증 필요)
```

### Users

```
GET    /api/v1/users            유저 목록 (인증 필요)
GET    /api/v1/users/:id        유저 상세 (인증 필요)
PATCH  /api/v1/users/:id        유저 수정 (인증 필요, 본인 or ADMIN)
DELETE /api/v1/users/:id        유저 삭제 (ADMIN 전용)
```

### Health

```
GET    /health                  서버 상태 확인
```

---

## 주요 패턴 설명

### JWT 인증 흐름

1. `/sign-up` 또는 `/login` → `access_token` + `refresh_token` 발급
2. API 요청 시 `Authorization: Bearer <access_token>` 헤더 포함
3. `access_token` 만료(15분) 시 `/refresh`로 갱신 → **Refresh Token Rotation** 적용
4. 로그아웃 시 모든 refresh token 무효화

### Refresh Token Rotation

보안 강화를 위해 refresh 요청 시 기존 토큰을 무효화하고 새 토큰을 발급합니다. 탈취된 refresh token 재사용을 감지할 수 있습니다.

### 에러 응답 형식

모든 에러는 일관된 형식으로 반환됩니다:

```json
{
  "error": {
    "code": "USER_NOT_FOUND",
    "message": "User not found"
  }
}
```

---

## 커스터마이징 — 새 도메인 추가

새 비즈니스 도메인(예: `post`)을 추가하는 방법:

### 1. Prisma 스키마 추가

```prisma
// prisma/schema.prisma
model Post {
  id        String   @id @default(cuid())
  title     String
  content   String
  authorId  String   @map("author_id")
  createdAt DateTime @default(now()) @map("created_at")
  updatedAt DateTime @updatedAt @map("updated_at")

  author User @relation(fields: [authorId], references: [id])

  @@map("posts")
}
```

```bash
npx prisma migrate dev --name add-post
```

### 2. Domain 레이어 생성

```
src/domain/post/
├── post.model.ts       ← Post 타입
├── post.repository.ts  ← Prisma CRUD
└── post.service.ts     ← 비즈니스 로직
```

### 3. DTO 추가

```
src/dtos/post.dto.ts    ← Zod 스키마
```

### 4. 라우터 추가

```
src/api/v1/routes/post.routes.ts
```

`src/api/v1/routes/index.ts`에 라우터 등록:

```typescript
import postRoutes from './post.routes';
router.use('/posts', postRoutes);
```

---

## 테스트 실행

```bash
# 테스트 DB 환경변수 설정 (선택)
export TEST_DATABASE_URL="postgresql://user:password@localhost:5432/express_base_test"

# 테스트 실행
npm test

# 커버리지 포함
npm run test:coverage
```

---

## 환경변수 전체 목록

`.env.example` 파일을 참고하세요.

| 변수 | 필수 | 설명 |
|---|---|---|
| `DATABASE_URL` | ✅ | PostgreSQL 연결 문자열 |
| `JWT_ACCESS_SECRET` | ✅ | Access token 서명 키 (32자 이상) |
| `JWT_REFRESH_SECRET` | ✅ | Refresh token 서명 키 (32자 이상) |
| `JWT_ACCESS_EXPIRES_IN` | — | Access token 만료 (기본: 15m) |
| `JWT_REFRESH_EXPIRES_IN` | — | Refresh token 만료 (기본: 30d) |
| `AWS_REGION` | — | S3 리전 |
| `AWS_ACCESS_KEY_ID` | — | S3 액세스 키 |
| `AWS_SECRET_ACCESS_KEY` | — | S3 시크릿 키 |
| `AWS_S3_BUCKET` | — | S3 버킷 이름 |
| `CORS_ORIGINS` | — | 허용 Origin (기본: localhost:3000) |
| `PORT` | — | 서버 포트 (기본: 3000) |
