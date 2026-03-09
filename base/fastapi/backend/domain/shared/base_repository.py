"""
Generic base repository providing CRUD operations for SQLModel entities.

Usage:
    class UserRepository(BaseRepository[User]):
        def __init__(self, session: AsyncSession):
            super().__init__(session, User)

        async def find_by_email(self, email: str) -> Optional[User]:
            stmt = select(User).where(User.email == email)
            result = await self.session.execute(stmt)
            return result.scalar_one_or_none()
"""

from datetime import UTC, datetime
from typing import Any, Dict, Generic, List, Optional, Type, TypeVar

from sqlalchemy.exc import IntegrityError, SQLAlchemyError
from sqlalchemy.ext.asyncio import AsyncSession
from sqlmodel import SQLModel, func, select

import logging

logger = logging.getLogger(__name__)

ModelType = TypeVar("ModelType", bound=SQLModel)


class BaseRepository(Generic[ModelType]):
    """
    Base repository providing common CRUD operations.

    Provides:
    - Basic CRUD (get, list, create, update, delete)
    - Batch operations (bulk_create, bulk_update)
    - Query utilities (find_by, filter_by, count, exists)
    """

    def __init__(self, session: AsyncSession, model: Type[ModelType]):
        self.session = session
        self.model = model

    async def get_async(self, id: str | int) -> Optional[ModelType]:
        """Get entity by primary key ID."""
        try:
            statement = select(self.model).where(self.model.id == id)  # type: ignore
            result = await self.session.execute(statement)
            return result.scalar_one_or_none()
        except SQLAlchemyError as e:
            logger.exception(f"Error fetching {self.model.__name__} id={id}: {e}")
            return None

    async def list_async(
        self,
        skip: int = 0,
        limit: Optional[int] = None,
        order_by: Optional[str] = None,
    ) -> List[ModelType]:
        """List entities with optional pagination and ordering."""
        try:
            statement = select(self.model)

            if order_by:
                if order_by.startswith("-"):
                    field_name = order_by[1:]
                    if hasattr(self.model, field_name):
                        statement = statement.order_by(
                            getattr(self.model, field_name).desc()
                        )
                else:
                    if hasattr(self.model, order_by):
                        statement = statement.order_by(getattr(self.model, order_by))

            if skip:
                statement = statement.offset(skip)
            if limit:
                statement = statement.limit(limit)

            results = await self.session.execute(statement)
            return list(results.scalars().all())

        except SQLAlchemyError as e:
            logger.exception(f"Error listing {self.model.__name__}: {e}")
            return []

    async def create_async(self, **kwargs: Any) -> ModelType:
        """Create and persist a new entity."""
        try:
            entity = self.model(**kwargs)
            self.session.add(entity)
            await self.session.commit()
            await self.session.refresh(entity)
            logger.debug(f"Created {self.model.__name__} id={entity.id}")  # type: ignore
            return entity  # type: ignore

        except IntegrityError as e:
            await self.session.rollback()
            logger.exception(f"Integrity error creating {self.model.__name__}: {e}")
            raise
        except SQLAlchemyError as e:
            await self.session.rollback()
            logger.exception(f"Error creating {self.model.__name__}: {e}")
            raise

    async def update_async(self, id: str | int, **kwargs: Any) -> Optional[ModelType]:
        """Update entity fields by ID."""
        try:
            entity = await self.get_async(id)
            if not entity:
                return None

            for key, value in kwargs.items():
                if hasattr(entity, key):
                    setattr(entity, key, value)

            if hasattr(entity, "updated_at"):
                entity.updated_at = datetime.now(UTC)  # type: ignore

            self.session.add(entity)
            await self.session.commit()
            await self.session.refresh(entity)
            logger.debug(f"Updated {self.model.__name__} id={id}")
            return entity

        except SQLAlchemyError as e:
            await self.session.rollback()
            logger.exception(f"Error updating {self.model.__name__} id={id}: {e}")
            raise

    async def delete_async(self, id: str | int) -> bool:
        """Hard delete entity by ID. Returns True if deleted."""
        try:
            entity = await self.get_async(id)
            if not entity:
                return False

            await self.session.delete(entity)
            await self.session.commit()
            logger.debug(f"Deleted {self.model.__name__} id={id}")
            return True

        except SQLAlchemyError as e:
            await self.session.rollback()
            logger.exception(f"Error deleting {self.model.__name__} id={id}: {e}")
            raise

    async def count_async(self, filters: Optional[Dict[str, Any]] = None) -> int:
        """Count entities with optional field-value filters."""
        try:
            statement = select(func.count()).select_from(self.model)

            if filters:
                for field, value in filters.items():
                    if hasattr(self.model, field):
                        if value is None:
                            statement = statement.where(
                                getattr(self.model, field).is_(None)
                            )
                        else:
                            statement = statement.where(
                                getattr(self.model, field) == value
                            )

            result = await self.session.execute(statement)
            return result.scalar_one()

        except SQLAlchemyError as e:
            logger.exception(f"Error counting {self.model.__name__}: {e}")
            return 0

    async def exists_async(self, **kwargs: Any) -> bool:
        """Check if entity exists matching given field-value pairs."""
        try:
            statement = select(self.model)
            for field, value in kwargs.items():
                if hasattr(self.model, field):
                    statement = statement.where(getattr(self.model, field) == value)
            statement = statement.limit(1)
            result = await self.session.execute(statement)
            return result.scalar_one_or_none() is not None
        except SQLAlchemyError as e:
            logger.exception(f"Error checking existence for {self.model.__name__}: {e}")
            return False

    async def find_by_async(self, **kwargs: Any) -> Optional[ModelType]:
        """Find a single entity matching all given field-value pairs."""
        try:
            statement = select(self.model)
            for field, value in kwargs.items():
                if hasattr(self.model, field):
                    statement = statement.where(getattr(self.model, field) == value)
            result = await self.session.execute(statement)
            return result.scalar_one_or_none()
        except SQLAlchemyError as e:
            logger.exception(f"Error finding {self.model.__name__}: {e}")
            return None

    async def filter_by_async(
        self,
        skip: int = 0,
        limit: Optional[int] = None,
        order_by: Optional[str] = None,
        order_by_desc: bool = False,
        **kwargs: Any,
    ) -> List[ModelType]:
        """Filter entities by field values with optional pagination."""
        try:
            statement = select(self.model)
            for field, value in kwargs.items():
                if hasattr(self.model, field):
                    if isinstance(value, list):
                        statement = statement.where(
                            getattr(self.model, field).in_(value)
                        )
                    else:
                        statement = statement.where(getattr(self.model, field) == value)

            if order_by and hasattr(self.model, order_by):
                col = getattr(self.model, order_by)
                statement = statement.order_by(col.desc() if order_by_desc else col)

            if skip:
                statement = statement.offset(skip)
            if limit:
                statement = statement.limit(limit)

            results = await self.session.execute(statement)
            return list(results.scalars().all())

        except SQLAlchemyError as e:
            logger.exception(f"Error filtering {self.model.__name__}: {e}")
            return []

    async def bulk_create_async(self, entities: List[Dict[str, Any]]) -> List[ModelType]:
        """Create multiple entities in a single transaction."""
        created = []
        try:
            for entity_data in entities:
                entity = self.model(**entity_data)
                self.session.add(entity)
                created.append(entity)

            await self.session.commit()
            for entity in created:
                await self.session.refresh(entity)

            logger.info(f"Bulk created {len(created)} {self.model.__name__} entities")
            return created

        except SQLAlchemyError as e:
            await self.session.rollback()
            logger.exception(f"Error in bulk create for {self.model.__name__}: {e}")
            raise

    async def commit_async(self) -> None:
        """Commit current transaction."""
        try:
            await self.session.commit()
        except SQLAlchemyError as e:
            await self.session.rollback()
            logger.exception(f"Error committing transaction: {e}")
            raise

    async def rollback_async(self) -> None:
        """Rollback current transaction."""
        await self.session.rollback()
