# Task — base/express/ 상세 실행 계획

> **문서 역할:** `plan.md`에서 정의한 구성 요소를 순서대로 구현하는 세부 작업 목록.

---

## 작업 순서

### Step 1 — 프로젝트 초기화 ✅

- [x] `package.json` — 의존성 정의 (express, prisma, zod, jsonwebtoken, bcryptjs 등)
- [x] `tsconfig.json` — TypeScript strict mode 설정
- [x] `.gitignore` — node_modules, .env, dist/ 제외
- [x] `.env.example` — 전체 환경변수 목록 (주석 포함)

### Step 2 — 설정 & DB 레이어 ✅

- [x] `src/config/config.ts` — Zod env 검증
- [x] `src/db/prisma.ts` — PrismaClient 싱글톤
- [x] `prisma/schema.prisma` — User 모델 + RefreshToken 모델

### Step 3 — 에러 레이어 ✅

- [x] `src/errors/error.types.ts` — 에러 코드 enum
- [x] `src/errors/app.error.ts` — AppError 클래스 (정적 팩토리 메서드 포함)

### Step 4 — 유틸리티 ✅

- [x] `src/utils/logger.ts` — Pino 로거
- [x] `src/utils/password.ts` — bcrypt hash/compare
- [x] `src/utils/jwt.ts` — access/refresh 토큰 생성/검증
- [x] `src/utils/s3.ts` — presigned POST/GET, delete (AWS SDK v3)

### Step 5 — 미들웨어 ✅

- [x] `src/middleware/error.handler.ts` — Express 에러 핸들러 (ZodError + AppError)
- [x] `src/middleware/auth.middleware.ts` — JWT 검증 미들웨어 + requireRole
- [x] `src/middleware/validate.ts` — Zod 요청 검증 미들웨어

### Step 6 — Domain 레이어 ✅

- [x] `src/domain/shared/base.repository.ts` — Generic CRUD 인터페이스
- [x] `src/domain/user/user.model.ts` — User 타입 정의 + toSafeUser()
- [x] `src/domain/user/user.repository.ts` — Prisma 기반 User CRUD + RefreshToken
- [x] `src/domain/user/user.service.ts` — User 비즈니스 로직
- [x] `src/domain/user/auth.service.ts` — JWT 인증 + Refresh Token Rotation

### Step 7 — DTO 레이어 ✅

- [x] `src/dtos/auth.dto.ts` — 회원가입/로그인/갱신 Zod 스키마
- [x] `src/dtos/user.dto.ts` — 유저 수정 + 페이지네이션 Zod 스키마

### Step 8 — API 라우터 ✅

- [x] `src/api/v1/routes/auth.routes.ts` — Auth 라우터 (5개 엔드포인트)
- [x] `src/api/v1/routes/user.routes.ts` — User 라우터 (4개 엔드포인트)
- [x] `src/api/v1/routes/index.ts` — v1 라우터 통합
- [x] `src/app.ts` — Express 앱 팩토리 (cors, helmet, pino-http)
- [x] `src/server.ts` — 서버 엔트리포인트 (graceful shutdown)

### Step 9 — 인프라 ✅

- [x] `Dockerfile` — node:20-alpine 멀티스테이지 빌드 (deps/builder/runner)
- [x] `docker-compose.yaml` — postgres + api 서비스 (healthcheck 포함)
- [x] `README.md` — Quick Start / 구조 / 커스터마이징 가이드

### Step 10 — 테스트 ✅

- [x] `tests/helpers/test.app.ts` — 테스트용 앱 팩토리
- [x] `tests/auth.test.ts` — Auth 통합 테스트 (회원가입/로그인/me/refresh/logout/health)

---

## 완료 기준

- [x] 모든 소스 파일 생성 완료 (37개 파일)
- [x] TypeScript strict mode 설정
- [x] Definition of Done 체크리스트 100% 충족

---

**Phase 1 작업 완료: 2026-03-01** ✅

---

## Phase 2 — express-backend-guidelines 스킬 작성 (현재)

> **목표:** `base/express/`의 패턴을 `.claude/skills/express-backend-guidelines/SKILL.md`로 정제.
> Phase 2-B 병렬 세션 규칙 준수 — `skill-rules.json` 수정 금지, 담당 디렉토리만 작성.

### Step 1 — 스킬 파일 생성 ✅

- [x] `.claude/skills/express-backend-guidelines/SKILL.md` 작성 (489줄)
  - YAML frontmatter (name, description, triggers 15개)
  - 핵심 패턴 5개 + 코드 예시
  - 트리거 키워드 15개
  - 안티패턴 섹션 (5개)

### Step 2 — 완성 기준 검증 ✅

- [x] SKILL.md 줄 수 확인 (489줄 / 500줄 이하)
- [x] YAML frontmatter 필수 필드 확인
- [x] 핵심 패턴 5개 + 코드 예시 각각 포함 확인
- [x] 트리거 키워드 15개 확인
- [x] 안티패턴 섹션 포함 확인

---

**Phase 2 완료: 2026-03-05** ✅

*Phase 2 시작: 2026-03-05*
*Last updated: 2026-03-05 (완료)*
