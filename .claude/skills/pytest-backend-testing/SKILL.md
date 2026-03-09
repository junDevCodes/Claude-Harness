---
name: pytest-backend-testing
description: Comprehensive pytest testing guide for FastAPI backends. Covers unit testing, integration testing, async patterns, mocking, fixtures, coverage, and FastAPI-specific testing with TestClient. Use when writing or updating test code for backend services, repositories, or API routes.
triggers:
  - pytest
  - unit test
  - integration test
  - test coverage
  - mock
  - fixture
  - TestClient
  - async test
  - conftest
  - pytest-asyncio
---

# Pytest Backend Testing Guidelines

## Purpose

Complete guide for writing comprehensive tests for FastAPI backend applications using pytest, pytest-asyncio, and FastAPI TestClient. Emphasizes async testing, proper mocking, layered testing (repository -> service -> router), and achieving high test coverage.

## When to Use This Skill

- Writing new test files for backend code
- Testing repositories, services, or API routes
- Setting up test fixtures and mocks
- Debugging failing tests
- Improving test coverage
- Writing async tests with pytest-asyncio
- Testing database operations
- Using FastAPI TestClient for route testing

---

## Quick Start

### New Test File Checklist

Creating tests for new code? Follow this checklist:

- [ ] Create test file: `tests/unit/{domain}/test_{module}.py`
- [ ] Import pytest and pytest-asyncio
- [ ] Set up necessary fixtures (session, client, etc.)
- [ ] Use `@pytest.mark.asyncio` for async tests
- [ ] Follow AAA pattern: Arrange, Act, Assert
- [ ] Mock external dependencies
- [ ] Test both success and error cases
- [ ] Verify coverage meets 80% threshold
- [ ] Use descriptive test names: `test_<what>_<when>_<expected>`

### Test Coverage Checklist

Ensuring good coverage? Check these:

- [ ] Test all public methods/functions
- [ ] Test error handling and exceptions
- [ ] Test edge cases and boundary conditions
- [ ] Test validation logic
- [ ] Mock external dependencies (database, APIs)
- [ ] Verify async/await behavior
- [ ] Run `pytest --cov=backend --cov-report=term-missing`
- [ ] Check coverage report for gaps
- [ ] Aim for 80%+ coverage

---

## Project Testing Structure

```
backend/
  tests/
    conftest.py              # Global fixtures
    unit/
      domain/
        {entity}/
          test_{entity}_repository.py
          test_{entity}_service.py
      middleware/
        test_error_handler.py
      utils/
        test_utils.py
    integration/             # End-to-end tests
      test_{entity}_api.py
      test_auth_flow.py
```

---

## Common Test Patterns Quick Reference

### Basic Async Test

```python
import pytest
from sqlmodel.ext.asyncio.session import AsyncSession

@pytest.mark.asyncio
async def test_get_entity_by_id(db_session: AsyncSession):
    # Arrange
    entity_id = "test-entity-id"

    # Act
    result = await repository.get_by_id(entity_id)

    # Assert
    assert result is not None
    assert result.id == entity_id
```

### Mocking Database Session

```python
from unittest.mock import AsyncMock, MagicMock

@pytest.mark.asyncio
async def test_create_entity_success():
    # Arrange
    mock_session = AsyncMock(spec=AsyncSession)
    mock_session.execute = AsyncMock()
    mock_session.commit = AsyncMock()

    # Act
    service = EntityService(mock_session)
    result = await service.create_entity(data)

    # Assert
    assert mock_session.commit.called
```

### Testing FastAPI Routes

```python
from fastapi.testclient import TestClient
from backend.main import create_application

@pytest.fixture
def client():
    app = create_application()
    return TestClient(app)

def test_get_entity_endpoint(client):
    # Act
    response = client.get("/api/v1/entities/test-id")

    # Assert
    assert response.status_code == 200
    assert response.json()["id"] == "test-id"
```

---

## Test Organization Principles

### Test Structure (AAA Pattern)

1. **Arrange**: Set up test data, mocks, fixtures
2. **Act**: Execute the code under test
3. **Assert**: Verify the expected outcome

### Test Naming Convention

```python
# Pattern: test_<what>_<when>_<expected>
def test_create_entity_with_valid_data_returns_entity()
def test_get_entity_when_not_found_raises_not_found_error()
def test_update_entity_with_duplicate_name_raises_conflict_error()
```

### Test Organization

- **Unit tests**: Test individual functions/methods in isolation
- **Integration tests**: Test multiple components working together
- **Group related tests**: Use test classes for related functionality

---

## Topic Guides

### Testing Architecture

**Three-Layer Testing Strategy:**
1. **Repository Layer**: Test database queries, CRUD operations
2. **Service Layer**: Test business logic, orchestration
3. **Router Layer**: Test API endpoints, request/response handling

**Key Concepts:**
- Mock dependencies at layer boundaries
- Test each layer independently
- Use integration tests for end-to-end flows
- Maintain test isolation

**[Complete Guide: resources/testing-architecture.md](resources/testing-architecture.md)**

---

### Unit Testing

**Unit Test Best Practices:**
- Test single responsibility
- Mock external dependencies
- Fast execution (no database, no network)
- Independent and isolated
- Test both success and failure paths

```python
@pytest.mark.asyncio
async def test_entity_service_create():
    # Mock repository
    mock_repo = AsyncMock()
    mock_repo.create = AsyncMock(return_value=entity_model)

    # Test service logic
    service = EntityService(mock_repo)
    result = await service.create_entity(data)

    assert result.name == data.name
```

**[Complete Guide: resources/unit-testing.md](resources/unit-testing.md)**

---

### Integration Testing

**Integration Test Focus:**
- Test multiple components together
- Use real database (test database)
- Verify end-to-end workflows
- Test API contracts

```python
@pytest.mark.asyncio
async def test_create_entity_flow(db_session, client):
    # Full flow: API -> Service -> Repository -> DB
    response = client.post("/api/v1/entities", json=entity_data)
    assert response.status_code == 201

    # Verify in database
    entity = await db_session.get(Entity, response.json()["id"])
    assert entity is not None
```

**[Complete Guide: resources/integration-testing.md](resources/integration-testing.md)**

---

### Async Testing

**Async Test Patterns:**
- Use `@pytest.mark.asyncio` decorator
- Configure pytest-asyncio in conftest.py
- Mock async functions with AsyncMock
- Test async context managers
- Handle async exceptions

```python
from unittest.mock import AsyncMock

@pytest.mark.asyncio
async def test_async_function():
    mock_func = AsyncMock(return_value="result")
    result = await mock_func()
    assert result == "result"
    mock_func.assert_awaited_once()
```

**[Complete Guide: resources/async-testing.md](resources/async-testing.md)**

---

### Mocking & Fixtures

**Mocking Strategy:**
- Mock external dependencies (database, APIs, S3)
- Use pytest fixtures for reusable test data
- Mock at layer boundaries
- Use MagicMock for sync, AsyncMock for async

```python
import pytest

@pytest.fixture
def sample_entity():
    return Entity(
        id="test-id",
        name="Test Entity",
        bio="Test bio"
    )

@pytest.fixture
async def db_session():
    async with get_test_session() as session:
        yield session
        await session.rollback()
```

**[Complete Guide: resources/mocking-fixtures.md](resources/mocking-fixtures.md)**

---

### Coverage Best Practices

**Coverage Strategy:**
- Aim for 80%+ coverage (project requirement)
- Focus on critical business logic
- Test error paths and edge cases
- Use coverage reports to find gaps

```bash
# Run tests with coverage
pytest --cov=backend --cov-report=term-missing

# Generate HTML report
pytest --cov=backend --cov-report=html

# Check coverage threshold
pytest --cov=backend --cov-fail-under=80
```

**[Complete Guide: resources/coverage-best-practices.md](resources/coverage-best-practices.md)**

---

### FastAPI Testing

**FastAPI Test Patterns:**
- Use TestClient for route testing
- Test request validation
- Test response serialization
- Test authentication/authorization
- Test error handling middleware

```python
from fastapi.testclient import TestClient

def test_create_entity_endpoint(client: TestClient):
    response = client.post(
        "/api/v1/entities",
        json={"name": "Entity", "bio": "Bio"}
    )
    assert response.status_code == 201
    data = response.json()
    assert data["name"] == "Entity"
```

**[Complete Guide: resources/fastapi-testing.md](resources/fastapi-testing.md)**

---

## Core Principles

1. **Test Isolation**: Each test runs independently, no shared state
2. **AAA Pattern**: Arrange, Act, Assert for clear test structure
3. **Async Testing**: Use pytest-asyncio for async code
4. **Mock Dependencies**: Mock external systems (database, APIs)
5. **Layered Testing**: Test each layer (repository, service, router) separately
6. **Coverage Goals**: Aim for 80%+ coverage, focus on business logic
7. **Descriptive Names**: Clear test names explain what, when, expected
8. **Error Testing**: Test both success and failure paths
9. **Fast Tests**: Unit tests should be fast (no real database)
10. **Fixtures**: Use fixtures for reusable test data and setup

---

## Anti-Patterns

### 1. 테스트 간 공유 가변 상태
- BAD: 모듈 레벨 리스트에 테스트 데이터 누적
- GOOD: 각 테스트에서 fixture로 독립 데이터 생성

### 2. 구현 세부사항 테스트
- BAD: private 메서드 호출 횟수 assert
- GOOD: public API의 입출력 결과만 assert

### 3. 단위 테스트에서 실제 DB 사용
- BAD: 실제 PostgreSQL 세션으로 unit test
- GOOD: AsyncMock 세션 + in-memory fixture

### 4. 에러 경로 미테스트
- BAD: happy path만 테스트
- GOOD: 404, 401, 409, 500 등 에러 시나리오 별도 테스트

### 5. 과도한 모킹 (over-mocking)
- BAD: 모든 의존성을 Mock -- 실제 동작 미검증
- GOOD: Repository만 Mock, Service 로직은 실제 실행

---

## Quick Reference: Test Template

> **[Complete template: resources/test-template.md](resources/test-template.md)**

---

## Related Skills

- **fastapi-backend-guidelines**: Backend development patterns (what you're testing)
- **error-tracking**: Error handling patterns to test

---

**Skill Status**: Modular structure with progressive loading for optimal context management
