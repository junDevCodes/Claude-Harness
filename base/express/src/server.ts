import { createApp } from './app';
import { config } from './config/config';
import { logger } from './utils/logger';
import { prisma } from './db/prisma';

async function bootstrap() {
  // DB 연결 확인
  await prisma.$connect();
  logger.info('Database connected');

  const app = createApp();

  app.listen(config.PORT, () => {
    logger.info(`Server running on http://localhost:${config.PORT}`);
    logger.info(`Environment: ${config.NODE_ENV}`);
  });

  // Graceful shutdown
  const shutdown = async (signal: string) => {
    logger.info(`${signal} received, shutting down gracefully...`);
    await prisma.$disconnect();
    process.exit(0);
  };

  process.on('SIGTERM', () => shutdown('SIGTERM'));
  process.on('SIGINT', () => shutdown('SIGINT'));
}

bootstrap().catch((err) => {
  console.error('Failed to start server:', err);
  process.exit(1);
});
