package com.base.domain.user;

import com.base.auth.repository.RefreshTokenRepository;
import com.base.common.exception.BusinessException;
import com.base.common.exception.ErrorCode;
import com.base.domain.user.dto.UpdateUserRequest;
import com.base.domain.user.dto.UserResponse;
import com.base.domain.user.entity.User;
import com.base.domain.user.repository.UserRepository;
import com.base.domain.user.service.UserService;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.PageImpl;
import org.springframework.data.domain.PageRequest;
import org.springframework.data.domain.Pageable;

import java.util.List;
import java.util.Optional;

import static org.assertj.core.api.Assertions.assertThat;
import static org.assertj.core.api.Assertions.assertThatThrownBy;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.BDDMockito.given;
import static org.mockito.Mockito.verify;

@ExtendWith(MockitoExtension.class)
@DisplayName("UserService 단위 테스트")
class UserServiceTest {

    @Mock
    private UserRepository userRepository;

    @Mock
    private RefreshTokenRepository refreshTokenRepository;

    @InjectMocks
    private UserService userService;

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
    @DisplayName("내 정보 조회 성공")
    void getMe_success() {
        // given
        given(userRepository.findById(1L)).willReturn(Optional.of(testUser));

        // when
        UserResponse response = userService.getMe(1L);

        // then
        assertThat(response.email()).isEqualTo("test@example.com");
        assertThat(response.username()).isEqualTo("testuser");
        assertThat(response.role()).isEqualTo("USER");
    }

    @Test
    @DisplayName("내 정보 조회 실패 - 사용자 없음")
    void getMe_fail_userNotFound() {
        // given
        given(userRepository.findById(99L)).willReturn(Optional.empty());

        // when & then
        assertThatThrownBy(() -> userService.getMe(99L))
                .isInstanceOf(BusinessException.class)
                .satisfies(e -> assertThat(((BusinessException) e).getErrorCode())
                        .isEqualTo(ErrorCode.USER_NOT_FOUND));
    }

    @Test
    @DisplayName("내 정보 수정 성공")
    void updateMe_success() {
        // given
        UpdateUserRequest request = new UpdateUserRequest("newusername");
        given(userRepository.findById(1L)).willReturn(Optional.of(testUser));
        given(userRepository.existsByUsername("newusername")).willReturn(false);

        // when
        UserResponse response = userService.updateMe(1L, request);

        // then
        assertThat(response.username()).isEqualTo("newusername");
    }

    @Test
    @DisplayName("내 정보 수정 실패 - 사용자명 중복")
    void updateMe_fail_usernameExists() {
        // given
        UpdateUserRequest request = new UpdateUserRequest("existinguser");
        given(userRepository.findById(1L)).willReturn(Optional.of(testUser));
        given(userRepository.existsByUsername("existinguser")).willReturn(true);

        // when & then
        assertThatThrownBy(() -> userService.updateMe(1L, request))
                .isInstanceOf(BusinessException.class)
                .satisfies(e -> assertThat(((BusinessException) e).getErrorCode())
                        .isEqualTo(ErrorCode.USERNAME_ALREADY_EXISTS));
    }

    @Test
    @DisplayName("회원 탈퇴 성공")
    void deleteMe_success() {
        // given
        given(userRepository.findById(1L)).willReturn(Optional.of(testUser));

        // when
        userService.deleteMe(1L);

        // then
        verify(refreshTokenRepository).deleteByUserId(1L);
        verify(userRepository).delete(testUser);
    }

    @Test
    @DisplayName("유저 목록 조회 성공")
    void getAllUsers_success() {
        // given
        Pageable pageable = PageRequest.of(0, 20);
        Page<User> userPage = new PageImpl<>(List.of(testUser), pageable, 1);
        given(userRepository.findAll(any(Pageable.class))).willReturn(userPage);

        // when
        Page<UserResponse> result = userService.getAllUsers(pageable);

        // then
        assertThat(result.getTotalElements()).isEqualTo(1);
        assertThat(result.getContent().get(0).email()).isEqualTo("test@example.com");
    }
}
