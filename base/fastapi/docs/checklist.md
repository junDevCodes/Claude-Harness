# Checklist — base/fastapi/ 완료 기준

> **문서 역할:** base/fastapi/ Definition of Done 체크리스트.

---

## 공통 필수 항목

- [x] `docs/plan.md` 존재 및 내용 채워져 있음
- [x] `docs/task.md` 존재 및 내용 채워져 있음
- [x] `docs/history.md` 존재 및 내용 채워져 있음
- [x] `docs/checklist.md` 존재 및 내용 채워져 있음
- [x] `README.md` — 프로젝트 구조, 실행 방법, 커스터마이징 포인트 포함

## 웹 백엔드 필수 항목

- [x] JWT 인증 구현 (access token + refresh token)
- [x] 기본 CRUD 예시 (User 엔티티 — GET/POST/PATCH/DELETE)
- [x] `.env.example` — 전체 환경변수 목록 포함
- [x] `Dockerfile` 포함
- [x] `docker-compose.yaml` 포함
- [x] 테스트 템플릿 — `tests/test_auth.py` (pytest-asyncio)

## 추가 구성 확인

- [x] DDD 구조 (domain/user, domain/shared)
- [x] 읽기/쓰기 DB 분리 (orm.py)
- [x] Generic BaseRepository (CRUD + batch operations)
- [x] 에러 핸들러 미들웨어
- [x] S3 presigned URL 유틸리티
- [x] 비즈니스 도메인 미포함 (범용성 유지)
- [x] Mock Auth 지원 (개발/테스트용)

---

## 최종 검증 체크

```bash
# 1. 패키지 설치 확인
cd base/fastapi
uv venv && source .venv/bin/activate
uv pip install -e .

# 2. 서버 기동 확인 (DB 없이도 서버 실행 자체는 가능)
ENVIRONMENT=development \
WRITE_DB_USER=test WRITE_DB_PASSWORD=test WRITE_DB_HOST=localhost \
WRITE_DB_PORT=5432 WRITE_DB_NAME=test \
READ_DB_USER=test READ_DB_PASSWORD=test READ_DB_HOST=localhost \
READ_DB_PORT=5432 READ_DB_NAME=test \
JWT_SECRET_KEY=test-secret \
uvicorn backend.main:app --port 8000

# 3. Health check
curl http://localhost:8000/api/v1/health

# 4. Docker 빌드 확인
docker build -t fastapi-base .

# 5. Docker Compose 기동
cp .env.example .env  # 값 채우기
docker-compose up
```

---

*Last updated: 2026-03-01*
