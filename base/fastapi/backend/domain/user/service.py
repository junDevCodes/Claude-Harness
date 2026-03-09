"""
User domain service with business logic.

Handles CRUD operations for User entity.
"""

import logging
from typing import Optional

from sqlmodel.ext.asyncio.session import AsyncSession

from backend.domain.user.repository import UserRepository
from backend.dtos.user import (
    UserCreateRequest,
    UserListResponse,
    UserResponse,
    UserSummaryResponse,
    UserUpdateRequest,
)
from backend.error import NotFoundError

logger = logging.getLogger(__name__)


class UserService:
    """Service for user management CRUD operations."""

    def __init__(self, session: AsyncSession):
        self.session = session
        self._user_repo = UserRepository(session)

    async def create_user(self, request: UserCreateRequest) -> UserResponse:
        """Create a new user."""
        existing = await self._user_repo.find_by_email(request.email)
        if existing:
            raise ValueError(f"User with email {request.email} already exists")

        user = await self._user_repo.create_async(
            email=request.email,
            name=request.name,
            auth_type=request.auth_type,
        )

        logger.info(f"Created user {user.id}")
        return self._to_response(user)

    async def get_user(self, user_id: str) -> UserResponse:
        """Get user by ID."""
        user = await self._user_repo.get_async(user_id)
        if not user or user.deleted_at is not None:
            raise NotFoundError(f"User {user_id} not found")
        return self._to_response(user)

    async def update_user(self, user_id: str, request: UserUpdateRequest) -> UserResponse:
        """Update user fields."""
        user = await self._user_repo.get_async(user_id)
        if not user or user.deleted_at is not None:
            raise NotFoundError(f"User {user_id} not found")

        update_data: dict = {}
        if request.name is not None:
            update_data["name"] = request.name
        if request.status is not None:
            update_data["status"] = request.status

        if update_data:
            user = await self._user_repo.update_async(user_id, **update_data)

        logger.info(f"Updated user {user_id}")
        return self._to_response(user)  # type: ignore

    async def delete_user(self, user_id: str) -> bool:
        """Soft delete user."""
        result = await self._user_repo.soft_delete(user_id)
        if result:
            logger.info(f"Soft deleted user {user_id}")
        return result

    async def list_users(
        self,
        skip: int = 0,
        limit: int = 20,
        query: Optional[str] = None,
    ) -> UserListResponse:
        """List users with pagination."""
        users = await self._user_repo.list_async(
            skip=skip,
            limit=limit,
            order_by="-created_at",
        )
        total = await self._user_repo.count_async(filters={"deleted_at": None})

        return UserListResponse(
            users=[self._to_summary(u) for u in users],
            total=total,
            skip=skip,
            limit=limit,
        )

    def _to_response(self, user) -> UserResponse:  # type: ignore
        return UserResponse(
            id=user.id,
            email=user.email,
            name=user.name or "",
            auth_type=user.auth_type,
            status=user.status,
            is_admin=user.is_admin,
            created_at=user.created_at,
            updated_at=user.updated_at,
        )

    def _to_summary(self, user) -> UserSummaryResponse:  # type: ignore
        return UserSummaryResponse(
            id=user.id,
            email=user.email,
            name=user.name or "",
            status=user.status,
            created_at=user.created_at,
        )
