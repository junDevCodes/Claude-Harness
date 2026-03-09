"""
User domain SQLModel models.

Table Structure:
- User: Core user entity with authentication info (soft delete supported)

Customization:
  - Add new fields to User as needed
  - Create additional related models in this file
  - Maintain soft delete pattern (deleted_at) for audit trail
"""

from datetime import datetime, timezone
from typing import Optional

from sqlalchemy import Boolean
from sqlmodel import Column, DateTime, Field, SQLModel, Text

from ulid import ULID

from backend.domain.user.enums import AuthTypeEnum, UserStatusEnum


def generate_user_id() -> str:
    """Generate prefixed ULID for User."""
    return f"usr_{ULID()}"


class User(SQLModel, table=True):
    """
    Core user entity.

    Supports soft delete via deleted_at.
    Password is stored hashed in auth_provider_id for email auth.
    """

    __tablename__ = "user"

    id: str = Field(
        default_factory=generate_user_id,
        primary_key=True,
        max_length=30,
    )

    # Authentication
    auth_type: AuthTypeEnum = Field(
        default=AuthTypeEnum.EMAIL,
        sa_column=Column(Text, nullable=False),
    )
    auth_provider_id: Optional[str] = Field(
        default=None,
        sa_column=Column(Text, nullable=True),
    )  # hashed password for email auth; provider UID for social auth

    # Basic info
    email: Optional[str] = Field(
        default=None,
        sa_column=Column(Text, nullable=True, unique=True, index=True),
    )
    name: Optional[str] = Field(
        default=None,
        sa_column=Column(Text, nullable=True),
    )

    # Status
    status: UserStatusEnum = Field(
        default=UserStatusEnum.ACTIVE,
        sa_column=Column(Text, nullable=False, index=True),
    )

    # Admin flag
    is_admin: bool = Field(
        default=False,
        sa_column=Column(Boolean, nullable=False, server_default="false"),
    )

    # Timestamps
    created_at: datetime = Field(
        sa_column=Column(DateTime(timezone=True), nullable=False),
        default_factory=lambda: datetime.now(tz=timezone.utc),
    )
    updated_at: datetime = Field(
        sa_column=Column(DateTime(timezone=True), nullable=False),
        default_factory=lambda: datetime.now(tz=timezone.utc),
    )
    deleted_at: Optional[datetime] = Field(
        sa_column=Column(DateTime(timezone=True), nullable=True),
        default=None,
    )
