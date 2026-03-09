---
name: aws-guidelines
description: AWS 인프라 및 서비스 활용 가이드라인. S3 presigned URL(POST/GET/PUT) 패턴, ECS Fargate 배포, ECR 이미지 관리, IAM 최소 권한 원칙, CloudWatch 로깅, GitHub Actions CI/CD 파이프라인. AWS 리소스 구성, 배포 자동화, 스토리지 연동 작업 시 사용.
triggers:
  - aws
  - s3
  - s3 upload
  - s3 presigned
  - presigned url
  - ecs
  - ecs deploy
  - ecr
  - iam
  - iam role
  - iam policy
  - cloudwatch
  - aws deploy
  - aws infrastructure
  - github actions deploy
  - fargate
---

# AWS Infrastructure & Services Guidelines

## Purpose

AWS 서비스를 활용한 인프라 구성, 배포 자동화, 스토리지 연동의 표준 패턴을 제공한다. `base/` 스택들에서 공통으로 사용하는 AWS 패턴을 크로스 스택으로 정리한다.

> 상세 코드: `resources/ecs-deployment.md` (CI/CD, Task Definition, ECR), `resources/iam-policies.md` (IAM 정책)

## Architecture Overview

```
Client ──→ ALB ──→ ECS (Fargate) ──→ RDS (PostgreSQL)
  │                   │
  │ (presigned POST)  │ (IAM Role)
  ↓                   ↓
  S3 Bucket       CloudWatch Logs

CI/CD: GitHub Actions → ECR → ECS Task Definition → Rolling Update
```

---

## Pattern 1: S3 Presigned URL — 클라이언트 직접 업로드

### 핵심 원칙

- 파일은 **절대 백엔드 서버를 경유하지 않는다** — presigned URL로 클라이언트가 S3에 직접 업로드
- 업로드: presigned **POST** (boto3, AWS SDK v3) 또는 presigned **PUT** (Java AWS SDK v2)
- 다운로드: presigned **GET** — 만료 시간 제한으로 접근 통제
- `content-length-range` 조건으로 서버 측 파일 크기 제한 필수

### 업로드 흐름

```
1. Client → Backend: POST /api/v1/upload/presigned-url (s3_key, content_type)
2. Backend → S3: generate_presigned_post() with conditions
3. Backend → Client: { url, fields }
4. Client → S3: POST url with fields + file (직접 업로드)
```

### Python (boto3) — Presigned POST

```python
import boto3
from botocore.exceptions import ClientError

def get_s3_client():
    return boto3.client(
        "s3",
        aws_access_key_id=settings.aws_access_key_id,
        aws_secret_access_key=settings.aws_secret_access_key,
        region_name=settings.aws_region,
    )

def generate_presigned_post(
    s3_key: str,
    content_type: str = "application/octet-stream",
    max_size_bytes: int = 50 * 1024 * 1024,
    expires_in: int = 300,
) -> dict | None:
    try:
        s3 = get_s3_client()
        return s3.generate_presigned_post(
            Bucket=settings.s3_bucket_name,
            Key=s3_key,
            Fields={"Content-Type": content_type},
            Conditions=[
                {"Content-Type": content_type},
                ["content-length-range", 1, max_size_bytes],
            ],
            ExpiresIn=expires_in,
        )
    except ClientError:
        logger.exception(f"Presigned POST failed: key={s3_key}")
        return None

def generate_presigned_get_url(s3_key: str, expires_in: int = 3600) -> str | None:
    try:
        s3 = get_s3_client()
        return s3.generate_presigned_url(
            "get_object",
            Params={"Bucket": settings.s3_bucket_name, "Key": s3_key},
            ExpiresIn=expires_in,
        )
    except ClientError:
        logger.exception(f"Presigned GET failed: key={s3_key}")
        return None

def delete_object(s3_key: str) -> bool:
    try:
        s3 = get_s3_client()
        s3.delete_object(Bucket=settings.s3_bucket_name, Key=s3_key)
        return True
    except ClientError:
        logger.exception(f"Delete failed: key={s3_key}")
        return False
```

### TypeScript (AWS SDK v3) — Presigned POST

```typescript
import { S3Client, DeleteObjectCommand, GetObjectCommand } from '@aws-sdk/client-s3';
import { createPresignedPost } from '@aws-sdk/s3-presigned-post';
import { getSignedUrl } from '@aws-sdk/s3-request-presigner';

// ECS에서는 credentials: undefined (IAM Role 자동 사용)
const s3Client = new S3Client({
  region: config.AWS_REGION,
  credentials: config.AWS_ACCESS_KEY_ID
    ? { accessKeyId: config.AWS_ACCESS_KEY_ID, secretAccessKey: config.AWS_SECRET_ACCESS_KEY }
    : undefined,
});

export async function createPresignedUploadUrl(
  key: string, contentType: string, maxSizeMB = 50,
): Promise<{ url: string; fields: Record<string, string> }> {
  const { url, fields } = await createPresignedPost(s3Client, {
    Bucket: BUCKET,
    Key: key,
    Conditions: [
      ['content-length-range', 1, maxSizeMB * 1024 * 1024],
      ['starts-with', '$Content-Type', contentType.split('/')[0]],
    ],
    Fields: { 'Content-Type': contentType },
    Expires: 300,
  });
  return { url, fields };
}
```

### Java (AWS SDK v2) — Presigned PUT

```java
// S3Config.java — S3Client + S3Presigner Bean 등록
@Bean
public S3Client s3Client() {
    return S3Client.builder()
            .region(Region.of(region))
            .credentialsProvider(StaticCredentialsProvider.create(
                    AwsBasicCredentials.create(accessKey, secretKey)))
            .build();
}

// S3Uploader.java — Presigned PUT URL 생성
public PresignedUrlResponse generatePresignedPutUrl(
        String directory, String filename, String contentType, int expiryMinutes) {
    String key = directory + "/" + UUID.randomUUID() + "." + extractExtension(filename);
    PutObjectPresignRequest presignRequest = PutObjectPresignRequest.builder()
            .signatureDuration(Duration.ofMinutes(expiryMinutes))
            .putObjectRequest(req -> req.bucket(bucket).key(key).contentType(contentType))
            .build();
    URL presignedUrl = s3Presigner.presignPutObject(presignRequest).url();
    return new PresignedUrlResponse(presignedUrl.toString(), buildUrl(key), key);
}
```

### S3 Key 네이밍 규칙

```
uploads/{entity}/{uuid}.{ext}       ← 사용자 업로드 파일
uploads/users/{uuid}.webp           ← 프로필 이미지
thumbnails/{entity}/{uuid}.webp     ← 썸네일 (클라이언트 압축 후 업로드)
```

---

## Pattern 2: ECS Fargate 배포

> 전체 GitHub Actions 워크플로우 + Task Definition JSON: `resources/ecs-deployment.md`

### 배포 파이프라인

```
GitHub Actions → ECR (Docker push) → ECS Task Definition 갱신 → Rolling Update
```

### 핵심 단계 요약

```yaml
# .github/workflows/deploy.yaml 핵심 구조
steps:
  # 1. OIDC 인증 (Access Key 미사용)
  - uses: aws-actions/configure-aws-credentials@v4
    with:
      role-to-assume: arn:aws:iam::123456789012:role/github-actions-deploy

  # 2. ECR 로그인 + Docker build/push (SHA + latest 태그)
  - uses: aws-actions/amazon-ecr-login@v2

  # 3. Task Definition 이미지 갱신
  - uses: aws-actions/amazon-ecs-render-task-definition@v1

  # 4. ECS 서비스 배포 (안정성 대기)
  - uses: aws-actions/amazon-ecs-deploy-task-definition@v2
    with:
      wait-for-service-stability: true
```

### Task Definition 핵심 설정

| 항목 | 설명 |
|---|---|
| `executionRoleArn` | ECR pull + CloudWatch 로그 전송 권한 |
| `taskRoleArn` | S3, SQS 등 애플리케이션 서비스 접근 권한 |
| `secrets` | SSM Parameter Store / Secrets Manager → 민감 정보 주입 |
| `healthCheck` | `curl -f http://localhost:8000/health` 주기적 확인 |
| `logConfiguration` | `awslogs` 드라이버 → CloudWatch 자동 전송 |

---

## Pattern 3: ECR 이미지 관리

> ECR 라이프사이클 정책 + 태깅 전략 상세: `resources/ecs-deployment.md`

### Docker 멀티 스테이지 빌드 표준

| 스택 | Base Image | Runner | Non-root User |
|---|---|---|---|
| FastAPI | `python:3.12-slim` | uv + uvicorn | (미설정 — 프로덕션 시 추가 필요) |
| Django | `python:3.12-slim` | COPY --from=builder | `django:1001` |
| Express | `node:20-alpine` | npm ci --omit=dev | `node` |
| NestJS | `node:20-alpine` | pnpm --prod | `appuser` |
| Spring Boot | `temurin:21-jdk` | `21-jre-alpine` | `spring` |

### 이미지 태깅 규칙

```bash
# 항상 두 태그: commit SHA (롤백용) + latest (최신)
docker push $ECR_REGISTRY/$REPO:$COMMIT_SHA
docker push $ECR_REGISTRY/$REPO:latest
# 릴리스: 시맨틱 버전 추가
docker push $ECR_REGISTRY/$REPO:v1.2.3
```

### ECR 라이프사이클 정책 요약

- `v` prefix 태그: 최근 10개 유지, 나머지 만료
- untagged 이미지: 7일 후 자동 만료

---

## Pattern 4: IAM 최소 권한 원칙

> 전체 IAM 정책 JSON: `resources/iam-policies.md`

### 역할 분리 원칙

```
GitHub Actions Role     ← ECR push + ECS deploy만 (OIDC 인증)
ECS Execution Role      ← ECR pull + CloudWatch + SSM 읽기
ECS Task Role           ← S3/SQS 등 애플리케이션 서비스
Developer Role          ← 읽기 전용 (CloudWatch, ECS describe)
```

### 핵심 규칙

- **Resource는 항상 특정** — `*` 와일드카드 금지
- **Action은 최소한만** — `s3:*` 대신 `s3:PutObject`, `s3:GetObject` 등 명시
- **Condition 활용** — 경로, 태그, IP 제한 추가
- **GitHub Actions는 OIDC** — IAM Access Key 미사용

### S3 최소 권한 예시

```json
{
  "Action": ["s3:PutObject", "s3:GetObject", "s3:DeleteObject"],
  "Resource": "arn:aws:s3:::my-bucket/uploads/*"
}
```

---

## Pattern 5: CloudWatch 로깅 및 모니터링

### 로그 그룹 구조

```
/ecs/my-backend         ← ECS 컨테이너 로그 (awslogs 드라이버)
/ecs/my-backend/access  ← 액세스 로그 (선택)
```

### 필수 CloudWatch Alarms

| 알람 | 조건 | 심각도 |
|---|---|---|
| CPU 과부하 | CPU Utilization > 80% | Warning → Scale Out |
| 메모리 과부하 | Memory Utilization > 80% | Warning → Scale Out |
| 서비스 다운 | Running Task Count = 0 | **Critical** |
| 서버 오류 | ALB 5xx Rate > 1% | Error |
| 응답 지연 | ALB Response Time > 2s | Warning |

### ECS Auto Scaling

```json
{
  "TargetTrackingScalingPolicyConfiguration": {
    "TargetValue": 70.0,
    "PredefinedMetricSpecification": {
      "PredefinedMetricType": "ECSServiceAverageCPUUtilization"
    },
    "ScaleInCooldown": 300,
    "ScaleOutCooldown": 60
  },
  "MinCapacity": 1,
  "MaxCapacity": 4
}
```

---

## Environment Variables 표준

### 필수 AWS 환경변수

```bash
# S3
AWS_ACCESS_KEY_ID=AKIA...           # 로컬 전용, ECS에서는 Task Role 사용
AWS_SECRET_ACCESS_KEY=...           # 로컬 전용
AWS_REGION=ap-northeast-2
AWS_S3_BUCKET=my-bucket

# ECS (CI/CD용)
ECR_REPOSITORY=my-backend
ECS_CLUSTER=my-cluster
ECS_SERVICE=my-service
```

### 환경별 자격증명 전략

| 환경 | 방식 | 설명 |
|---|---|---|
| 로컬 개발 | Access Key | `.env` 파일 |
| ECS Fargate | **IAM Task Role** | 환경변수 불필요, SDK 자동 감지 |
| GitHub Actions | **OIDC** | `role-to-assume`, Access Key 미사용 |
| EC2 | **Instance Profile** | IAM Role 연결, SDK 자동 감지 |

---

## Anti-Patterns

### 1. 백엔드 파일 프록시 금지

```python
# BAD: 파일이 백엔드 서버 경유 — 메모리/대역폭 낭비
@router.post("/upload")
async def upload(file: UploadFile):
    s3.put_object(Body=file.file.read())

# GOOD: presigned URL로 클라이언트 직접 업로드
@router.post("/upload/presigned-url")
async def get_presigned_url(request: PresignedRequest):
    return generate_presigned_post(request.s3_key, request.content_type)
```

### 2. 하드코딩 자격증명 금지

```python
# BAD
client = boto3.client("s3", aws_access_key_id="AKIA...")
# GOOD: 환경변수
client = boto3.client("s3", aws_access_key_id=settings.aws_access_key_id)
# BEST: ECS에서는 자격증명 생략 (Task Role 자동)
client = boto3.client("s3", region_name=settings.aws_region)
```

### 3. IAM 와일드카드 권한 금지

```json
// BAD
{ "Action": "s3:*", "Resource": "*" }
// GOOD: 특정 버킷의 특정 경로만
{ "Action": ["s3:PutObject", "s3:GetObject"], "Resource": "arn:aws:s3:::my-bucket/uploads/*" }
```

### 4. ECR latest 태그만 사용 금지

```bash
# BAD: latest만 — 롤백 불가
docker push $REPO:latest
# GOOD: commit SHA + latest 병행
docker push $REPO:$COMMIT_SHA && docker push $REPO:latest
```

### 5. Presigned URL 보안 미설정 금지

```python
# BAD: 만료 24시간 + 크기 제한 없음
generate_presigned_post(s3_key, expires_in=86400)
# GOOD: 업로드 5분, 50MB 제한 (함수 기본값 활용)
generate_presigned_post(s3_key, expires_in=300, max_size_bytes=50 * 1024 * 1024)
```

### 6. Docker root 실행 / 단일 스테이지 금지

```dockerfile
# BAD: root 실행 + 빌드 도구 포함
FROM node:20
RUN npm install && npm run build
CMD ["node", "dist/server.js"]

# GOOD: 멀티 스테이지 + non-root
FROM node:20-alpine AS builder
RUN npm ci && npm run build
FROM node:20-alpine
COPY --from=builder /app/dist ./dist
RUN npm ci --omit=dev
USER node
CMD ["node", "dist/server.js"]
```

---

## Quick Reference

### S3 Presigned URL 기본값

| 항목 | 업로드 (POST/PUT) | 다운로드 (GET) |
|---|---|---|
| 만료 시간 | 300초 (5분) | 3600초 (1시간) |
| 최대 크기 | 50MB | N/A |

### SDK 버전별 Presigned 방식

| SDK | 업로드 | 다운로드 |
|---|---|---|
| boto3 (Python) | `generate_presigned_post()` | `generate_presigned_url("get_object")` |
| AWS SDK v3 (TS) | `createPresignedPost()` | `getSignedUrl(GetObjectCommand)` |
| AWS SDK v2 (Java) | `presignPutObject()` | `presignGetObject()` |

### ECS Fargate 리소스 가이드

| 프로필 | CPU | Memory | 용도 |
|---|---|---|---|
| Small | 256 (0.25 vCPU) | 512 MB | 경량 API |
| Medium | 512 (0.5 vCPU) | 1024 MB | 일반 백엔드 API |
| Large | 1024 (1 vCPU) | 2048 MB | 이미지 처리 |
| XLarge | 2048 (2 vCPU) | 4096 MB | 고부하 배치 |
