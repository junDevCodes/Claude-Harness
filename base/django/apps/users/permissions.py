"""Custom DRF permission classes."""
from rest_framework.permissions import BasePermission, IsAuthenticated


class IsAdminUser(BasePermission):
    """Allow access only to users with role=ADMIN."""

    def has_permission(self, request, view) -> bool:
        return bool(
            request.user
            and request.user.is_authenticated
            and request.user.is_admin
        )


class IsSelfOrAdmin(BasePermission):
    """Allow access to the resource owner or an admin user."""

    def has_object_permission(self, request, view, obj) -> bool:
        if not request.user or not request.user.is_authenticated:
            return False
        return obj == request.user or request.user.is_admin
