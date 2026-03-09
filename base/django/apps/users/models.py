"""
Custom User model using email as the primary identifier.

AUTH_USER_MODEL = 'users.User' must be set in settings BEFORE the first migration.
"""
from django.contrib.auth.models import AbstractBaseUser, PermissionsMixin
from django.db import models
from django.utils import timezone

from .managers import UserManager


class UserRole(models.TextChoices):
    USER = "USER", "User"
    ADMIN = "ADMIN", "Admin"


class User(AbstractBaseUser, PermissionsMixin):
    """
    Custom User model.

    - Login identifier: email (not username)
    - Password: hashed via Django's PBKDF2 (set_password)
    - Roles: USER (default) / ADMIN

    Customization guide:
        - Add domain-specific profile fields here (e.g. avatar, bio)
        - Or create a separate Profile model with OneToOneField(User)
    """
    email = models.EmailField(unique=True, db_index=True)
    name = models.CharField(max_length=150, blank=True)
    role = models.CharField(
        max_length=10,
        choices=UserRole.choices,
        default=UserRole.USER,
    )

    # Django internals
    is_active = models.BooleanField(default=True)
    is_staff = models.BooleanField(default=False)

    created_at = models.DateTimeField(default=timezone.now)
    updated_at = models.DateTimeField(auto_now=True)

    objects = UserManager()

    USERNAME_FIELD = "email"
    REQUIRED_FIELDS = ["name"]

    class Meta:
        db_table = "users"
        ordering = ["-created_at"]
        verbose_name = "user"
        verbose_name_plural = "users"

    def __str__(self) -> str:
        return self.email

    @property
    def is_admin(self) -> bool:
        return self.role == UserRole.ADMIN
