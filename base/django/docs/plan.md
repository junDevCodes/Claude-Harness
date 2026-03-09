# Plan — base/django/ 베이스 코드 구성 계획

> **문서 역할:** `base/django/` Django 5 + DRF + SimpleJWT 베이스 코드의 전체 구성 계획서.
> 세부 작업은 `task.md`, 맥락은 `history.md`, 점검은 `checklist.md`에서 관리한다.

---

## 스택 목표

**Django 5** + **DRF** + **SimpleJWT** + **PostgreSQL** + **S3** + **Docker** 기반의
범용 백엔드 베이스 코드. 새 프로젝트 시작 시 비즈니스 도메인만 추가하면 즉시 운영 가능한
수준의 인증/CRUD/인프라 구조를 제공한다.

---

## 디렉토리 구조

```
base/django/
├── docs/                         ← 4문서 체계
│   ├── plan.md
│   ├── task.md
│   ├── history.md
│   └── checklist.md
├── config/                       ← Django 프로젝트 설정
│   ├── __init__.py
│   ├── settings/
│   │   ├── __init__.py
│   │   ├── base.py               ← 공통 설정
│   │   ├── development.py        ← 개발 환경 설정
│   │   └── production.py         ← 프로덕션 환경 설정
│   ├── urls.py                   ← 루트 URL 라우터
│   ├── wsgi.py
│   └── asgi.py
├── apps/
│   ├── users/                    ← 사용자 도메인 (CustomUser)
│   │   ├── __init__.py
│   │   ├── admin.py
│   │   ├── apps.py
│   │   ├── models.py             ← AbstractBaseUser 기반 CustomUser
│   │   ├── managers.py           ← CustomUserManager
│   │   ├── serializers.py        ← 회원가입/조회/수정 시리얼라이저
│   │   ├── views.py              ← DRF ViewSet
│   │   ├── urls.py
│   │   └── tests.py              ← 단위 + 통합 테스트
│   └── core/                     ← 공통 유틸리티
│       ├── __init__.py
│       ├── exceptions.py         ← 커스텀 예외 + 핸들러
│       ├── pagination.py         ← 기본 페이지네이션
│       └── utils/
│           ├── __init__.py
│           └── s3.py             ← S3 파일 업로드 유틸리티
├── manage.py
├── requirements.txt              ← 프로덕션 의존성
├── requirements-dev.txt          ← 개발 의존성 추가
├── Dockerfile
├── docker-compose.yaml
├── .env.example
└── README.md
```

---

## 핵심 설계 결정

### 인증 전략

- **SimpleJWT** 사용: access token (30분) + refresh token (7일)
- **Custom User Model** (`AbstractBaseUser`): email을 username 대신 사용
- **Token Blacklist** 적용: 로그아웃 시 refresh token 무효화
- 토큰 갱신 엔드포인트: `POST /api/v1/auth/token/refresh/`

### 설정 분리 전략

- `config/settings/base.py` — 공통 (앱, 미들웨어, DRF, JWT 설정)
- `config/settings/development.py` — SQLite or local PostgreSQL, DEBUG=True
- `config/settings/production.py` — RDS PostgreSQL, SSL, DEBUG=False
- `DJANGO_SETTINGS_MODULE` 환경변수로 선택

### 데이터베이스

- PostgreSQL (psycopg 3 - async 지원)
- production.py에서 SSL 연결 옵션 포함

### S3 연동

- presigned POST URL 방식 (직접 업로드)
- boto3 기반 `apps/core/utils/s3.py`
- `generate_presigned_post()` 유틸리티 제공

### API 구조

```
/api/v1/
├── auth/
│   ├── register/         POST — 회원가입
│   ├── login/            POST — 로그인 (access + refresh 반환)
│   ├── logout/           POST — 로그아웃 (refresh 블랙리스트)
│   ├── token/refresh/    POST — access token 갱신
│   └── me/               GET  — 내 정보 조회
└── users/
    ├── /                 GET  — 사용자 목록 (관리자)
    ├── {id}/             GET  — 사용자 상세
    ├── {id}/             PUT/PATCH — 사용자 수정
    └── {id}/             DELETE — 사용자 삭제
```

---

## Phase 구성

### Phase 1 — 문서 작성 ✅ 완료
- [x] `docs/plan.md` 작성
- [x] `docs/task.md` 작성
- [x] `docs/history.md` 작성
- [x] `docs/checklist.md` 작성

### Phase 2 — 프로젝트 기반 ✅ 완료
- [x] Django 프로젝트 초기화 (`config/`)
- [x] 설정 분리 (`base`, `development`, `production`)
- [x] `manage.py`, `wsgi.py`, `asgi.py`

### Phase 3 — 핵심 앱 구현 ✅ 완료
- [x] `apps/core/` — 예외, 페이지네이션, S3 유틸리티
- [x] `apps/users/` — CustomUser 모델, 인증, CRUD

### Phase 4 — 인프라 & 환경 ✅ 완료
- [x] `requirements.txt` / `requirements-dev.txt`
- [x] `Dockerfile` / `docker-compose.yaml`
- [x] `.env.example`
- [x] `README.md`

### Phase 5 — 테스트 ✅ 완료
- [x] `apps/users/tests.py` — 인증 + CRUD 테스트 (18개 케이스)

---

### Phase 6 — `django-backend-guidelines` 스킬 작성 (현재 Phase)

> **목적:** `base/django/` 베이스 코드의 핵심 패턴을 Claude Code Skill 형태로 추출하여
> `.claude/skills/django-backend-guidelines/SKILL.md`로 작성한다.

**스킬 수록 핵심 패턴 5종:**

1. **커스텀 User 모델** — `AbstractBaseUser` + `UserManager`, email 로그인 식별자
2. **DRF ViewSet + Serializer** — `ModelViewSet`, `get_serializer_class()`, `get_permissions()`
3. **SimpleJWT 인증** — settings, `RegisterView`/`LoginView`, Token Blacklist 로그아웃
4. **settings/ 환경 분리** — `base.py`, `development.py`, `production.py` 서브패키지 전략
5. **커스텀 예외 핸들러** — `custom_exception_handler`, `APIException` 서브클래스

**병렬 세션 작업 범위:**
- 담당 세션은 `.claude/skills/django-backend-guidelines/` 디렉토리만 생성·수정
- `skill-rules.json` 수정 금지 (Phase 2-D 일괄 세션 전용)
- `docs/` 루트 파일 수정 금지

---

*Last updated: 2026-03-05 (Phase 1~5 완료 → Phase 6 스킬 작성 착수)*
