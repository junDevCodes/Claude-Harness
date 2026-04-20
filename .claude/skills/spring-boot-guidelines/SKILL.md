---
name: spring-boot-guidelines
description: Spring Boot 3 + Java 21 백엔드 — Controller→Service→Repository 계층, JPA Entity(Lombok+@PrePersist), Spring Security JWT Filter(jjwt 0.12.x + Refresh Rotation), GlobalExceptionHandler, ApiResponse<T>, @Transactional(readOnly), @PreAuthorize 메서드 인가.
triggers:
  - spring boot
  - spring security
  - jpa entity
  - spring controller
  - restcontroller
  - transactional
  - jwt filter
  - hibernate
  - jwtprovider
  - refreshtoken
  - businessexception
  - errorcode
  - globalexceptionhandler
  - spring repository
  - spring service
---

# Spring Boot 3 Backend Guidelines

## Purpose

Spring Boot 3 + Java 21 + Spring Security + JPA 기반 REST API 개발 표준 패턴.
`base/spring-boot/` 베이스 코드의 핵심 패턴을 정리한 실전 가이드.

## 프로젝트 구조

```
src/main/java/com/base/
├── BaseApplication.java
├── common/
│   ├── exception/       # ErrorCode, BusinessException, GlobalExceptionHandler
│   └── response/        # ApiResponse<T>
├── config/              # SecurityConfig, S3Config
├── security/            # JwtProvider, JwtAuthenticationFilter, CustomUserDetailsService
├── auth/                # RefreshToken entity/repo, AuthService, AuthController
├── domain/
│   └── user/            # User entity, UserRepository, UserService, UserController
└── utils/               # S3Uploader
```

---

## Pattern 1: 계층 구조 (Controller → Service → Repository)

**핵심 규칙:**
- Controller: HTTP 처리, DTO 검증, `@AuthenticationPrincipal`로 userId 추출
- Service: 비즈니스 로직, `@Transactional` 관리, BusinessException 발생
- Repository: JPA 쿼리, `JpaRepository` 확장

**Controller 패턴:**
```java
@RestController
@RequiredArgsConstructor
@RequestMapping("/api/v1/users")
public class UserController {

    private final UserService userService;

    @GetMapping("/me")
    public ApiResponse<UserResponse> getMe(@AuthenticationPrincipal Long userId) {
        return ApiResponse.ok(userService.getMe(userId));
    }

    @PatchMapping("/me")
    public ApiResponse<UserResponse> updateMe(
            @AuthenticationPrincipal Long userId,
            @Valid @RequestBody UpdateUserRequest request) {
        return ApiResponse.ok(userService.updateMe(userId, request));
    }

    @DeleteMapping("/me")
    @ResponseStatus(HttpStatus.NO_CONTENT)
    public ApiResponse<Void> deleteMe(@AuthenticationPrincipal Long userId) {
        userService.deleteMe(userId);
        return ApiResponse.noContent();
    }
}
```

**Service 패턴 (`@Transactional` 분리):**
```java
@Service
@RequiredArgsConstructor
public class UserService {

    private final UserRepository userRepository;

    @Transactional(readOnly = true)   // 조회: readOnly = true (flush 없음, 성능↑)
    public UserResponse getMe(Long userId) {
        return UserResponse.from(findUserById(userId));
    }

    @Transactional                    // 변경: 기본 트랜잭션
    public UserResponse updateMe(Long userId, UpdateUserRequest request) {
        User user = findUserById(userId);
        // 중복 체크
        if (!user.getUsername().equals(request.username())
                && userRepository.existsByUsername(request.username())) {
            throw new BusinessException(ErrorCode.USERNAME_ALREADY_EXISTS);
        }
        user.updateUsername(request.username());  // 변경 감지(dirty checking)
        return UserResponse.from(user);
    }

    // private 조회 헬퍼 → 재사용 + 예외 중앙화
    private User findUserById(Long userId) {
        return userRepository.findById(userId)
                .orElseThrow(() -> new BusinessException(ErrorCode.USER_NOT_FOUND));
    }
}
```

---

## Pattern 2: JPA Entity

```java
@Entity
@Table(name = "users")
@Getter
@NoArgsConstructor(access = AccessLevel.PROTECTED)  // JPA 기본 생성자 (외부 new 방지)
@AllArgsConstructor
@Builder
public class User {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(unique = true, nullable = false)
    private String email;

    @Column(nullable = false)
    private String password;

    @Column(unique = true, nullable = false, length = 30)
    private String username;

    @Enumerated(EnumType.STRING)   // DB에 "USER"/"ADMIN" 문자열 저장
    @Column(nullable = false)
    private Role role;

    @Column(name = "created_at", nullable = false, updatable = false)
    private LocalDateTime createdAt;

    @Column(name = "updated_at", nullable = false)
    private LocalDateTime updatedAt;

    @PrePersist
    protected void onCreate() {      // INSERT 시 자동 설정
        createdAt = LocalDateTime.now();
        updatedAt = LocalDateTime.now();
    }

    @PreUpdate
    protected void onUpdate() {      // UPDATE 시 자동 설정
        updatedAt = LocalDateTime.now();
    }

    // 비즈니스 메서드 — setter 대신 의미 있는 메서드 이름
    public void updateUsername(String username) {
        this.username = username;
    }

    public enum Role { USER, ADMIN }
}
```

**DTO는 Java Record 사용:**
```java
public record SignUpRequest(
    @NotBlank @Email String email,
    @NotBlank @Size(min = 8) String password,
    @NotBlank @Size(max = 30) String username
) {}

public record UserResponse(Long id, String email, String username, String role) {
    public static UserResponse from(User user) {
        return new UserResponse(user.getId(), user.getEmail(),
                user.getUsername(), user.getRole().name());
    }
}
```

---

## Pattern 3: JWT 인증 (JwtProvider + Refresh Token Rotation)

**JwtProvider (jjwt 0.12.x):**
```java
@Component
public class JwtProvider {

    private final SecretKey secretKey;
    private final long accessTokenExpiry;
    private final long refreshTokenExpiry;

    public JwtProvider(@Value("${jwt.secret}") String secret,
                       @Value("${jwt.access-token-expiry}") long accessExpiry,
                       @Value("${jwt.refresh-token-expiry}") long refreshExpiry) {
        this.secretKey = Keys.hmacShaKeyFor(Decoders.BASE64.decode(secret));
        this.accessTokenExpiry = accessExpiry;
        this.refreshTokenExpiry = refreshExpiry;
    }

    public String createAccessToken(Long userId, String email, String role) {
        return Jwts.builder()
                .subject(String.valueOf(userId))
                .claim("email", email)
                .claim("role", role)
                .claim("type", "access")
                .issuedAt(Date.from(Instant.now()))
                .expiration(Date.from(Instant.now().plusSeconds(accessTokenExpiry)))
                .signWith(secretKey)
                .compact();
    }

    public Claims parseToken(String token) {
        try {
            return Jwts.parser().verifyWith(secretKey).build()
                    .parseSignedClaims(token).getPayload();
        } catch (ExpiredJwtException e) {
            throw new BusinessException(ErrorCode.EXPIRED_TOKEN);
        } catch (JwtException | IllegalArgumentException e) {
            throw new BusinessException(ErrorCode.INVALID_TOKEN);
        }
    }
}
```

**Refresh Token Rotation — 1유저 1토큰:**
```java
// 로그인/회원가입 공통 토큰 발급 로직
private TokenResponse issueTokens(User user) {
    String accessToken = jwtProvider.createAccessToken(...);
    String refreshToken = jwtProvider.createRefreshToken(user.getId());
    LocalDateTime expiresAt = LocalDateTime.now().plusSeconds(jwtProvider.getRefreshTokenExpiry());

    // 기존 토큰이 있으면 rotate, 없으면 신규 저장
    refreshTokenRepository.findByUserId(user.getId()).ifPresentOrElse(
        existing -> existing.rotate(refreshToken, expiresAt),  // 기존 토큰 갱신
        () -> refreshTokenRepository.save(RefreshToken.builder()
                .userId(user.getId()).token(refreshToken).expiresAt(expiresAt).build())
    );

    return TokenResponse.of(accessToken, refreshToken);
}

// /auth/refresh — 토큰 갱신
public TokenResponse refresh(RefreshRequest request) {
    if (!jwtProvider.isRefreshToken(request.refreshToken()))
        throw new BusinessException(ErrorCode.INVALID_TOKEN);

    RefreshToken stored = refreshTokenRepository.findByToken(request.refreshToken())
            .orElseThrow(() -> new BusinessException(ErrorCode.REFRESH_TOKEN_NOT_FOUND));

    if (stored.isExpired()) {
        refreshTokenRepository.delete(stored);
        throw new BusinessException(ErrorCode.REFRESH_TOKEN_EXPIRED);
    }
    // 새 토큰 발급 후 기존 토큰 rotate (userId는 JWT 파싱 or stored에서 획득)
    Long userId = jwtProvider.getUserId(request.refreshToken());
    User user = userRepository.findById(userId).orElseThrow(() -> new BusinessException(ErrorCode.USER_NOT_FOUND));
    String newAccess = jwtProvider.createAccessToken(userId, user.getEmail(), user.getRole().name());
    String newRefresh = jwtProvider.createRefreshToken(userId);
    stored.rotate(newRefresh, LocalDateTime.now().plusSeconds(jwtProvider.getRefreshTokenExpiry()));
    return TokenResponse.of(newAccess, newRefresh);
}
```

---

## Pattern 4: 예외 처리 (ErrorCode + BusinessException + GlobalExceptionHandler)

**ErrorCode Enum — HTTP 상태 + 에러코드 중앙 관리:**
```java
@Getter
@RequiredArgsConstructor
public enum ErrorCode {
    INVALID_INPUT_VALUE(HttpStatus.BAD_REQUEST, "INVALID_INPUT_VALUE", "잘못된 입력값입니다."),
    METHOD_NOT_ALLOWED(HttpStatus.METHOD_NOT_ALLOWED, "METHOD_NOT_ALLOWED", "지원하지 않는 HTTP Method입니다."),
    ACCESS_DENIED(HttpStatus.FORBIDDEN, "ACCESS_DENIED", "접근 권한이 없습니다."),
    INTERNAL_SERVER_ERROR(HttpStatus.INTERNAL_SERVER_ERROR, "INTERNAL_SERVER_ERROR", "서버 오류가 발생했습니다."),
    INVALID_CREDENTIALS(HttpStatus.UNAUTHORIZED, "INVALID_CREDENTIALS", "이메일 또는 비밀번호가 올바르지 않습니다."),
    EXPIRED_TOKEN(HttpStatus.UNAUTHORIZED, "EXPIRED_TOKEN", "만료된 토큰입니다."),
    INVALID_TOKEN(HttpStatus.UNAUTHORIZED, "INVALID_TOKEN", "유효하지 않은 토큰입니다."),
    REFRESH_TOKEN_NOT_FOUND(HttpStatus.UNAUTHORIZED, "REFRESH_TOKEN_NOT_FOUND", "리프레시 토큰을 찾을 수 없습니다."),
    REFRESH_TOKEN_EXPIRED(HttpStatus.UNAUTHORIZED, "REFRESH_TOKEN_EXPIRED", "리프레시 토큰이 만료되었습니다."),
    USER_NOT_FOUND(HttpStatus.NOT_FOUND, "USER_NOT_FOUND", "사용자를 찾을 수 없습니다."),
    EMAIL_ALREADY_EXISTS(HttpStatus.CONFLICT, "EMAIL_ALREADY_EXISTS", "이미 사용 중인 이메일입니다."),
    USERNAME_ALREADY_EXISTS(HttpStatus.CONFLICT, "USERNAME_ALREADY_EXISTS", "이미 사용 중인 사용자명입니다.");

    private final HttpStatus httpStatus;
    private final String code;
    private final String message;
}
```

**BusinessException — 도메인 예외의 기반 클래스:**
```java
@Getter
public class BusinessException extends RuntimeException {
    private final ErrorCode errorCode;

    public BusinessException(ErrorCode errorCode) {
        super(errorCode.getMessage());
        this.errorCode = errorCode;
    }
}
```

**GlobalExceptionHandler:**
```java
@Slf4j
@RestControllerAdvice
public class GlobalExceptionHandler {

    @ExceptionHandler(BusinessException.class)
    public ResponseEntity<ApiResponse<Void>> handleBusinessException(BusinessException e) {
        ErrorCode errorCode = e.getErrorCode();
        log.warn("BusinessException: code={}, message={}", errorCode.getCode(), e.getMessage());
        return ResponseEntity.status(errorCode.getHttpStatus())
                .body(ApiResponse.error(e.getMessage(), errorCode.getCode()));
    }

    @ExceptionHandler(MethodArgumentNotValidException.class)
    public ResponseEntity<ApiResponse<Void>> handleValidation(MethodArgumentNotValidException e) {
        String message = e.getBindingResult().getFieldErrors().stream()
                .map(err -> err.getField() + ": " + err.getDefaultMessage())
                .collect(Collectors.joining(", "));
        return ResponseEntity.badRequest()
                .body(ApiResponse.error(message, ErrorCode.INVALID_INPUT_VALUE.getCode()));
    }

    @ExceptionHandler(AccessDeniedException.class)
    public ResponseEntity<ApiResponse<Void>> handleAccessDeniedException(AccessDeniedException e) {
        return ResponseEntity.status(HttpStatus.FORBIDDEN)
                .body(ApiResponse.error(ErrorCode.ACCESS_DENIED.getMessage(), ErrorCode.ACCESS_DENIED.getCode()));
    }

    @ExceptionHandler(Exception.class)
    public ResponseEntity<ApiResponse<Void>> handleException(Exception e) {
        log.error("Unhandled exception", e);
        return ResponseEntity.internalServerError()
                .body(ApiResponse.error(
                        ErrorCode.INTERNAL_SERVER_ERROR.getMessage(),
                        ErrorCode.INTERNAL_SERVER_ERROR.getCode()));
    }
}
```

---

## Pattern 5: ApiResponse<T> 표준 응답 래퍼

```java
@Getter
@JsonInclude(JsonInclude.Include.NON_NULL)  // null 필드 JSON 직렬화 제외
public class ApiResponse<T> {

    private final boolean success;
    private final T data;
    private final String message;
    private final String code;

    private ApiResponse(boolean success, T data, String message, String code) {
        this.success = success; this.data = data; this.message = message; this.code = code;
    }
    public static <T> ApiResponse<T> ok(T data) { return new ApiResponse<>(true, data, "OK", "SUCCESS"); }
    public static <T> ApiResponse<T> ok(T data, String msg) { return new ApiResponse<>(true, data, msg, "SUCCESS"); }
    public static <T> ApiResponse<T> created(T data) { return new ApiResponse<>(true, data, "Created", "CREATED"); }
    public static ApiResponse<Void> noContent() { return new ApiResponse<>(true, null, "No Content", "NO_CONTENT"); }
    public static <T> ApiResponse<T> error(String message, String code) { return new ApiResponse<>(false, null, message, code); }
}
```

**Controller 응답 예시:**
```java
// 200 OK
return ApiResponse.ok(userService.getMe(userId));

// 201 Created — 메서드에 @ResponseStatus(HttpStatus.CREATED) 적용
return ApiResponse.created(authService.signUp(request));

// 204 No Content — 메서드에 @ResponseStatus(HttpStatus.NO_CONTENT) 적용
return ApiResponse.noContent();
```

---

## Spring Security 설정 포인트

```java
@Configuration
@EnableWebSecurity
@EnableMethodSecurity           // @PreAuthorize 활성화
@RequiredArgsConstructor
public class SecurityConfig {

    private final JwtAuthenticationFilter jwtAuthFilter;

    @Bean
    public SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
        return http
            .csrf(AbstractHttpConfigurer::disable)
            .sessionManagement(s -> s.sessionCreationPolicy(STATELESS))  // JWT 무상태
            .authorizeHttpRequests(auth -> auth
                .requestMatchers("/api/v1/auth/**", "/api/v1/health").permitAll()
                .anyRequest().authenticated())
            .addFilterBefore(jwtAuthFilter, UsernamePasswordAuthenticationFilter.class)
            .build();
    }

    @Bean
    public PasswordEncoder passwordEncoder() { return new BCryptPasswordEncoder(); }
}

// 메서드 레벨 인가
@GetMapping("/admin/users")
@PreAuthorize("hasRole('ADMIN')")
public ApiResponse<Page<UserResponse>> getAllUsers(Pageable pageable) { ... }
```

---

## 환경 분리 포인트

| 설정 | local | prod |
|---|---|---|
| `jpa.hibernate.ddl-auto` | `create-drop` | `validate` |
| `spring.profiles.active` | `local` | `prod` |
| DB credentials | `.env` 직접 | 환경변수 주입 |

`application.yaml`에서 모든 민감 값은 `${ENV_VAR}` 형태로 주입. `.env.example`에 전체 목록 유지.

---

## Anti-patterns (하지 말 것)

```java
// ❌ Controller에서 직접 Repository 호출
@GetMapping("/me")
public User getMe(@AuthenticationPrincipal CustomUserDetails details) {
    return userRepository.findById(details.getUserId()).get();  // 레이어 침범
}

// ✅ Service를 통해 호출 (ApiResponse<T> 직접 반환)
public ApiResponse<UserResponse> getMe(@AuthenticationPrincipal Long userId) {
    return ApiResponse.ok(userService.getMe(userId));
}

// ❌ 변경 메서드에 @Transactional(readOnly = true)
@Transactional(readOnly = true)
public void updateUser(...) { user.updateName(name); }  // dirty checking 안됨!

// ✅ 조회만 readOnly, 변경은 기본 트랜잭션
@Transactional(readOnly = true) public UserResponse getUser(...) { ... }
@Transactional public UserResponse updateUser(...) { ... }

// ❌ Entity에 public setter 노출
public void setUsername(String username) { this.username = username; }

// ✅ 의미 있는 비즈니스 메서드
public void updateUsername(String username) { this.username = username; }

// ❌ @Enumerated 없이 enum 저장 (숫자로 저장 → 순서 변경 시 버그)
@Column private Role role;

// ✅ STRING 타입으로 저장
@Enumerated(EnumType.STRING) @Column private Role role;

// ❌ RuntimeException 직접 throw
throw new RuntimeException("User not found");

// ✅ ErrorCode 기반 BusinessException
throw new BusinessException(ErrorCode.USER_NOT_FOUND);
```

---

## Quick Reference: 신규 도메인 추가 체크리스트

- [ ] `domain/{name}/entity/{Name}.java` — `@Entity`, Lombok, `@PrePersist`
- [ ] `domain/{name}/repository/{Name}Repository.java` — `extends JpaRepository<>`
- [ ] `domain/{name}/dto/` — Java Record (Request/Response)
- [ ] `domain/{name}/service/{Name}Service.java` — `@Service`, `@Transactional`
- [ ] `domain/{name}/controller/{Name}Controller.java` — `@RestController`
- [ ] `ErrorCode` enum에 도메인별 에러 코드 추가
- [ ] `SecurityConfig`에 공개 URL 경로 추가 (필요 시)

---

## 관련 스킬

- **docker-guidelines**: Dockerfile 멀티 스테이지 빌드 | **database-guidelines**: JPA N+1, 인덱스 전략
