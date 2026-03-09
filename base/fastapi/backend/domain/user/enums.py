"""User domain enumerations."""

from enum import Enum


class AuthTypeEnum(str, Enum):
    """Authentication type."""
    EMAIL = "email"
    GOOGLE = "google"
    KAKAO = "kakao"


class UserStatusEnum(str, Enum):
    """User account status."""
    ACTIVE = "active"
    INACTIVE = "inactive"
    SUSPENDED = "suspended"
