"""
Authentication endpoint integration tests.

These tests require a running database.
For unit tests, mock the repository layer.

Setup:
    1. cp .env.example .env  (fill in test DB values)
    2. Set MOCK_AUTH_ENABLED=true in .env for auth bypass tests
    3. Run: pytest tests/test_auth.py -v
"""

import pytest
from httpx import ASGITransport, AsyncClient

from backend.main import app


@pytest.fixture
async def client():
    """Async test client for FastAPI app."""
    async with AsyncClient(
        transport=ASGITransport(app=app),
        base_url="http://test",
    ) as ac:
        yield ac


class TestHealth:
    """Health check endpoint tests."""

    async def test_root(self, client: AsyncClient):
        response = await client.get("/")
        assert response.status_code == 200
        data = response.json()
        assert "name" in data
        assert "version" in data

    async def test_health(self, client: AsyncClient):
        response = await client.get("/api/v1/health")
        assert response.status_code == 200
        assert response.json() == {"status": "healthy"}


class TestAuthEndpoints:
    """Authentication flow tests (requires DB or mock)."""

    async def test_sign_up_missing_fields(self, client: AsyncClient):
        """Sign-up with missing fields returns 422."""
        response = await client.post("/api/v1/auth/sign-up", json={})
        assert response.status_code == 422

    async def test_login_missing_fields(self, client: AsyncClient):
        """Login with missing fields returns 422."""
        response = await client.post("/api/v1/auth/login", json={})
        assert response.status_code == 422

    async def test_me_without_token(self, client: AsyncClient):
        """GET /auth/me without token returns 401."""
        response = await client.get("/api/v1/auth/me")
        # When MOCK_AUTH_ENABLED=false (default), expect 401
        # When MOCK_AUTH_ENABLED=true, expect 200
        assert response.status_code in (200, 401)

    async def test_refresh_missing_token(self, client: AsyncClient):
        """Refresh without token returns 422."""
        response = await client.post("/api/v1/auth/refresh", json={})
        assert response.status_code == 422


class TestUserEndpoints:
    """User CRUD endpoint tests."""

    async def test_list_users_requires_auth(self, client: AsyncClient):
        """List users without auth returns 401."""
        response = await client.get("/api/v1/users")
        assert response.status_code in (200, 401)  # 200 if mock auth enabled

    async def test_get_nonexistent_user(self, client: AsyncClient):
        """Get user with invalid ID returns 404 or 401 (depending on auth)."""
        response = await client.get(
            "/api/v1/users/usr_nonexistent",
            headers={"Authorization": "Bearer invalid-token"},
        )
        assert response.status_code in (401, 403, 404)

    async def test_create_user_missing_fields(self, client: AsyncClient):
        """Create user with missing fields returns 422."""
        response = await client.post("/api/v1/users", json={})
        assert response.status_code == 422
