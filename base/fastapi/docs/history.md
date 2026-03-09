# History — base/fastapi/ 작업 맥락

> **문서 역할:** base/fastapi/ 생성 작업의 이력 및 주요 결정 사항.

---

## 현재 상태

- **상태:** 생성 완료 (Definition of Done 충족)
- **생성일:** 2026-03-01
- **기반:** `backend/` 코드를 범용화하여 생성

---

## 작업 이력

### 2026-03-01 — base/fastapi/ 초기 생성

**배경:**
- `backend/` 코드가 이미 DDD 구조 + JWT + Read/Write DB 분리로 잘 구성되어 있음
- 비즈니스 도메인 특화 코드(artist, artwork 등)는 이미 제거된 상태였음
- User 도메인 모델에 비즈니스 특화 필드 다수 → 단순화 필요
- S3 유틸리티가 backend/에 없었으므로 새로 작성

**주요 결정 사항:**

| 결정 | 내용 |
|---|---|
| User 모델 단순화 | firebase_id, phone, gender, birth_year, 비즈니스 특화 모델들 모두 제거. email + name + status + is_admin + auth_type만 유지 |
| enum 단순화 | AuthTypeEnum(email/google/kakao), UserStatusEnum(active/inactive/suspended)만 유지 |
| S3 유틸리티 신규 추가 | presigned POST, presigned GET URL, delete 기능 포함 |
| DTO 필드 변경 | `app_auth_token` → `access_token` (더 일반적인 명칭으로 변경) |
| 에러 계층 단순화 | UserNotFoundSignupRequiredError (Firebase 전용) 제거 |
| Dockerfile 단순화 | OpenCV 의존성 제거, 기본 python:3.12.3-slim 사용 |
| docker-compose 재작성 | postgres + api 2-서비스 구조 (기존은 테스트 DB만 있었음) |
| 의존성 최소화 | anthropic, openai, pgvector, firebase-admin, playwright 등 비즈니스 패키지 제거 |

---

## 다음 세션에서 이어갈 작업

- `base/nextjs/` 생성 (Task 1-A-2 두 번째 단계)
- alembic 마이그레이션 예시 추가 가능 (선택)
- Phase 1-B: 7개 신규 스택 병렬 작업

---

*Last updated: 2026-03-01*
