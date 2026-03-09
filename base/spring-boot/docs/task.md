# Task — base/spring-boot/ 세부 작업 계획

> **문서 역할:** plan.md 기반 세부 실행 계획. 작업 완료 시 history.md로 이동.

---

## 현재 작업: Spring Boot 베이스 코드 신규 생성

### Task 1: 문서 작성 (docs/ 4문서)

| 항목 | 상태 |
|---|---|
| `docs/plan.md` — 구성 계획 | ✅ |
| `docs/task.md` — 세부 작업 | ✅ |
| `docs/history.md` — 맥락 문서 | ✅ |
| `docs/checklist.md` — 완료 기준 | ✅ |

---

### Task 2: 빌드 설정

| 항목 | 상태 |
|---|---|
| `build.gradle.kts` — Gradle Kotlin DSL | ✅ |
| `settings.gradle.kts` — 프로젝트 이름 | ✅ |
| `gradle/wrapper/gradle-wrapper.properties` | ✅ |
| `gradlew` (Unix 실행 스크립트) | ✅ |
| `gradlew.bat` (Windows 실행 스크립트) | ✅ |

---

### Task 3: 공통 모듈 (common/)

| 항목 | 상태 |
|---|---|
| `BaseApplication.java` — Spring Boot 진입점 | ✅ |
| `common/response/ApiResponse.java` | ✅ |
| `common/exception/ErrorCode.java` | ✅ |
| `common/exception/BusinessException.java` | ✅ |
| `common/exception/GlobalExceptionHandler.java` | ✅ |

---

### Task 4: 설정 (config/)

| 항목 | 상태 |
|---|---|
| `config/SecurityConfig.java` — Security + CORS | ✅ |
| `config/S3Config.java` — AWS S3 클라이언트 | ✅ |

---

### Task 5: 보안/JWT (security/)

| 항목 | 상태 |
|---|---|
| `security/JwtProvider.java` — 생성/검증/파싱 | ✅ |
| `security/JwtAuthenticationFilter.java` | ✅ |
| `security/CustomUserDetailsService.java` | ✅ |

---

### Task 6: 인증 (auth/)

| 항목 | 상태 |
|---|---|
| `auth/entity/RefreshToken.java` | ✅ |
| `auth/repository/RefreshTokenRepository.java` | ✅ |
| `auth/dto/SignUpRequest.java` | ✅ |
| `auth/dto/LoginRequest.java` | ✅ |
| `auth/dto/TokenResponse.java` | ✅ |
| `auth/dto/RefreshRequest.java` | ✅ |
| `auth/service/AuthService.java` | ✅ |
| `auth/controller/AuthController.java` | ✅ |

---

### Task 7: 도메인 - User (domain/user/)

| 항목 | 상태 |
|---|---|
| `domain/user/entity/User.java` | ✅ |
| `domain/user/repository/UserRepository.java` | ✅ |
| `domain/user/dto/UserResponse.java` | ✅ |
| `domain/user/dto/UpdateUserRequest.java` | ✅ |
| `domain/user/service/UserService.java` | ✅ |
| `domain/user/controller/UserController.java` | ✅ |

---

### Task 8: 유틸리티

| 항목 | 상태 |
|---|---|
| `utils/S3Uploader.java` — presigned URL 업로드 | ✅ |

---

### Task 9: 리소스 설정

| 항목 | 상태 |
|---|---|
| `application.yaml` — 공통 설정 | ✅ |
| `application-local.yaml` — 로컬 환경 | ✅ |
| `application-prod.yaml` — 프로덕션 환경 | ✅ |

---

### Task 10: 테스트

| 항목 | 상태 |
|---|---|
| `auth/AuthServiceTest.java` — 단위 테스트 | ✅ |
| `domain/user/UserServiceTest.java` — 단위 테스트 | ✅ |

---

### Task 11: 배포 설정 + 문서

| 항목 | 상태 |
|---|---|
| `Dockerfile` — 멀티 스테이지 빌드 | ✅ |
| `docker-compose.yaml` — app + PostgreSQL | ✅ |
| `.env.example` — 전체 환경변수 목록 | ✅ |
| `README.md` — 구조/실행법/커스터마이징 | ✅ |

---

---

### Task 12: spring-boot-guidelines 스킬 작성 (Phase 2-B)

> **세션:** 2026-03-05 | 병렬 세션 — `.claude/skills/spring-boot-guidelines/` 디렉토리만 생성
> **격리 규칙:** `skill-rules.json`, `README.md`, 루트 `docs/` 수정 금지

| 항목 | 내용 | 상태 |
|---|---|---|
| `SKILL.md` 작성 | 500줄 이하, YAML frontmatter, 핵심 패턴 5개 | ✅ |
| 계층 구조 패턴 | Controller → Service → Repository | ✅ |
| JWT 인증 패턴 | JwtProvider, JwtAuthenticationFilter, RefreshToken Rotation | ✅ |
| 예외 처리 패턴 | ErrorCode, BusinessException, GlobalExceptionHandler | ✅ |
| 응답 래퍼 패턴 | ApiResponse<T> (success/data/message/code) | ✅ |
| Entity 패턴 | JPA + Lombok + @PrePersist/@PreUpdate | ✅ |
| 안티패턴 섹션 | Spring Boot 흔한 실수 목록 | ✅ |
| 트리거 키워드 5개 이상 | spring boot, spring security, jpa 등 | ✅ |

---

*Last updated: 2026-03-05*
