# FastAPI Base — 범용 스타터 킷

**FastAPI + SQLModel + PostgreSQL + JWT + S3** 기반 백엔드 스타터 킷.

새 프로젝트 시작 시 이 디렉토리를 복사하여 즉시 사용 가능한 API 서버를 구성한다.

---

## 기술 스택

| 항목 | 기술 |
|---|---|
| 언어 | Python 3.12.3 |
| 프레임워크 | FastAPI 0.116+ |
| ORM | SQLModel + SQLAlchemy (async) |
| DB 드라이버 | asyncpg (PostgreSQL) |
| 인증 | PyJWT (HS256) |
| 패스워드 | bcrypt |
| ID 생성 | ULID (prefix 포함, e.g. `usr_01ABC...`) |
| 파일 업로드 | AWS S3 (presigned POST) |
| 패키지 매니저 | uv |
| 컨테이너 | Docker + Docker Compose |

---

## 프로젝트 구조

```
base/fastapi/
├── backend/
│   ├── api/
│   │   └── v1/
│   │       └── routers/
│   │           ├── auth.py        # 인증 엔드포인트 (sign-up/login/refresh/me)
│   │           └── user.py        # User CRUD 엔드포인트
│   ├── core/
│   │   └── config.py              # Pydantic Settings (환경변수 관리)
│   ├── db/
│   │   └── orm.py                 # Read/Write 세션 팩토리
│   ├── domain/
│   │   ├── shared/
│   │   │   └── base_repository.py # Generic CRUD BaseRepository
│   │   └── user/
│   │       ├── model.py           # User SQLModel (soft delete)
│   │       ├── enums.py           # AuthTypeEnum, UserStatusEnum
│   │       ├── repository.py      # UserRepository (find_by_email, soft_delete)
│   │       ├── service.py         # UserService (비즈니스 로직)
│   │       └── auth_service.py    # AuthService + get_user_id dependency
│   ├── dtos/
│   │   ├── auth.py                # Login/SignUp/Refresh DTOs
│   │   └── user.py                # User 요청/응답 DTOs
│   ├── error/
│   │   └── __init__.py            # AppException 계층 정의
│   ├── middleware/
│   │   └── error_handler.py       # 전역 예외 핸들러
│   ├── utils/
│   │   ├── logger.py              # 로깅 설정
│   │   ├── password.py            # bcrypt 유틸리티
│   │   └── s3.py                  # S3 presigned URL 생성/삭제
│   └── main.py                    # 앱 팩토리 (create_application)
├── tests/
│   └── test_auth.py               # 인증 엔드포인트 테스트 템플릿
├── docs/                          # 4문서 (plan/task/history/checklist)
├── .env.example                   # 환경변수 목록
├── Dockerfile
├── docker-compose.yaml
├── pyproject.toml
└── README.md
```

---

## 빠른 시작

### 1. 환경 설정

```bash
cp .env.example .env
# .env 파일을 열어 DB 접속 정보, JWT 시크릿 키 등을 채운다
```

### 2. 로컬 실행 (uv)

```bash
# 가상환경 생성 및 패키지 설치
uv venv
source .venv/bin/activate  # Windows: .venv\Scripts\activate
uv pip install -e .
uv pip install -e .[dev]   # 개발 의존성 포함

# 개발 서버 실행
uvicorn backend.main:app --reload --port 8000
```

### 3. Docker Compose 실행

```bash
# DB + API 동시 실행
docker-compose up

# 빌드 포함
docker-compose up --build
```

### 4. API 확인

```
http://localhost:8000/docs        # Swagger UI (개발 환경만)
http://localhost:8000/api/v1/health  # Health check
```

---

## API 엔드포인트

### 인증 (`/api/v1/auth`)

| Method | Path | 설명 | 인증 |
|---|---|---|---|
| POST | `/auth/sign-up` | 이메일 회원가입 | 불필요 |
| POST | `/auth/login` | 이메일 로그인 | 불필요 |
| POST | `/auth/refresh` | 토큰 갱신 | refresh token |
| GET | `/auth/me` | 현재 사용자 정보 | access token |

### 사용자 (`/api/v1/users`)

| Method | Path | 설명 | 인증 |
|---|---|---|---|
| GET | `/users` | 사용자 목록 | access token |
| POST | `/users` | 사용자 생성 | 불필요 |
| GET | `/users/{id}` | 사용자 조회 | access token |
| PATCH | `/users/{id}` | 사용자 수정 | access token |
| DELETE | `/users/{id}` | 사용자 삭제 (soft) | access token |

---

## 인증 흐름

```
1. POST /auth/sign-up  → { access_token, refresh_token }
2. POST /auth/login    → { access_token, refresh_token }
3. GET  /auth/me       → Authorization: Bearer {access_token}
4. POST /auth/refresh  → { refresh_token } → { access_token, refresh_token }
```

---

## 커스터마이징 포인트

### 1. 새 도메인 추가

```
backend/domain/
└── [새도메인]/
    ├── model.py      ← SQLModel 테이블 정의
    ├── enums.py      ← Enum 정의 (필요 시)
    ├── repository.py ← BaseRepository 상속
    └── service.py    ← 비즈니스 로직
```

라우터 추가:
```python
# backend/api/v1/routers/[새도메인].py
router = APIRouter(prefix="/[새도메인]", tags=["[새도메인]"])

# backend/main.py
from backend.api.v1.routers.[새도메인] import router as new_router
app.include_router(new_router, prefix="/api/v1")
```

### 2. DB 마이그레이션 (alembic)

```bash
uv pip install alembic
alembic init alembic
# alembic/env.py에서 SQLModel metadata 연결
alembic revision --autogenerate -m "initial"
alembic upgrade head
```

### 3. Read/Write DB 분리

- Write 작업: `Depends(get_write_session_dependency)`
- Read 작업: `Depends(get_read_session_dependency)`
- 단일 DB 사용 시: `.env`에 READ/WRITE 값을 동일하게 설정

### 4. S3 파일 업로드 추가

```python
from backend.utils.s3 import generate_presigned_post

@router.post("/upload")
async def get_upload_url(filename: str, user_id: str = Depends(get_user_id)):
    s3_key = f"uploads/{user_id}/{filename}"
    presigned = generate_presigned_post(s3_key, content_type="image/jpeg")
    return presigned
```

### 5. CORS 도메인 추가

`backend/core/config.py`의 `cors_origins`에 프로덕션 도메인 추가:
```python
prod_origins: list[str] = [
    "https://yourdomain.com",
    "https://app.yourdomain.com",
]
```

---

## 개발 팁

### Mock Auth (토큰 없이 개발)

`.env`에 설정:
```
MOCK_AUTH_ENABLED=true
```

모든 인증 요청이 `mock-user-001`로 처리된다. **프로덕션에서는 절대 사용 금지.**

### 테스트 실행

```bash
uv pip install -e .[dev]
pytest tests/ -v
```

---

## 환경변수 전체 목록

`.env.example` 참고.

| 변수 | 설명 | 기본값 |
|---|---|---|
| `ENVIRONMENT` | 배포 환경 | `development` |
| `WRITE_DB_*` | Write DB 접속 정보 | — |
| `READ_DB_*` | Read DB 접속 정보 | — |
| `JWT_SECRET_KEY` | JWT 서명 키 | — |
| `JWT_ALGORITHM` | JWT 알고리즘 | `HS256` |
| `ACCESS_TOKEN_EXPIRE_MINUTES` | 액세스 토큰 만료 | `60` |
| `REFRESH_TOKEN_EXPIRE_DAYS` | 리프레시 토큰 만료 | `30` |
| `MOCK_AUTH_ENABLED` | Mock 인증 활성화 | `false` |
| `AWS_ACCESS_KEY_ID` | AWS 액세스 키 | — |
| `AWS_SECRET_ACCESS_KEY` | AWS 시크릿 키 | — |
| `AWS_REGION` | AWS 리전 | `ap-northeast-2` |
| `S3_BUCKET_NAME` | S3 버킷 이름 | — |
