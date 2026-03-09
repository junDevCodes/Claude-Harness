import axios, { AxiosError, InternalAxiosRequestConfig } from 'axios';
import { API_BASE_URL } from '@/constants';
import { getTokens, saveTokens, clearTokens } from './secureStore';

// ─────────────────────────────────────────
// Axios instance
// ─────────────────────────────────────────

export const api = axios.create({
  baseURL: API_BASE_URL,
  timeout: 10_000,
  headers: { 'Content-Type': 'application/json' },
});

// ─────────────────────────────────────────
// Request interceptor — Bearer 토큰 첨부
// ─────────────────────────────────────────

api.interceptors.request.use(
  async (config: InternalAxiosRequestConfig) => {
    const { accessToken } = await getTokens();
    if (accessToken) {
      config.headers.Authorization = `Bearer ${accessToken}`;
    }
    return config;
  },
  (error) => Promise.reject(error),
);

// ─────────────────────────────────────────
// Response interceptor — 401 → refresh → 재시도
// Queue 패턴: refresh 중 들어온 요청을 대기열에 쌓아 1회만 갱신
// ─────────────────────────────────────────

type FailedQueueItem = {
  resolve: (token: string) => void;
  reject: (error: AxiosError) => void;
};

let isRefreshing = false;
let failedQueue: FailedQueueItem[] = [];

function processQueue(error: AxiosError | null, token: string | null): void {
  failedQueue.forEach(({ resolve, reject }) => {
    if (error) reject(error);
    else resolve(token!);
  });
  failedQueue = [];
}

api.interceptors.response.use(
  (response) => response,
  async (error: AxiosError) => {
    const original = error.config as InternalAxiosRequestConfig & {
      _retry?: boolean;
    };

    // 401이 아니거나 이미 retry한 요청이면 그대로 reject
    if (error.response?.status !== 401 || original._retry) {
      return Promise.reject(error);
    }

    // refresh 중이면 queue에 추가 후 대기
    if (isRefreshing) {
      return new Promise<string>((resolve, reject) => {
        failedQueue.push({ resolve, reject });
      }).then((token) => {
        original.headers.Authorization = `Bearer ${token}`;
        return api(original);
      });
    }

    original._retry = true;
    isRefreshing = true;

    try {
      const { refreshToken } = await getTokens();
      if (!refreshToken) throw new Error('No refresh token');

      const { data } = await axios.post<{
        access_token: string;
        refresh_token: string;
      }>(`${API_BASE_URL}/auth/refresh`, { refresh_token: refreshToken });

      await saveTokens(data.access_token, data.refresh_token);
      api.defaults.headers.common.Authorization = `Bearer ${data.access_token}`;

      processQueue(null, data.access_token);
      original.headers.Authorization = `Bearer ${data.access_token}`;
      return api(original);
    } catch (refreshError) {
      await clearTokens();
      processQueue(refreshError as AxiosError, null);
      return Promise.reject(refreshError);
    } finally {
      isRefreshing = false;
    }
  },
);
