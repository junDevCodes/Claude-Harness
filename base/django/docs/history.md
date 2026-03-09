# History — base/django/ 작업 맥락

> **문서 역할:** 이전까지 진행된 작업 내용과 다음 세션을 위한 맥락 기록.

---

## 2026-03-01 — Phase 1: 계획 수립 및 문서 작성

### 배경

- `docs/plan.md` Phase 1-B 작업 항목 중 `base/django/` 구현 착수
- 기존 완성된 `base/fastapi/` 구조를 참고하여 Django 5 + DRF 방식으로 재설계
- 루트 `docs/`는 수정하지 않고 `base/django/docs/`만 작업

### 설계 결정 사항

#### 1. CustomUser 모델 (`AbstractBaseUser`)
- `username` 대신 `email`을 로그인 식별자로 사용
- `AbstractBaseUser` 사용 (Django 기본 `AbstractUser`보다 유연)
- 필드: `email`, `name`, `role` (USER/ADMIN), `is_active`, `created_at`, `updated_at`
- `AUTH_USER_MODEL = 'users.User'` 설정 필수

#### 2. 설정 분리 (`config/settings/`)
- `base.py`: 공통 (INSTALLED_APPS, DRF, JWT, CORS 등)
- `development.py`: DEBUG=True, SQLite 또는 로컬 PostgreSQL
- `production.py`: DEBUG=False, RDS PostgreSQL, SSL, 보안 헤더
- 환경변수 `DJANGO_SETTINGS_MODULE`로 선택

#### 3. JWT 인증 (SimpleJWT)
- access token: 30분
- refresh token: 7일
- Token Blacklist 앱으로 로그아웃 시 refresh 무효화
- 커스텀 응답 형식 (DRF 기본 응답 대신 통일된 JSON 구조)

#### 4. DRF 구조
- `APIView` 기반 인증 뷰 (register, login, logout, me)
- `ModelViewSet` 기반 사용자 CRUD
- `DefaultRouter`로 URL 자동 생성

#### 5. S3 연동
- boto3 presigned POST 방식 (FastAPI 베이스와 동일)
- `apps/core/utils/s3.py`에 `generate_presigned_post()` 구현

#### 6. 테스트
- Django `TestCase` + DRF `APITestCase` 혼용
- Pytest-django도 `requirements-dev.txt`에 포함

### 파악한 fastapi 베이스 구조 참고 패턴
- DDD 방식 → Django는 앱 기반이므로 `apps/users/`, `apps/core/` 구조 채택
- 설정 분리 → `config/settings/` 서브패키지 방식
- S3 유틸리티 → `apps/core/utils/s3.py`로 동일 패턴 유지

---

## 2026-03-01 — Phase 2~5: 전체 구현 완료

### 완료된 작업

**Phase 2 — 프로젝트 설정:**
- `config/settings/base.py`: INSTALLED_APPS, DRF, SimpleJWT, CORS 공통 설정
- `config/settings/development.py`: DATABASE_URL 또는 개별 DB 변수, BrowsableAPI 활성화
- `config/settings/production.py`: DEBUG=False, HTTPS 헤더, WhiteNoise static, RDS
- `config/urls.py`, `wsgi.py`, `asgi.py`, `manage.py`

**Phase 3 — 핵심 앱:**
- `apps/core/exceptions.py`: `custom_exception_handler` (DRF 기본 핸들러 교체), 커스텀 예외 5종
- `apps/core/pagination.py`: `StandardResultsSetPagination` + `LargeResultsSetPagination`
- `apps/core/utils/s3.py`: `generate_presigned_post()`, `generate_presigned_get()`, `delete_object()`
- `apps/users/models.py`: `AbstractBaseUser` 기반 `User` (email 로그인, role: USER/ADMIN)
- `apps/users/managers.py`: `UserManager` (create_user, create_superuser)
- `apps/users/serializers.py`: `RegisterSerializer`, `UserSerializer`, `UserUpdateSerializer`, `LoginSerializer`
- `apps/users/views.py`: `RegisterView`, `LoginView`, `LogoutView`, `MeView`, `UserViewSet`
- `apps/users/permissions.py`: `IsAdminUser`, `IsSelfOrAdmin`
- `apps/users/urls.py`: auth + users 라우터

**Phase 4 — 인프라:**
- `requirements.txt`: Django 5.1, DRF 3.15, SimpleJWT 5.3, psycopg 3, boto3, whitenoise, gunicorn
- `requirements-dev.txt`: pytest-django, factory-boy, ruff, black, mypy, django-stubs
- `Dockerfile`: multi-stage (builder + runtime), 비root 유저, collectstatic
- `docker-compose.yaml`: app + db(PostgreSQL 16), healthcheck, volume
- `.env.example`: 전체 환경변수 포함
- `pytest.ini`: pytest-django 기본 설정
- `README.md`: 구조, API, 실행법, 테스트, 커스터마이징 가이드

**Phase 5 — 테스트:**
- `apps/users/tests.py`: 18개 테스트 케이스
  - RegisterTests (4), LoginTests (4), TokenRefreshTests (2), LogoutTests (3), MeTests (2), UserCRUDTests (7)

---

## 다음 세션 진입 시 참고 사항

### 현재 상태
- **전체 구현 완료** — Definition of Done 전 항목 충족
- 최종 검증 (docker-compose 실행, manage.py test) 미수행

### 추가 커스터마이징 포인트
- `apps/users/models.py`에 프로필 필드 추가 또는 별도 Profile 모델 생성
- `config/urls.py`에 새 도메인 앱 URL include 추가
- `apps/users/views.py` `UserViewSet.get_queryset()`에 도메인 필터링 추가

### 주의 사항
- `AUTH_USER_MODEL` 이미 `base.py`에 포함됨 — 첫 마이그레이션 전 확인
- Token Blacklist 사용 → `INSTALLED_APPS`에 `rest_framework_simplejwt.token_blacklist` 포함 확인
- production.py는 `dj_database_url`을 import하므로 `requirements.txt`에 포함됨

---

*Last updated: 2026-03-01 (전체 구현 완료)*
