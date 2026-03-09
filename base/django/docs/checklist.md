# Checklist — base/django/ 완료 기준

> **문서 역할:** Definition of Done 기준 항목별 점검표.

---

## 공통 필수 항목

- [x] `docs/plan.md` 존재 및 내용 완비
- [x] `docs/task.md` 존재 및 내용 완비
- [x] `docs/history.md` 존재 및 내용 완비
- [x] `docs/checklist.md` 존재 및 내용 완비
- [x] `README.md` — 구조, 실행 방법, 커스터마이징 포인트 포함

---

## 웹 백엔드 필수 항목

### JWT 인증

- [x] `POST /api/v1/auth/register/` — 회원가입 → access + refresh token 반환
- [x] `POST /api/v1/auth/login/` — 로그인 → access + refresh token 반환
- [x] `POST /api/v1/auth/logout/` — 로그아웃 → refresh token 블랙리스트 등록
- [x] `POST /api/v1/auth/token/refresh/` — access token 갱신
- [x] `GET /api/v1/auth/me/` — 인증 필요, 내 정보 반환
- [x] access token 만료 시간: 30분
- [x] refresh token 만료 시간: 7일

### 기본 CRUD (User)

- [x] `GET /api/v1/users/` — 사용자 목록 (관리자 권한)
- [x] `GET /api/v1/users/{id}/` — 사용자 상세
- [x] `PUT /api/v1/users/{id}/` — 사용자 수정 (본인 또는 관리자)
- [x] `PATCH /api/v1/users/{id}/` — 사용자 부분 수정
- [x] `DELETE /api/v1/users/{id}/` — 사용자 삭제 (관리자)

### 환경 파일

- [x] `.env.example` — 아래 변수 포함
  - [x] `DJANGO_SETTINGS_MODULE`
  - [x] `SECRET_KEY`
  - [x] `DATABASE_URL` 또는 `DB_HOST`, `DB_PORT`, `DB_NAME`, `DB_USER`, `DB_PASSWORD`
  - [x] `AWS_ACCESS_KEY_ID`
  - [x] `AWS_SECRET_ACCESS_KEY`
  - [x] `AWS_S3_BUCKET_NAME`
  - [x] `AWS_S3_REGION`
  - [x] `CORS_ALLOWED_ORIGINS`

### Docker

- [x] `Dockerfile` — multi-stage 빌드 (builder + runtime)
- [x] `docker-compose.yaml` — app + postgres 서비스 포함
- [x] `docker-compose.yaml` — volume 마운트 및 healthcheck 포함

---

## 코드 품질

- [x] `apps/core/exceptions.py` — 커스텀 예외 핸들러 등록
- [x] `apps/core/pagination.py` — 기본 페이지네이션 클래스
- [x] `apps/core/utils/s3.py` — `generate_presigned_post()` 구현
- [x] `config/settings/base.py` — DRF 기본 설정 포함
- [x] `config/settings/production.py` — HTTPS, 보안 헤더 설정 포함
- [x] `AUTH_USER_MODEL = 'users.User'` 설정 확인
- [x] 패스워드 해시 (Django 기본 PBKDF2 사용)

---

## 테스트

- [x] `apps/users/tests.py` 존재
- [x] 회원가입 성공 케이스 테스트
- [x] 로그인 성공 → JWT 발급 테스트
- [x] 토큰 갱신 테스트
- [x] 인증 없이 보호 라우트 접근 → 401 반환 테스트
- [x] 사용자 정보 조회 테스트 (GET /api/v1/auth/me/)

---

## 최종 검증

> 아래 항목은 실제 환경에서 직접 실행으로 검증 필요.

- [ ] `docker-compose up` 실행 후 서버 기동 확인
- [ ] `POST /api/v1/auth/register/` → 201 응답
- [ ] `POST /api/v1/auth/login/` → access + refresh token 포함 응답
- [ ] `GET /api/v1/auth/me/` (Authorization: Bearer {token}) → 200 응답
- [ ] `python manage.py test` → 전체 테스트 통과
- [ ] README.md 실행 방법대로 서버 기동 성공

---

---

## Phase 6 — `django-backend-guidelines` 스킬 작성 체크리스트 (진행 중)

> 병렬 세션 격리 규칙: `.claude/skills/django-backend-guidelines/`만 작성.

### 사전 확인

- [x] `skill-rules.json` 수정 금지 확인
- [x] `.claude/skills/README.md` 수정 금지 확인
- [x] 루트 `docs/` 4문서 수정 금지 확인

### SKILL.md 완성 기준 (Definition of Done)

- [x] `.claude/skills/django-backend-guidelines/SKILL.md` 생성
- [x] YAML frontmatter (`name`, `description`, `triggers`) 포함
- [x] 핵심 패턴 5개 + 각 코드 예시 포함
- [x] 트리거 키워드 10개 이상
- [x] 안티패턴 섹션 포함
- [x] 500줄 이하 확인 (452줄)

### 패턴별 체크

| 패턴 | 코드 예시 | 상태 |
|---|---|---|
| 커스텀 User 모델 (`AbstractBaseUser`) | 있음 | ✅ |
| DRF ViewSet + `get_permissions`, `get_serializer_class` | 있음 | ✅ |
| SimpleJWT 설정 + 뷰 흐름 + Token Blacklist | 있음 | ✅ |
| `settings/` 환경 분리 전략 | 있음 | ✅ |
| 커스텀 예외 핸들러 (`custom_exception_handler`) | 있음 | ✅ |
| 안티패턴 명시 | — | ✅ |

---

*Last updated: 2026-03-05 (Phase 6 스킬 작성 착수)*
