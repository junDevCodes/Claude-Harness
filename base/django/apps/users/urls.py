"""URL routing for the users app."""
from django.urls import include, path
from rest_framework.routers import DefaultRouter
from rest_framework_simplejwt.views import TokenRefreshView

from .views import LoginView, LogoutView, MeView, RegisterView, UserViewSet

# ---------------------------------------------------------------------------
# Auth endpoints
# ---------------------------------------------------------------------------
auth_patterns = [
    path("register/", RegisterView.as_view(), name="auth-register"),
    path("login/", LoginView.as_view(), name="auth-login"),
    path("logout/", LogoutView.as_view(), name="auth-logout"),
    path("token/refresh/", TokenRefreshView.as_view(), name="auth-token-refresh"),
    path("me/", MeView.as_view(), name="auth-me"),
]

# ---------------------------------------------------------------------------
# User CRUD endpoints
# ---------------------------------------------------------------------------
router = DefaultRouter()
router.register(r"users", UserViewSet, basename="users")

urlpatterns = [
    path("auth/", include(auth_patterns)),
    path("", include(router.urls)),
]
