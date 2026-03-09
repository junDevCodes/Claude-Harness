"""
Views for authentication and user CRUD.

Auth endpoints:
    POST /api/v1/auth/register/       — Register + return tokens
    POST /api/v1/auth/login/          — Login + return tokens
    POST /api/v1/auth/logout/         — Blacklist refresh token
    POST /api/v1/auth/token/refresh/  — Refresh access token
    GET  /api/v1/auth/me/             — Current user info

User CRUD (admin or self):
    GET    /api/v1/users/             — List all users (admin only)
    GET    /api/v1/users/{id}/        — User detail
    PUT    /api/v1/users/{id}/        — Full update
    PATCH  /api/v1/users/{id}/        — Partial update
    DELETE /api/v1/users/{id}/        — Delete (admin only)
"""
from django.contrib.auth import get_user_model
from rest_framework import status
from rest_framework.permissions import AllowAny, IsAuthenticated
from rest_framework.request import Request
from rest_framework.response import Response
from rest_framework.views import APIView
from rest_framework.viewsets import ModelViewSet
from rest_framework_simplejwt.exceptions import TokenError
from rest_framework_simplejwt.tokens import RefreshToken
from rest_framework_simplejwt.views import TokenRefreshView

from apps.core.exceptions import NotFoundError
from apps.core.pagination import StandardResultsSetPagination

from .permissions import IsAdminUser, IsSelfOrAdmin
from .serializers import (
    LoginSerializer,
    RegisterSerializer,
    UserSerializer,
    UserUpdateSerializer,
)

User = get_user_model()


def _success(data: dict | list, status_code: int = status.HTTP_200_OK) -> Response:
    return Response({"success": True, "data": data}, status=status_code)


# ---------------------------------------------------------------------------
# Auth Views
# ---------------------------------------------------------------------------

class RegisterView(APIView):
    """
    POST /api/v1/auth/register/

    Body: { email, name, password, password_confirm }
    Returns: { user, tokens: { access, refresh } }
    """
    permission_classes = [AllowAny]

    def post(self, request: Request) -> Response:
        serializer = RegisterSerializer(data=request.data)
        serializer.is_valid(raise_exception=True)
        user = serializer.save()
        data = serializer.to_representation(user)
        return _success(data, status.HTTP_201_CREATED)


class LoginView(APIView):
    """
    POST /api/v1/auth/login/

    Body: { email, password }
    Returns: { user, tokens: { access, refresh } }
    """
    permission_classes = [AllowAny]

    def post(self, request: Request) -> Response:
        serializer = LoginSerializer(data=request.data, context={"request": request})
        serializer.is_valid(raise_exception=True)
        user = serializer.validated_data["user"]

        refresh = RefreshToken.for_user(user)
        return _success({
            "user": UserSerializer(user).data,
            "tokens": {
                "access": str(refresh.access_token),
                "refresh": str(refresh),
            },
        })


class LogoutView(APIView):
    """
    POST /api/v1/auth/logout/

    Body: { refresh }
    Blacklists the provided refresh token.
    """
    permission_classes = [IsAuthenticated]

    def post(self, request: Request) -> Response:
        refresh_token = request.data.get("refresh")
        if not refresh_token:
            return Response(
                {"success": False, "error": {"code": "bad_request", "message": "refresh token is required."}},
                status=status.HTTP_400_BAD_REQUEST,
            )

        try:
            token = RefreshToken(refresh_token)
            token.blacklist()
        except TokenError as e:
            return Response(
                {"success": False, "error": {"code": "bad_request", "message": str(e)}},
                status=status.HTTP_400_BAD_REQUEST,
            )

        return Response({"success": True, "message": "Logged out successfully."}, status=status.HTTP_200_OK)


class MeView(APIView):
    """
    GET /api/v1/auth/me/

    Returns current authenticated user's profile.
    """
    permission_classes = [IsAuthenticated]

    def get(self, request: Request) -> Response:
        return _success(UserSerializer(request.user).data)


# TokenRefreshView is already provided by SimpleJWT
# Mapped to POST /api/v1/auth/token/refresh/ in urls.py


# ---------------------------------------------------------------------------
# User CRUD ViewSet
# ---------------------------------------------------------------------------

class UserViewSet(ModelViewSet):
    """
    User CRUD.

    List/Create — Admin only
    Retrieve/Update/Delete — Owner or Admin

    Customization:
        - Override get_queryset() to add domain-specific filtering
        - Add extra actions with @action decorator
    """
    queryset = User.objects.all().order_by("-created_at")
    pagination_class = StandardResultsSetPagination
    http_method_names = ["get", "put", "patch", "delete", "head", "options"]

    def get_serializer_class(self):
        if self.action in ("update", "partial_update"):
            return UserUpdateSerializer
        return UserSerializer

    def get_permissions(self):
        if self.action == "list":
            return [IsAdminUser()]
        if self.action == "destroy":
            return [IsAdminUser()]
        return [IsSelfOrAdmin()]

    def get_object(self):
        obj = super().get_object()
        self.check_object_permissions(self.request, obj)
        return obj

    def list(self, request: Request, *args, **kwargs) -> Response:
        queryset = self.filter_queryset(self.get_queryset())
        page = self.paginate_queryset(queryset)
        if page is not None:
            serializer = self.get_serializer(page, many=True)
            return self.get_paginated_response(serializer.data)
        serializer = self.get_serializer(queryset, many=True)
        return _success(serializer.data)

    def retrieve(self, request: Request, *args, **kwargs) -> Response:
        instance = self.get_object()
        return _success(self.get_serializer(instance).data)

    def update(self, request: Request, *args, **kwargs) -> Response:
        partial = kwargs.pop("partial", False)
        instance = self.get_object()
        serializer = self.get_serializer(instance, data=request.data, partial=partial)
        serializer.is_valid(raise_exception=True)
        serializer.save()
        return _success(serializer.to_representation(instance))

    def destroy(self, request: Request, *args, **kwargs) -> Response:
        instance = self.get_object()
        instance.delete()
        return Response({"success": True, "message": "User deleted."}, status=status.HTTP_200_OK)
