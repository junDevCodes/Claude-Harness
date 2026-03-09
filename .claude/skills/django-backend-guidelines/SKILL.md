---
name: django-backend-guidelines
description: Django 5 + DRF + SimpleJWT 백엔드 개발 가이드라인. AbstractBaseUser 커스텀 유저 모델, DRF ModelViewSet/APIView, SimpleJWT 인증(access+refresh+blacklist), settings/ 환경 분리, 커스텀 예외 핸들러 패턴.
triggers:
  - django
  - drf
  - django rest framework
  - simplejwt
  - ModelViewSet
  - APIView
  - serializer
  - AbstractBaseUser
  - custom user model
  - django 백엔드
  - makemigrations
  - pytest-django
  - gunicorn
  - UserManager
  - token blacklist
  - psycopg
---

# Django 5 + DRF + SimpleJWT 백엔드 가이드라인

## 언제 이 스킬을 쓰는가

Django View/ViewSet/Serializer 작성, SimpleJWT 인증, 커스텀 User 모델 설계,
settings/ 환경 분리, 커스텀 예외 핸들러/권한 클래스 작성 시.

---

## 프로젝트 구조

```
project/
├── config/
│   ├── settings/
│   │   ├── base.py          ← 공통 설정
│   │   ├── development.py   ← DEBUG=True, 로컬 DB
│   │   └── production.py    ← DEBUG=False, RDS, HTTPS
│   └── urls.py
├── apps/
│   ├── core/
│   │   ├── exceptions.py    ← 커스텀 예외 + DRF 핸들러
│   │   ├── pagination.py
│   │   └── utils/s3.py
│   └── users/
│       ├── models.py        ← AbstractBaseUser 기반 User
│       ├── managers.py
│       ├── serializers.py
│       ├── views.py         ← APIView + ModelViewSet
│       ├── permissions.py
│       └── urls.py
└── manage.py
```

---

## 패턴 1 — 커스텀 User 모델 (AbstractBaseUser)

> **핵심:** `AUTH_USER_MODEL` 설정은 **첫 마이그레이션 전에** 반드시 설정.
> 이후 변경은 마이그레이션 충돌을 일으키므로 프로젝트 시작 시 결정.

```python
# apps/users/models.py
from django.contrib.auth.models import AbstractBaseUser, PermissionsMixin
from django.db import models
from django.utils import timezone
from .managers import UserManager


class UserRole(models.TextChoices):
    USER = "USER", "User"
    ADMIN = "ADMIN", "Admin"


class User(AbstractBaseUser, PermissionsMixin):
    email = models.EmailField(unique=True, db_index=True)
    name = models.CharField(max_length=150, blank=True)
    role = models.CharField(max_length=10, choices=UserRole.choices, default=UserRole.USER)
    is_active = models.BooleanField(default=True)
    is_staff = models.BooleanField(default=False)
    created_at = models.DateTimeField(default=timezone.now)
    updated_at = models.DateTimeField(auto_now=True)

    objects = UserManager()

    USERNAME_FIELD = "email"       # 로그인 식별자
    REQUIRED_FIELDS = ["name"]     # createsuperuser 필수 입력

    class Meta:
        db_table = "users"
        ordering = ["-created_at"]

    @property
    def is_admin(self) -> bool:
        return self.role == UserRole.ADMIN
```

```python
# apps/users/managers.py
from django.contrib.auth.models import BaseUserManager


class UserManager(BaseUserManager):
    def create_user(self, email, password=None, **extra_fields):
        if not email:
            raise ValueError("Email is required.")
        user = self.model(email=self.normalize_email(email), **extra_fields)
        user.set_password(password)  # PBKDF2 해시
        user.save(using=self._db)
        return user

    def create_superuser(self, email, password, **extra_fields):
        extra_fields.setdefault("is_staff", True)
        extra_fields.setdefault("is_superuser", True)  # PermissionsMixin 필수
        extra_fields.setdefault("is_active", True)
        extra_fields.setdefault("role", "ADMIN")
        return self.create_user(email, password, **extra_fields)
```

```python
# config/settings/base.py
AUTH_USER_MODEL = "users.User"  # 첫 마이그레이션 전 필수
```

---

## 패턴 2 — DRF ViewSet + Serializer

> **핵심:** `get_permissions()` + `get_serializer_class()`로 액션별 분기.
> 응답 형식은 `{"success": True, "data": ...}` 통일.

```python
# apps/users/serializers.py
from django.contrib.auth import authenticate, get_user_model
from rest_framework import serializers
from rest_framework_simplejwt.tokens import RefreshToken

User = get_user_model()


class RegisterSerializer(serializers.ModelSerializer):
    password = serializers.CharField(write_only=True, min_length=8)
    password_confirm = serializers.CharField(write_only=True)

    class Meta:
        model = User
        fields = ("email", "name", "password", "password_confirm")

    def validate(self, attrs):
        if attrs["password"] != attrs.pop("password_confirm"):
            raise serializers.ValidationError({"password_confirm": "Passwords do not match."})
        return attrs

    def create(self, validated_data):
        return User.objects.create_user(**validated_data)

    def to_representation(self, instance):
        refresh = RefreshToken.for_user(instance)
        return {
            "user": UserSerializer(instance).data,
            "tokens": {"access": str(refresh.access_token), "refresh": str(refresh)},
        }


class UserSerializer(serializers.ModelSerializer):
    class Meta:
        model = User
        fields = ("id", "email", "name", "role", "is_active", "created_at", "updated_at")
        read_only_fields = fields


class UserUpdateSerializer(serializers.ModelSerializer):
    class Meta:
        model = User
        fields = ("name",)

    def to_representation(self, instance):
        return UserSerializer(instance).data


class LoginSerializer(serializers.Serializer):
    email = serializers.EmailField()
    password = serializers.CharField(write_only=True)

    def validate(self, attrs):
        user = authenticate(
            request=self.context.get("request"),
            username=attrs["email"],
            password=attrs["password"],
        )
        if not user:
            raise serializers.ValidationError("Invalid email or password.")
        if not user.is_active:
            raise serializers.ValidationError("Account is disabled.")
        attrs["user"] = user
        return attrs
```

```python
# apps/users/views.py (ViewSet 부분)
from django.contrib.auth import get_user_model
from rest_framework.viewsets import ModelViewSet
from rest_framework.response import Response
from rest_framework import status
from apps.core.pagination import StandardResultsSetPagination
from .permissions import IsAdminUser, IsSelfOrAdmin
from .serializers import UserSerializer, UserUpdateSerializer

User = get_user_model()


def _success(data, status_code=status.HTTP_200_OK):
    return Response({"success": True, "data": data}, status=status_code)


class UserViewSet(ModelViewSet):
    queryset = User.objects.all().order_by("-created_at")
    pagination_class = StandardResultsSetPagination  # apps/core/pagination.py
    http_method_names = ["get", "put", "patch", "delete", "head", "options"]

    def get_serializer_class(self):
        if self.action in ("update", "partial_update"):
            return UserUpdateSerializer
        return UserSerializer

    def get_permissions(self):
        if self.action in ("list", "destroy"):
            return [IsAdminUser()]
        return [IsSelfOrAdmin()]

    def retrieve(self, request, *args, **kwargs):
        return _success(self.get_serializer(self.get_object()).data)

    def destroy(self, request, *args, **kwargs):
        self.get_object().delete()
        return Response({"success": True, "message": "Deleted."})
```

```python
# apps/users/permissions.py
from rest_framework.permissions import BasePermission


class IsAdminUser(BasePermission):
    def has_permission(self, request, view):
        return bool(request.user and request.user.is_authenticated and request.user.is_admin)


class IsSelfOrAdmin(BasePermission):
    def has_permission(self, request, view):
        return bool(request.user and request.user.is_authenticated)  # 인증 필수

    def has_object_permission(self, request, view, obj):
        return request.user.is_admin or obj == request.user
```

---

## 패턴 3 — SimpleJWT 인증 (settings + View + Blacklist)

> **핵심:** access(30분) + refresh(7일) + Token Blacklist(로그아웃 무효화).

```python
# config/settings/base.py
from datetime import timedelta

INSTALLED_APPS = [
    ...
    "rest_framework_simplejwt",
    "rest_framework_simplejwt.token_blacklist",  # 로그아웃 blacklist 필수
]

REST_FRAMEWORK = {
    "DEFAULT_AUTHENTICATION_CLASSES": ["rest_framework_simplejwt.authentication.JWTAuthentication"],
    "DEFAULT_PERMISSION_CLASSES": ["rest_framework.permissions.IsAuthenticated"],
    "EXCEPTION_HANDLER": "apps.core.exceptions.custom_exception_handler",
}

SIMPLE_JWT = {
    "ACCESS_TOKEN_LIFETIME": timedelta(minutes=30),
    "REFRESH_TOKEN_LIFETIME": timedelta(days=7),
    "ROTATE_REFRESH_TOKENS": True,       # refresh 갱신 시 새 토큰 발급
    "BLACKLIST_AFTER_ROTATION": True,    # 이전 refresh 자동 무효화
    "AUTH_HEADER_TYPES": ("Bearer",),
}
```

```python
# apps/users/views.py (Auth 뷰)
from rest_framework.views import APIView
from rest_framework.permissions import AllowAny, IsAuthenticated
from rest_framework_simplejwt.tokens import RefreshToken
from rest_framework_simplejwt.exceptions import TokenError


class RegisterView(APIView):
    permission_classes = [AllowAny]

    def post(self, request):
        serializer = RegisterSerializer(data=request.data)
        serializer.is_valid(raise_exception=True)
        user = serializer.save()
        return _success(serializer.to_representation(user), status.HTTP_201_CREATED)


class LoginView(APIView):
    permission_classes = [AllowAny]

    def post(self, request):
        serializer = LoginSerializer(data=request.data, context={"request": request})
        serializer.is_valid(raise_exception=True)
        user = serializer.validated_data["user"]
        refresh = RefreshToken.for_user(user)
        return _success({
            "user": UserSerializer(user).data,
            "tokens": {"access": str(refresh.access_token), "refresh": str(refresh)},
        })


class MeView(APIView):
    permission_classes = [IsAuthenticated]

    def get(self, request):
        return _success(UserSerializer(request.user).data)


class LogoutView(APIView):
    permission_classes = [IsAuthenticated]

    def post(self, request):
        refresh_token = request.data.get("refresh")
        if not refresh_token:
            return Response({"success": False, "error": {"code": "bad_request", "message": "refresh token required."}}, status=400)
        try:
            RefreshToken(refresh_token).blacklist()
        except TokenError as e:
            return Response({"success": False, "error": {"code": "bad_request", "message": str(e)}}, status=400)
        return Response({"success": True, "message": "Logged out."})
```

```python
# apps/users/urls.py — 앱별 URL 분리 (DRF 모범사례)
# config/urls.py: path("api/v1/", include("apps.users.urls"))
from django.urls import include, path
from rest_framework.routers import DefaultRouter
from rest_framework_simplejwt.views import TokenRefreshView
from .views import RegisterView, LoginView, LogoutView, MeView, UserViewSet

auth_patterns = [
    path("register/", RegisterView.as_view(), name="auth-register"),
    path("login/", LoginView.as_view(), name="auth-login"),
    path("logout/", LogoutView.as_view(), name="auth-logout"),
    path("token/refresh/", TokenRefreshView.as_view(), name="auth-token-refresh"),
    path("me/", MeView.as_view(), name="auth-me"),
]

router = DefaultRouter()
router.register(r"users", UserViewSet, basename="users")

urlpatterns = [
    path("auth/", include(auth_patterns)),
    path("", include(router.urls)),
]
```

---

## 패턴 4 — settings/ 환경 분리

> **핵심:** `config/settings/` 서브패키지. `DJANGO_SETTINGS_MODULE`로 환경 전환.

```python
# development.py
from .base import *

DEBUG = True
DATABASES = {
    "default": {
        "ENGINE": "django.db.backends.postgresql",
        "NAME": os.environ.get("DB_NAME", "mydb"),
        "USER": os.environ.get("DB_USER", "postgres"),
        "PASSWORD": os.environ.get("DB_PASSWORD", ""),
        "HOST": os.environ.get("DB_HOST", "localhost"),
        "PORT": os.environ.get("DB_PORT", "5432"),
    }
}
```

```python
# production.py
from .base import *
import dj_database_url

DEBUG = False
DATABASES = {"default": dj_database_url.config(conn_max_age=600, ssl_require=True)}
SECURE_SSL_REDIRECT = True
SESSION_COOKIE_SECURE = True
CSRF_COOKIE_SECURE = True
```

```bash
# 실행 시 환경 지정
DJANGO_SETTINGS_MODULE=config.settings.development python manage.py runserver
DJANGO_SETTINGS_MODULE=config.settings.production gunicorn config.wsgi:application
```

---

## 패턴 5 — 커스텀 예외 핸들러

> **핵심:** DRF 기본 예외 응답을 `{"success": False, "error": {...}}` 형식으로 통일.

```python
# apps/core/exceptions.py
from rest_framework.exceptions import APIException
from rest_framework.response import Response
from rest_framework.views import exception_handler
import logging

logger = logging.getLogger(__name__)


def custom_exception_handler(exc, context):
    response = exception_handler(exc, context)

    if response is not None:
        data = response.data
        if isinstance(data, dict) and "detail" in data:
            detail = data["detail"]
            return Response(
                {"success": False, "error": {"code": getattr(detail, "code", "error"), "message": str(detail)}},
                status=response.status_code,
            )
        return Response(
            {"success": False, "error": {"code": "validation_error", "message": "Validation error.", "detail": data}},
            status=response.status_code,
        )

    logger.exception("Unhandled exception: %s", exc)
    return Response(
        {"success": False, "error": {"code": "internal_server_error", "message": "Unexpected error."}},
        status=500,
    )


class BadRequestError(APIException):
    status_code = 400; default_code = "bad_request"
class UnauthorizedError(APIException):
    status_code = 401; default_code = "unauthorized"
class ForbiddenError(APIException):
    status_code = 403; default_code = "forbidden"
class NotFoundError(APIException):
    status_code = 404; default_code = "not_found"
class ConflictError(APIException):
    status_code = 409; default_code = "conflict"
```

---

## 안티패턴
```python
# ❌ 기본 User 모델 — 프로젝트 중간에 변경 불가
from django.contrib.auth.models import User  # username 기반, 이메일 로그인 불편
# ✅ 처음부터 AbstractBaseUser + AUTH_USER_MODEL = "users.User"

# ❌ View에서 직접 쿼리 + 404 처리 누락
user = User.objects.filter(pk=pk).first()  # None 반환 시 처리 없음
# ✅ ViewSet.get_object() 또는 get_object_or_404 사용

# ❌ 비밀번호 평문 저장
user.password = request.data["password"]
# ✅ set_password() 또는 create_user() — PBKDF2 해시 보장
user.set_password(request.data["password"])

# ❌ IsSelfOrAdmin에 has_permission() 누락 — 비인증 통과 위험
# has_permission 없으면 BasePermission 기본값 True → 비인증 사용자 접근 가능
# ✅ has_permission() + has_object_permission() 모두 구현

# ❌ 로그아웃 시 토큰 미무효화
class LogoutView(APIView):
    def post(self, request):
        return Response({"success": True})  # 토큰 만료까지 계속 유효
# ✅ Token Blacklist 사용
RefreshToken(request.data.get("refresh")).blacklist()

# ❌ settings.py 단일 파일에 환경별 값 하드코딩
DEBUG = True; DATABASES = {"default": {"HOST": "localhost"}}
# ✅ settings/base.py + development.py + production.py 환경 분리
```

---

## 관련 스킬

- **fastapi-backend-guidelines**: FastAPI async 패턴
- **pytest-backend-testing**: pytest-django 테스트 패턴
- **docker-guidelines**: Dockerfile, docker-compose
- **database-guidelines**: ERD, 인덱스, 읽기/쓰기 분리
