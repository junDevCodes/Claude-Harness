import { create } from 'zustand';
import { api } from '@/lib/api';
import { saveTokens, getTokens, clearTokens } from '@/lib/secureStore';
import type { User, AuthTokens, LoginRequest, RegisterRequest } from '@/types';

// ─────────────────────────────────────────
// State shape
// ─────────────────────────────────────────

interface AuthState {
  user: User | null;
  accessToken: string | null;
  refreshToken: string | null;
  isAuthenticated: boolean;
  /** 앱 초기 토큰 검증 중 true — SplashScreen 제어에 사용 */
  isLoading: boolean;

  initialize: () => Promise<void>;
  login: (data: LoginRequest) => Promise<void>;
  register: (data: RegisterRequest) => Promise<void>;
  logout: () => Promise<void>;
  setTokens: (tokens: AuthTokens) => Promise<void>;
  clearAuth: () => Promise<void>;
}

// ─────────────────────────────────────────
// Store
// ─────────────────────────────────────────

export const useAuthStore = create<AuthState>((set, get) => ({
  user: null,
  accessToken: null,
  refreshToken: null,
  isAuthenticated: false,
  isLoading: true,

  /**
   * 앱 시작 시 한 번 호출.
   * SecureStore → 토큰 읽기 → GET /auth/me 검증 → store 세팅
   */
  initialize: async () => {
    try {
      const { accessToken, refreshToken } = await getTokens();
      if (!accessToken || !refreshToken) {
        set({ isLoading: false });
        return;
      }

      const { data: user } = await api.get<User>('/auth/me', {
        headers: { Authorization: `Bearer ${accessToken}` },
      });

      set({
        user,
        accessToken,
        refreshToken,
        isAuthenticated: true,
        isLoading: false,
      });
    } catch {
      // 토큰 만료 또는 서버 오류 — 인증 해제
      await clearTokens();
      set({ isLoading: false });
    }
  },

  login: async (data: LoginRequest) => {
    const { data: res } = await api.post<{ user: User } & AuthTokens>(
      '/auth/login',
      data,
    );
    await get().setTokens({
      access_token: res.access_token,
      refresh_token: res.refresh_token,
    });
    set({ user: res.user, isAuthenticated: true });
  },

  register: async (data: RegisterRequest) => {
    const { data: res } = await api.post<{ user: User } & AuthTokens>(
      '/auth/register',
      data,
    );
    await get().setTokens({
      access_token: res.access_token,
      refresh_token: res.refresh_token,
    });
    set({ user: res.user, isAuthenticated: true });
  },

  logout: async () => {
    try {
      await api.post('/auth/logout');
    } catch {
      // 서버 오류 무시 — 로컬 상태는 반드시 클리어
    }
    await get().clearAuth();
  },

  setTokens: async ({ access_token, refresh_token }: AuthTokens) => {
    await saveTokens(access_token, refresh_token);
    set({ accessToken: access_token, refreshToken: refresh_token });
  },

  clearAuth: async () => {
    await clearTokens();
    set({
      user: null,
      accessToken: null,
      refreshToken: null,
      isAuthenticated: false,
    });
  },
}));
