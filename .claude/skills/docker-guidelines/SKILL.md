---
name: docker-guidelines
description: >
  크로스 스택 Docker 컨테이너화 가이드라인. 멀티 스테이지 빌드(Python/Node/Java),
  non-root 실행, 레이어 캐시 최적화, docker-compose 서비스 구성, health check,
  .dockerignore 패턴. Dockerfile 및 docker-compose.yaml 작성 시 사용.
triggers:
  - docker
  - dockerfile
  - docker-compose
  - docker compose
  - container
  - containerize
  - multi-stage build
  - docker image
  - docker build
  - docker run
  - docker health check
  - dockerignore
  - non-root container
  - docker volume
  - docker network
---

# Docker Containerization Guidelines

## Purpose

`base/*/Dockerfile` 및 `base/*/docker-compose.yaml` 패턴을 통합한 크로스 스택 Docker 컨테이너화 표준.
Python(FastAPI/Django), Node.js(Express/NestJS), Java(Spring Boot) 스택에서 검증된 프로덕션 패턴을 일관되게 적용한다.

## When to Use This Skill

- Dockerfile 신규 작성 또는 기존 Dockerfile 리팩토링 시
- docker-compose.yaml로 로컬 개발 환경 구성 시
- 멀티 스테이지 빌드 최적화가 필요할 때
- 컨테이너 보안(non-root 실행) 설정 시
- health check 구성 또는 서비스 의존성 관리 시
- .dockerignore 작성 또는 이미지 크기 최적화 시

---

## Core Patterns

### Pattern 1 — Multi-Stage Build

프로덕션 이미지 크기를 최소화하기 위해 빌드 의존성과 런타임을 분리한다.

#### Python — pip 멀티 스테이지 (base/django/ 기반)

```dockerfile
# ── Stage 1: builder ──
FROM python:3.12-slim AS builder
RUN apt-get update && apt-get install -y --no-install-recommends \
    libpq-dev gcc && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY requirements.txt .
RUN pip install --no-cache-dir --prefix=/install -r requirements.txt

# ── Stage 2: runtime ──
FROM python:3.12-slim AS runtime
RUN apt-get update && apt-get install -y --no-install-recommends \
    libpq5 && rm -rf /var/lib/apt/lists/*
COPY --from=builder /install /usr/local
WORKDIR /app
COPY . .
EXPOSE 8000
CMD ["gunicorn", "config.wsgi:application", "--bind", "0.0.0.0:8000"]
```

- `--prefix=/install`로 패키지 격리 설치 → `COPY --from=builder`로 런타임에 복사
- `gcc`, `libpq-dev`는 builder에만 존재하고 런타임 이미지에는 `libpq5`만 포함
- **참고:** `base/fastapi/`는 `uv` 패키지 매니저를 사용하는 단일 스테이지 빌드 (non-root 미적용 상태)

#### Node.js — pnpm + corepack (base/nestjs/ 기반)

```dockerfile
# ── Stage 1: builder ──
FROM node:20-alpine AS builder
WORKDIR /app
RUN corepack enable && corepack prepare pnpm@latest --activate
COPY package.json pnpm-lock.yaml ./
RUN pnpm install --frozen-lockfile
COPY . .
RUN pnpm db:generate && pnpm build

# ── Stage 2: runner ──
FROM node:20-alpine AS runner
WORKDIR /app
ENV NODE_ENV=production
RUN corepack enable && corepack prepare pnpm@latest --activate
COPY package.json pnpm-lock.yaml ./
RUN pnpm install --frozen-lockfile --prod
COPY --from=builder /app/dist ./dist
COPY --from=builder /app/node_modules/.prisma ./node_modules/.prisma
COPY prisma ./prisma
EXPOSE 3000
CMD ["node", "dist/main"]
```

- 2단계 분리: 빌드(dev 의존성 포함) → 런타임(`--prod` 의존성만)
- Prisma 사용 시 `.prisma` 클라이언트와 `prisma/` 스키마 별도 복사 필수
- **참고:** `base/express/`는 3단계(deps→builder→runner) + `npm ci --omit=dev` 패턴 사용

#### Java (Spring Boot)

```dockerfile
# ── Stage 1: builder ──
FROM eclipse-temurin:21-jdk-alpine AS builder
WORKDIR /app
COPY gradlew settings.gradle.kts build.gradle.kts ./
COPY gradle ./gradle
RUN chmod +x gradlew && ./gradlew dependencies --no-daemon --quiet
COPY src ./src
RUN ./gradlew bootJar --no-daemon -x test

# ── Stage 2: runner ──
FROM eclipse-temurin:21-jre-alpine AS runner
WORKDIR /app
COPY --from=builder /app/build/libs/*.jar app.jar
EXPOSE 8080
ENTRYPOINT ["java", \
  "-Djava.security.egd=file:/dev/./urandom", \
  "-Dspring.profiles.active=${SPRING_PROFILES_ACTIVE:-prod}", \
  "-jar", "app.jar"]
```

- `jdk-alpine` → `jre-alpine`으로 전환하여 ~150MB 절감
- Gradle 빌드 파일 먼저 복사 → `dependencies` 캐시 → 소스 복사

---

### Pattern 2 — Non-Root User Execution

컨테이너 내에서 root 권한 실행을 금지한다. 모든 프로덕션 이미지에 전용 사용자를 생성한다.
실제 스택별 사용자명은 다를 수 있다 (예: `django`, `spring`, `appuser`). 아래는 범용 템플릿이다.

#### Alpine 기반 (Node.js / Java)

```dockerfile
RUN addgroup -S appgroup && adduser -S appuser -G appgroup
COPY --chown=appuser:appgroup --from=builder /app/dist ./dist
USER appuser
```

#### Debian 기반 (Python)

```dockerfile
RUN addgroup --system --gid 1001 app \
    && adduser --system --uid 1001 --gid 1001 --no-create-home app
COPY --chown=app:app . .
USER app
```

#### Node.js 내장 사용자

```dockerfile
# node:20-alpine에는 'node' 사용자가 내장
COPY --chown=node:node --from=builder /app/dist ./dist
USER node
```

| Base Image | 권장 사용자 | 생성 방법 |
|---|---|---|
| `node:20-alpine` | `node` | 내장 (별도 생성 불필요) |
| `python:3.12-slim` | `app` | `adduser --system` |
| `eclipse-temurin:21-jre-alpine` | `spring` | `addgroup -S && adduser -S` |

---

### Pattern 3 — Layer Cache Optimization

**변경 빈도가 낮은 레이어를 먼저, 높은 레이어를 나중에** 배치한다.

```dockerfile
# GOOD — 변경 빈도 순서
WORKDIR /app
COPY package.json pnpm-lock.yaml ./   # 거의 변경 안 됨
RUN pnpm install --frozen-lockfile     # lock 변경 시에만 재실행
COPY . .                               # 자주 변경됨
RUN npm run build
```

```dockerfile
# BAD — 소스 변경 → 이후 모든 레이어 무효화
COPY . .
RUN pnpm install --frozen-lockfile     # 매번 재설치됨
RUN npm run build
```

**스택별 캐시 전략:**

| 스택 | 의존성 파일 | 캐시 방식 |
|---|---|---|
| Python (pip) | `requirements.txt` | `--prefix=/install` |
| Python (uv) | `pyproject.toml`, `uv.lock` | uv 캐시 마운트 |
| Node.js (pnpm) | `package.json`, `pnpm-lock.yaml` | node_modules 레이어 |
| Java (Gradle) | `build.gradle.kts`, `gradle/` | `dependencies` 태스크 |

#### BuildKit 캐시 마운트

```dockerfile
# Python
RUN --mount=type=cache,target=/root/.cache/pip \
    pip install -r requirements.txt

# Node.js
RUN --mount=type=cache,target=/root/.local/share/pnpm/store \
    pnpm install --frozen-lockfile

# Java
RUN --mount=type=cache,target=/root/.gradle \
    ./gradlew bootJar --no-daemon
```

---

### Pattern 4 — docker-compose Service Composition

로컬 개발 환경을 `docker-compose.yaml`로 구성한다.
DB health check + `depends_on condition` 패턴으로 서비스 시작 순서를 보장한다.

```yaml
services:
  db:
    image: postgres:16-alpine
    restart: unless-stopped
    environment:
      POSTGRES_USER: ${DB_USER:-app}
      POSTGRES_PASSWORD: ${DB_PASSWORD:-password}
      POSTGRES_DB: ${DB_NAME:-appdb}
    ports:
      - "${DB_PORT:-5432}:5432"
    volumes:
      - postgres_data:/var/lib/postgresql/data
    healthcheck:
      test: ["CMD-SHELL", "pg_isready -U ${DB_USER:-app} -d ${DB_NAME:-appdb}"]
      interval: 10s
      timeout: 5s
      retries: 5
      start_period: 20s

  app:
    build:
      context: .
      dockerfile: Dockerfile
    restart: unless-stopped
    ports:
      - "${APP_PORT:-8000}:8000"
    environment:
      DATABASE_URL: "postgresql://${DB_USER:-app}:${DB_PASSWORD:-password}@db:5432/${DB_NAME:-appdb}"
      JWT_SECRET: ${JWT_SECRET:-dev-secret-change-in-production}
    depends_on:
      db:
        condition: service_healthy
    # 권장: app 레벨 health check (base 스택에는 미구현, 프로덕션 추가 권장)
    healthcheck:
      test: ["CMD-SHELL", "wget -qO- http://localhost:8000/health || exit 1"]
      interval: 30s
      timeout: 5s
      retries: 3
      start_period: 10s
    networks:
      - app-net

volumes:
  postgres_data:

networks:
  app-net:
    driver: bridge
```

**핵심 원칙:**
- `depends_on` + `condition: service_healthy` — DB 준비 완료 후 앱 시작
- `start_period: 20s` — DB 초기화 유예 시간
- `${VAR:-default}` — `.env` 없이도 기본값으로 실행 가능
- Named volumes — 데이터 영속성 보장
- Custom network — 서비스 간 DNS 기반 통신 (`db`를 호스트명으로 사용)

#### Dockerfile 내 Health Check (오케스트레이션 환경용)

```dockerfile
HEALTHCHECK --interval=30s --timeout=5s --start-period=10s --retries=3 \
  CMD wget --no-verbose --tries=1 --spider http://localhost:8000/health || exit 1
```

#### ORM 마이그레이션 연동

```yaml
# Prisma (NestJS) — entry point는 스택에 따라 dist/main 또는 dist/server.js
app:
  command: sh -c "npx prisma migrate deploy && node dist/main"

# Django
app:
  command: sh -c "python manage.py migrate --no-input && gunicorn config.wsgi:application --bind 0.0.0.0:8000"

# Alembic (FastAPI) — base/fastapi에는 미구현, 프로덕션 추가 권장
app:
  command: sh -c "alembic upgrade head && uvicorn backend.main:app --host 0.0.0.0 --port 8000"
```

| Health Check 파라미터 | DB 권장 | App 권장 (프로덕션) |
|---|---|---|
| `interval` | 5-10s | 30s |
| `timeout` | 5s | 5s |
| `retries` | 5 | 3 |
| `start_period` | 20s (선택) | 10s (선택) |

---

### Pattern 5 — .dockerignore

불필요한 파일이 빌드 컨텍스트에 포함되지 않도록 `.dockerignore`를 반드시 작성한다.

```
# === 공통 ===
.git
.gitignore
.vscode
.idea
*.swp
.env
.env.*
!.env.example
*.md
!README.md
docs/
LICENSE
Dockerfile*
docker-compose*
.dockerignore
.github/

# === Python ===
__pycache__/
*.pyc
*.pyo
.mypy_cache/
.ruff_cache/
.pytest_cache/
.venv/
venv/
*.egg-info/
htmlcov/
.coverage

# === Node.js ===
node_modules/
npm-debug.log*
pnpm-debug.log*
.next/
dist/
coverage/
.turbo/
.eslintcache
*.tsbuildinfo

# === Java ===
build/
.gradle/
target/
*.class
*.jar
!gradle/wrapper/gradle-wrapper.jar
```

프로젝트 스택에 맞는 섹션만 선택하여 사용한다. 효과:
- 빌드 컨텍스트 전송 시간 단축 (`.git/` 제외만으로 수백 MB 절감)
- 시크릿 유출 방지 (`.env` 파일 이미지 포함 차단)

---

## Anti-Patterns

### ❌ Root 사용자로 컨테이너 실행

컨테이너 탈출(container escape) 시 호스트 시스템이 직접 위험에 노출된다.
→ Pattern 2의 non-root 사용자 패턴을 반드시 적용한다.

### ❌ 단일 스테이지로 프로덕션 이미지 생성

빌드 도구(gcc, node-gyp, JDK)가 프로덕션 이미지에 포함되어 크기가 2-5배 증가한다.

```dockerfile
# BAD — gcc, libpq-dev가 런타임에 불필요하게 포함
FROM python:3.12-slim
RUN apt-get update && apt-get install -y libpq-dev gcc
RUN pip install -r requirements.txt
COPY . .
CMD ["gunicorn", "app:app"]
```

→ Pattern 1의 멀티 스테이지 빌드를 적용한다.

### ❌ COPY . . 을 의존성 설치 전에 실행

소스 코드 변경 시 의존성 설치 레이어 캐시가 무효화되어 매번 재설치된다.
→ Pattern 3의 레이어 순서 전략을 적용한다.

### ❌ .dockerignore 없이 빌드

`.git/`(수백 MB), `node_modules/`, `.env`(시크릿) 등이 빌드 컨텍스트에 포함된다.
→ Pattern 5의 .dockerignore 템플릿을 적용한다.

### ❌ latest 태그만 사용

버전 고정 없이 `latest`를 사용하면 빌드 재현성이 보장되지 않는다.

```dockerfile
# BAD
FROM node:latest
FROM postgres:latest

# GOOD — 마이너 버전까지 고정
FROM node:20-alpine
FROM postgres:16-alpine
```

### ❌ depends_on만 사용 (health check 없이)

`depends_on`만으로는 서비스 "시작"만 보장하지 "준비"는 보장하지 않는다.

```yaml
# BAD
depends_on:
  - db

# GOOD — health check 통과 후 앱 시작
depends_on:
  db:
    condition: service_healthy
```

---

## References

- `base/fastapi/Dockerfile` — Python 단일 스테이지 (uv, non-root 미적용)
- `base/django/Dockerfile` — Python 2단계 멀티 스테이지
- `base/express/Dockerfile` — Node.js 3단계 멀티 스테이지
- `base/nestjs/Dockerfile` — Node.js 2단계 + corepack pnpm
- `base/spring-boot/Dockerfile` — Java 2단계 (JDK → JRE)
- `base/*/docker-compose.yaml` — PostgreSQL + App 서비스 구성

## 관련 스킬

- **aws-guidelines**: ECR 이미지 푸시, ECS 배포 연동
- **fastapi-backend-guidelines**: FastAPI Dockerfile + uvicorn 설정
- **express-backend-guidelines**: Express Dockerfile + Prisma 마이그레이션
- **spring-boot-guidelines**: Spring Boot Dockerfile + Gradle 빌드

---

**Skill Status**: COMPLETE | **Base Reference**: `base/*/Dockerfile`, `base/*/docker-compose.yaml`
