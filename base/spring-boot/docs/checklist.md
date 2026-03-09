# Checklist — base/spring-boot/ 완료 기준

> **문서 역할:** Definition of Done 체크리스트. 작업 전 기준 확인, 완료 시 최종 검증.

---

## 공통 필수 항목

- [x] `docs/plan.md` — 구성 계획 및 설계 결정
- [x] `docs/task.md` — 세부 작업 항목 및 진행 상태
- [x] `docs/history.md` — 작업 맥락 및 주요 결정
- [x] `docs/checklist.md` — 완료 기준 체크리스트
- [x] `README.md` — 프로젝트 구조, 실행 방법, 커스터마이징 포인트

---

## 웹 백엔드 필수 항목

### JWT 인증
- [x] 회원가입 API (`POST /api/v1/auth/sign-up`)
- [x] 로그인 API (`POST /api/v1/auth/login`) — access + refresh 발급
- [x] 토큰 갱신 API (`POST /api/v1/auth/refresh`) — access token 재발급
- [x] 로그아웃 API (`POST /api/v1/auth/logout`) — refresh token 삭제
- [x] JWT Access Token (15분 만료)
- [x] JWT Refresh Token (7일 만료, DB 저장)
- [x] `JwtAuthenticationFilter` — 요청별 JWT 검증

### User CRUD
- [x] 내 정보 조회 (`GET /api/v1/users/me`)
- [x] 내 정보 수정 (`PATCH /api/v1/users/me`)
- [x] 회원 탈퇴 (`DELETE /api/v1/users/me`)
- [x] 특정 유저 조회 (`GET /api/v1/users/{id}`)
- [x] 유저 목록 조회 (`GET /api/v1/users/` — ADMIN 전용)

### 환경 설정
- [x] `application.yaml` — 공통 설정 (프로파일 분기)
- [x] `application-local.yaml` — 로컬 (ddl-auto: create-drop)
- [x] `application-prod.yaml` — 프로덕션 (ddl-auto: validate)
- [x] `.env.example` — 전체 환경변수 목록

### 배포
- [x] `Dockerfile` — 멀티 스테이지 빌드 (Builder + Runner)
- [x] `docker-compose.yaml` — app + PostgreSQL 서비스

### 테스트
- [x] `AuthServiceTest.java` — 회원가입/로그인 단위 테스트
- [x] `UserServiceTest.java` — User CRUD 단위 테스트

---

## 추가 항목

- [x] `GlobalExceptionHandler` — `@RestControllerAdvice` 전역 예외 처리
- [x] `ApiResponse<T>` — 표준 응답 래퍼 (success/data/message/code)
- [x] `ErrorCode` enum — HTTP 상태 + 메시지 중앙 관리
- [x] `S3Uploader` — presigned URL 기반 파일 업로드
- [x] Gradle Kotlin DSL (`build.gradle.kts`)
- [x] Spring Security 6 + CORS 설정
- [x] `@EnableMethodSecurity` + `@PreAuthorize` 예시

---

## 최종 검증 항목

- [x] 전체 파일 구조가 `docs/plan.md`의 계획과 일치
- [x] 모든 Java 파일이 `com.base` 패키지 하위에 위치
- [x] `application.yaml`의 환경변수 참조가 `.env.example`과 일치
- [x] `Dockerfile`이 `./gradlew bootJar`로 빌드 가능한 구조
- [x] `docker-compose.yaml`이 app + db 서비스 포함
- [x] 테스트가 Mockito 기반 독립 단위 테스트 구조
- [x] `README.md`에 실행 방법 및 커스터마이징 가이드 포함

---

---

## spring-boot-guidelines 스킬 완성 기준 (Phase 2-B)

> Definition of Done — 스킬이 완성으로 인정되는 기준

### 필수 항목

- [x] `.claude/skills/spring-boot-guidelines/SKILL.md` 존재
- [x] SKILL.md **500줄 이하** (정확히 500줄)
- [x] YAML frontmatter — `name`, `description`, `triggers` 포함
- [x] 핵심 패턴 **5개 이상** (각 패턴에 코드 예시 포함)
  - [x] 계층 구조 (Controller → Service → Repository)
  - [x] JPA Entity 패턴 (Lombok + @PrePersist)
  - [x] JWT 인증 패턴 (JwtProvider + Refresh Token Rotation)
  - [x] 예외 처리 패턴 (ErrorCode + BusinessException + GlobalExceptionHandler)
  - [x] ApiResponse<T> 표준 응답 래퍼
- [x] 트리거 키워드 **5개 이상** 명시 (15개 등록)
- [x] 안티패턴 섹션 포함
- [x] `skill-rules.json` 등록은 Phase 2-D에서 처리 (이 세션에서 미처리 확인)

### 병렬 세션 격리 준수 확인

- [x] `skill-rules.json` 수정하지 않음 ✅
- [x] `.claude/skills/README.md` 수정하지 않음 ✅
- [x] 루트 `docs/` 파일 수정하지 않음 ✅
- [x] `CLAUDE.md`, `base_code_plan.md` 수정하지 않음 ✅

---

*Last updated: 2026-03-05*
