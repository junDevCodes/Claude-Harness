/**
 * Auth Integration Tests
 *
 * 전제 조건:
 * - TEST_DATABASE_URL 환경변수 설정 (또는 DATABASE_URL을 테스트 DB로 지정)
 * - `npx prisma migrate deploy` 실행 후 테스트 실행
 *
 * 실행: npx jest --runInBand
 */
import request from 'supertest';
import { Express } from 'express';
import { buildTestApp, cleanDatabase, disconnectDatabase } from './helpers/test.app';

// 테스트 환경 변수 설정
process.env.NODE_ENV = 'test';
process.env.DATABASE_URL = process.env.TEST_DATABASE_URL ?? process.env.DATABASE_URL ?? '';
process.env.JWT_ACCESS_SECRET = 'test-access-secret-must-be-at-least-32-chars!!';
process.env.JWT_REFRESH_SECRET = 'test-refresh-secret-must-be-at-least-32-chars!!';
process.env.JWT_ACCESS_EXPIRES_IN = '15m';
process.env.JWT_REFRESH_EXPIRES_IN = '30d';

describe('Auth API', () => {
  let app: Express;

  beforeAll(async () => {
    app = buildTestApp();
    await cleanDatabase();
  });

  afterAll(async () => {
    await cleanDatabase();
    await disconnectDatabase();
  });

  const testUser = {
    email: 'test@example.com',
    username: 'testuser',
    password: 'Password123!',
  };

  describe('POST /api/v1/auth/sign-up', () => {
    it('should register a new user and return tokens', async () => {
      const res = await request(app).post('/api/v1/auth/sign-up').send(testUser);

      expect(res.status).toBe(201);
      expect(res.body.user).toMatchObject({
        email: testUser.email,
        username: testUser.username,
      });
      expect(res.body.tokens.accessToken).toBeDefined();
      expect(res.body.tokens.refreshToken).toBeDefined();
      // 비밀번호 해시가 응답에 포함되지 않아야 함
      expect(res.body.user.passwordHash).toBeUndefined();
    });

    it('should return 409 if email already exists', async () => {
      const res = await request(app).post('/api/v1/auth/sign-up').send(testUser);

      expect(res.status).toBe(409);
      expect(res.body.error.code).toBe('EMAIL_ALREADY_EXISTS');
    });

    it('should return 400 for invalid email', async () => {
      const res = await request(app)
        .post('/api/v1/auth/sign-up')
        .send({ ...testUser, email: 'not-an-email' });

      expect(res.status).toBe(400);
      expect(res.body.error.code).toBe('VALIDATION_ERROR');
    });

    it('should return 400 for short password', async () => {
      const res = await request(app)
        .post('/api/v1/auth/sign-up')
        .send({ ...testUser, email: 'new@example.com', password: 'short' });

      expect(res.status).toBe(400);
    });
  });

  describe('POST /api/v1/auth/login', () => {
    it('should login with valid credentials', async () => {
      const res = await request(app).post('/api/v1/auth/login').send({
        email: testUser.email,
        password: testUser.password,
      });

      expect(res.status).toBe(200);
      expect(res.body.tokens.accessToken).toBeDefined();
      expect(res.body.tokens.refreshToken).toBeDefined();
    });

    it('should return 401 for wrong password', async () => {
      const res = await request(app).post('/api/v1/auth/login').send({
        email: testUser.email,
        password: 'WrongPassword!',
      });

      expect(res.status).toBe(401);
      expect(res.body.error.code).toBe('INVALID_CREDENTIALS');
    });

    it('should return 401 for non-existent email', async () => {
      const res = await request(app).post('/api/v1/auth/login').send({
        email: 'nobody@example.com',
        password: testUser.password,
      });

      expect(res.status).toBe(401);
    });
  });

  describe('GET /api/v1/auth/me', () => {
    let accessToken: string;

    beforeAll(async () => {
      const res = await request(app).post('/api/v1/auth/login').send({
        email: testUser.email,
        password: testUser.password,
      });
      accessToken = res.body.tokens.accessToken;
    });

    it('should return current user info', async () => {
      const res = await request(app)
        .get('/api/v1/auth/me')
        .set('Authorization', `Bearer ${accessToken}`);

      expect(res.status).toBe(200);
      expect(res.body.user.email).toBe(testUser.email);
    });

    it('should return 401 without token', async () => {
      const res = await request(app).get('/api/v1/auth/me');

      expect(res.status).toBe(401);
    });

    it('should return 401 with invalid token', async () => {
      const res = await request(app)
        .get('/api/v1/auth/me')
        .set('Authorization', 'Bearer invalid.token.here');

      expect(res.status).toBe(401);
    });
  });

  describe('POST /api/v1/auth/refresh', () => {
    let refreshToken: string;

    beforeAll(async () => {
      const res = await request(app).post('/api/v1/auth/login').send({
        email: testUser.email,
        password: testUser.password,
      });
      refreshToken = res.body.tokens.refreshToken;
    });

    it('should issue new access token', async () => {
      const res = await request(app)
        .post('/api/v1/auth/refresh')
        .send({ refreshToken });

      expect(res.status).toBe(200);
      expect(res.body.accessToken).toBeDefined();
      expect(res.body.refreshToken).toBeDefined();
    });

    it('should return 401 for invalid refresh token', async () => {
      const res = await request(app)
        .post('/api/v1/auth/refresh')
        .send({ refreshToken: 'invalid-token' });

      expect(res.status).toBe(401);
    });
  });

  describe('DELETE /api/v1/auth/logout', () => {
    it('should logout and return 204', async () => {
      const loginRes = await request(app).post('/api/v1/auth/login').send({
        email: testUser.email,
        password: testUser.password,
      });
      const { accessToken } = loginRes.body.tokens;

      const res = await request(app)
        .delete('/api/v1/auth/logout')
        .set('Authorization', `Bearer ${accessToken}`);

      expect(res.status).toBe(204);
    });
  });

  describe('GET /health', () => {
    it('should return 200 OK', async () => {
      const res = await request(app).get('/health');
      expect(res.status).toBe(200);
      expect(res.body.status).toBe('ok');
    });
  });
});
