"""
FastAPI application entry point.

Create application with create_application() factory.
Run with: uvicorn backend.main:app --reload --port 8000
"""

from contextlib import asynccontextmanager

from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware

from backend.core.config import settings
from backend.api.v1.routers.auth import router as auth_router
from backend.api.v1.routers.user import router as user_router
from backend.middleware.error_handler import register_exception_handlers
from backend.utils.logger import setup_logging


@asynccontextmanager
async def lifespan(app: FastAPI):
    """Application lifespan — startup/shutdown hooks."""
    setup_logging()

    # Safety check: mock auth must not run in production
    if settings.mock_auth_enabled and settings.environment == "production":
        raise RuntimeError(
            "CRITICAL: MOCK_AUTH_ENABLED=true is not allowed in production. "
            "Set MOCK_AUTH_ENABLED=false."
        )

    if settings.mock_auth_enabled:
        import logging
        logging.warning(
            "Mock authentication is ENABLED — for development/testing only."
        )

    yield
    # Shutdown cleanup (add connection pool teardown here if needed)


def create_application() -> FastAPI:
    """Application factory — configure FastAPI with routers and middleware."""

    app = FastAPI(
        title="Backend API",
        version="1.0.0",
        description="FastAPI starter kit with DDD + JWT + Read/Write DB separation",
        docs_url="/docs" if settings.is_development else None,
        redoc_url="/redoc" if settings.is_development else None,
        openapi_url="/openapi.json",
        lifespan=lifespan,
    )

    # Exception handlers
    register_exception_handlers(app)

    # CORS
    app.add_middleware(
        CORSMiddleware,
        allow_origins=settings.cors_origins,
        allow_credentials=False,
        allow_methods=["*"],
        allow_headers=["*"],
        allow_origin_regex=settings.cors_origin_regex,
    )

    # Routers
    app.include_router(auth_router, prefix="/api/v1")
    app.include_router(user_router, prefix="/api/v1")

    @app.get("/")
    async def root() -> dict[str, str]:
        return {"name": "Backend API", "version": "1.0.0"}

    @app.get("/api/v1/health")
    async def health() -> dict[str, str]:
        return {"status": "healthy"}

    return app


app = create_application()
