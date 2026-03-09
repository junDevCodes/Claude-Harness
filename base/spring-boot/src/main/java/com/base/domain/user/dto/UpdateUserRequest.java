package com.base.domain.user.dto;

import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;

public record UpdateUserRequest(
        @NotBlank(message = "사용자명은 필수입니다.")
        @Size(min = 2, max = 30, message = "사용자명은 2~30자 사이여야 합니다.")
        String username
) {}
