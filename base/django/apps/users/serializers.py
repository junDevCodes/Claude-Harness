"""
Serializers for the users app.

Provides:
    - RegisterSerializer   — create new user
    - UserSerializer       — read user data
    - UserUpdateSerializer — partial update user data
    - LoginSerializer      — validate credentials, return tokens
"""
from django.contrib.auth import authenticate
from rest_framework import serializers
from rest_framework_simplejwt.tokens import RefreshToken

from .models import User


class RegisterSerializer(serializers.ModelSerializer):
    """Register a new user and return JWT tokens."""

    password = serializers.CharField(write_only=True, min_length=8, style={"input_type": "password"})
    password_confirm = serializers.CharField(write_only=True, style={"input_type": "password"})

    class Meta:
        model = User
        fields = ("email", "name", "password", "password_confirm")

    def validate(self, attrs: dict) -> dict:
        if attrs["password"] != attrs.pop("password_confirm"):
            raise serializers.ValidationError({"password_confirm": "Passwords do not match."})
        return attrs

    def create(self, validated_data: dict) -> User:
        return User.objects.create_user(
            email=validated_data["email"],
            name=validated_data.get("name", ""),
            password=validated_data["password"],
        )

    def to_representation(self, instance: User) -> dict:
        """Return user data plus fresh JWT tokens after registration."""
        refresh = RefreshToken.for_user(instance)
        return {
            "user": UserSerializer(instance).data,
            "tokens": {
                "access": str(refresh.access_token),
                "refresh": str(refresh),
            },
        }


class UserSerializer(serializers.ModelSerializer):
    """Read-only serializer for User data."""

    class Meta:
        model = User
        fields = ("id", "email", "name", "role", "is_active", "created_at", "updated_at")
        read_only_fields = fields


class UserUpdateSerializer(serializers.ModelSerializer):
    """Partial update serializer — name only (email/role changes require separate flows)."""

    class Meta:
        model = User
        fields = ("name",)

    def to_representation(self, instance: User) -> dict:
        return UserSerializer(instance).data


class LoginSerializer(serializers.Serializer):
    """Authenticate user with email + password, return JWT tokens."""

    email = serializers.EmailField()
    password = serializers.CharField(write_only=True, style={"input_type": "password"})

    def validate(self, attrs: dict) -> dict:
        email = attrs.get("email")
        password = attrs.get("password")

        user = authenticate(request=self.context.get("request"), username=email, password=password)

        if not user:
            raise serializers.ValidationError("Invalid email or password.", code="authorization")

        if not user.is_active:
            raise serializers.ValidationError("User account is disabled.", code="authorization")

        attrs["user"] = user
        return attrs
