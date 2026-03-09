"""
AWS S3 utilities — presigned POST URL generation.

Usage:
    from apps.core.utils.s3 import generate_presigned_post, generate_presigned_get

    # Generate presigned POST for direct client-side upload
    result = generate_presigned_post(
        object_key="uploads/users/avatar.jpg",
        content_type="image/jpeg",
        max_size_mb=10,
        expires_in=300,
    )
    # result = {"url": "...", "fields": {...}}
    # Client POSTs directly to S3 using result["url"] + result["fields"]
"""
import logging
from typing import Any

import boto3
from botocore.exceptions import ClientError
from django.conf import settings

logger = logging.getLogger(__name__)


def _get_s3_client():
    return boto3.client(
        "s3",
        region_name=settings.AWS_S3_REGION,
        aws_access_key_id=settings.AWS_ACCESS_KEY_ID,
        aws_secret_access_key=settings.AWS_SECRET_ACCESS_KEY,
    )


def generate_presigned_post(
    object_key: str,
    content_type: str = "application/octet-stream",
    max_size_mb: int = 50,
    expires_in: int = 300,
) -> dict[str, Any]:
    """
    Generate an S3 presigned POST URL for direct client-side upload.

    Args:
        object_key: S3 object key (e.g. "uploads/users/avatar.jpg")
        content_type: MIME type of the file
        max_size_mb: Maximum allowed file size in megabytes
        expires_in: URL expiration in seconds (default: 5 minutes)

    Returns:
        {"url": str, "fields": dict}

    Raises:
        RuntimeError: If the presigned URL could not be generated
    """
    max_size_bytes = max_size_mb * 1024 * 1024

    try:
        s3_client = _get_s3_client()
        response = s3_client.generate_presigned_post(
            Bucket=settings.AWS_S3_BUCKET_NAME,
            Key=object_key,
            Fields={"Content-Type": content_type},
            Conditions=[
                {"Content-Type": content_type},
                ["content-length-range", 1, max_size_bytes],
            ],
            ExpiresIn=expires_in,
        )
        return response
    except ClientError as e:
        logger.exception("Failed to generate presigned POST URL for key=%s", object_key)
        raise RuntimeError(f"S3 presigned POST generation failed: {e}") from e


def generate_presigned_get(
    object_key: str,
    expires_in: int = 3600,
) -> str:
    """
    Generate an S3 presigned GET URL for temporary file access.

    Args:
        object_key: S3 object key
        expires_in: URL expiration in seconds (default: 1 hour)

    Returns:
        Presigned GET URL string

    Raises:
        RuntimeError: If the presigned URL could not be generated
    """
    try:
        s3_client = _get_s3_client()
        url = s3_client.generate_presigned_url(
            "get_object",
            Params={"Bucket": settings.AWS_S3_BUCKET_NAME, "Key": object_key},
            ExpiresIn=expires_in,
        )
        return url
    except ClientError as e:
        logger.exception("Failed to generate presigned GET URL for key=%s", object_key)
        raise RuntimeError(f"S3 presigned GET generation failed: {e}") from e


def delete_object(object_key: str) -> None:
    """
    Delete an object from S3.

    Args:
        object_key: S3 object key to delete

    Raises:
        RuntimeError: If deletion failed
    """
    try:
        s3_client = _get_s3_client()
        s3_client.delete_object(Bucket=settings.AWS_S3_BUCKET_NAME, Key=object_key)
    except ClientError as e:
        logger.exception("Failed to delete S3 object key=%s", object_key)
        raise RuntimeError(f"S3 delete failed: {e}") from e
