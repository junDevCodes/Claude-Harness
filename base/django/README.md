# base/django — Django 5 + DRF + SimpleJWT 베이스 코드

범용 Django 백엔드 베이스 코드. 새 프로젝트 시작 시 이 구조를 복사하여 도메인 앱만 추가하면 된다.

---

## 스택

| 항목 | 버전/라이브러리 |
|---|---|
| Python | 3.12 |
| Django | 5.1 |
| Django REST Framework | 3.15 |
| JWT 인증 | djangorestframework-simplejwt 5.3 |
| 데이터베이스 | PostgreSQL 16 (psycopg 3) |
| 파일 스토리지 | AWS S3 (boto3) |
| 컨테이너 | Docker + docker-compose |
| 프로덕션 서버 | Gunicorn |

---

## 프로젝트 구조

```
base/django/
├── config/
│   ├── settings/
│   │   ├── base.py          # 공통 설정
│   │   ├── development.py   # 개발 환경
│   │   └── production.py    # 프로덕션 환경
│   ├── urls.py              # 루트 URL 라우터
│   ├── wsgi.py
│   └── asgi.py
├── apps/
│   ├── core/                # 공통 유틸리티
│   │   ├── exceptions.py    # 커스텀 예외 핸들러
│   │   ├── pagination.py    # 페이지네이션
│   │   └── utils/s3.py      # S3 유틸리티
│   └── users/               # 사용자 도메인
│       ├── models.py        # CustomUser (AbstractBaseUser)
│       ├── managers.py      # UserManager
│       ├── serializers.py   # 시리얼라이저
│       ├── views.py         # Auth + CRUD 뷰
│       ├── urls.py          # URL 라우터
│       ├── permissions.py   # 커스텀 권한
│       └── tests.py         # 테스트
├── manage.py
├── requirements.txt
├── requirements-dev.txt
├── Dockerfile
├── docker-compose.yaml
└── .env.example
```

---

## API 엔드포인트

```
POST   /api/v1/auth/register/       회원가입 → access + refresh token 반환
POST   /api/v1/auth/login/          로그인 → access + refresh token 반환
POST   /api/v1/auth/logout/         로그아웃 → refresh token 블랙리스트
POST   /api/v1/auth/token/refresh/  access token 갱신
GET    /api/v1/auth/me/             내 정보 조회 (인증 필요)

GET    /api/v1/users/               사용자 목록 (관리자 전용)
GET    /api/v1/users/{id}/          사용자 상세 (본인 또는 관리자)
PUT    /api/v1/users/{id}/          사용자 수정 (본인 또는 관리자)
PATCH  /api/v1/users/{id}/          사용자 부분 수정
DELETE /api/v1/users/{id}/          사용자 삭제 (관리자 전용)
```

---

## 빠른 시작

### 1. Docker Compose로 실행 (권장)

```bash
cp .env.example .env
# .env 파일 값 수정 (SECRET_KEY 등)

docker-compose up --build
```

서버: http://localhost:8000

### 2. 로컬 개발 환경

```bash
# 가상환경 생성 및 의존성 설치
python -m venv .venv
source .venv/bin/activate        # Windows: .venv\Scripts\activate
pip install -r requirements-dev.txt

# 환경변수 설정
cp .env.example .env
# .env 파일 수정

# PostgreSQL 실행 (docker-compose db 서비스만 별도 실행)
docker-compose up db -d

# 마이그레이션 및 개발 서버
python manage.py migrate
python manage.py createsuperuser
python manage.py runserver
```

서버: http://localhost:8000
Admin: http://localhost:8000/admin/

---

## 테스트 실행

```bash
# Django test runner
python manage.py test apps.users -v 2

# pytest-django (더 상세한 출력)
pytest apps/users/tests.py -v

# 커버리지 포함
pytest --cov=apps --cov-report=term-missing
```

---

## 코드 품질

```bash
# 포맷팅
black .
isort .

# 린팅
ruff check --fix .

# 타입 체크
mypy .
```

---

## 커스터마이징 포인트

### 새 도메인 앱 추가

```bash
python manage.py startapp {domain_name}
mv {domain_name} apps/{domain_name}
```

`config/settings/base.py`에 `LOCAL_APPS`에 `apps.{domain_name}` 추가.

### User 모델 확장

`apps/users/models.py`의 `User` 클래스에 필드 추가:

```python
# 직접 필드 추가
avatar_url = models.URLField(blank=True)
bio = models.TextField(blank=True)

# 또는 별도 Profile 모델 생성
class Profile(models.Model):
    user = models.OneToOneField(User, on_delete=models.CASCADE, related_name="profile")
    avatar_url = models.URLField(blank=True)
```

### JWT 토큰 만료 시간 변경

`config/settings/base.py`:

```python
SIMPLE_JWT = {
    "ACCESS_TOKEN_LIFETIME": timedelta(minutes=60),   # 기본: 30분
    "REFRESH_TOKEN_LIFETIME": timedelta(days=30),     # 기본: 7일
}
```

### S3 파일 업로드 흐름

```python
from apps.core.utils.s3 import generate_presigned_post

# 1. 클라이언트가 요청
result = generate_presigned_post(
    object_key="uploads/users/avatar.jpg",
    content_type="image/jpeg",
    max_size_mb=10,
)
# 2. result["url"] + result["fields"]를 클라이언트에 반환
# 3. 클라이언트가 S3에 직접 POST 업로드
```

---

## 프로덕션 배포 체크리스트

- [ ] `SECRET_KEY` 강력한 랜덤 문자열로 변경
- [ ] `DJANGO_SETTINGS_MODULE=config.settings.production` 설정
- [ ] `ALLOWED_HOSTS` 실제 도메인으로 설정
- [ ] `DATABASE_URL` RDS 등 프로덕션 DB로 설정
- [ ] `DEBUG=False` 확인 (production.py에서 자동 설정)
- [ ] HTTPS 인증서 설정 (Nginx/ALB)
- [ ] AWS S3 버킷 + IAM 권한 설정
- [ ] `python manage.py collectstatic` 실행
