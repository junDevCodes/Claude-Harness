---
name: express-backend-guidelines
description: Express.js + TypeScript 백엔드 개발 가이드라인. DDD 구조(Router→Service→Repository), Prisma ORM, JWT Refresh Token Rotation, AppError 계층, Zod 검증, S3 presigned URL 패턴. Express 라우터, 서비스, 레포지토리, 미들웨어, DTO 작성 시 사용.
triggers:
  - express
  - express.js
  - express backend
  - express router
  - express middleware
  - express typescript
  - express prisma
  - express jwt
  - express api
  - node.js backend
  - nodejs express
  - express service
  - express repository
  - express error handler
  - express s3
---

# Express.js + TypeScript Backend Guidelines

## Purpose

`base/express/` 기반 Express.js + TypeScript 백엔드 개발 표준. DDD 레이어 구조, Prisma ORM, JWT Refresh Token Rotation, Zod 검증, S3 연동 패턴을 일관되게 적용한다.

## When to Use This Skill

- Express 라우터 / 서비스 / 레포지토리 생성
- JWT 인증 미들웨어 구현
- AppError 기반 에러 처리 추가
- Zod DTO 스키마 정의
- S3 presigned URL 연동
- Prisma 모델 및 마이그레이션 작업

---

## Quick Start

### New API Endpoint Checklist

- [ ] 라우터 `src/api/v1/routes/{domain}.routes.ts`에 엔드포인트 정의
- [ ] `authenticate` 미들웨어로 JWT 검증 (인증 필요 시)
- [ ] `validateBody(schema)` / `validateQuery(schema)` 미들웨어로 Zod 검증
- [ ] Service 호출 (라우터에서 직접 Repository 접근 금지)
- [ ] `try/catch → next(err)` 또는 `AppError` throw
- [ ] 응답: 래퍼 없이 직접 반환 (`res.json(result)`, Create는 `res.status(201).json(result)`)

### New Domain Checklist

- [ ] `src/domain/{entity}/` 디렉토리 생성
- [ ] `{entity}.model.ts` — 타입 정의 + `toSafe{Entity}()` 변환 함수
- [ ] `{entity}.repository.ts` — Prisma 기반, `BaseRepository<T>` 구현
- [ ] `{entity}.service.ts` — 비즈니스 로직, AppError throw
- [ ] `src/dtos/{entity}.dto.ts` — Zod 스키마
- [ ] `src/api/v1/routes/{entity}.routes.ts` — 라우터
- [ ] `src/api/v1/routes/index.ts`에 라우터 등록
- [ ] `prisma/schema.prisma`에 모델 추가 + `prisma migrate dev`

---

## Project Structure

```
src/
├── api/v1/routes/        # 라우터 (HTTP 계층만)
├── config/config.ts      # Zod env 검증
├── db/prisma.ts          # PrismaClient 싱글톤
├── domain/
│   ├── shared/base.repository.ts  # Generic CRUD 인터페이스
│   └── {entity}/
│       ├── {entity}.model.ts
│       ├── {entity}.repository.ts
│       ├── {entity}.service.ts
│       └── auth.service.ts        # 인증 서비스 (auth 도메인)
├── dtos/                 # Zod 스키마 (입출력 검증)
├── errors/               # AppError 계층
├── middleware/           # authenticate, validate, error.handler
└── utils/                # jwt, password, s3, logger
```

---

## Core Patterns

### Pattern 1 — DDD 3-Layer Architecture

라우터는 HTTP만, 서비스는 비즈니스 로직, 레포지토리는 DB만 담당한다.

```typescript
// src/api/v1/routes/user.routes.ts — HTTP 계층
router.get('/', authenticate, validateQuery(paginationSchema), async (req, res, next) => {
  try {
    const result = await userService.getUsers(req.query as PaginationOptions);
    res.json(result);
  } catch (err) {
    next(err);
  }
});

// src/domain/user/user.service.ts — 비즈니스 로직
export class UserService {
  constructor(private readonly userRepo: UserRepository) {}

  async getUsers(options: PaginationOptions) {
    return this.userRepo.findAll(options);
  }

  async updateUser(id: string, data: UpdateUserInput): Promise<SafeUser> {
    const user = await this.userRepo.findById(id);
    if (!user) throw AppError.notFound('User not found');
    return this.userRepo.update(id, data);
  }
}

// src/domain/user/user.repository.ts — DB 계층
export class UserRepository implements BaseRepository<SafeUser, CreateUserInput, UpdateUserInput> {
  async findById(id: string): Promise<SafeUser | null> {
    const user = await prisma.user.findUnique({ where: { id } });
    return user ? toSafeUser(user as User) : null;
  }

  async findAll(options: PaginationOptions): Promise<PaginatedResult<SafeUser>> {
    const page  = Math.max(1, options.page ?? 1);
    const limit = Math.min(100, Math.max(1, options.limit ?? 20));
    const skip = (page - 1) * limit;
    const [users, total] = await prisma.$transaction([
      prisma.user.findMany({ skip, take: limit, orderBy: { createdAt: 'desc' } }),
      prisma.user.count(),
    ]);
    return buildPaginatedResult(users.map(toSafeUser), total, page, limit);
  }
}
```

---

### Pattern 2 — JWT Refresh Token Rotation

access token(15분) + refresh token(30일, DB 저장). Refresh 시 기존 토큰 즉시 무효화.

```typescript
// src/utils/jwt.ts
export function signAccessToken(payload: { sub: string; email: string; role: string }): string {
  return jwt.sign(payload, config.JWT_ACCESS_SECRET, { expiresIn: config.JWT_ACCESS_EXPIRES_IN });
}

export function signRefreshToken(payload: { sub: string; tokenId: string }): string {
  return jwt.sign(payload, config.JWT_REFRESH_SECRET, { expiresIn: config.JWT_REFRESH_EXPIRES_IN });
}

// src/domain/user/auth.service.ts
async refresh(refreshToken: string): Promise<AuthTokens> {
  const payload = verifyRefreshToken(refreshToken);
  const stored = await this.userRepo.findRefreshToken(payload.tokenId);
  if (!stored || stored.revokedAt || stored.expiresAt < new Date()) {
    throw new AppError('Invalid or expired refresh token', 401, 'INVALID_TOKEN');
  }
  await this.userRepo.revokeRefreshToken(payload.tokenId);  // Rotation
  const user = await this.userRepo.findById(payload.sub);
  if (!user) throw AppError.notFound('User not found');
  return this.createTokens(user);
}

private async createTokens(user: SafeUser): Promise<AuthTokens> {
  const tokenId = ulid();
  const accessToken = signAccessToken({ sub: user.id, email: user.email, role: user.role });
  const refreshToken = signRefreshToken({ sub: user.id, tokenId });
  await this.userRepo.createRefreshToken({
    id: tokenId, token: refreshToken, userId: user.id,
    expiresAt: new Date(Date.now() + parseDurationMs(config.JWT_REFRESH_EXPIRES_IN)),
  });
  return { accessToken, refreshToken };
}
```

**인증 미들웨어:**

```typescript
// src/middleware/auth.middleware.ts
export function authenticate(req: Request, _res: Response, next: NextFunction): void {
  const authHeader = req.headers.authorization;
  if (!authHeader?.startsWith('Bearer ')) {
    throw AppError.unauthorized('Missing or malformed authorization header');
  }
  try {
    req.user = verifyAccessToken(authHeader.slice(7));
    next();
  } catch (err) {
    next(err);
  }
}

export function requireRole(...roles: string[]) {
  return (req: Request, _res: Response, next: NextFunction): void => {
    if (!req.user) throw AppError.unauthorized('Authentication required');
    if (!roles.includes(req.user.role)) throw AppError.forbidden('Insufficient permissions');
    next();
  };
}
```

---

### Pattern 3 — AppError 계층

정적 팩토리 메서드로 일관된 에러 생성. `isOperational: true`로 운영/개발 에러 구분.

```typescript
// src/errors/app.error.ts
export class AppError extends Error {
  public readonly statusCode: number;
  public readonly code: ErrorCodeType;
  public readonly isOperational = true;

  constructor(message: string, statusCode: number, code: ErrorCodeType) {
    super(message);
    this.statusCode = statusCode;
    this.code = code;
    Object.setPrototypeOf(this, AppError.prototype);
    Error.captureStackTrace(this, this.constructor);
  }

  static unauthorized(message = 'Unauthorized')  { return new AppError(message, 401, 'UNAUTHORIZED'); }
  static forbidden(message = 'Forbidden')        { return new AppError(message, 403, 'FORBIDDEN'); }
  static notFound(message = 'Resource not found'){ return new AppError(message, 404, 'NOT_FOUND'); }
  static badRequest(message: string)             { return new AppError(message, 400, 'BAD_REQUEST'); }
  static conflict(message: string, code: ErrorCodeType) { return new AppError(message, 409, code); }
  static internal(message = 'Internal server error')    { return new AppError(message, 500, 'INTERNAL_SERVER_ERROR'); }
}

// src/middleware/error.handler.ts — Express 에러 미들웨어 (반드시 4개 파라미터)
export function errorHandler(err: Error, req: Request, res: Response, _next: NextFunction): void {
  if (err instanceof ZodError) {
    res.status(400).json({
      error: {
        code: 'VALIDATION_ERROR',
        message: 'Request validation failed',
        details: err.flatten().fieldErrors,
      },
    });
    return;
  }

  if (err instanceof AppError && err.isOperational) {
    res.status(err.statusCode).json({
      error: { code: err.code, message: err.message },
    });
    return;
  }

  logger.error({ err, req: { method: req.method, url: req.url } }, 'Unhandled error');
  res.status(500).json({ error: { code: 'INTERNAL_SERVER_ERROR', message: 'An unexpected error occurred' } });
}

export function notFoundHandler(req: Request, res: Response): void {
  res.status(404).json({ error: { code: 'NOT_FOUND', message: `${req.method} ${req.path} not found` } });
}
```

---

### Pattern 4 — Zod DTO + validate 미들웨어

요청 검증을 미들웨어로 분리. 라우터는 이미 검증된 데이터만 받는다.

```typescript
// src/dtos/auth.dto.ts
export const SignUpDto = z.object({
  email:    z.string().email(),
  username: z.string().min(3).max(30).regex(/^[a-zA-Z0-9_]+$/, 'alphanumeric and underscore only'),
  password: z.string().min(8).max(100),
});
export type SignUpInput = z.infer<typeof SignUpDto>;

export const LoginDto = z.object({
  email:    z.string().email(),
  password: z.string().min(1),
});

// src/middleware/validate.ts
export function validateBody<T>(schema: ZodSchema<T>) {
  return (req: Request, _res: Response, next: NextFunction): void => {
    const result = schema.safeParse(req.body);
    if (!result.success) {
      next(result.error);  // errorHandler가 ZodError 처리
      return;
    }
    req.body = result.data;
    next();
  };
}

export function validateQuery<T>(schema: ZodSchema<T>) {
  return (req: Request, _res: Response, next: NextFunction): void => {
    const result = schema.safeParse(req.query);
    if (!result.success) { next(result.error); return; }
    req.query = result.data as typeof req.query;
    next();
  };
}

// 라우터에서 사용
router.post('/sign-up', validateBody(SignUpDto), async (req, res, next) => {
  try {
    const result = await authService.signUp(req.body as SignUpInput);
    res.status(201).json(result);
  } catch (err) {
    next(err);
  }
});
```

---

### Pattern 5 — S3 Presigned URL (AWS SDK v3)

파일 업로드는 presigned POST, 다운로드는 presigned GET. 클라이언트가 S3에 직접 업로드.

```typescript
// src/utils/s3.ts
import { S3Client, DeleteObjectCommand, GetObjectCommand } from '@aws-sdk/client-s3';
import { createPresignedPost } from '@aws-sdk/s3-presigned-post';  // 별도 패키지
import { getSignedUrl } from '@aws-sdk/s3-request-presigner';

const s3 = new S3Client({
  region: config.AWS_REGION,
  ...(config.AWS_ACCESS_KEY_ID && config.AWS_SECRET_ACCESS_KEY && {
    credentials: { accessKeyId: config.AWS_ACCESS_KEY_ID, secretAccessKey: config.AWS_SECRET_ACCESS_KEY },
  }),
});

// 업로드용: presigned POST (조건 포함, 클라이언트 직접 업로드)
export async function createPresignedUploadUrl(
  key: string,
  contentType: string,
  maxSizeMB = 50,
): Promise<{ url: string; fields: Record<string, string> }> {
  return createPresignedPost(s3, {
    Bucket: config.AWS_S3_BUCKET,
    Key: key,
    Conditions: [
      ['content-length-range', 1, maxSizeMB * 1024 * 1024],
      ['starts-with', '$Content-Type', contentType.split('/')[0]],
    ],
    Fields: { 'Content-Type': contentType },
    Expires: config.AWS_S3_PRESIGNED_EXPIRES,
  });
}

// 다운로드용: presigned GET
export async function createPresignedGetUrl(key: string): Promise<string> {
  return getSignedUrl(s3, new GetObjectCommand({ Bucket: config.AWS_S3_BUCKET, Key: key }), {
    expiresIn: config.AWS_S3_PRESIGNED_EXPIRES,
  });
}

router.post('/upload', authenticate, async (req, res, next) => {
  try {
    const { key, contentType } = req.body;
    const result = await createPresignedUploadUrl(`uploads/${ulid()}-${key}`, contentType);
    res.json(result);
  } catch (err) { next(err); }
});
```

---

## Environment Variables

```bash
# .env.example 기준
DATABASE_URL="postgresql://..."
JWT_ACCESS_SECRET="..."
JWT_REFRESH_SECRET="..."
JWT_ACCESS_EXPIRES_IN="15m"
JWT_REFRESH_EXPIRES_IN="30d"
AWS_REGION="ap-northeast-2"
AWS_S3_BUCKET="..."
AWS_S3_PRESIGNED_EXPIRES=3600   # 초 단위
# AWS_ACCESS_KEY_ID / AWS_SECRET_ACCESS_KEY → ECS IAM Role 사용 시 불필요
```

---

## Prisma Schema 기본 패턴

```prisma
// prisma/schema.prisma
enum UserRole { USER ADMIN }

model User {
  id           String         @id @default(cuid())
  email        String         @unique
  username     String         @unique
  passwordHash String         @map("password_hash")
  role         UserRole       @default(USER)
  isActive     Boolean        @default(true)  @map("is_active")
  createdAt    DateTime       @default(now()) @map("created_at")
  updatedAt    DateTime       @updatedAt      @map("updated_at")
  refreshTokens RefreshToken[]
  @@map("users")
}

model RefreshToken {
  id        String    @id
  token     String    @unique
  userId    String    @map("user_id")
  expiresAt DateTime  @map("expires_at")
  revokedAt DateTime? @map("revoked_at")
  createdAt DateTime  @default(now()) @map("created_at")
  user      User      @relation(fields: [userId], references: [id], onDelete: Cascade)
  @@index([userId])
  @@map("refresh_tokens")
}
```

---

## Anti-Patterns

### ❌ 라우터에서 Prisma 직접 호출

```typescript
// BAD — 레이어 혼재
router.get('/users', async (req, res) => {
  const users = await prisma.user.findMany();  // ← 라우터에서 DB 직접 접근
  res.json(users);
});

// GOOD — Repository → Service → Router 3-layer
router.get('/users', authenticate, async (req, res, next) => {
  try {
    const result = await userService.getUsers(req.query);
    res.json(result);
  } catch (err) { next(err); }
});
```

### ❌ 에러를 res.json()으로 직접 응답

```typescript
// BAD — 에러 처리 중앙화 불가
router.post('/login', async (req, res) => {
  if (!user) {
    return res.status(401).json({ message: 'Not found' });  // ← 파편화
  }
});

// GOOD — AppError + next(err)로 중앙 핸들러 위임
router.post('/login', async (req, res, next) => {
  try {
    if (!user) throw AppError.unauthorized('Invalid credentials');
    res.json(result);
  } catch (err) { next(err); }
});
```

### ❌ Refresh Token을 로컬 변수/메모리에 저장

```typescript
// BAD — 서버 재시작 시 전체 무효화, revoke 불가
const refreshTokens = new Set<string>();

// GOOD — DB 저장 + ULID tokenId + Rotation
// revokeRefreshToken(tokenId) → revokedAt = now()
// refresh 요청마다 기존 토큰 무효화 + 새 토큰 발급
```

### ❌ 비밀번호 필드를 API 응답에 포함

```typescript
// BAD
res.json({ id: user.id, email: user.email, passwordHash: user.passwordHash });

// GOOD — toSafeUser() 변환 함수 사용
export function toSafeUser(user: User): SafeUser {
  const { passwordHash, ...safe } = user;
  return safe;
}
```

### ❌ AWS SDK v2 사용

```typescript
// BAD — AWS SDK v2 (tree-shaking 불가)
import AWS from 'aws-sdk';
const s3 = new AWS.S3();

// GOOD — AWS SDK v3 (패키지 분리, 모듈화)
import { S3Client } from '@aws-sdk/client-s3';
import { createPresignedPost } from '@aws-sdk/s3-presigned-post';  // 별도 패키지 필수
import { getSignedUrl } from '@aws-sdk/s3-request-presigner';
```

---

## References
`base/express/` · `src/domain/user/` (DDD) · `src/middleware/` (auth/validate/error) · `tests/auth.test.ts` (Jest+supertest)
