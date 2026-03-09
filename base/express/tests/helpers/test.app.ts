import { createApp } from '../../src/app';
import { prisma } from '../../src/db/prisma';

export function buildTestApp() {
  return createApp();
}

export async function cleanDatabase() {
  await prisma.refreshToken.deleteMany();
  await prisma.user.deleteMany();
}

export async function disconnectDatabase() {
  await prisma.$disconnect();
}
