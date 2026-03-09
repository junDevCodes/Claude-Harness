import {
  S3Client,
  DeleteObjectCommand,
  GetObjectCommand,
} from '@aws-sdk/client-s3';
import { createPresignedPost } from '@aws-sdk/s3-presigned-post';
import { getSignedUrl } from '@aws-sdk/s3-request-presigner';
import { config } from '../config/config';

const s3Client = new S3Client({
  region: config.AWS_REGION,
  credentials:
    config.AWS_ACCESS_KEY_ID && config.AWS_SECRET_ACCESS_KEY
      ? {
          accessKeyId: config.AWS_ACCESS_KEY_ID,
          secretAccessKey: config.AWS_SECRET_ACCESS_KEY,
        }
      : undefined,
});

const BUCKET = config.AWS_S3_BUCKET ?? '';
const EXPIRES_IN = config.AWS_S3_PRESIGNED_EXPIRES;

export interface PresignedPostResult {
  url: string;
  fields: Record<string, string>;
}

/**
 * 파일 업로드용 presigned POST URL 생성
 * @param key S3 오브젝트 키 (경로 포함)
 * @param contentType MIME 타입 (예: image/webp)
 * @param maxSizeMB 최대 파일 크기 (MB)
 */
export async function createPresignedUploadUrl(
  key: string,
  contentType: string,
  maxSizeMB = 50,
): Promise<PresignedPostResult> {
  const { url, fields } = await createPresignedPost(s3Client, {
    Bucket: BUCKET,
    Key: key,
    Conditions: [
      ['content-length-range', 1, maxSizeMB * 1024 * 1024],
      ['starts-with', '$Content-Type', contentType.split('/')[0]],
    ],
    Fields: { 'Content-Type': contentType },
    Expires: EXPIRES_IN,
  });

  return { url, fields };
}

/**
 * 파일 다운로드용 presigned GET URL 생성
 */
export async function createPresignedGetUrl(key: string): Promise<string> {
  const command = new GetObjectCommand({ Bucket: BUCKET, Key: key });
  return getSignedUrl(s3Client, command, { expiresIn: EXPIRES_IN });
}

/**
 * S3 오브젝트 삭제
 */
export async function deleteObject(key: string): Promise<void> {
  await s3Client.send(new DeleteObjectCommand({ Bucket: BUCKET, Key: key }));
}

/**
 * S3 Public URL 생성 (버킷이 public인 경우)
 */
export function getPublicUrl(key: string): string {
  return `https://${BUCKET}.s3.${config.AWS_REGION}.amazonaws.com/${key}`;
}
