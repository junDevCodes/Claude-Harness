package com.base.auth;

import com.base.auth.dto.LoginRequest;
import com.base.auth.dto.RefreshRequest;
import com.base.auth.dto.SignUpRequest;
import com.base.auth.dto.TokenResponse;
import com.base.auth.entity.RefreshToken;
import com.base.auth.repository.RefreshTokenRepository;
import com.base.auth.service.AuthService;
import com.base.common.exception.BusinessException;
import com.base.common.exception.ErrorCode;
import com.base.domain.user.entity.User;
import com.base.domain.user.repository.UserRepository;
import com.base.security.JwtProvider;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;
import org.springframework.security.crypto.password.PasswordEncoder;

import java.time.LocalDateTime;
import java.util.Optional;

import static org.assertj.core.api.Assertions.assertThat;
import static org.assertj.core.api.Assertions.assertThatThrownBy;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyLong;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.BDDMockito.given;
import static org.mockito.Mockito.verify;

@ExtendWith(MockitoExtension.class)
@DisplayName("AuthService 단위 테스트")
class AuthServiceTest {

    @Mock
    private UserRepository userRepository;

    @Mock
    private RefreshTokenRepository refreshTokenRepository;

    @Mock
    private JwtProvider jwtProvider;

    @Mock
    private PasswordEncoder passwordEncoder;

    @InjectMocks
    private AuthService authService;

    private User testUser;

    @BeforeEach
    void setUp() {
        testUser = User.builder()
                .email("test@example.com")
                .password("encodedPassword")
                .username("testuser")
                .role(User.Role.USER)
                .build();
    }

    @Test
    @DisplayName("회원가입 성공")
    void signUp_success() {
        // given
        SignUpRequest request = new SignUpRequest("test@example.com", "password123", "testuser");
        given(userRepository.existsByEmail(request.email())).willReturn(false);
        given(userRepository.existsByUsername(request.username())).willReturn(false);
        given(passwordEncoder.encode(anyString())).willReturn("encodedPassword");
        given(userRepository.save(any(User.class))).willReturn(testUser);
        given(jwtProvider.createAccessToken(anyLong(), anyString(), anyString())).willReturn("access-token");
        given(jwtProvider.createRefreshToken(anyLong())).willReturn("refresh-token");
        given(jwtProvider.getRefreshTokenExpiry()).willReturn(604800L);
        given(refreshTokenRepository.findByUserId(anyLong())).willReturn(Optional.empty());

        // when
        TokenResponse response = authService.signUp(request);

        // then
        assertThat(response.accessToken()).isEqualTo("access-token");
        assertThat(response.refreshToken()).isEqualTo("refresh-token");
        assertThat(response.tokenType()).isEqualTo("Bearer");
        verify(userRepository).save(any(User.class));
    }

    @Test
    @DisplayName("회원가입 실패 - 이메일 중복")
    void signUp_fail_emailExists() {
        // given
        SignUpRequest request = new SignUpRequest("test@example.com", "password123", "testuser");
        given(userRepository.existsByEmail(request.email())).willReturn(true);

        // when & then
        assertThatThrownBy(() -> authService.signUp(request))
                .isInstanceOf(BusinessException.class)
                .satisfies(e -> assertThat(((BusinessException) e).getErrorCode())
                        .isEqualTo(ErrorCode.EMAIL_ALREADY_EXISTS));
    }

    @Test
    @DisplayName("로그인 성공")
    void login_success() {
        // given
        LoginRequest request = new LoginRequest("test@example.com", "password123");
        given(userRepository.findByEmail(request.email())).willReturn(Optional.of(testUser));
        given(passwordEncoder.matches(request.password(), testUser.getPassword())).willReturn(true);
        given(jwtProvider.createAccessToken(anyLong(), anyString(), anyString())).willReturn("access-token");
        given(jwtProvider.createRefreshToken(anyLong())).willReturn("refresh-token");
        given(jwtProvider.getRefreshTokenExpiry()).willReturn(604800L);
        given(refreshTokenRepository.findByUserId(anyLong())).willReturn(Optional.empty());

        // when
        TokenResponse response = authService.login(request);

        // then
        assertThat(response.accessToken()).isEqualTo("access-token");
        assertThat(response.refreshToken()).isEqualTo("refresh-token");
    }

    @Test
    @DisplayName("로그인 실패 - 이메일 없음")
    void login_fail_emailNotFound() {
        // given
        LoginRequest request = new LoginRequest("notfound@example.com", "password123");
        given(userRepository.findByEmail(request.email())).willReturn(Optional.empty());

        // when & then
        assertThatThrownBy(() -> authService.login(request))
                .isInstanceOf(BusinessException.class)
                .satisfies(e -> assertThat(((BusinessException) e).getErrorCode())
                        .isEqualTo(ErrorCode.INVALID_CREDENTIALS));
    }

    @Test
    @DisplayName("로그인 실패 - 비밀번호 불일치")
    void login_fail_wrongPassword() {
        // given
        LoginRequest request = new LoginRequest("test@example.com", "wrongPassword");
        given(userRepository.findByEmail(request.email())).willReturn(Optional.of(testUser));
        given(passwordEncoder.matches(request.password(), testUser.getPassword())).willReturn(false);

        // when & then
        assertThatThrownBy(() -> authService.login(request))
                .isInstanceOf(BusinessException.class)
                .satisfies(e -> assertThat(((BusinessException) e).getErrorCode())
                        .isEqualTo(ErrorCode.INVALID_CREDENTIALS));
    }

    @Test
    @DisplayName("토큰 갱신 실패 - 리프레시 토큰 없음")
    void refresh_fail_tokenNotFound() {
        // given
        RefreshRequest request = new RefreshRequest("unknown-refresh-token");
        given(jwtProvider.isRefreshToken(request.refreshToken())).willReturn(true);
        given(jwtProvider.getUserId(request.refreshToken())).willReturn(1L);
        given(refreshTokenRepository.findByToken(request.refreshToken())).willReturn(Optional.empty());

        // when & then
        assertThatThrownBy(() -> authService.refresh(request))
                .isInstanceOf(BusinessException.class)
                .satisfies(e -> assertThat(((BusinessException) e).getErrorCode())
                        .isEqualTo(ErrorCode.REFRESH_TOKEN_NOT_FOUND));
    }

    @Test
    @DisplayName("로그아웃 - refresh token 삭제")
    void logout_success() {
        // when
        authService.logout(1L);

        // then
        verify(refreshTokenRepository).deleteByUserId(1L);
    }
}
