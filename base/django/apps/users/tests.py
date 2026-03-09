"""
Test suite for the users app.

Covers:
    - Registration (success, duplicate email, password mismatch)
    - Login (success, wrong password, inactive user)
    - Token refresh
    - Logout (blacklist refresh token)
    - Protected route access (me/)
    - User CRUD (retrieve, update, delete, admin-only list)

Run:
    python manage.py test apps.users
    # or with pytest-django:
    pytest apps/users/tests.py -v
"""
from django.test import TestCase
from django.urls import reverse
from rest_framework import status
from rest_framework.test import APIClient, APITestCase
from rest_framework_simplejwt.tokens import RefreshToken

from .models import User, UserRole


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def create_user(email: str = "user@example.com", password: str = "testpass123", **kwargs) -> User:
    return User.objects.create_user(email=email, password=password, name=kwargs.get("name", "Test User"), **kwargs)


def create_admin(email: str = "admin@example.com", password: str = "adminpass123") -> User:
    return User.objects.create_superuser(email=email, password=password, name="Admin User")


def get_tokens_for_user(user: User) -> dict:
    refresh = RefreshToken.for_user(user)
    return {"access": str(refresh.access_token), "refresh": str(refresh)}


# ---------------------------------------------------------------------------
# Registration Tests
# ---------------------------------------------------------------------------

class RegisterTests(APITestCase):
    url = "/api/v1/auth/register/"

    def test_register_success(self):
        payload = {
            "email": "new@example.com",
            "name": "New User",
            "password": "securepass123",
            "password_confirm": "securepass123",
        }
        response = self.client.post(self.url, payload, format="json")

        self.assertEqual(response.status_code, status.HTTP_201_CREATED)
        data = response.json()
        self.assertTrue(data["success"])
        self.assertIn("tokens", data["data"])
        self.assertIn("access", data["data"]["tokens"])
        self.assertIn("refresh", data["data"]["tokens"])
        self.assertEqual(data["data"]["user"]["email"], payload["email"])

    def test_register_duplicate_email(self):
        create_user(email="dup@example.com")
        payload = {
            "email": "dup@example.com",
            "name": "User",
            "password": "securepass123",
            "password_confirm": "securepass123",
        }
        response = self.client.post(self.url, payload, format="json")
        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)

    def test_register_password_mismatch(self):
        payload = {
            "email": "mismatch@example.com",
            "name": "User",
            "password": "securepass123",
            "password_confirm": "differentpass",
        }
        response = self.client.post(self.url, payload, format="json")
        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)

    def test_register_missing_fields(self):
        response = self.client.post(self.url, {"email": "x@example.com"}, format="json")
        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)


# ---------------------------------------------------------------------------
# Login Tests
# ---------------------------------------------------------------------------

class LoginTests(APITestCase):
    url = "/api/v1/auth/login/"

    def setUp(self):
        self.user = create_user(email="login@example.com", password="testpass123")

    def test_login_success(self):
        payload = {"email": "login@example.com", "password": "testpass123"}
        response = self.client.post(self.url, payload, format="json")

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        data = response.json()
        self.assertTrue(data["success"])
        self.assertIn("access", data["data"]["tokens"])
        self.assertIn("refresh", data["data"]["tokens"])

    def test_login_wrong_password(self):
        payload = {"email": "login@example.com", "password": "wrongpass"}
        response = self.client.post(self.url, payload, format="json")
        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)

    def test_login_nonexistent_user(self):
        payload = {"email": "nobody@example.com", "password": "testpass123"}
        response = self.client.post(self.url, payload, format="json")
        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)

    def test_login_inactive_user(self):
        self.user.is_active = False
        self.user.save()
        payload = {"email": "login@example.com", "password": "testpass123"}
        response = self.client.post(self.url, payload, format="json")
        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)


# ---------------------------------------------------------------------------
# Token Refresh Tests
# ---------------------------------------------------------------------------

class TokenRefreshTests(APITestCase):
    url = "/api/v1/auth/token/refresh/"

    def setUp(self):
        self.user = create_user()
        self.tokens = get_tokens_for_user(self.user)

    def test_refresh_success(self):
        payload = {"refresh": self.tokens["refresh"]}
        response = self.client.post(self.url, payload, format="json")
        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertIn("access", response.json())

    def test_refresh_invalid_token(self):
        payload = {"refresh": "invalid.token.here"}
        response = self.client.post(self.url, payload, format="json")
        self.assertEqual(response.status_code, status.HTTP_401_UNAUTHORIZED)


# ---------------------------------------------------------------------------
# Logout Tests
# ---------------------------------------------------------------------------

class LogoutTests(APITestCase):
    url = "/api/v1/auth/logout/"

    def setUp(self):
        self.user = create_user()
        self.tokens = get_tokens_for_user(self.user)
        self.client.credentials(HTTP_AUTHORIZATION=f"Bearer {self.tokens['access']}")

    def test_logout_success(self):
        payload = {"refresh": self.tokens["refresh"]}
        response = self.client.post(self.url, payload, format="json")
        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertTrue(response.json()["success"])

    def test_logout_without_refresh_token(self):
        response = self.client.post(self.url, {}, format="json")
        self.assertEqual(response.status_code, status.HTTP_400_BAD_REQUEST)

    def test_logout_unauthenticated(self):
        self.client.credentials()  # remove auth header
        response = self.client.post(self.url, {"refresh": self.tokens["refresh"]}, format="json")
        self.assertEqual(response.status_code, status.HTTP_401_UNAUTHORIZED)


# ---------------------------------------------------------------------------
# Me (Current User) Tests
# ---------------------------------------------------------------------------

class MeTests(APITestCase):
    url = "/api/v1/auth/me/"

    def setUp(self):
        self.user = create_user(email="me@example.com", name="Me User")
        self.tokens = get_tokens_for_user(self.user)

    def test_get_me_authenticated(self):
        self.client.credentials(HTTP_AUTHORIZATION=f"Bearer {self.tokens['access']}")
        response = self.client.get(self.url)

        self.assertEqual(response.status_code, status.HTTP_200_OK)
        data = response.json()
        self.assertTrue(data["success"])
        self.assertEqual(data["data"]["email"], self.user.email)

    def test_get_me_unauthenticated(self):
        response = self.client.get(self.url)
        self.assertEqual(response.status_code, status.HTTP_401_UNAUTHORIZED)


# ---------------------------------------------------------------------------
# User CRUD Tests
# ---------------------------------------------------------------------------

class UserCRUDTests(APITestCase):

    def setUp(self):
        self.admin = create_admin()
        self.user = create_user(email="crud@example.com", name="CRUD User")
        self.admin_tokens = get_tokens_for_user(self.admin)
        self.user_tokens = get_tokens_for_user(self.user)

    def _auth(self, tokens: dict) -> None:
        self.client.credentials(HTTP_AUTHORIZATION=f"Bearer {tokens['access']}")

    def test_list_users_as_admin(self):
        self._auth(self.admin_tokens)
        response = self.client.get("/api/v1/users/")
        self.assertEqual(response.status_code, status.HTTP_200_OK)

    def test_list_users_as_regular_user_is_forbidden(self):
        self._auth(self.user_tokens)
        response = self.client.get("/api/v1/users/")
        self.assertEqual(response.status_code, status.HTTP_403_FORBIDDEN)

    def test_retrieve_own_profile(self):
        self._auth(self.user_tokens)
        response = self.client.get(f"/api/v1/users/{self.user.pk}/")
        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.json()["data"]["email"], self.user.email)

    def test_retrieve_other_user_as_user_is_forbidden(self):
        other = create_user(email="other@example.com")
        self._auth(self.user_tokens)
        response = self.client.get(f"/api/v1/users/{other.pk}/")
        self.assertEqual(response.status_code, status.HTTP_403_FORBIDDEN)

    def test_update_own_name(self):
        self._auth(self.user_tokens)
        response = self.client.patch(
            f"/api/v1/users/{self.user.pk}/",
            {"name": "Updated Name"},
            format="json",
        )
        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.json()["data"]["name"], "Updated Name")

    def test_delete_user_as_admin(self):
        target = create_user(email="delete@example.com")
        self._auth(self.admin_tokens)
        response = self.client.delete(f"/api/v1/users/{target.pk}/")
        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertFalse(User.objects.filter(pk=target.pk).exists())

    def test_delete_user_as_regular_user_is_forbidden(self):
        other = create_user(email="nodelete@example.com")
        self._auth(self.user_tokens)
        response = self.client.delete(f"/api/v1/users/{other.pk}/")
        self.assertEqual(response.status_code, status.HTTP_403_FORBIDDEN)
