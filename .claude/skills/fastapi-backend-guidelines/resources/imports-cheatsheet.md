# Common Imports Cheatsheet

Quick reference for frequently used imports in the FastAPI backend.

## FastAPI

```python
from fastapi import APIRouter, Depends, HTTPException, Query, Request
from fastapi.responses import StreamingResponse
```

## SQLModel & SQLAlchemy

```python
from sqlmodel import select, or_, and_, col
from sqlmodel.ext.asyncio.session import AsyncSession
from sqlalchemy import func, desc
from sqlalchemy.orm import selectinload
```

## Database Sessions

```python
from backend.db.orm import get_write_session_dependency, get_read_session_dependency
```

## Pydantic

```python
from pydantic import BaseModel, Field, field_validator, EmailStr
```

## Domain Imports (example)

```python
from backend.domain.user.model import User, UserProfile
from backend.domain.user.service import UserService
from backend.dtos.user import UserResponse, UserCreateRequest
from backend.error import NotFoundError, ForbiddenError, UnauthorizedError
```

## Common Standard Library

```python
from typing import List, Optional, Dict, Any
from ulid import ULID
```
