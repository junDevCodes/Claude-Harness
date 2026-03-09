---
name: fastapi-backend-guidelines
description: FastAPI backend development guidelines for Python async applications. Domain-Driven Design with FastAPI routers, SQLModel/SQLAlchemy ORM, repository pattern, service layer, async/await patterns, Pydantic validation, and error handling. Use when creating APIs, routes, services, repositories, or working with backend code.
triggers:
  - fastapi
  - FastAPI
  - SQLModel
  - SQLAlchemy
  - pydantic
  - uvicorn
  - alembic
  - async python backend
  - fastapi router
  - fastapi service
---

# FastAPI Backend Development Guidelines

## Purpose

Comprehensive guide for modern FastAPI development with async Python, emphasizing Domain-Driven Design, layered architecture (Router -> Service -> Repository), SQLModel ORM, and async best practices.

## When to Use This Skill

- Creating new API routes or endpoints
- Building domain services and business logic
- Implementing repositories for data access
- Setting up database models with SQLModel
- Async/await patterns and error handling
- Organizing backend code with DDD
- Pydantic validation and DTOs

---

## Quick Start

### New API Route Checklist

- [ ] Define route in `backend/api/v1/routers/{domain}.py`
- [ ] Use FastAPI dependency injection for session
- [ ] Use `get_read_session_dependency` for reads, `get_write_session_dependency` for writes
- [ ] Call service layer (don't access repository directly)
- [ ] Use Pydantic DTOs for request/response
- [ ] Handle errors with custom exceptions
- [ ] Add proper HTTP status codes
- [ ] Use async/await throughout
- [ ] Use type hints on all parameters

### New Domain Feature Checklist

- [ ] Create `backend/domain/{domain}/` directory
- [ ] Create `model.py` - SQLModel database models with ULID ID generation
- [ ] Create `repository.py` - Data access layer extending BaseRepository
- [ ] Create `service.py` - Business logic layer
- [ ] Create DTOs in `backend/dtos/{domain}.py`
- [ ] Create router in `backend/api/v1/routers/{domain}.py`
- [ ] Register router in `main.py`

---

## Project Structure

```
backend/
  backend/
    main.py                  # FastAPI app creation with lifespan

    api/
      v1/
        routers/             # API route handlers
          auth.py            # Login, signup, OAuth
          user.py            # User management
          upload.py          # S3 presigned URLs

    domain/                  # Domain-Driven Design
      {domain}/
        model.py             # SQLModel models
        repository.py        # Data access layer
        service.py           # Business logic
      shared/
        base_repository.py   # Generic BaseRepository

    dtos/                    # Pydantic DTOs
      auth.py                # Login, signup DTOs
      user.py                # User, profile DTOs

    db/
      orm.py                 # Read/Write session management

    core/
      config.py              # Pydantic Settings configuration

    middleware/
      error_handler.py       # ErrorHandlerMiddleware

    utils/                   # S3, password hashing, etc.
    error/                   # Custom exceptions
      __init__.py            # AppException, NotFoundError, etc.
```

> **[Common imports cheatsheet: resources/imports-cheatsheet.md](resources/imports-cheatsheet.md)**

---

## Topic Guides

### Layered Architecture

**Three-Layer Pattern:**
1. **Router Layer**: API endpoints, request validation, response formatting
2. **Service Layer**: Business logic, orchestration, domain rules
3. **Repository Layer**: Data access, queries, database operations

**Key Concepts:**
- Routers call Services (never Repositories directly)
- Services orchestrate business logic
- Repositories handle all database operations
- Each layer has clear responsibilities
- Async/await throughout the stack
- Read/Write session separation

**[Complete Guide: resources/layered-architecture.md](resources/layered-architecture.md)**

---

### API Routes & Routers

**PRIMARY PATTERN: FastAPI Routers**
- Create routers in `backend/api/v1/routers/`
- Use dependency injection for sessions
- Use `get_read_session_dependency` for GET requests
- Use `get_write_session_dependency` for POST/PATCH/DELETE
- Async route handlers with REST conventions

```python
from fastapi import APIRouter, Depends
from sqlmodel.ext.asyncio.session import AsyncSession
from backend.db.orm import get_read_session_dependency, get_write_session_dependency

router = APIRouter(prefix="/users", tags=["users"])

@router.get("/{user_id}")
async def get_user(
    user_id: str,
    session: AsyncSession = Depends(get_read_session_dependency),
) -> UserResponse:
    service = UserService(session)
    return await service.get_user(user_id)

@router.post("", status_code=201)
async def create_user(
    request: UserCreateRequest,
    session: AsyncSession = Depends(get_write_session_dependency),
) -> UserResponse:
    service = UserService(session)
    return await service.create_user(request)
```

**[Complete Guide: resources/api-routes.md](resources/api-routes.md)**

---

### Database & ORM

**SQLModel + SQLAlchemy:**
- SQLModel for models (combines SQLAlchemy + Pydantic)
- Async sessions with asyncpg driver
- Read/Write session separation with caching
- Repository pattern for all queries
- ULID-based ID generation with prefixes

```python
from sqlmodel import SQLModel, Field, Column, DateTime, Text
from datetime import datetime, timezone
from ulid import ULID

def generate_user_id() -> str:
    return f"usr_{ULID()}"

class User(SQLModel, table=True):
    __tablename__ = "user"

    id: str = Field(
        default_factory=generate_user_id,
        primary_key=True,
        max_length=30,
    )
    phone: str = Field(sa_column=Column(Text, nullable=False, unique=True))
    name: str = Field(sa_column=Column(Text, nullable=False))

    # Soft delete + timestamps
    deleted_at: Optional[datetime] = Field(
        sa_column=Column(DateTime(timezone=True), nullable=True),
        default=None,
    )
    created_at: datetime = Field(
        sa_column=Column(DateTime(timezone=True), nullable=False),
        default_factory=lambda: datetime.now(tz=timezone.utc),
    )
```

**[Complete Guide: resources/database-orm.md](resources/database-orm.md)**

---

### Domain-Driven Design

**Domain Organization:**
- Each domain in `backend/domain/{name}/`
- Contains: `model.py`, `repository.py`, `service.py`
- Clear separation of concerns
- Business logic in services
- Data access in repositories

**[Complete Guide: resources/domain-driven-design.md](resources/domain-driven-design.md)**

---

### Service Layer

**Service Pattern:**
- Business logic orchestration
- Domain rule enforcement
- Calls repositories for data
- Returns DTOs, not models directly
- Transaction management
- Uses asyncio.gather for parallel queries

```python
class UserService:
    def __init__(self, session: AsyncSession):
        self.session = session
        self._user_repo = UserRepository(session)
        self._profile_repo = UserProfileRepository(session)

    async def get_user_detail(self, user_id: str) -> UserDetailResponse:
        user = await self._user_repo.get_by_id(user_id)
        if not user:
            raise NotFoundError(f"User {user_id} not found")
        return self._to_detail_response(user)

    async def get_dashboard_data(self) -> dict:
        # Parallel queries with asyncio.gather
        total, monthly = await asyncio.gather(
            self._get_total_count(),
            self._get_monthly_count(),
        )
        return {"total": total, "monthly": monthly}
```

**[Complete Guide: resources/service-layer.md](resources/service-layer.md)**

---

### Repository Pattern

**Repository Pattern:**
- Encapsulates data access
- Extends BaseRepository for CRUD
- Domain-specific queries
- Returns domain models
- All queries are async
- Soft delete support

```python
from backend.domain.shared.base_repository import BaseRepository

class UserRepository(BaseRepository[User]):
    def __init__(self, session: AsyncSession):
        super().__init__(session, User)

    async def find_by_phone(self, phone: str) -> Optional[User]:
        stmt = select(User).where(
            User.phone == phone,
            User.deleted_at.is_(None),
        )
        result = await self.session.execute(stmt)
        return result.scalar_one_or_none()
```

> N+1 방지: `asyncio.gather` + DataLoader 패턴 활용 -> **[resources/repository-pattern.md](resources/repository-pattern.md)**

**[Complete Guide: resources/repository-pattern.md](resources/repository-pattern.md)**

---

### DTOs & Validation

**Pydantic DTOs:**
- Request/Response data transfer objects
- Validation with Pydantic
- Separate from domain models
- Located in `backend/dtos/`
- Use field_validator for enum validation

```python
from pydantic import BaseModel, Field, field_validator

class AdminBasicInfoUpdateRequest(BaseModel):
    name: Optional[str] = Field(None, description="User name")
    status: Optional[str] = Field(None, description="User status")
    is_admin: Optional[bool] = Field(None, description="Admin flag")
    birth_year: Optional[int] = Field(None, ge=1940, le=2010)

    model_config = {"extra": "forbid"}  # Reject unknown fields

    @field_validator("status")
    @classmethod
    def validate_status(cls, v: Optional[str]) -> Optional[str]:
        if v is not None:
            valid_values = [e.value for e in UserStatusEnum]
            if v not in valid_values:
                raise ValueError(f"Invalid status: {v}. Valid: {valid_values}")
        return v
```

**[Complete Guide: resources/dtos-validation.md](resources/dtos-validation.md)**

---

### Async/Await Patterns

**Async Best Practices:**
- Use async/await throughout
- Async database sessions
- Proper session cleanup
- Avoid blocking operations in async context
- Use asyncio.gather for parallel queries

```python
# Parallel queries with asyncio.gather
async def get_dashboard_data(self) -> dict:
    total, monthly, weekly, today = await asyncio.gather(
        self._get_total_count(),
        self._get_monthly_count(),
        self._get_weekly_count(),
        self._get_today_count(),
    )
    return {
        "total_members": total,
        "monthly_members": monthly,
        "weekly_members": weekly,
        "today_members": today,
    }
```

**[Complete Guide: resources/async-patterns.md](resources/async-patterns.md)**

---

### Error Handling

**Error Handling Strategy:**
- Custom exception classes in `backend/error/`
- HTTP exception mapping via ErrorHandlerMiddleware
- Consistent error responses

```python
# backend/error/__init__.py
class AppException(Exception):
    def __init__(self, message: str):
        self.message = message
        super().__init__(self.message)

class NotFoundError(AppException): pass
class ForbiddenError(AppException): pass
class UnauthorizedError(AppException): pass

# In service
if not user:
    raise NotFoundError(f"User {user_id} not found")

# ErrorHandlerMiddleware: NotFoundError -> 404, ForbiddenError -> 403
```

**[Complete Guide: resources/error-handling.md](resources/error-handling.md)**

---

### Complete Examples

Full working examples: Complete domain (model + repository + service + router), CRUD operations with async, complex queries with SQLModel, authentication patterns, S3 presigned URL generation, pagination, N+1 prevention with DataLoader.

**[Complete Guide: resources/complete-examples.md](resources/complete-examples.md)**

---

## Core Principles

1. **Layered Architecture**: Router -> Service -> Repository (never skip layers)
2. **Async Everything**: Use async/await with read/write session separation
3. **Repository Pattern**: All data access through repositories, services for business logic
4. **DTOs & Validation**: Pydantic DTOs for API, type hints everywhere
5. **ULID + Soft Delete**: ULID IDs with entity prefixes, deleted_at instead of hard delete

---

## Anti-Patterns

### 1. async 라우트에서 sync 블로킹 호출
- BAD: `time.sleep()`, `requests.get()` in async def
- GOOD: `await asyncio.sleep()`, `httpx.AsyncClient()`

### 2. Router에서 Repository 직접 호출
- BAD: router -> repository (Service 계층 우회)
- GOOD: router -> service -> repository

### 3. Session 관리 미흡
- BAD: `session = get_session()` 후 close 안 함
- GOOD: `async with get_write_session() as session:`

### 4. raw SQL 사용
- BAD: `session.exec(text("SELECT * FROM users"))`
- GOOD: `session.exec(select(User).where(User.id == id))`

### 5. 하드코딩 ID
- BAD: `id: int = Field(primary_key=True)`
- GOOD: `id: str = Field(default_factory=lambda: str(ULID()), primary_key=True)`

---

## Quick Reference: New Domain Template

> **[Complete template: resources/domain-template.md](resources/domain-template.md)**

---

## Related Skills

- **nextjs-frontend-guidelines**: Frontend patterns that consume this API
- **error-tracking**: Error tracking with Sentry (backend integration)
- **pytest-backend-testing**: Testing patterns for FastAPI backends

---

**Skill Status**: Modular structure with progressive loading for optimal context management
