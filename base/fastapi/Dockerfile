FROM python:3.12.3-slim

ENV PYTHONUNBUFFERED=1
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y --no-install-recommends wget curl ca-certificates && \
    rm -rf /var/lib/apt/lists/*

# Install uv (fast Python package manager)
ADD https://astral.sh/uv/install.sh /uv-installer.sh
RUN sh /uv-installer.sh && rm /uv-installer.sh
ENV PATH="/root/.local/bin/:$PATH"
ENV UV_SYSTEM_PYTHON=1
ENV UV_COMPILE_BYTECODE=1

WORKDIR /app

COPY backend/__init__.py ./backend/__init__.py
COPY pyproject.toml ./pyproject.toml
COPY README.md ./README.md

RUN uv sync --compile-bytecode --no-dev

COPY backend ./backend

EXPOSE 8000

CMD ["uvicorn", "backend.main:app", "--host", "0.0.0.0", "--port", "8000", "--workers", "2"]
