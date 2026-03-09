"""
Authentication service for user domain.

Handles email/password authentication with JWT access + refresh tokens.
"""

from datetime import datetime, timedelta, timezone
from typing import Optional

import jwt
from fastapi import Depends, HTTPException, status
from fastapi.security import HTTPAuthorizationCredentials, HTTPBearer
from sqlmodel.ext.asyncio.session import AsyncSession

from backend.core.config import settings
from backend.domain.user.repository import UserRepository
from backend.dtos.auth import (
    EmailLoginRequestDto,
    EmailSignUpRequestDto,
    LoginResponseDto,
    RefreshTokenResponseDto,
)
from backend.utils.password import hash_password, verify_password

import logging

logger = logging.getLogger(__name__)

security = HTTPBearer(auto_error=False)


class AuthService:
    """Service for authentication operations (sign-up, login, token refresh)."""

    def __init__(self, session: AsyncSession):
        self.session = session
        self._user_repo = UserRepository(session)

    def _create_access_token(self, user_id: str) -> str:
        """Create short-lived JWT access token."""
        expire = datetime.now(timezone.utc) + timedelta(
            minutes=settings.access_token_expire_minutes
        )
        payload = {"sub": user_id, "exp": expire, "type": "access"}
        return jwt.encode(payload, settings.jwt_secret_key, algorithm=settings.jwt_algorithm)

    def _create_refresh_token(self, user_id: str) -> str:
        """Create long-lived JWT refresh token."""
        expire = datetime.now(timezone.utc) + timedelta(
            days=settings.refresh_token_expire_days
        )
        payload = {"sub": user_id, "exp": expire, "type": "refresh"}
        return jwt.encode(payload, settings.jwt_secret_key, algorithm=settings.jwt_algorithm)

    def _verify_token(self, token: str, token_type: str = "access") -> Optional[str]:
        """Verify JWT token and return user_id if valid."""
        try:
            payload = jwt.decode(
                token,
                settings.jwt_secret_key,
                algorithms=[settings.jwt_algorithm],
            )
            if payload.get("type") != token_type:
                return None
            return payload.get("sub")
        except (jwt.ExpiredSignatureError, jwt.InvalidTokenError):
            return None

    async def email_sign_up(self, request: EmailSignUpRequestDto) -> LoginResponseDto:
        """Register user with email and password."""
        existing = await self._user_repo.find_by_email(request.email)
        if existing:
            raise HTTPException(
                status_code=status.HTTP_409_CONFLICT,
                detail="Email already registered",
            )

        hashed_password = hash_password(request.password)
        user = await self._user_repo.create_async(
            email=request.email,
            name=request.username,
            auth_type="email",
            auth_provider_id=hashed_password,
        )

        logger.info(f"User signed up: {user.id}")
        access_token = self._create_access_token(user.id)
        refresh_token = self._create_refresh_token(user.id)

        return LoginResponseDto(
            user_id=user.id,
            access_token=access_token,
            refresh_token=refresh_token,
            username=user.name,
        )

    async def email_login(self, request: EmailLoginRequestDto) -> LoginResponseDto:
        """Authenticate user with email and password."""
        user = await self._user_repo.find_by_email(request.email)
        if not user or not user.auth_provider_id:
            raise HTTPException(
                status_code=status.HTTP_401_UNAUTHORIZED,
                detail="Invalid email or password",
            )

        if not verify_password(request.password, user.auth_provider_id):
            raise HTTPException(
                status_code=status.HTTP_401_UNAUTHORIZED,
                detail="Invalid email or password",
            )

        logger.info(f"User logged in: {user.id}")
        access_token = self._create_access_token(user.id)
        refresh_token = self._create_refresh_token(user.id)

        return LoginResponseDto(
            user_id=user.id,
            access_token=access_token,
            refresh_token=refresh_token,
            username=user.name,
        )

    async def refresh_access_token(self, refresh_token: str) -> RefreshTokenResponseDto:
        """Issue new access + refresh token pair using a valid refresh token."""
        user_id = self._verify_token(refresh_token, token_type="refresh")
        if not user_id:
            raise HTTPException(
                status_code=status.HTTP_401_UNAUTHORIZED,
                detail="Invalid or expired refresh token",
            )

        user = await self._user_repo.get_async(user_id)
        if not user or user.deleted_at is not None:
            raise HTTPException(
                status_code=status.HTTP_401_UNAUTHORIZED,
                detail="User not found",
            )

        new_access_token = self._create_access_token(user_id)
        new_refresh_token = self._create_refresh_token(user_id)

        return RefreshTokenResponseDto(
            access_token=new_access_token,
            refresh_token=new_refresh_token,
        )

    async def get_current_user_info(self, user_id: str) -> dict:
        """Fetch current user info dict."""
        user = await self._user_repo.get_async(user_id)
        if not user or user.deleted_at is not None:
            raise HTTPException(
                status_code=status.HTTP_404_NOT_FOUND,
                detail="User not found",
            )
        return {
            "id": user.id,
            "username": user.name or "User",
            "email": user.email,
            "auth_type": str(user.auth_type.value) if user.auth_type else "email",
            "is_admin": user.is_admin,
        }


async def get_user_id(
    credentials: Optional[HTTPAuthorizationCredentials] = Depends(security),
) -> str:
    """FastAPI dependency: extract and validate user_id from Bearer JWT token.

    Usage:
        user_id: str = Depends(get_user_id)
    """
    # Mock auth for development
    if settings.mock_auth_enabled:
        return "mock-user-001"

    if not credentials:
        raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="Missing authentication token",
        )

    try:
        payload = jwt.decode(
            credentials.credentials,
            settings.jwt_secret_key,
            algorithms=[settings.jwt_algorithm],
        )
        user_id = payload.get("sub")
        if not user_id:
            raise HTTPException(
                status_code=status.HTTP_401_UNAUTHORIZED,
                detail="Invalid token",
            )
        return user_id
    except jwt.ExpiredSignatureError:
        raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="Token expired",
        )
    except jwt.InvalidTokenError:
        raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="Invalid token",
        )
