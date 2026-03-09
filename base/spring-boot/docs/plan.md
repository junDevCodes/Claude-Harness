# Plan — base/spring-boot/ 베이스 코드 구성 계획

> **문서 역할:** 이 스택 베이스 코드의 전체 구성 계획 및 설계 결정을 기록하는 기획서.

---

## 목표

Spring Boot 3 + Java 21 기반의 범용 백엔드 스타터 킷.
새 프로젝트 시작 시 이 코드를 복사하고 비즈니스 로직만 추가하면 즉시 사용 가능한 상태.

---

## 기술 스택

| 분류 | 기술 | 버전 |
|---|---|---|
| Framework | Spring Boot | 3.3.x |
| Language | Java | 21 |
| ORM | Spring Data JPA + Hibernate | Spring Boot managed |
| Database | PostgreSQL | 16 |
| Security | Spring Security 6 + JWT | jjwt 0.12.x |
| File Storage | AWS SDK S3 | v2 (2.28.x) |
| Build | Gradle | 8.x (Kotlin DSL) |
| Container | Docker + Docker Compose | - |
| Test | JUnit 5 + Mockito | Spring Boot managed |

---

## 프로젝트 구조 계획

```
base/spring-boot/
├── src/main/java/com/base/
│   ├── BaseApplication.java
│   ├── common/
│   │   ├── response/ApiResponse.java        # 표준 API 응답 래퍼
│   │   └── exception/
│   │       ├── ErrorCode.java               # 에러 코드 enum
│   │       ├── BusinessException.java       # 비즈니스 예외
│   │       └── GlobalExceptionHandler.java  # @RestControllerAdvice
│   ├── config/
│   │   ├── SecurityConfig.java              # Spring Security Filter Chain
│   │   └── S3Config.java                   # AWS S3 클라이언트 Bean
│   ├── security/
│   │   ├── JwtProvider.java                 # JWT 생성/검증/파싱
│   │   ├── JwtAuthenticationFilter.java     # OncePerRequestFilter
│   │   └── CustomUserDetailsService.java    # UserDetailsService 구현
│   ├── auth/
│   │   ├── entity/RefreshToken.java         # Refresh Token 엔티티 (DB 저장)
│   │   ├── repository/RefreshTokenRepository.java
│   │   ├── dto/
│   │   │   ├── SignUpRequest.java
│   │   │   ├── LoginRequest.java
│   │   │   ├── TokenResponse.java
│   │   │   └── RefreshRequest.java
│   │   ├── service/AuthService.java
│   │   └── controller/AuthController.java
│   ├── domain/user/
│   │   ├── entity/User.java
│   │   ├── repository/UserRepository.java
│   │   ├── dto/
│   │   │   ├── UserResponse.java
│   │   │   └── UpdateUserRequest.java
│   │   ├── service/UserService.java
│   │   └── controller/UserController.java
│   └── utils/
│       └── S3Uploader.java
├── src/main/resources/
│   ├── application.yaml                     # 공통 + 프로파일 분기
│   ├── application-local.yaml               # 로컬 개발 환경
│   └── application-prod.yaml                # 프로덕션 환경
├── src/test/java/com/base/
│   ├── auth/AuthServiceTest.java
│   └── domain/user/UserServiceTest.java
├── Dockerfile
├── docker-compose.yaml
├── .env.example
└── README.md
```

---

## 핵심 설계 결정

### JWT 전략
- **Access Token:** 15분 만료 (환경변수 `JWT_ACCESS_TOKEN_EXPIRY` 초 단위)
- **Refresh Token:** 7일 만료, `refresh_tokens` 테이블에 저장 (Rotate 전략)
- **전달 방식:** `Authorization: Bearer <token>` 헤더
- **Payload:** `userId`, `email`, `role`

### Spring Security 설계
- `SessionCreationPolicy.STATELESS` — 서버 세션 미사용
- CSRF 비활성화 (REST API)
- `JwtAuthenticationFilter` → `UsernamePasswordAuthenticationFilter` 앞에 등록
- `@EnableMethodSecurity` — 메서드 레벨 인가(`@PreAuthorize`) 지원

### 표준 API 응답 형식
```json
{
  "success": true,
  "data": { ... },
  "message": "OK",
  "code": "SUCCESS"
}
```
실패 시:
```json
{
  "success": false,
  "data": null,
  "message": "사용자를 찾을 수 없습니다.",
  "code": "USER_NOT_FOUND"
}
```

### 환경 분리 전략
| 프로파일 | DB ddl-auto | show-sql | 용도 |
|---|---|---|---|
| `local` | `create-drop` | `true` | 로컬 개발 (docker-compose DB) |
| `prod` | `validate` | `false` | 운영 (외부 PostgreSQL) |

### 패키지 기명 규칙
- Root package: `com.base`
- 도메인 패키지: `com.base.domain.{entity}`
- 인증 패키지: `com.base.auth`
- 보안 패키지: `com.base.security`
- 공통 패키지: `com.base.common`

---

## API 엔드포인트 설계

### Auth (`/api/v1/auth`)
| Method | Path | 설명 | 인증 |
|---|---|---|---|
| POST | `/sign-up` | 회원가입 | 불필요 |
| POST | `/login` | 로그인 (access + refresh 발급) | 불필요 |
| POST | `/refresh` | Access Token 갱신 | 불필요 (refresh 필요) |
| POST | `/logout` | 로그아웃 (refresh 삭제) | 필요 |

### User (`/api/v1/users`)
| Method | Path | 설명 | 인증 |
|---|---|---|---|
| GET | `/me` | 내 정보 조회 | 필요 |
| PATCH | `/me` | 내 정보 수정 | 필요 |
| DELETE | `/me` | 회원 탈퇴 | 필요 |
| GET | `/{id}` | 특정 유저 조회 | 불필요 |
| GET | `/` | 유저 목록 (ADMIN 전용) | ADMIN |

### Health
| Method | Path | 설명 |
|---|---|---|
| GET | `/api/v1/health` | 헬스체크 |

---

## 완성 기준 (Definition of Done)

- [ ] `docs/` 4문서 작성 (plan/task/history/checklist)
- [ ] Spring Boot 3 + Java 21 프로젝트 빌드 구조 (Gradle Kotlin DSL)
- [ ] Spring Security + JWT Filter 동작
- [ ] Auth API (회원가입/로그인/토큰갱신/로그아웃)
- [ ] User CRUD (조회/수정/삭제)
- [ ] 전역 예외 처리 (GlobalExceptionHandler) + ApiResponse 래퍼
- [ ] application.yaml 환경 분리 (local / prod)
- [ ] S3 파일 업로드 유틸리티
- [ ] `Dockerfile` + `docker-compose.yaml`
- [ ] `.env.example` + `README.md`
- [ ] 테스트 템플릿 2개 이상 (AuthServiceTest, UserServiceTest)

---

*Last updated: 2026-03-02 (초기 작성)*
