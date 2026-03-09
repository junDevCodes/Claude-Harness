# Checklist — base/nestjs/ 완료 기준 체크리스트

> **문서 역할:** Definition of Done 항목을 점검하는 작업 확인서.
> 작업 완료 시 각 항목을 체크하고 최종 검증한다.

---

## 공통 필수 항목

| 항목 | 상태 |
|---|---|
| `docs/plan.md` 존재 및 내용 작성 | ✅ |
| `docs/task.md` 존재 및 내용 작성 | ✅ |
| `docs/history.md` 존재 및 내용 작성 | ✅ |
| `docs/checklist.md` 존재 및 내용 작성 | ✅ |
| `README.md` — 구조/실행/커스터마이징 포함 | ✅ |

---

## 웹 백엔드 필수 항목

### 인증 (JWT)

| 항목 | 상태 |
|---|---|
| access token 발급 (15분 만료) | ✅ |
| refresh token 발급 (7일 만료) | ✅ |
| refresh token DB 저장 (bcrypt hash) | ✅ |
| JWT Guard (access) — `@UseGuards(JwtAuthGuard)` | ✅ |
| JWT Refresh Guard — `@UseGuards(JwtRefreshGuard)` | ✅ |
| POST /auth/register | ✅ |
| POST /auth/login | ✅ |
| POST /auth/refresh | ✅ |
| POST /auth/logout | ✅ |
| GET /auth/me | ✅ |

### User CRUD 예시

| 항목 | 상태 |
|---|---|
| GET /users/me (현재 사용자 조회) | ✅ |
| PATCH /users/me (프로필 수정) | ✅ |
| DELETE /users/me (계정 삭제) | ✅ |
| Repository 패턴 (UserRepository) | ✅ |

### 공통 인프라

| 항목 | 상태 |
|---|---|
| GlobalExceptionFilter (표준 에러 응답) | ✅ |
| TransformInterceptor (응답 래핑) | ✅ |
| ValidationPipe (class-validator) | ✅ |
| Swagger 문서 (`/api`) | ✅ |
| ConfigModule (.env 로드) | ✅ |
| PrismaModule (Global) | ✅ |

### 환경 / 설정

| 항목 | 상태 |
|---|---|
| `.env.example` (전체 환경변수 목록) | ✅ |
| `Dockerfile` (멀티 스테이지) | ✅ |
| `docker-compose.yaml` (postgres + app) | ✅ |
| `prisma/schema.prisma` (User 모델) | ✅ |

### 테스트

| 항목 | 상태 |
|---|---|
| `test/auth.e2e-spec.ts` (register→login→me→refresh→logout 흐름) | ✅ |
| `test/jest-e2e.json` | ✅ |

---

## 최종 검증 체크

- [x] 모든 소스 파일 작성 완료
- [x] Prisma 스키마 (User 모델 + hashedRefreshToken)
- [x] JWT 양방향 전략 (access / refresh) 구현
- [x] Repository 패턴으로 Controller → Service → Repository → Prisma 분리
- [x] `@CurrentUser()` 데코레이터로 Guard 이후 사용자 정보 접근
- [x] 모든 응답 `{ success, data, timestamp }` 형식 통일
- [x] E2E 테스트 전체 흐름 커버 (11개 케이스)
- [x] Docker 멀티 스테이지 빌드 (non-root 사용자)
- [x] README.md 커스터마이징 포인트 포함

---

*Created: 2026-03-02*
*Completed: 2026-03-02*
