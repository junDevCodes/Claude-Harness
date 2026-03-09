"""
User domain repository with common query patterns.

Extend this class to add domain-specific queries.
"""

from datetime import datetime, timezone
from typing import Optional

from sqlalchemy import and_
from sqlmodel import select
from sqlmodel.ext.asyncio.session import AsyncSession

from backend.domain.shared.base_repository import BaseRepository
from backend.domain.user.model import User


class UserRepository(BaseRepository[User]):
    """Repository for User entity."""

    def __init__(self, session: AsyncSession):
        super().__init__(session, User)

    async def find_by_email(self, email: str) -> Optional[User]:
        """Find active user by email."""
        stmt = select(User).where(
            and_(
                User.email == email,
                User.deleted_at.is_(None),
            )
        )
        result = await self.session.execute(stmt)
        return result.scalar_one_or_none()

    async def soft_delete(self, user_id: str) -> bool:
        """Soft delete user by setting deleted_at timestamp."""
        user = await self.get_async(user_id)
        if not user or user.deleted_at is not None:
            return False

        user.deleted_at = datetime.now(tz=timezone.utc)
        user.updated_at = datetime.now(tz=timezone.utc)
        self.session.add(user)
        await self.session.commit()
        return True
