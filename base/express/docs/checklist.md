# Checklist — base/express/ 완료 기준

> **문서 역할:** Definition of Done 충족 여부를 체크하는 최종 검증 목록.

---

## 공통 필수 항목

- [x] `base/express/docs/plan.md` 존재 및 내용 채워져 있음
- [x] `base/express/docs/task.md` 존재 및 내용 채워져 있음
- [x] `base/express/docs/history.md` 존재 및 내용 채워져 있음
- [x] `base/express/docs/checklist.md` 존재 및 내용 채워져 있음
- [x] `README.md` — 프로젝트 구조, 실행 방법, 커스터마이징 포인트 포함

## 웹 백엔드 필수 항목

- [x] JWT 인증 구현
  - [x] access token 발급 (짧은 만료, 15분) — `src/utils/jwt.ts`
  - [x] refresh token 발급 (긴 만료, 30일) — `src/utils/jwt.ts`
  - [x] refresh → 새 access token 발급 API — `POST /api/v1/auth/refresh`
  - [x] refresh token DB 저장 + 무효화(revoke) 처리 — `user.repository.ts`
  - [x] Refresh Token Rotation — `auth.service.ts`
- [x] 기본 CRUD 예시 (User 엔티티)
  - [x] CREATE (회원가입) — `POST /api/v1/auth/sign-up`
  - [x] READ (유저 목록, 단건 조회) — `GET /api/v1/users`, `GET /api/v1/users/:id`
  - [x] UPDATE (유저 정보 수정) — `PATCH /api/v1/users/:id`
  - [x] DELETE (유저 삭제) — `DELETE /api/v1/users/:id`
- [x] `.env.example` — 전체 환경변수 목록 포함 (주석 포함)
- [x] `Dockerfile` 존재 — node:20-alpine 멀티스테이지 빌드
- [x] `docker-compose.yaml` — postgres + api 포함
- [x] 테스트 템플릿 — `tests/auth.test.ts` (Jest + supertest)

## 코드 품질

- [x] TypeScript strict mode 적용 (`tsconfig.json`)
- [x] 요청 유효성 검증 (Zod DTO) — `src/dtos/`, `src/middleware/validate.ts`
- [x] 일관된 에러 응답 형식 (`{ error: { code, message } }`)
- [x] 비밀번호 bcrypt 해싱 (bcryptjs, SALT_ROUNDS=12)
- [x] JWT 시크릿 환경변수 처리 — Zod config 검증
- [x] SQL Injection 방지 — Prisma ORM 사용

## 구조 완성도

- [x] DDD 구조: `domain/user/` 레이어 분리 (model / repository / service / auth.service)
- [x] Repository 패턴: `user.repository.ts` (DB 접근 추상화)
- [x] DTO 계층: `src/dtos/` (Zod 스키마 기반 입출력 분리)
- [x] 미들웨어: 인증(`auth.middleware.ts`), 에러(`error.handler.ts`), 검증(`validate.ts`)
- [x] 설정: 환경변수 Zod 검증 (`config.ts`) — 앱 시작 시 실패 방지

## 문서 완성도

- [x] `README.md` Quick Start 섹션 존재
- [x] `README.md` 디렉토리 구조 설명
- [x] `README.md` 커스터마이징 가이드 (새 도메인 추가 방법)
- [x] `.env.example` 주석으로 각 변수 설명

---

**Phase 1 최종 검증: 2026-03-01 — 모든 항목 충족, Definition of Done 완료** ✅

---

## Phase 2 체크리스트 — express-backend-guidelines 스킬

### 스킬 완성 기준 (Definition of Done)

- [x] `.claude/skills/express-backend-guidelines/SKILL.md` 파일 존재
- [x] SKILL.md **500줄 이하** (500줄 — P1/P2 품질 수정 반영)
- [x] YAML frontmatter: `name`, `description`, `triggers` 필드 포함 (triggers 15개)
- [x] 핵심 패턴 5개 + 각 패턴에 코드 예시
- [x] 트리거 키워드 15개 명시
- [x] 안티패턴 섹션 포함 (5개)

### 내용 품질 기준

- [x] DDD 구조 패턴 포함 (`domain/[entity]/` 레이어 분리)
- [x] JWT Refresh Token Rotation 패턴 포함
- [x] AppError 계층 패턴 포함 (정적 팩토리 메서드)
- [x] Zod DTO 유효성 검증 패턴 포함 (`validateBody`/`validateQuery`)
- [x] S3 presigned POST 패턴 포함 (AWS SDK v3)
- [x] Express 앱 팩토리 패턴 포함 (`createApp()`)
- [x] 에러 응답 형식 실제 코드와 일치 (`{ error: { code, message } }`)

### 병렬 세션 격리 규칙 준수

- [x] `skill-rules.json` 미수정 (Phase 2-D 전용)
- [x] `.claude/skills/README.md` 미수정 (Phase 2-D 전용)
- [x] 루트 `docs/` 미수정
- [x] 타 스킬 디렉토리 미수정

---

*Phase 2 체크리스트 추가: 2026-03-05*
*Last updated: 2026-03-05 (P1/P2 품질 점검 수정 완료)*
