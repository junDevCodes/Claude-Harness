# Plan — base/fastapi/ 구성 계획

> **문서 역할:** `base/fastapi/` 베이스 코드의 전체 구성 목표와 방향.

---

## 목표

FastAPI 기반 백엔드 프로젝트를 시작할 때 즉시 사용 가능한 **범용 스타터 킷**.

비즈니스 도메인 로직 없이 인프라 패턴만 포함하며, 도메인을 추가하면 프로덕션 수준 API를 빠르게 구성할 수 있다.

---

## 핵심 구성 요소

| 구성 요소 | 구현 방식 | 상태 |
|---|---|---|
| DDD 구조 | domain/user + domain/shared | ✅ |
| Read/Write DB 분리 | orm.py (asyncpg + SQLModel) | ✅ |
| JWT 인증 | access token (1h) + refresh token (30d) | ✅ |
| User CRUD API | GET/POST/PATCH/DELETE /api/v1/users | ✅ |
| Auth API | /auth/sign-up, /auth/login, /auth/refresh, /auth/me | ✅ |
| 에러 핸들러 | AppException 계층, DB 에러 처리 | ✅ |
| S3 업로드 유틸리티 | presigned POST/GET + delete | ✅ |
| 테스트 템플릿 | tests/test_auth.py (pytest-asyncio) | ✅ |
| Dockerfile | python:3.12.3-slim + uv | ✅ |
| docker-compose.yaml | postgres + api 서비스 | ✅ |
| .env.example | 전체 환경변수 목록 | ✅ |
| README.md | 구조 + 실행 방법 + 커스터마이징 | ✅ |

---

## 기술 스택

- **Python**: 3.12.3
- **FastAPI**: 0.116+
- **ORM**: SQLModel + SQLAlchemy (async)
- **DB Driver**: asyncpg (PostgreSQL)
- **Auth**: PyJWT (HS256)
- **Password**: bcrypt
- **ID**: ULID (prefixed, e.g. `usr_01ABCDEF...`)
- **Package Manager**: uv
- **Container**: Docker + Docker Compose

---

## 디렉토리 구조

```
base/fastapi/
├── backend/
│   ├── api/v1/routers/    ← auth.py, user.py
│   ├── core/              ← config.py (Pydantic Settings)
│   ├── db/                ← orm.py (Read/Write 세션 팩토리)
│   ├── domain/
│   │   ├── shared/        ← base_repository.py (Generic CRUD)
│   │   └── user/          ← model, enums, repository, service, auth_service
│   ├── dtos/              ← auth.py, user.py (Pydantic DTOs)
│   ├── error/             ← AppException 계층
│   ├── middleware/        ← error_handler.py
│   └── utils/             ← logger.py, password.py, s3.py
├── tests/
│   └── test_auth.py
├── docs/                  ← 4문서 (plan/task/history/checklist)
├── .env.example
├── Dockerfile
├── docker-compose.yaml
├── pyproject.toml
└── README.md
```

---

*Created: 2026-03-01*
