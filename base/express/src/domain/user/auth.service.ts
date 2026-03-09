import { ulid } from 'ulidx';
import { UserRepository, CreateUserInput } from './user.repository';
import { SafeUser } from './user.model';
import { hashPassword, comparePassword } from '../../utils/password';
import { signAccessToken, signRefreshToken, verifyRefreshToken } from '../../utils/jwt';
import { AppError } from '../../errors/app.error';
import { config } from '../../config/config';

export interface SignUpInput {
  email: string;
  username: string;
  password: string;
}

export interface LoginInput {
  email: string;
  password: string;
}

export interface AuthTokens {
  accessToken: string;
  refreshToken: string;
}

export interface AuthResult {
  user: SafeUser;
  tokens: AuthTokens;
}

// 만료 기간 문자열 → ms 변환
function parseDurationMs(duration: string): number {
  const units: Record<string, number> = {
    m: 60 * 1000,
    h: 60 * 60 * 1000,
    d: 24 * 60 * 60 * 1000,
  };
  const match = duration.match(/^(\d+)([mhd])$/);
  if (!match) return 30 * 24 * 60 * 60 * 1000; // 기본 30일
  return parseInt(match[1]) * units[match[2]];
}

export class AuthService {
  constructor(private readonly userRepo: UserRepository) {}

  async signUp(input: SignUpInput): Promise<AuthResult> {
    const existingEmail = await this.userRepo.findByEmail(input.email);
    if (existingEmail) {
      throw AppError.conflict('Email already exists', 'EMAIL_ALREADY_EXISTS');
    }

    const existingUsername = await this.userRepo.findByUsername(input.username);
    if (existingUsername) {
      throw AppError.conflict('Username already exists', 'USERNAME_ALREADY_EXISTS');
    }

    const passwordHash = await hashPassword(input.password);
    const user = await this.userRepo.create({
      email: input.email,
      username: input.username,
      passwordHash,
    });

    const tokens = await this.createTokens(user);
    return { user, tokens };
  }

  async login(input: LoginInput): Promise<AuthResult> {
    const user = await this.userRepo.findByEmail(input.email);
    if (!user) {
      throw new AppError('Invalid email or password', 401, 'INVALID_CREDENTIALS');
    }

    const isValid = await comparePassword(input.password, user.passwordHash);
    if (!isValid) {
      throw new AppError('Invalid email or password', 401, 'INVALID_CREDENTIALS');
    }

    const safeUser = await this.userRepo.findById(user.id);
    if (!safeUser) throw AppError.internal();

    const tokens = await this.createTokens(safeUser);
    return { user: safeUser, tokens };
  }

  async refresh(refreshToken: string): Promise<AuthTokens> {
    const payload = verifyRefreshToken(refreshToken);

    const storedToken = await this.userRepo.findRefreshToken(payload.tokenId);
    if (!storedToken || storedToken.revokedAt || storedToken.expiresAt < new Date()) {
      throw new AppError('Invalid or expired refresh token', 401, 'INVALID_TOKEN');
    }

    // Refresh Token Rotation: 기존 토큰 무효화 후 새 토큰 발급
    await this.userRepo.revokeRefreshToken(payload.tokenId);

    const user = await this.userRepo.findById(payload.sub);
    if (!user) throw AppError.notFound('User not found');

    return this.createTokens(user);
  }

  async logout(userId: string): Promise<void> {
    await this.userRepo.revokeAllUserRefreshTokens(userId);
  }

  private async createTokens(user: SafeUser): Promise<AuthTokens> {
    const tokenId = ulid();
    const expiresAt = new Date(Date.now() + parseDurationMs(config.JWT_REFRESH_EXPIRES_IN));

    const accessToken = signAccessToken({
      sub: user.id,
      email: user.email,
      role: user.role,
    });

    const refreshToken = signRefreshToken({ sub: user.id, tokenId });

    await this.userRepo.createRefreshToken({
      id: tokenId,
      token: refreshToken,
      userId: user.id,
      expiresAt,
    });

    return { accessToken, refreshToken };
  }
}
