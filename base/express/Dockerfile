# ── Stage 1: Dependencies ─────────────────────────────
FROM node:20-alpine AS deps

WORKDIR /app

# pnpm 설치 (선택적, npm도 사용 가능)
RUN npm install -g pnpm

COPY package.json pnpm-lock.yaml* package-lock.json* ./
RUN pnpm install --frozen-lockfile 2>/dev/null || npm ci

# ── Stage 2: Builder ──────────────────────────────────
FROM node:20-alpine AS builder

WORKDIR /app

COPY --from=deps /app/node_modules ./node_modules
COPY . .

# Prisma Client 생성
RUN npx prisma generate

# TypeScript 빌드
RUN npm run build

# ── Stage 3: Runner ───────────────────────────────────
FROM node:20-alpine AS runner

WORKDIR /app

ENV NODE_ENV=production

# 프로덕션 의존성만 설치
COPY package.json package-lock.json* ./
RUN npm ci --omit=dev

# Prisma Client
COPY --from=builder /app/node_modules/.prisma ./node_modules/.prisma
COPY --from=builder /app/node_modules/@prisma ./node_modules/@prisma

# 빌드 결과물
COPY --from=builder /app/dist ./dist

# Prisma 마이그레이션 파일
COPY prisma ./prisma

# 보안: 루트 사용자 대신 node 사용
USER node

EXPOSE 3000

CMD ["node", "dist/server.js"]
