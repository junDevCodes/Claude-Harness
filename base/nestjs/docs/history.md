# History — base/nestjs/ 작업 맥락 문서

> **문서 역할:** 이전까지 진행된 작업 내용과 다음 세션 맥락을 기록한다.

---

## 세션 1 — 2026-03-02 (전체 구현 완료)

### 배경

- **Phase 1-B** 신규 베이스 코드 생성 작업 (병렬 작업 중 NestJS 담당 세션)
- `base/nextjs/`를 구조 참고용으로 분석 후 NestJS 패턴으로 재해석
- 이 세션에서 `base/nestjs/` 최초 생성 및 전체 구현 완료

### 참고한 기존 코드

- `base/nextjs/` — 4문서 체계, api.ts 자동 갱신 패턴 참고
- `base/fastapi/` — JWT 인증 흐름 (access/refresh), Repository 패턴 참고
- `base/spring-boot/` — 표준 응답 래핑 패턴(ApiResponse) 참고

### 주요 결정 사항

#### 기술 스택 결정

| 항목 | 결정 | 이유 |
|---|---|---|
| NestJS 버전 | 10.x | 안정 LTS, Prisma 5와 호환성 확보 |
| ORM | Prisma 5 | TypeScript 친화적, 타입 자동 생성 |
| JWT 라이브러리 | @nestjs/jwt + passport-jwt | NestJS 공식 패키지 |
| Refresh 전략 | DB 저장 (Rotate 전략, bcrypt hash) | 보안성 우수, 탈취 시 무효화 가능 |
| 응답 형식 | TransformInterceptor 래핑 | `{ success, data, timestamp }` 전역 통일 |
| 예외 처리 | HttpExceptionFilter | `{ success: false, statusCode, message, path, timestamp }` |
| 테스트 | Supertest E2E | 전체 인증 흐름 11케이스 커버 |

#### 아키텍처 결정

- **Repository 패턴** 도입: Controller → Service → Repository → Prisma
- **@CurrentUser() 데코레이터**: Guard 이후 JWT 페이로드 추출
- **ConfigModule (isGlobal)**: 전역 설정, .env 자동 로드
- **PrismaModule (@Global())**: 한 번 등록으로 전체 모듈 주입

#### Refresh Token 전략 상세

```
로그인: refresh token 생성 → bcrypt hash → DB.hashedRefreshToken 저장
갱신 요청: JwtRefreshGuard → Strategy.validate() → DB 해시 비교 → 새 토큰 발급
로그아웃: DB.hashedRefreshToken = null 설정
```

#### 모듈 의존성 구조

```
AppModule
  ├── ConfigModule (isGlobal)
  ├── PrismaModule (isGlobal)
  ├── AuthModule
  │   └── imports: [UserModule, PassportModule, JwtModule]
  └── UserModule
      └── exports: [UserService, UserRepository]
```

### 생성된 파일 목록

```
base/nestjs/
├── docs/ (4문서)
├── prisma/schema.prisma
├── src/
│   ├── app.module.ts
│   ├── main.ts
│   ├── auth/ (9파일)
│   ├── user/ (6파일)
│   ├── prisma/ (2파일)
│   └── common/ (3파일)
├── test/
│   ├── auth.e2e-spec.ts (11 테스트 케이스)
│   └── jest-e2e.json
├── .env.example
├── .gitignore
├── Dockerfile
├── docker-compose.yaml
├── nest-cli.json
├── package.json
├── README.md
└── tsconfig.json
```

### Definition of Done 충족 확인

| 항목 | 결과 |
|---|---|
| docs/ 4문서 | ✅ |
| JWT 인증 (access + refresh) | ✅ |
| User CRUD 예시 | ✅ |
| .env.example | ✅ |
| Dockerfile + docker-compose.yaml | ✅ |
| 테스트 템플릿 (e2e 11케이스) | ✅ |
| README.md | ✅ |

---

## 다음 세션 맥락

### 추가 개선 가능 항목 (선택)

- 단위 테스트 추가 (auth.service.spec.ts, user.service.spec.ts — Mockito 스타일 Jest)
- S3 파일 업로드 유틸리티 추가 (`src/common/s3/`)
- Role-based 접근 제어 추가 (`@Roles()` 데코레이터, `RolesGuard`)
- 이메일 인증 플로우 추가 (nodemailer 연동)

### Phase 1-C 진입 조건

- Phase 1-B 7개 스택 모두 완료 후 루트 docs/ 일괄 업데이트 진행
- 현재: `base/nestjs/` ✅ 완료

---

*Last updated: 2026-03-02 (전체 구현 완료)*
