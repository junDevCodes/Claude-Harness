/**
 * authStore 단위 테스트
 *
 * 실행: npx jest tests/authStore.test.ts
 */

// expo-secure-store mock
jest.mock('expo-secure-store', () => ({
  setItemAsync: jest.fn().mockResolvedValue(undefined),
  getItemAsync: jest.fn().mockResolvedValue(null),
  deleteItemAsync: jest.fn().mockResolvedValue(undefined),
}));

// expo-constants mock
jest.mock('expo-constants', () => ({
  default: {
    expoConfig: {
      extra: { apiUrl: 'http://localhost:8000/api/v1' },
    },
  },
}));

// axios mock
jest.mock('../lib/api', () => ({
  api: {
    get: jest.fn(),
    post: jest.fn(),
    defaults: { headers: { common: {} } },
  },
}));

import { useAuthStore } from '../store/authStore';
import { api } from '../lib/api';
import * as SecureStore from 'expo-secure-store';

const mockedApi = api as jest.Mocked<typeof api>;

// ─────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────

function resetStore() {
  useAuthStore.setState({
    user: null,
    accessToken: null,
    refreshToken: null,
    isAuthenticated: false,
    isLoading: false,
  });
}

// ─────────────────────────────────────────
// Tests
// ─────────────────────────────────────────

describe('useAuthStore — 초기 상태', () => {
  beforeEach(resetStore);

  it('초기 상태는 미인증이어야 한다', () => {
    const { isAuthenticated, user, accessToken } = useAuthStore.getState();
    expect(isAuthenticated).toBe(false);
    expect(user).toBeNull();
    expect(accessToken).toBeNull();
  });
});

describe('useAuthStore — clearAuth', () => {
  beforeEach(resetStore);

  it('clearAuth 호출 시 상태가 초기화되어야 한다', async () => {
    useAuthStore.setState({
      user: { id: 1, email: 'test@example.com', username: 'tester', created_at: '2024-01-01' },
      accessToken: 'access_123',
      refreshToken: 'refresh_456',
      isAuthenticated: true,
    });

    await useAuthStore.getState().clearAuth();

    const { user, accessToken, refreshToken, isAuthenticated } =
      useAuthStore.getState();
    expect(user).toBeNull();
    expect(accessToken).toBeNull();
    expect(refreshToken).toBeNull();
    expect(isAuthenticated).toBe(false);
  });

  it('clearAuth 호출 시 SecureStore 토큰이 삭제되어야 한다', async () => {
    await useAuthStore.getState().clearAuth();
    expect(SecureStore.deleteItemAsync).toHaveBeenCalledWith('access_token');
    expect(SecureStore.deleteItemAsync).toHaveBeenCalledWith('refresh_token');
  });
});

describe('useAuthStore — initialize (토큰 없음)', () => {
  beforeEach(() => {
    resetStore();
    useAuthStore.setState({ isLoading: true });
    (SecureStore.getItemAsync as jest.Mock).mockResolvedValue(null);
  });

  it('SecureStore에 토큰 없으면 미인증 상태로 종료', async () => {
    await useAuthStore.getState().initialize();

    const { isAuthenticated, isLoading } = useAuthStore.getState();
    expect(isAuthenticated).toBe(false);
    expect(isLoading).toBe(false);
  });
});

describe('useAuthStore — initialize (토큰 있음, 유효)', () => {
  const mockUser = {
    id: 1,
    email: 'test@example.com',
    username: 'tester',
    created_at: '2024-01-01',
  };

  beforeEach(() => {
    resetStore();
    useAuthStore.setState({ isLoading: true });
    (SecureStore.getItemAsync as jest.Mock)
      .mockResolvedValueOnce('access_token_value')
      .mockResolvedValueOnce('refresh_token_value');
    mockedApi.get = jest.fn().mockResolvedValue({ data: mockUser });
  });

  it('유효한 토큰으로 initialize 시 인증 상태가 되어야 한다', async () => {
    await useAuthStore.getState().initialize();

    const { isAuthenticated, user, isLoading } = useAuthStore.getState();
    expect(isAuthenticated).toBe(true);
    expect(user).toEqual(mockUser);
    expect(isLoading).toBe(false);
  });
});

describe('useAuthStore — login', () => {
  const mockUser = {
    id: 2,
    email: 'user@example.com',
    username: 'newuser',
    created_at: '2024-06-01',
  };

  beforeEach(() => {
    resetStore();
    mockedApi.post = jest.fn().mockResolvedValue({
      data: {
        user: mockUser,
        access_token: 'new_access',
        refresh_token: 'new_refresh',
      },
    });
  });

  it('login 성공 시 isAuthenticated가 true가 되어야 한다', async () => {
    await useAuthStore.getState().login({ email: 'user@example.com', password: 'pass1234' });

    const { isAuthenticated, user } = useAuthStore.getState();
    expect(isAuthenticated).toBe(true);
    expect(user?.email).toBe('user@example.com');
  });

  it('login 성공 시 SecureStore에 토큰이 저장되어야 한다', async () => {
    await useAuthStore.getState().login({ email: 'user@example.com', password: 'pass1234' });

    expect(SecureStore.setItemAsync).toHaveBeenCalledWith('access_token', 'new_access');
    expect(SecureStore.setItemAsync).toHaveBeenCalledWith('refresh_token', 'new_refresh');
  });
});
