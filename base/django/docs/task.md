# Task — base/django/ 세부 작업 계획

> **문서 역할:** `plan.md`에서 정의한 Phase별 작업의 **세부 실행 항목**.

---

## Phase 1 — 문서 작성 ✅ 완료

| 항목 | 상태 |
|---|---|
| `docs/plan.md` 작성 | ✅ |
| `docs/task.md` 작성 | ✅ |
| `docs/history.md` 작성 | ✅ |
| `docs/checklist.md` 작성 | ✅ |

---

## Phase 2 — 프로젝트 기반 ✅ 완료

| 항목 | 파일 | 상태 |
|---|---|---|
| Django 프로젝트 설정 공통 | `config/settings/base.py` | ✅ |
| Django 개발 환경 설정 | `config/settings/development.py` | ✅ |
| Django 프로덕션 설정 | `config/settings/production.py` | ✅ |
| 루트 URL 라우터 | `config/urls.py` | ✅ |
| WSGI 엔트리포인트 | `config/wsgi.py` | ✅ |
| ASGI 엔트리포인트 | `config/asgi.py` | ✅ |
| Django 관리 스크립트 | `manage.py` | ✅ |
| 설정 패키지 init | `config/__init__.py`, `config/settings/__init__.py` | ✅ |

---

## Phase 3 — 핵심 앱 구현 ✅ 완료

### 3-1. core 앱 (공통 유틸리티)

| 항목 | 파일 | 상태 |
|---|---|---|
| 앱 패키지 초기화 | `apps/core/__init__.py` | ✅ |
| 커스텀 예외 클래스 + DRF 예외 핸들러 | `apps/core/exceptions.py` | ✅ |
| 커스텀 페이지네이션 | `apps/core/pagination.py` | ✅ |
| S3 유틸리티 (presigned POST) | `apps/core/utils/s3.py` | ✅ |
| utils 패키지 초기화 | `apps/core/utils/__init__.py` | ✅ |

### 3-2. users 앱

| 항목 | 파일 | 상태 |
|---|---|---|
| 앱 패키지 초기화 | `apps/users/__init__.py` | ✅ |
| CustomUser 모델 (AbstractBaseUser + email 로그인) | `apps/users/models.py` | ✅ |
| CustomUserManager | `apps/users/managers.py` | ✅ |
| Django Admin 등록 | `apps/users/admin.py` | ✅ |
| 앱 설정 | `apps/users/apps.py` | ✅ |
| 회원가입/조회/수정 시리얼라이저 | `apps/users/serializers.py` | ✅ |
| 인증 뷰 (register, login, logout, me) | `apps/users/views.py` | ✅ |
| 사용자 CRUD 뷰셋 | `apps/users/views.py` | ✅ |
| 커스텀 권한 클래스 | `apps/users/permissions.py` | ✅ |
| URL 라우터 | `apps/users/urls.py` | ✅ |

---

## Phase 4 — 인프라 & 환경 ✅ 완료

| 항목 | 파일 | 상태 |
|---|---|---|
| 프로덕션 의존성 | `requirements.txt` | ✅ |
| 개발 의존성 | `requirements-dev.txt` | ✅ |
| Docker 이미지 빌드 (multi-stage) | `Dockerfile` | ✅ |
| 로컬 개발 환경 컨테이너 | `docker-compose.yaml` | ✅ |
| 환경변수 샘플 | `.env.example` | ✅ |
| pytest 설정 | `pytest.ini` | ✅ |
| gitignore | `.gitignore` | ✅ |
| 프로젝트 설명서 | `README.md` | ✅ |

---

## Phase 5 — 테스트 ✅ 완료

| 항목 | 파일 | 상태 |
|---|---|---|
| 회원가입 테스트 (성공, 중복, 비밀번호 불일치) | `apps/users/tests.py` | ✅ |
| 로그인 (JWT 발급, 실패, 비활성) 테스트 | `apps/users/tests.py` | ✅ |
| 토큰 갱신 테스트 | `apps/users/tests.py` | ✅ |
| 로그아웃 (블랙리스트) 테스트 | `apps/users/tests.py` | ✅ |
| Me 엔드포인트 (인증/비인증) 테스트 | `apps/users/tests.py` | ✅ |
| 사용자 CRUD (권한 포함) 테스트 | `apps/users/tests.py` | ✅ |

---

## Definition of Done 체크

| 기준 | 충족 여부 |
|---|---|
| `docs/` 4문서 존재 및 내용 있음 | ✅ |
| JWT 인증 (access + refresh) 구현 | ✅ |
| 기본 CRUD 예시 (User) 구현 | ✅ |
| `.env.example` 완비 | ✅ |
| `Dockerfile` + `docker-compose.yaml` 포함 | ✅ |
| 테스트 템플릿 1개 이상 | ✅ |
| `README.md` 포함 | ✅ |

---

## Phase 6 — `django-backend-guidelines` 스킬 작성 (진행 중)

> `base/django/` 베이스 코드 기반으로 Claude Code Skill 작성.
> 담당 세션은 `.claude/skills/django-backend-guidelines/` 디렉토리만 작성.

### 스킬 파일 구성

| 파일 | 내용 | 상태 |
|---|---|---|
| `.claude/skills/django-backend-guidelines/SKILL.md` | 메인 스킬 (452줄) | ✅ |

### 수록 패턴 항목

| 패턴 | 참조 파일 | 상태 |
|---|---|---|
| YAML frontmatter (name, description, triggers 10개+) | — | ✅ |
| 커스텀 User 모델 (`AbstractBaseUser`) + 코드 예시 | `apps/users/models.py` | ✅ |
| DRF ViewSet 패턴 (`get_permissions`, `get_serializer_class`) + 코드 예시 | `apps/users/views.py` | ✅ |
| SimpleJWT 인증 흐름 (settings, view, blacklist) + 코드 예시 | `config/settings/base.py`, `apps/users/views.py` | ✅ |
| settings/ 환경 분리 전략 + 코드 예시 | `config/settings/` | ✅ |
| 커스텀 예외 핸들러 패턴 + 코드 예시 | `apps/core/exceptions.py` | ✅ |
| 안티패턴 섹션 | — | ✅ |
| 500줄 이하 확인 (452줄) | — | ✅ |

### 완료 기준 (Definition of Done)

- [ ] `SKILL.md` 존재 및 500줄 이하
- [ ] YAML frontmatter: `name`, `description`, `triggers` 포함
- [ ] 핵심 패턴 3개 이상 + 각 패턴 코드 예시
- [ ] 트리거 키워드 5개 이상
- [ ] 안티패턴 섹션 포함
- [ ] `skill-rules.json` 미수정 (Phase 2-D 전용)

---

*Last updated: 2026-03-05 (Phase 6 스킬 작성 착수)*
