"""
Custom exception classes and DRF exception handler.

Register in settings.py:
    REST_FRAMEWORK = {
        "EXCEPTION_HANDLER": "apps.core.exceptions.custom_exception_handler",
    }
"""
import logging
from typing import Any

from rest_framework import status
from rest_framework.exceptions import APIException
from rest_framework.response import Response
from rest_framework.views import exception_handler

logger = logging.getLogger(__name__)


# ---------------------------------------------------------------------------
# Unified error response format
# ---------------------------------------------------------------------------

def _error_response(code: str, message: str, status_code: int, detail: Any = None) -> Response:
    payload: dict[str, Any] = {
        "success": False,
        "error": {
            "code": code,
            "message": message,
        },
    }
    if detail is not None:
        payload["error"]["detail"] = detail
    return Response(payload, status=status_code)


# ---------------------------------------------------------------------------
# Custom exception handler (replaces DRF default)
# ---------------------------------------------------------------------------

def custom_exception_handler(exc: Exception, context: dict) -> Response | None:
    response = exception_handler(exc, context)

    if response is not None:
        status_code = response.status_code
        data = response.data

        # Try to extract a meaningful message from DRF response data
        if isinstance(data, dict):
            detail = data.get("detail")
            if detail:
                message = str(detail)
                code = detail.code if hasattr(detail, "code") else "error"
            else:
                message = "Validation error"
                code = "validation_error"
                return _error_response(code, message, status_code, detail=data)
        elif isinstance(data, list):
            message = "Validation error"
            code = "validation_error"
            return _error_response(code, message, status_code, detail=data)
        else:
            message = str(data)
            code = "error"

        return _error_response(code, message, status_code)

    # Log unexpected exceptions
    logger.exception("Unhandled exception: %s", exc)
    return _error_response(
        "internal_server_error",
        "An unexpected error occurred.",
        status.HTTP_500_INTERNAL_SERVER_ERROR,
    )


# ---------------------------------------------------------------------------
# Custom exception classes
# ---------------------------------------------------------------------------

class BadRequestError(APIException):
    status_code = status.HTTP_400_BAD_REQUEST
    default_code = "bad_request"
    default_detail = "Bad request."


class UnauthorizedError(APIException):
    status_code = status.HTTP_401_UNAUTHORIZED
    default_code = "unauthorized"
    default_detail = "Authentication credentials were not provided or are invalid."


class ForbiddenError(APIException):
    status_code = status.HTTP_403_FORBIDDEN
    default_code = "forbidden"
    default_detail = "You do not have permission to perform this action."


class NotFoundError(APIException):
    status_code = status.HTTP_404_NOT_FOUND
    default_code = "not_found"
    default_detail = "The requested resource was not found."


class ConflictError(APIException):
    status_code = status.HTTP_409_CONFLICT
    default_code = "conflict"
    default_detail = "A conflict occurred with the current state of the resource."
