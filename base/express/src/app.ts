import express from 'express';
import cors from 'cors';
import helmet from 'helmet';
import cookieParser from 'cookie-parser';
import pinoHttp from 'pino-http';
import { config } from './config/config';
import { logger } from './utils/logger';
import v1Router from './api/v1/routes/index';
import { errorHandler, notFoundHandler } from './middleware/error.handler';

export function createApp() {
  const app = express();

  // ── Security ──────────────────────────────────────
  app.use(helmet());
  app.use(
    cors({
      origin: config.CORS_ORIGINS.split(',').map((o) => o.trim()),
      credentials: true,
    }),
  );

  // ── Request Parsing ───────────────────────────────
  app.use(express.json({ limit: '1mb' }));
  app.use(express.urlencoded({ extended: true }));
  app.use(cookieParser());

  // ── Logging ───────────────────────────────────────
  if (config.NODE_ENV !== 'test') {
    app.use(pinoHttp({ logger }));
  }

  // ── Health Check ──────────────────────────────────
  app.get('/health', (_req, res) => {
    res.status(200).json({ status: 'ok', timestamp: new Date().toISOString() });
  });

  // ── API Routes ────────────────────────────────────
  app.use('/api/v1', v1Router);

  // ── Error Handling ────────────────────────────────
  app.use(notFoundHandler);
  app.use(errorHandler);

  return app;
}
