# History — base/spring-boot/ 작업 맥락 문서

> **문서 역할:** 작업 이력과 주요 결정 사항을 기록. 세션 재시작 시 이 문서로 맥락 복원.

---

## 현재 상태

- **Phase:** Phase 1-B (신규 베이스 코드 생성)
- **작업:** base/spring-boot/ 전체 생성 (2026-03-02 시작)
- **진행:** 모든 파일 생성 완료

---

## 작업 이력

### 2026-03-02 — base/spring-boot/ 신규 생성

**배경:**
- Phase 1-A에서 base/fastapi/, base/nextjs/ 마이그레이션 완료
- Phase 1-B에서 7개 신규 스택 병렬 생성 시작
- 이 세션에서 Spring Boot 스택 담당

**생성한 파일 목록:**
- `docs/` 4문서 (plan/task/history/checklist)
- `build.gradle.kts`, `settings.gradle.kts`, gradle wrapper
- `BaseApplication.java`
- `common/response/ApiResponse.java` — 표준 API 응답 래퍼
- `common/exception/` — ErrorCode, BusinessException, GlobalExceptionHandler
- `config/SecurityConfig.java`, `config/S3Config.java`
- `security/` — JwtProvider, JwtAuthenticationFilter, CustomUserDetailsService
- `auth/` — RefreshToken entity/repo, dto 4종, AuthService, AuthController
- `domain/user/` — User entity/repo, dto 2종, UserService, UserController
- `utils/S3Uploader.java`
- `src/main/resources/` — application.yaml, -local.yaml, -prod.yaml
- `src/test/` — AuthServiceTest, UserServiceTest
- `Dockerfile`, `docker-compose.yaml`, `.env.example`, `README.md`

---

## 주요 결정 사항

| 결정 | 내용 | 이유 |
|---|---|---|
| Gradle Kotlin DSL | `build.gradle.kts` 사용 | 타입 안전성, Spring Initializr 기본값 |
| jjwt 0.12.x | JWT 라이브러리 선택 | Spring Boot 3 호환, Builder API 개선 |
| DB Refresh Token | 리프레시 토큰을 DB에 저장 | Rotate 전략, 강제 무효화 가능 |
| `STATELESS` 세션 | REST API 표준 | JWT 기반 무상태 인증 |
| 멀티 스테이지 Dockerfile | Builder + Runner 분리 | 최종 이미지 경량화 |
| Java 21 | `--enable-preview` 미적용 | 안정성 우선 (Virtual Threads는 opt-in) |
| `com.base` 패키지 | 루트 패키지명 | 범용 베이스 코드 표준화 |
| `application-local/prod` | 환경 분리 | local: create-drop, prod: validate |
| `@EnableMethodSecurity` | 메서드 레벨 인가 | `@PreAuthorize("hasRole('ADMIN')")` 지원 |
| AWS SDK v2 | S3Uploader | v1 deprecated, v2가 현재 표준 |

---

## 특이사항 및 주의점

- `gradle/wrapper/gradle-wrapper.jar`는 바이너리이므로 포함 불가.
  → 프로젝트 사용 시 `gradle wrapper` 명령으로 생성 필요 (README에 안내).
- `application-local.yaml`의 `ddl-auto: create-drop`은 로컬 전용.
  운영 배포 시 반드시 `prod` 프로파일 사용.
- JWT secret은 최소 256bit (32바이트) 이상 권장.
  `.env.example`에 64자 hex 예시 포함.
- S3Uploader는 presigned URL 방식 사용 (직접 업로드 아님).

---

## 다음 세션 이어갈 작업

- 이 스택 작업은 완료됨.
- 루트 docs/ 업데이트는 Phase 1-B 전체 완료 후 별도 세션에서 진행.
- 다음 Phase 1-B 세션: express, nestjs, react-native, cpp-embedded 중 하나.

---

---

### 2026-03-05 — spring-boot-guidelines 스킬 작성 (Phase 2-B)

**배경:**
- Phase 1-C 완료 후 Phase 2 병렬 세션 전략 수립
- Task 2-A(CLAUDE.md 격리 규칙 추가) 완료 → 병렬 세션 시작 가능
- 이 세션에서 `spring-boot-guidelines` 스킬 담당

**핵심 작업 내용:**
- `.claude/skills/spring-boot-guidelines/SKILL.md` 생성
- `base/spring-boot/` 소스에서 핵심 5대 패턴 추출 및 문서화
- 병렬 세션 격리 원칙 준수 (`skill-rules.json` 수정 없음)

**참조한 소스 파일:**
- `JwtProvider.java` — jjwt 0.12.x, access/refresh 생성 + 파싱
- `AuthService.java` — Refresh Token Rotation 전략
- `GlobalExceptionHandler.java` — @RestControllerAdvice 패턴
- `UserService.java` — @Transactional(readOnly = true) 분리 패턴
- `User.java` — JPA Entity + Lombok + @PrePersist

**주요 결정:**
- SKILL.md 단일 파일 구성 (resources/ 불필요, 패턴 5개 + 코드 예시로 500줄 내 완결)
- skill-rules.json 등록은 Phase 2-D 일괄 세션에서 처리

**다음 세션 이어갈 작업:**
- 이 스킬 작업 완료됨
- 다음: Phase 2-D 일괄 통합 세션에서 `skill-rules.json`에 트리거 규칙 추가 필요

---

*Last updated: 2026-03-05 (spring-boot-guidelines 스킬 작성)*
