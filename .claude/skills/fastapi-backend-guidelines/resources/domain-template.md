# Quick Reference: New Domain Template

Complete template for creating a new domain feature with model, repository, service, DTOs, and router.

## Model

```python
# backend/domain/myfeature/model.py
from sqlmodel import SQLModel, Field, Column, DateTime, Text
from datetime import datetime, timezone
from typing import Optional
from ulid import ULID

def generate_myfeature_id() -> str:
    return f"mf_{ULID()}"

class MyFeature(SQLModel, table=True):
    __tablename__ = "my_feature"

    id: str = Field(
        default_factory=generate_myfeature_id,
        primary_key=True,
        max_length=30,
    )
    name: str = Field(sa_column=Column(Text, nullable=False))

    created_at: datetime = Field(
        sa_column=Column(DateTime(timezone=True), nullable=False),
        default_factory=lambda: datetime.now(tz=timezone.utc),
    )
    deleted_at: Optional[datetime] = Field(
        sa_column=Column(DateTime(timezone=True), nullable=True),
        default=None,
    )
```

## Repository

```python
# backend/domain/myfeature/repository.py
from backend.domain.shared.base_repository import BaseRepository
from sqlmodel import select
from sqlmodel.ext.asyncio.session import AsyncSession
from typing import Optional

class MyFeatureRepository(BaseRepository[MyFeature]):
    def __init__(self, session: AsyncSession):
        super().__init__(session, MyFeature)

    async def find_by_name(self, name: str) -> Optional[MyFeature]:
        stmt = select(MyFeature).where(
            MyFeature.name == name,
            MyFeature.deleted_at.is_(None),
        )
        result = await self.session.execute(stmt)
        return result.scalar_one_or_none()
```

## Service

```python
# backend/domain/myfeature/service.py
from sqlmodel.ext.asyncio.session import AsyncSession
from backend.error import NotFoundError

class MyFeatureService:
    def __init__(self, session: AsyncSession):
        self.session = session
        self._repository = MyFeatureRepository(session)

    async def get_feature(self, id: str) -> MyFeatureResponse:
        feature = await self._repository.get_by_id(id)
        if not feature:
            raise NotFoundError(f"Feature {id} not found")
        return MyFeatureResponse.model_validate(feature)
```

## DTOs

```python
# backend/dtos/myfeature.py
from pydantic import BaseModel, Field
from datetime import datetime

class MyFeatureResponse(BaseModel):
    id: str
    name: str
    created_at: datetime

class MyFeatureCreateRequest(BaseModel):
    name: str = Field(..., min_length=1, max_length=255)
```

## Router

```python
# backend/api/v1/routers/myfeature.py
from fastapi import APIRouter, Depends
from sqlmodel.ext.asyncio.session import AsyncSession
from backend.db.orm import get_read_session_dependency, get_write_session_dependency

router = APIRouter(prefix="/myfeature", tags=["myfeature"])

@router.get("/{id}")
async def get_feature(
    id: str,
    session: AsyncSession = Depends(get_read_session_dependency),
) -> MyFeatureResponse:
    service = MyFeatureService(session)
    return await service.get_feature(id)

@router.post("", status_code=201)
async def create_feature(
    request: MyFeatureCreateRequest,
    session: AsyncSession = Depends(get_write_session_dependency),
) -> MyFeatureResponse:
    service = MyFeatureService(session)
    return await service.create_feature(request)
```
