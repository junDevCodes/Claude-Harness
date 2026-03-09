# Base Spring Boot

Spring Boot 3 + Java 21 기반 범용 백엔드 스타터 킷.

JWT 인증(access + refresh), User CRUD, S3 파일 업로드, Docker 배포 구성이 포함되어 있다.

---

## 기술 스택

| 분류 | 기술 | 버전 |
|---|---|---|
| Framework | Spring Boot | 3.3.x |
| Language | Java | 21 |
| ORM | Spring Data JPA + Hibernate | - |
| Database | PostgreSQL | 16 |
| Security | Spring Security 6 + JWT (jjwt) | 0.12.x |
| File Storage | AWS SDK S3 | v2 |
| Build | Gradle | 8.x (Kotlin DSL) |

---

## 프로젝트 구조

```
src/main/java/com/base/
├── BaseApplication.java
├── common/
│   ├── response/ApiResponse.java        # 표준 API 응답 래퍼
│   └── exception/
│       ├── ErrorCode.java               # 에러 코드 enum (HTTP 상태 + 메시지)
│       ├── BusinessException.java       # 비즈니스 예외 (ErrorCode 포함)
│       └── GlobalExceptionHandler.java  # @RestControllerAdvice 전역 처리
├── config/
│   ├── SecurityConfig.java              # Spring Security + CORS 설정
│   └── S3Config.java                   # AWS S3 클라이언트 Bean
├── security/
│   ├── JwtProvider.java                 # JWT 생성/검증/파싱
│   ├── JwtAuthenticationFilter.java     # Bearer 토큰 인증 필터
│   └── CustomUserDetailsService.java    # UserDetailsService 구현
├── auth/
│   ├── entity/RefreshToken.java         # Refresh Token (DB 저장)
│   ├── repository/RefreshTokenRepository.java
│   ├── dto/                             # SignUpRequest, LoginRequest, TokenResponse, RefreshRequest
│   ├── service/AuthService.java         # 회원가입/로그인/토큰갱신/로그아웃
│   └── controller/AuthController.java
├── domain/user/
│   ├── entity/User.java
│   ├── repository/UserRepository.java
│   ├── dto/                             # UserResponse, UpdateUserRequest
│   ├── service/UserService.java         # User CRUD
│   └── controller/UserController.java
└── utils/
    └── S3Uploader.java                  # 직접 업로드 + Presigned PUT URL
```

---

## API 엔드포인트

### Auth
| Method | Path | 설명 | 인증 |
|---|---|---|---|
| POST | `/api/v1/auth/sign-up` | 회원가입 | 불필요 |
| POST | `/api/v1/auth/login` | 로그인 | 불필요 |
| POST | `/api/v1/auth/refresh` | Access Token 갱신 | 불필요 |
| POST | `/api/v1/auth/logout` | 로그아웃 | 필요 |

### User
| Method | Path | 설명 | 인증 |
|---|---|---|---|
| GET | `/api/v1/users/me` | 내 정보 조회 | 필요 |
| PATCH | `/api/v1/users/me` | 내 정보 수정 | 필요 |
| DELETE | `/api/v1/users/me` | 회원 탈퇴 | 필요 |
| GET | `/api/v1/users/{id}` | 특정 유저 조회 | 불필요 |
| GET | `/api/v1/users` | 유저 목록 (페이징) | ADMIN |

### Health
| Method | Path | 설명 |
|---|---|---|
| GET | `/api/v1/health` | 헬스체크 |

---

## 빠른 시작

### 1. 사전 요구사항

- Java 21+
- Docker & Docker Compose
- (선택) Gradle 8.x — 없으면 `gradle wrapper` 명령으로 생성

### 2. Gradle Wrapper 생성 (최초 1회)

```bash
# Gradle이 설치되어 있을 경우
gradle wrapper

# 또는 Spring Initializr 프로젝트를 기반으로 시작
```

### 3. 환경 변수 설정

```bash
cp .env.example .env
# .env 파일을 열어 JWT_SECRET, DB_PASSWORD 등 설정
```

> **중요:** `JWT_SECRET`은 Base64 인코딩된 256비트 이상 비밀키여야 한다.
> ```bash
> openssl rand -base64 64
> ```

### 4. Docker Compose로 실행

```bash
# app + PostgreSQL 동시 기동
docker-compose up --build
```

서버 확인: `http://localhost:8080/api/v1/health`

### 5. 로컬 개발 (IDE)

```bash
# PostgreSQL만 Docker로 기동
docker-compose up db

# 빌드 후 실행
./gradlew bootRun --args='--spring.profiles.active=local'
```

---

## 테스트 실행

```bash
# 전체 테스트
./gradlew test

# 테스트 리포트 확인
open build/reports/tests/test/index.html
```

---

## JWT 인증 흐름

```
1. POST /api/v1/auth/sign-up or /login
   → Response: { accessToken, refreshToken, tokenType: "Bearer" }

2. 보호된 API 요청 시:
   Authorization: Bearer <accessToken>

3. Access Token 만료 시:
   POST /api/v1/auth/refresh
   Body: { refreshToken: "..." }
   → Response: { accessToken (새것), refreshToken (Rotate), tokenType }

4. 로그아웃:
   POST /api/v1/auth/logout (Authorization 헤더 필요)
   → DB에서 Refresh Token 삭제
```

---

## 커스터마이징 포인트

### 1. 패키지명 변경
```
com.base → com.{company}.{project}
```
IDE의 Refactor > Rename Package 기능 사용.

### 2. 새 도메인 추가
`domain/user/` 구조를 복사하여 새 도메인 추가:
```
domain/{entity}/
├── entity/{Entity}.java
├── repository/{Entity}Repository.java
├── dto/
├── service/{Entity}Service.java
└── controller/{Entity}Controller.java
```

### 3. ErrorCode 추가
`common/exception/ErrorCode.java`에 새 에러 코드 추가:
```java
PAYMENT_FAILED(HttpStatus.BAD_REQUEST, "PAYMENT_FAILED", "결제에 실패했습니다."),
```

### 4. 공개/보호 엔드포인트 설정
`config/SecurityConfig.java`의 `filterChain` 메서드에서:
```java
.requestMatchers("/api/v1/public/**").permitAll()
.requestMatchers("/api/v1/admin/**").hasRole("ADMIN")
```

### 5. 메서드 레벨 인가
`@EnableMethodSecurity`가 이미 활성화되어 있으므로:
```java
@PreAuthorize("hasRole('ADMIN')")
@GetMapping("/admin-only")
public ApiResponse<...> adminOnly() { ... }
```

### 6. S3 업로드 사용
```java
// 직접 업로드 (서버에서 파일 수신 후)
String fileUrl = s3Uploader.upload(fileBytes, "images", originalFilename);

// Presigned URL 방식 (클라이언트 직접 업로드)
S3Uploader.PresignedUrlResponse res =
    s3Uploader.generatePresignedPutUrl("images", filename, "image/jpeg", 10);
// res.uploadUrl() → 클라이언트에 전달, HTTP PUT으로 직접 업로드
// res.fileUrl() → DB에 저장할 최종 URL
```

---

## 환경 분리

| 프로파일 | ddl-auto | show-sql | 용도 |
|---|---|---|---|
| `local` | `create-drop` | `true` | 로컬 개발 |
| `prod` | `validate` | `false` | 운영 배포 |

---

## Definition of Done (완성 기준)

- [x] docs/ 4문서 (plan/task/history/checklist)
- [x] Spring Boot 3 + Java 21 프로젝트 구조
- [x] Spring Security + JWT Filter 동작
- [x] Auth API (회원가입/로그인/토큰갱신/로그아웃)
- [x] User CRUD + 페이지네이션 (ADMIN 전용 목록)
- [x] GlobalExceptionHandler + ApiResponse 래퍼
- [x] application.yaml 환경 분리 (local / prod)
- [x] S3 파일 업로드 유틸리티
- [x] Dockerfile (멀티 스테이지) + docker-compose.yaml
- [x] .env.example + README.md
- [x] 테스트 2개 (AuthServiceTest, UserServiceTest)
