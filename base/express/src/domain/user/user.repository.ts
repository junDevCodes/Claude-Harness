import { prisma } from '../../db/prisma';
import { User, SafeUser, toSafeUser } from './user.model';
import {
  BaseRepository,
  PaginationOptions,
  PaginatedResult,
  buildPaginatedResult,
} from '../shared/base.repository';

export interface CreateUserInput {
  email: string;
  username: string;
  passwordHash: string;
}

export interface UpdateUserInput {
  username?: string;
  isActive?: boolean;
}

export class UserRepository
  implements BaseRepository<SafeUser, CreateUserInput, UpdateUserInput>
{
  async findById(id: string): Promise<SafeUser | null> {
    const user = await prisma.user.findUnique({ where: { id } });
    return user ? toSafeUser(user as User) : null;
  }

  async findByIdWithPassword(id: string): Promise<User | null> {
    return prisma.user.findUnique({ where: { id } }) as Promise<User | null>;
  }

  async findByEmail(email: string): Promise<User | null> {
    return prisma.user.findUnique({ where: { email } }) as Promise<User | null>;
  }

  async findByUsername(username: string): Promise<SafeUser | null> {
    const user = await prisma.user.findUnique({ where: { username } });
    return user ? toSafeUser(user as User) : null;
  }

  async findAll(options: PaginationOptions = {}): Promise<PaginatedResult<SafeUser>> {
    const page = Math.max(1, options.page ?? 1);
    const limit = Math.min(100, Math.max(1, options.limit ?? 20));
    const skip = (page - 1) * limit;

    const [users, total] = await prisma.$transaction([
      prisma.user.findMany({ skip, take: limit, orderBy: { createdAt: 'desc' } }),
      prisma.user.count(),
    ]);

    return buildPaginatedResult(
      users.map((u) => toSafeUser(u as User)),
      total,
      page,
      limit,
    );
  }

  async create(data: CreateUserInput): Promise<SafeUser> {
    const user = await prisma.user.create({ data });
    return toSafeUser(user as User);
  }

  async update(id: string, data: UpdateUserInput): Promise<SafeUser> {
    const user = await prisma.user.update({ where: { id }, data });
    return toSafeUser(user as User);
  }

  async delete(id: string): Promise<void> {
    await prisma.user.delete({ where: { id } });
  }

  // ── Refresh Token ──────────────────────────────────

  async createRefreshToken(data: {
    id: string;
    token: string;
    userId: string;
    expiresAt: Date;
  }): Promise<void> {
    await prisma.refreshToken.create({ data });
  }

  async findRefreshToken(tokenId: string) {
    return prisma.refreshToken.findUnique({ where: { id: tokenId } });
  }

  async revokeRefreshToken(tokenId: string): Promise<void> {
    await prisma.refreshToken.update({
      where: { id: tokenId },
      data: { revokedAt: new Date() },
    });
  }

  async revokeAllUserRefreshTokens(userId: string): Promise<void> {
    await prisma.refreshToken.updateMany({
      where: { userId, revokedAt: null },
      data: { revokedAt: new Date() },
    });
  }
}
