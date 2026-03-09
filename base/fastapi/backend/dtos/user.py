"""
User domain DTOs for request/response validation.
"""

from datetime import datetime
from typing import Optional

from pydantic import BaseModel, EmailStr, Field

from backend.domain.user.enums import AuthTypeEnum, UserStatusEnum


# ============================================================
# Request DTOs
# ============================================================


class UserCreateRequest(BaseModel):
    """Request to create a new user directly (admin use)."""
    email: EmailStr = Field(..., description="User email")
    name: str = Field(..., min_length=2, max_length=50, description="Display name")
    auth_type: AuthTypeEnum = Field(default=AuthTypeEnum.EMAIL)


class UserUpdateRequest(BaseModel):
    """Request to update user information."""
    name: Optional[str] = Field(None, min_length=2, max_length=50)
    status: Optional[UserStatusEnum] = None


# ============================================================
# Response DTOs
# ============================================================


class UserSummaryResponse(BaseModel):
    """Minimal user info for list views."""
    id: str
    email: Optional[str]
    name: str
    status: UserStatusEnum
    created_at: datetime


class UserResponse(BaseModel):
    """Full user response."""
    id: str
    email: Optional[str]
    name: str
    auth_type: AuthTypeEnum
    status: UserStatusEnum
    is_admin: bool
    created_at: datetime
    updated_at: datetime


class UserListResponse(BaseModel):
    """Paginated list of users."""
    users: list[UserSummaryResponse]
    total: int
    skip: int
    limit: int


class UserInfoDto(BaseModel):
    """Authenticated user info (used by /auth/me endpoint)."""
    id: str
    username: str
    email: Optional[str] = None
    auth_type: str
    is_admin: bool = False
