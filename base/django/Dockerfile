# ---------------------------------------------------------------------------
# Dockerfile — Django 5 base project
# ---------------------------------------------------------------------------
# Stage 1: Builder — install dependencies
# ---------------------------------------------------------------------------
FROM python:3.12-slim AS builder

WORKDIR /app

# Install system dependencies for psycopg and compilation
RUN apt-get update && apt-get install -y --no-install-recommends \
    libpq-dev \
    gcc \
    && rm -rf /var/lib/apt/lists/*

# Upgrade pip and install dependencies into /install (for copy)
COPY requirements.txt .
RUN pip install --upgrade pip \
    && pip install --prefix=/install --no-cache-dir -r requirements.txt

# ---------------------------------------------------------------------------
# Stage 2: Runtime — lean final image
# ---------------------------------------------------------------------------
FROM python:3.12-slim AS runtime

WORKDIR /app

# Runtime system dependencies only
RUN apt-get update && apt-get install -y --no-install-recommends \
    libpq5 \
    && rm -rf /var/lib/apt/lists/*

# Copy installed packages from builder
COPY --from=builder /install /usr/local

# Copy application source
COPY . .

# Create non-root user for security
RUN addgroup --system --gid 1001 django \
    && adduser --system --uid 1001 --gid 1001 --no-create-home django \
    && chown -R django:django /app

USER django

# Collect static files at build time
RUN DJANGO_SETTINGS_MODULE=config.settings.production \
    SECRET_KEY=placeholder \
    DATABASE_URL=postgres://placeholder \
    python manage.py collectstatic --no-input 2>/dev/null || true

EXPOSE 8000

# Entrypoint: run migrations then start gunicorn
CMD ["sh", "-c", \
    "python manage.py migrate --no-input && \
     gunicorn config.wsgi:application \
       --bind 0.0.0.0:8000 \
       --workers 2 \
       --timeout 120 \
       --access-logfile - \
       --error-logfile -"]
