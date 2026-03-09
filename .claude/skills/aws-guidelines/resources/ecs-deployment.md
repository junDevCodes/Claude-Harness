# ECS Deployment — 상세 가이드

> SKILL.md Pattern 2/3의 상세 코드 예시.

## GitHub Actions CI/CD 전체 워크플로우

```yaml
# .github/workflows/deploy.yaml
name: Deploy to ECS
on:
  push:
    branches: [main]

env:
  AWS_REGION: ap-northeast-2
  ECR_REPOSITORY: my-backend
  ECS_CLUSTER: my-cluster
  ECS_SERVICE: my-service
  TASK_DEFINITION: task-def.json

jobs:
  deploy:
    runs-on: ubuntu-latest
    permissions:
      id-token: write    # OIDC 토큰 요청
      contents: read
    steps:
      - uses: actions/checkout@v4

      # OIDC로 AWS 인증 (IAM Access Key 미사용)
      - uses: aws-actions/configure-aws-credentials@v4
        with:
          role-to-assume: arn:aws:iam::123456789012:role/github-actions-deploy
          aws-region: ${{ env.AWS_REGION }}

      - uses: aws-actions/amazon-ecr-login@v2
        id: ecr-login

      - name: Build and push Docker image
        env:
          ECR_REGISTRY: ${{ steps.ecr-login.outputs.registry }}
          IMAGE_TAG: ${{ github.sha }}
        run: |
          docker build -t $ECR_REGISTRY/$ECR_REPOSITORY:$IMAGE_TAG .
          docker build -t $ECR_REGISTRY/$ECR_REPOSITORY:latest .
          docker push $ECR_REGISTRY/$ECR_REPOSITORY:$IMAGE_TAG
          docker push $ECR_REGISTRY/$ECR_REPOSITORY:latest

      - name: Update ECS task definition
        id: task-def
        uses: aws-actions/amazon-ecs-render-task-definition@v1
        with:
          task-definition: ${{ env.TASK_DEFINITION }}
          container-name: app
          image: ${{ steps.ecr-login.outputs.registry }}/${{ env.ECR_REPOSITORY }}:${{ github.sha }}

      - name: Deploy to ECS
        uses: aws-actions/amazon-ecs-deploy-task-definition@v2
        with:
          task-definition: ${{ steps.task-def.outputs.task-definition }}
          service: ${{ env.ECS_SERVICE }}
          cluster: ${{ env.ECS_CLUSTER }}
          wait-for-service-stability: true
```

## ECS Task Definition 전체 구조

```json
{
  "family": "my-backend",
  "networkMode": "awsvpc",
  "requiresCompatibilities": ["FARGATE"],
  "cpu": "512",
  "memory": "1024",
  "executionRoleArn": "arn:aws:iam::123456789012:role/ecsTaskExecutionRole",
  "taskRoleArn": "arn:aws:iam::123456789012:role/ecsTaskRole",
  "containerDefinitions": [
    {
      "name": "app",
      "image": "123456789012.dkr.ecr.ap-northeast-2.amazonaws.com/my-backend:latest",
      "portMappings": [{ "containerPort": 8000, "protocol": "tcp" }],
      "environment": [
        { "name": "DATABASE_URL", "value": "" }
      ],
      "secrets": [
        {
          "name": "DB_PASSWORD",
          "valueFrom": "arn:aws:ssm:ap-northeast-2:123456789012:parameter/prod/db-password"
        }
      ],
      "logConfiguration": {
        "logDriver": "awslogs",
        "options": {
          "awslogs-group": "/ecs/my-backend",
          "awslogs-region": "ap-northeast-2",
          "awslogs-stream-prefix": "ecs"
        }
      },
      "healthCheck": {
        "command": ["CMD-SHELL", "curl -f http://localhost:8000/health || exit 1"],
        "interval": 30,
        "timeout": 5,
        "retries": 3
      }
    }
  ]
}
```

### 주요 설정 포인트

| 항목 | 설명 |
|---|---|
| `executionRoleArn` | ECR 이미지 pull, CloudWatch 로그 전송 권한 |
| `taskRoleArn` | 컨테이너 내부에서 AWS 서비스(S3, SQS 등) 접근 권한 |
| `secrets` | SSM Parameter Store / Secrets Manager에서 민감 정보 주입 |
| `healthCheck` | ECS가 컨테이너 상태를 주기적으로 확인 |

## ECR 라이프사이클 정책

```json
{
  "rules": [
    {
      "rulePriority": 1,
      "description": "Keep last 10 tagged images",
      "selection": {
        "tagStatus": "tagged",
        "tagPrefixList": ["v"],
        "countType": "imageCountMoreThan",
        "countNumber": 10
      },
      "action": { "type": "expire" }
    },
    {
      "rulePriority": 2,
      "description": "Expire untagged images older than 7 days",
      "selection": {
        "tagStatus": "untagged",
        "countType": "sinceImagePushed",
        "countUnit": "days",
        "countNumber": 7
      },
      "action": { "type": "expire" }
    }
  ]
}
```

## ECR 이미지 태깅 전략

```bash
# 항상 두 태그로 push: commit SHA + latest
docker tag app:latest $ECR_REGISTRY/$REPO:$COMMIT_SHA
docker tag app:latest $ECR_REGISTRY/$REPO:latest

# 릴리스 시 시맨틱 버전 태그 추가
docker tag app:latest $ECR_REGISTRY/$REPO:v1.2.3
```
