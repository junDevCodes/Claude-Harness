# History — base/express/ 작업 맥락

> **문서 역할:** 작업 진행 과정 및 주요 결정 사항 기록.

---

## 현재 상태

- **작업 완료:** 2026-03-01
- **상태:** Definition of Done 충족, 구현 완료

---

## 작업 이력

### 2026-03-01 — 전체 구현 완료

**생성 파일 목록 (37개):**

```
docs/ 4문서 (plan, task, history, checklist)
.env.example, .gitignore
package.json, tsconfig.json
Dockerfile, docker-compose.yaml
README.md
prisma/schema.prisma
src/config/config.ts
src/db/prisma.ts
src/errors/app.error.ts, error.types.ts
src/utils/logger.ts, password.ts, jwt.ts, s3.ts
src/middleware/auth.middleware.ts, error.handler.ts, validate.ts
src/domain/shared/base.repository.ts
src/domain/user/user.model.ts, user.repository.ts, user.service.ts, auth.service.ts
src/dtos/auth.dto.ts, user.dto.ts
src/api/v1/routes/auth.routes.ts, user.routes.ts, index.ts
src/app.ts, src/server.ts
tests/helpers/test.app.ts
tests/auth.test.ts
```

---

## 주요 결정 사항

| 결정 | 내용 | 이유 |
|---|---|---|
| Prisma ORM | TypeORM/Sequelize 대신 Prisma 선택 | 타입 안전성, 마이그레이션 UX 우수 |
| Zod 유효성 검증 | class-validator 대신 Zod | 런타임 타입 추론, 프레임워크 독립적 |
| JWT Refresh Token Rotation | refresh 요청 시 기존 토큰 무효화 | 보안: 탈취된 토큰 재사용 감지 |
| refresh token DB 저장 | 메모리/쿠키 대신 DB | 서버 재시작 후에도 revoke 가능 |
| AWS SDK v3 | v2 대신 v3 | 모듈화, tree-shaking, 유지보수 |
| `@aws-sdk/s3-presigned-post` | presigned POST는 별도 패키지 | AWS SDK v3 패키지 분리 구조 |
| Pino 로거 | winston 대신 Pino | 성능 우수, JSON 로그 기본 지원 |
| ULID (refresh token ID) | UUID 대신 ULID | 정렬 가능, URL 안전, 충돌 낮음 |
| access token 15분 | 짧은 만료 | 보안: refresh token으로 갱신 |
| bcrypt SALT_ROUNDS=12 | 10 대신 12 | 보안 강화 (2026년 권장 기준) |
| Dockerfile multi-stage | deps → builder → runner | 이미지 크기 최소화 |

---

## 아키텍처 핵심 패턴

### 에러 처리 흐름

```
Controller (라우터) → try/catch → next(err)
    ↓
error.handler.ts
    ├── ZodError → 400 VALIDATION_ERROR
    ├── AppError (isOperational) → 해당 statusCode
    └── Unknown → 500 INTERNAL_SERVER_ERROR
```

### JWT 인증 흐름

```
POST /auth/login
    ↓
auth.service.ts → createTokens()
    ├── signAccessToken({ sub, email, role }) → 15분
    └── signRefreshToken({ sub, tokenId: ulid() }) → 30일
         ↓ DB 저장 (refresh_tokens 테이블)

POST /auth/refresh
    ↓
verifyRefreshToken() → payload.tokenId
    ↓ DB에서 조회 (유효성 확인)
    ↓ revokeRefreshToken(tokenId)  ← Rotation
    ↓ createTokens() → 새 access + refresh 발급
```

---

---

## 2026-03-05 — Phase 2: express-backend-guidelines 스킬 작성 (시작)

**현재 작업:**

- `base/express/` 소스 코드 전체 검토 완료
- `.claude/skills/express-backend-guidelines/SKILL.md` 작성 시작

**추출할 핵심 패턴:**

1. DDD 구조 — `domain/[entity]/{model,repository,service,auth.service}.ts`
2. Refresh Token Rotation — ULID tokenId, DB 저장, revokeRefreshToken()
3. AppError 계층 — 정적 팩토리 메서드, isOperational 플래그
4. Zod DTO 패턴 — `src/dtos/`, validate 미들웨어, fieldErrors flatten
5. S3 presigned POST — AWS SDK v3, `@aws-sdk/s3-presigned-post`

**병렬 세션 준수 항목:**
- `skill-rules.json` 수정 금지 → Phase 2-D 일괄 처리
- `.claude/skills/README.md` 수정 금지 → Phase 2-D 일괄 처리

---

*Last updated: 2026-03-05 (Phase 2 스킬 작성 완료)*

---

## 2026-03-05 — Phase 2: express-backend-guidelines 스킬 완성

**생성 파일:**
- `.claude/skills/express-backend-guidelines/SKILL.md` (489줄)

**검증 결과 (Definition of Done):**
- [x] SKILL.md 500줄 이하 (489줄)
- [x] YAML frontmatter (name, description, triggers 15개)
- [x] 핵심 패턴 5개 + 코드 예시 (DDD, JWT Rotation, AppError, Zod DTO, S3)
- [x] 안티패턴 섹션 (5개 anti-pattern)
- [x] skill-rules.json 수정 금지 준수 (Phase 2-D 대기)

**다음 세션:**
- Phase 2-D 일괄 세션에서 `skill-rules.json`에 `express-backend-guidelines` 트리거 규칙 추가
