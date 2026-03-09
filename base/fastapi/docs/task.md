# Task — base/fastapi/ 세부 작업

> **문서 역할:** base/fastapi/ 생성 작업의 세부 실행 계획.

---

## Phase 1-A-2: base/fastapi/ 생성 (완료)

| 항목 | 상태 |
|---|---|
| DDD 구조 구성 (domain/user, domain/shared) | ✅ |
| 읽기/쓰기 DB 분리 (orm.py) | ✅ |
| JWT 인증 (auth_service.py) | ✅ |
| User 도메인 단순화 (User 모델만, 비즈니스 필드 제거) | ✅ |
| S3 업로드 유틸리티 추가 (utils/s3.py) | ✅ |
| 에러 핸들러 미들웨어 (Firebase 관련 제거) | ✅ |
| Auth API 라우터 (/auth/sign-up, /auth/login, /auth/refresh, /auth/me) | ✅ |
| User CRUD API 라우터 (GET/POST/PATCH/DELETE /users) | ✅ |
| main.py (create_application 팩토리) | ✅ |
| pyproject.toml (최소 deps) | ✅ |
| Dockerfile | ✅ |
| docker-compose.yaml (postgres + api) | ✅ |
| .env.example | ✅ |
| tests/test_auth.py (테스트 템플릿) | ✅ |
| docs/ 4문서 작성 | ✅ |
| README.md | ✅ |

---

## 다음 작업

- `base/nextjs/` 생성 (Task 1-A-2 두 번째 단계)
- Phase 1-B 병렬 작업 (7개 신규 스택)

---

*Last updated: 2026-03-01*
