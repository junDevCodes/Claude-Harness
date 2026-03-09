"""
AWS S3 utilities for file upload.

Pattern: Presigned POST URL flow
  1. Client requests presigned URL from backend
  2. Backend generates presigned POST with conditions
  3. Client uploads file DIRECTLY to S3 (no proxy through backend)

This avoids sending large files through the backend server.
"""

import logging
from typing import Any, Optional

import boto3
from botocore.exceptions import ClientError

from backend.core.config import settings

logger = logging.getLogger(__name__)


def get_s3_client():
    """Create an S3 client using configured credentials."""
    return boto3.client(
        "s3",
        aws_access_key_id=settings.aws_access_key_id,
        aws_secret_access_key=settings.aws_secret_access_key,
        region_name=settings.aws_region,
    )


def generate_presigned_post(
    s3_key: str,
    content_type: str = "application/octet-stream",
    max_size_bytes: int = 50 * 1024 * 1024,  # 50MB default
    expires_in: int = 300,  # 5 minutes
) -> Optional[dict[str, Any]]:
    """
    Generate a presigned POST URL for direct S3 upload.

    Args:
        s3_key: S3 object key (path in bucket)
        content_type: MIME type of the file
        max_size_bytes: Maximum allowed file size in bytes
        expires_in: URL expiration time in seconds

    Returns:
        Dict with 'url' and 'fields' for form POST, or None on error

    Usage (client-side):
        response = requests.post(
            presigned["url"],
            data=presigned["fields"],
            files={"file": file_content}
        )
    """
    try:
        s3 = get_s3_client()
        response = s3.generate_presigned_post(
            Bucket=settings.s3_bucket_name,
            Key=s3_key,
            Fields={"Content-Type": content_type},
            Conditions=[
                {"Content-Type": content_type},
                ["content-length-range", 1, max_size_bytes],
            ],
            ExpiresIn=expires_in,
        )
        return response
    except ClientError as e:
        logger.exception(f"Failed to generate presigned POST for key={s3_key}: {e}")
        return None


def generate_presigned_get_url(
    s3_key: str,
    expires_in: int = 3600,  # 1 hour default
) -> Optional[str]:
    """
    Generate a presigned GET URL for temporary file access.

    Args:
        s3_key: S3 object key
        expires_in: URL expiration time in seconds

    Returns:
        Presigned URL string, or None on error
    """
    try:
        s3 = get_s3_client()
        url: str = s3.generate_presigned_url(
            "get_object",
            Params={"Bucket": settings.s3_bucket_name, "Key": s3_key},
            ExpiresIn=expires_in,
        )
        return url
    except ClientError as e:
        logger.exception(f"Failed to generate presigned GET for key={s3_key}: {e}")
        return None


def delete_object(s3_key: str) -> bool:
    """
    Delete an object from S3.

    Args:
        s3_key: S3 object key to delete

    Returns:
        True if deleted successfully, False otherwise
    """
    try:
        s3 = get_s3_client()
        s3.delete_object(Bucket=settings.s3_bucket_name, Key=s3_key)
        logger.info(f"Deleted S3 object: {s3_key}")
        return True
    except ClientError as e:
        logger.exception(f"Failed to delete S3 object {s3_key}: {e}")
        return False
