---
name: nestjs-backend-guidelines
description: NestJS 10 + TypeScript + Prisma 5 백엔드 개발 가이드라인. Guard+Passport JWT 이중 전략, @CurrentUser() 데코레이터, Controller→Service→Repository 계층, TransformInterceptor 글로벌 응답 래핑, HttpExceptionFilter 예외 처리, Refresh Token Rotation 패턴. NestJS 모듈, 컨트롤러, 서비스, 레포지토리, Guard, DTO 작성 시 사용.
triggers:
  - nestjs
  - nest.js
  - nest backend
  - nestjs controller
  - nestjs service
  - nestjs guard
  - nestjs module
  - nestjs prisma
  - nestjs jwt
  - nestjs passport
  - nestjs interceptor
  - nestjs filter
  - nestjs decorator
  - nestjs repository
  - nestjs api
  - nestjs typescript
---

# NestJS + TypeScript Backend Guidelines

## Purpose

`base/nestjs/` 기반 NestJS + TypeScript + Prisma 백엔드 개발 표준. Guard/Passport JWT 인증, Repository 패턴, 글로벌 응답/예외 포맷, Refresh Token Rotation을 일관되게 적용한다.

## When to Use This Skill

- NestJS 모듈 / 컨트롤러 / 서비스 / 레포지토리 생성
- Guard + Passport JWT 인증 구현
- DTO class-validator 검증 정의
- 글로벌 응답 인터셉터 / 예외 필터 구성
- Prisma 모델 및 마이그레이션 작업

---

## Quick Start

### New API Endpoint Checklist

- [ ] Controller에 엔드포인트 추가 (`@Get`, `@Post`, `@Patch`, `@Delete`)
- [ ] 인증 필요 시 `@UseGuards(JwtAuthGuard)` + `@CurrentUser() user: JwtPayload`
- [ ] DTO에 `class-validator` 데코레이터 + `@ApiProperty` 작성
- [ ] Service 호출 (Controller에서 직접 Repository/Prisma 접근 금지)
- [ ] 반환값은 TransformInterceptor가 `{ success, data, timestamp }`로 자동 래핑

### New Domain Checklist

- [ ] `src/{domain}/` 디렉토리 + `dto/` 하위 디렉토리 생성
- [ ] `{domain}.repository.ts` → `{domain}.service.ts` → `{domain}.controller.ts`
- [ ] `{domain}.module.ts` — providers + exports 정의, `AppModule`에 import
- [ ] `prisma/schema.prisma`에 모델 추가 + `npx prisma migrate dev`

---

## Project Structure

```
src/
├── auth/
│   ├── dto/                    # RegisterDto, LoginDto
│   ├── guards/                 # JwtAuthGuard, JwtRefreshGuard
│   ├── strategies/             # JwtStrategy, JwtRefreshStrategy
│   ├── auth.controller.ts
│   ├── auth.module.ts
│   └── auth.service.ts
├── user/
│   ├── dto/                    # CreateUserDto, UpdateUserDto
│   ├── user.controller.ts
│   ├── user.module.ts
│   ├── user.repository.ts
│   └── user.service.ts
├── prisma/
│   ├── prisma.module.ts        # @Global() 전역 등록
│   └── prisma.service.ts       # extends PrismaClient
├── common/
│   ├── decorators/current-user.decorator.ts
│   ├── filters/http-exception.filter.ts
│   └── interceptors/transform.interceptor.ts
├── app.module.ts
└── main.ts                     # 글로벌 Pipe, Filter, Interceptor 등록
```

---

## Core Patterns

### Pattern 1 — Guard + Passport JWT Dual Strategy + @CurrentUser

access token(`'jwt'`)과 refresh token(`'jwt-refresh'`)을 별도 Strategy/Guard로 분리한다.

```typescript
// src/auth/strategies/jwt.strategy.ts — Access Token 검증
@Injectable()
export class JwtStrategy extends PassportStrategy(Strategy, 'jwt') {
  constructor(configService: ConfigService, private readonly userService: UserService) {
    super({
      jwtFromRequest: ExtractJwt.fromAuthHeaderAsBearerToken(),
      ignoreExpiration: false,
      secretOrKey: configService.get<string>('JWT_SECRET'),
    });
  }
  async validate(payload: JwtPayload) {
    const user = await this.userService.findById(payload.sub);
    if (!user) throw new UnauthorizedException('User not found');
    return payload;  // → request.user
  }
}

// src/auth/strategies/jwt-refresh.strategy.ts — Refresh Token 이중 검증
@Injectable()
export class JwtRefreshStrategy extends PassportStrategy(Strategy, 'jwt-refresh') {
  constructor(configService: ConfigService, private readonly userRepository: UserRepository) {
    super({
      jwtFromRequest: ExtractJwt.fromAuthHeaderAsBearerToken(),
      ignoreExpiration: false,
      secretOrKey: configService.get<string>('JWT_REFRESH_SECRET'),
      passReqToCallback: true,  // validate()에 request 전달
    });
  }
  async validate(req: any, payload: JwtPayload) {
    const refreshToken = req.headers.authorization?.replace('Bearer ', '').trim();
    const user = await this.userRepository.findById(payload.sub);
    if (!user?.hashedRefreshToken) throw new UnauthorizedException('Invalid refresh token');
    const isMatch = await bcrypt.compare(refreshToken, user.hashedRefreshToken);
    if (!isMatch) throw new UnauthorizedException('Invalid refresh token');
    return payload;
  }
}

// Guards — Strategy name과 1:1 매핑
@Injectable() export class JwtAuthGuard extends AuthGuard('jwt') {}
@Injectable() export class JwtRefreshGuard extends AuthGuard('jwt-refresh') {}
```

**@CurrentUser() 데코레이터** — Guard가 검증한 JWT 페이로드를 추출:

```typescript
// src/common/decorators/current-user.decorator.ts
export interface JwtPayload {
  sub: string; email: string; username: string; iat?: number; exp?: number;
}
export const CurrentUser = createParamDecorator(
  (_data: unknown, ctx: ExecutionContext): JwtPayload => ctx.switchToHttp().getRequest().user,
);

// 사용 — Guard 없이 @CurrentUser()만 쓰면 undefined (런타임 에러)
@UseGuards(JwtAuthGuard)
@Get('me')
getMe(@CurrentUser() user: JwtPayload) { return this.userService.findById(user.sub); }
```

---

### Pattern 2 — Controller → Service → Repository 계층

Controller는 HTTP만, Service는 비즈니스 로직, Repository는 Prisma DB 접근만 담당한다.

```typescript
// src/user/user.repository.ts — DB 접근만 (단일 책임)
@Injectable()
export class UserRepository {
  constructor(private readonly prisma: PrismaService) {}
  async findById(id: string) { return this.prisma.user.findUnique({ where: { id } }); }
  async findByEmail(email: string) { return this.prisma.user.findUnique({ where: { email } }); }
  async create(data: { email: string; password: string; username: string }) {
    return this.prisma.user.create({ data });
  }
  async update(id: string, data: Partial<{ username: string; hashedRefreshToken: string | null }>) {
    return this.prisma.user.update({ where: { id }, data });
  }
  async delete(id: string) { return this.prisma.user.delete({ where: { id } }); }
}

// src/user/user.service.ts — 비즈니스 로직 + 검증 + sanitize
@Injectable()
export class UserService {
  constructor(private readonly userRepository: UserRepository) {}
  async findById(id: string) {
    const user = await this.userRepository.findById(id);
    if (!user) throw new NotFoundException('User not found');
    return this.sanitize(user);
  }
  async updateMe(id: string, dto: UpdateUserDto) {
    await this.findById(id);  // 존재 확인
    return this.sanitize(await this.userRepository.update(id, dto));
  }
  async deleteMe(id: string) {
    await this.findById(id);
    await this.userRepository.delete(id);
  }
  private sanitize(user: any) {
    const { password, hashedRefreshToken, ...safe } = user;
    return safe;
  }
}

// src/user/user.controller.ts — HTTP 계층 (Guard + DTO)
@Controller('users')
export class UserController {
  constructor(private readonly userService: UserService) {}

  @UseGuards(JwtAuthGuard) @Get('me')
  getMe(@CurrentUser() user: JwtPayload) { return this.userService.findById(user.sub); }

  @UseGuards(JwtAuthGuard) @Patch('me')
  updateMe(@CurrentUser() user: JwtPayload, @Body() dto: UpdateUserDto) {
    return this.userService.updateMe(user.sub, dto);
  }

  @UseGuards(JwtAuthGuard) @Delete('me') @HttpCode(HttpStatus.NO_CONTENT)
  deleteMe(@CurrentUser() user: JwtPayload) { return this.userService.deleteMe(user.sub); }
}
```

---

### Pattern 3 — Global Response / Exception Format

TransformInterceptor(성공)와 HttpExceptionFilter(에러)를 `main.ts`에서 전역 등록한다.

```typescript
// src/common/interceptors/transform.interceptor.ts
@Injectable()
export class TransformInterceptor<T> implements NestInterceptor<T, { success: true; data: T; timestamp: string }> {
  intercept(_ctx: ExecutionContext, next: CallHandler) {
    return next.handle().pipe(
      map((data) => ({ success: true as const, data, timestamp: new Date().toISOString() })),
    );
  }
}

// src/common/filters/http-exception.filter.ts
@Catch(HttpException)
export class HttpExceptionFilter implements ExceptionFilter {
  catch(exception: HttpException, host: ArgumentsHost) {
    const ctx = host.switchToHttp();
    const status = exception.getStatus();
    const exRes = exception.getResponse();
    const message = typeof exRes === 'string' ? exRes : (exRes as any).message ?? exception.message;
    ctx.getResponse<Response>().status(status).json({
      success: false, statusCode: status,
      message: Array.isArray(message) ? message[0] : message,
      path: ctx.getRequest<Request>().url,
      timestamp: new Date().toISOString(),
    });
  }
}
```

**응답 형태:**
```json
{ "success": true,  "data": { "id": "clx1...", "email": "a@b.com" }, "timestamp": "..." }
{ "success": false, "statusCode": 401, "message": "Invalid credentials", "path": "/api/v1/auth/login", "timestamp": "..." }
```

---

### Pattern 4 — JWT Refresh Token Rotation

login/register → 토큰 쌍 발급 + refresh token 해시 DB 저장. refresh → 해시 검증 + 새 쌍 발급 (Rotation). logout → `hashedRefreshToken = null`.

```typescript
// src/auth/auth.service.ts
@Injectable()
export class AuthService {
  constructor(
    private readonly jwtService: JwtService,
    private readonly configService: ConfigService,
    private readonly userRepository: UserRepository,
  ) {}

  async register(dto: RegisterDto) {
    if (await this.userRepository.findByEmail(dto.email))
      throw new ConflictException('Email already in use');
    const user = await this.userRepository.create({
      email: dto.email, password: await bcrypt.hash(dto.password, 10), username: dto.username,
    });
    const tokens = await this.generateTokens(user.id, user.email, user.username);
    await this.saveHashedRefreshToken(user.id, tokens.refreshToken);
    return { user: this.sanitizeUser(user), ...tokens };
  }

  async login(dto: LoginDto) {
    const user = await this.userRepository.findByEmail(dto.email);
    if (!user || !(await bcrypt.compare(dto.password, user.password)))
      throw new UnauthorizedException('Invalid credentials');
    const tokens = await this.generateTokens(user.id, user.email, user.username);
    await this.saveHashedRefreshToken(user.id, tokens.refreshToken);
    return { user: this.sanitizeUser(user), ...tokens };
  }

  async refresh(userId: string, email: string, username: string) {
    const tokens = await this.generateTokens(userId, email, username);
    await this.saveHashedRefreshToken(userId, tokens.refreshToken);  // Rotation
    return tokens;
  }

  async logout(userId: string) {
    await this.userRepository.update(userId, { hashedRefreshToken: null });
  }

  private async generateTokens(userId: string, email: string, username: string) {
    const payload: JwtPayload = { sub: userId, email, username };
    const [accessToken, refreshToken] = await Promise.all([
      this.jwtService.signAsync(payload, {
        secret: this.configService.get('JWT_SECRET'), expiresIn: this.configService.get('JWT_EXPIRES_IN', '15m'),
      }),
      this.jwtService.signAsync(payload, {
        secret: this.configService.get('JWT_REFRESH_SECRET'), expiresIn: this.configService.get('JWT_REFRESH_EXPIRES_IN', '7d'),
      }),
    ]);
    return { accessToken, refreshToken };
  }

  private async saveHashedRefreshToken(userId: string, refreshToken: string) {
    await this.userRepository.update(userId, { hashedRefreshToken: await bcrypt.hash(refreshToken, 10) });
  }

  private sanitizeUser(user: any) { const { password, hashedRefreshToken, ...safe } = user; return safe; }
}
```

---

### Pattern 5 — Module Dependencies + Bootstrap

`@Global()` PrismaModule은 1회 등록으로 전역 사용. 외부 모듈에서 필요한 provider는 반드시 `exports`.

```typescript
// AppModule
@Module({
  imports: [ConfigModule.forRoot({ isGlobal: true }), PrismaModule, AuthModule, UserModule],
})
export class AppModule {}

// UserModule — exports 필수 (AuthModule에서 UserService, UserRepository 사용)
@Module({
  controllers: [UserController],
  providers: [UserService, UserRepository],
  exports: [UserService, UserRepository],
})
export class UserModule {}

// AuthModule — UserModule import
@Module({
  imports: [PassportModule, JwtModule.register({}), UserModule],
  controllers: [AuthController],
  providers: [AuthService, JwtStrategy, JwtRefreshStrategy, JwtAuthGuard, JwtRefreshGuard],
  exports: [AuthService],
})
export class AuthModule {}

// PrismaModule — @Global() 전역 등록
@Global()
@Module({ providers: [PrismaService], exports: [PrismaService] })
export class PrismaModule {}

// PrismaService
@Injectable()
export class PrismaService extends PrismaClient implements OnModuleInit, OnModuleDestroy {
  async onModuleInit() { await this.$connect(); }
  async onModuleDestroy() { await this.$disconnect(); }
}
```

**main.ts 글로벌 설정:**
```typescript
async function bootstrap() {
  const app = await NestFactory.create(AppModule);
  app.setGlobalPrefix('api/v1');
  app.enableCors({ origin: process.env.CORS_ORIGINS?.split(',') ?? ['http://localhost:3000'], credentials: true });
  app.useGlobalPipes(new ValidationPipe({ whitelist: true, forbidNonWhitelisted: true, transform: true }));
  app.useGlobalFilters(new HttpExceptionFilter());
  app.useGlobalInterceptors(new TransformInterceptor(), new ClassSerializerInterceptor(app.get(Reflector)));
  if (process.env.NODE_ENV !== 'production') {
    const config = new DocumentBuilder().setTitle('API').setVersion('1.0')
      .addBearerAuth({ type: 'http', scheme: 'bearer' }, 'access-token').build();
    SwaggerModule.setup('api', app, SwaggerModule.createDocument(app, config));
  }
  await app.listen(process.env.PORT ?? 3000);
}
```

---

## DTO + Validation + Prisma Schema

```typescript
// src/auth/dto/register.dto.ts
export class RegisterDto {
  @ApiProperty() @IsEmail() email: string;
  @ApiProperty({ minLength: 6 }) @IsString() @MinLength(6) password: string;
  @ApiProperty({ minLength: 2 }) @IsString() @MinLength(2) username: string;
}

// src/user/dto/update-user.dto.ts
export class UpdateUserDto {
  @ApiPropertyOptional() @IsOptional() @IsString() @MinLength(2) username?: string;
}
```

```prisma
model User {
  id                  String   @id @default(cuid())
  email               String   @unique
  password            String
  username            String
  hashedRefreshToken  String?     // bcrypt 해시, null = 로그아웃 상태
  createdAt           DateTime @default(now())
  updatedAt           DateTime @updatedAt
  @@map("users")
}
```

**환경변수** (`.env.example`):
```bash
PORT=3000                                    # 서버 포트
DATABASE_URL="postgresql://...?schema=public" # Prisma DB 연결
JWT_SECRET=access-token-secret               # Access Token 서명 키 (분리 필수)
JWT_EXPIRES_IN=15m                           # Access Token 만료
JWT_REFRESH_SECRET=refresh-token-secret      # Refresh Token 서명 키
JWT_REFRESH_EXPIRES_IN=7d                    # Refresh Token 만료
CORS_ORIGINS=http://localhost:3000           # 쉼표 구분 다중 origin
```

---

## Anti-Patterns

### 1. Controller에서 Prisma 직접 호출
```typescript
// BAD — 계층 혼재
@Get() async getUsers() { return this.prisma.user.findMany(); }
// GOOD — Repository → Service → Controller
@Get() async getUsers() { return this.userService.findAll(); }
```

### 2. Guard 없이 @CurrentUser() 사용
```typescript
// BAD — request.user가 undefined → 런타임 에러
@Get('me') getMe(@CurrentUser() user: JwtPayload) { ... }
// GOOD — @UseGuards(JwtAuthGuard) 필수 적용
@UseGuards(JwtAuthGuard) @Get('me') getMe(@CurrentUser() user: JwtPayload) { ... }
```

### 3. Access/Refresh에 같은 Secret 사용
```typescript
// BAD — access token으로 refresh 요청 성공 (토큰 혼용)
JwtModule.register({ secret: 'same-secret' })
// GOOD — Secret 분리: JWT_SECRET(access) + JWT_REFRESH_SECRET(refresh)
```

### 4. Module에서 exports 누락
```typescript
// BAD — AuthModule에서 UserService 주입 불가 (Nest can't resolve dependencies)
@Module({ providers: [UserService, UserRepository] }) export class UserModule {}
// GOOD — exports 명시
@Module({ providers: [UserService, UserRepository], exports: [UserService, UserRepository] })
export class UserModule {}
```

### 5. 비밀번호/해시를 API 응답에 포함
```typescript
// BAD — password, hashedRefreshToken 노출
return user;
// GOOD — sanitize 함수로 민감 필드 제거
const { password, hashedRefreshToken, ...safe } = user; return safe;
```

### 6. PrismaService를 모듈마다 개별 등록
```typescript
// BAD — 모듈마다 중복
@Module({ providers: [PrismaService, UserService] }) export class UserModule {}
// GOOD — PrismaModule @Global() 1회 등록 → 전역 사용
@Global() @Module({ providers: [PrismaService], exports: [PrismaService] })
export class PrismaModule {}
```

---

## References

`base/nestjs/` · `src/auth/` (Guard+Strategy+Service) · `src/common/` (Decorator+Filter+Interceptor) · `test/auth.e2e-spec.ts` (Supertest E2E 11 cases)
