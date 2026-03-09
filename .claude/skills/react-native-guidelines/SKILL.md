---
name: react-native-guidelines
description: React Native (Expo SDK 52) 개발 가이드라인. Expo Router v4 파일 기반 라우팅, Zustand 인증 상태 관리, Axios 401 자동 갱신 인터셉터, SecureStore 토큰 관리, selector 훅 패턴. React Native 화면/컴포넌트/인증/네비게이션 작업 시 사용.
triggers:
  - react native
  - expo
  - expo router
  - zustand
  - secure store
  - react-native
  - expo sdk
  - 모바일 앱 개발
  - expo 앱
  - jwt 인터셉터
  - 401 refresh
  - 토큰 갱신
  - expo-secure-store
  - expo router layout
---

# React Native (Expo) 개발 가이드라인

## 기술 스택

| 구분 | 기술 | 버전 |
|---|---|---|
| Runtime | Expo SDK | ~52.0.0 |
| Language | TypeScript | strict |
| Router | Expo Router | v4 (파일 기반) |
| State | Zustand | v5 |
| HTTP | Axios | JWT 자동 갱신 |
| Storage | expo-secure-store | 토큰 전용 |
| Test | jest-expo | 단위 테스트 |

---

## 패턴 1 — Expo Router v4 Route Groups + 인증 가드

Expo Router v4는 파일 시스템이 곧 라우터. `(auth)` / `(tabs)` 그룹으로 인증 분기.

### 디렉토리 구조

```
app/
├── _layout.tsx          # Root: Stack + SplashScreen 제어
├── (auth)/
│   ├── _layout.tsx      # 인증 완료 시 → (tabs) redirect
│   ├── login.tsx
│   └── register.tsx
└── (tabs)/
    ├── _layout.tsx      # 미인증 시 → login redirect
    ├── index.tsx
    └── profile.tsx
```

### Root layout — SplashScreen 제어

```tsx
// app/_layout.tsx
import { useEffect } from 'react';
import { Stack } from 'expo-router';
import * as SplashScreen from 'expo-splash-screen';
import { useAuthStore } from '@/store/authStore';
import { LoadingScreen } from '@/components/ui/LoadingScreen';

SplashScreen.preventAutoHideAsync();

export default function RootLayout() {
  const initialize = useAuthStore((s) => s.initialize);
  const isLoading = useAuthStore((s) => s.isLoading);

  useEffect(() => {
    initialize().finally(() => SplashScreen.hideAsync());
  }, []);

  if (isLoading) return <LoadingScreen />;

  return (
    <Stack screenOptions={{ headerShown: false }}>
      <Stack.Screen name="(auth)" />
      <Stack.Screen name="(tabs)" />
    </Stack>
  );
}
```

### 인증 가드 — _layout.tsx에서 Redirect

```tsx
// app/(tabs)/_layout.tsx — 미인증 시 login redirect
import { Redirect, Tabs } from 'expo-router';
import { useAuthStore } from '@/store/authStore';
import { COLORS } from '@/constants';

export default function TabsLayout() {
  const isAuthenticated = useAuthStore((s) => s.isAuthenticated);

  if (!isAuthenticated) {
    return <Redirect href="/(auth)/login" />;
  }

  return (
    <Tabs screenOptions={{ tabBarActiveTintColor: COLORS.primary }}>
      <Tabs.Screen name="index" options={{ title: '홈' }} />
      <Tabs.Screen name="profile" options={{ title: '프로필' }} />
    </Tabs>
  );
}

// app/(auth)/_layout.tsx — 인증 완료 시 tabs redirect
export default function AuthLayout() {
  const isAuthenticated = useAuthStore((s) => s.isAuthenticated);

  if (isAuthenticated) {
    return <Redirect href="/(tabs)" />;
  }

  return <Stack screenOptions={{ headerShown: false }} />;
}
```

---

## 패턴 2 — Zustand v5 인증 스토어

`initialize()` → SplashScreen 연동. `isLoading` 플래그로 초기화 완료 전 UI 차단.

```ts
// store/authStore.ts
import { create } from 'zustand';
import { api } from '@/lib/api';
import { saveTokens, getTokens, clearTokens } from '@/lib/secureStore';
import type { User, AuthTokens, LoginRequest, RegisterRequest } from '@/types';

interface AuthState {
  user: User | null;
  accessToken: string | null;
  refreshToken: string | null;
  isAuthenticated: boolean;
  isLoading: boolean;        // SplashScreen 제어용

  initialize: () => Promise<void>;
  login: (data: LoginRequest) => Promise<void>;
  register: (data: RegisterRequest) => Promise<void>;
  logout: () => Promise<void>;
  setTokens: (tokens: AuthTokens) => Promise<void>;
  clearAuth: () => Promise<void>;
}

export const useAuthStore = create<AuthState>((set, get) => ({
  user: null,
  accessToken: null,
  refreshToken: null,
  isAuthenticated: false,
  isLoading: true,

  // 앱 시작 시 1회 호출: SecureStore → GET /auth/me → 상태 복원
  initialize: async () => {
    try {
      const { accessToken, refreshToken } = await getTokens();
      if (!accessToken || !refreshToken) {
        set({ isLoading: false });
        return;
      }
      // request interceptor도 getTokens()를 비동기로 읽으므로
      // initialize 첫 호출 시점에는 명시적 헤더 주입이 더 안전
      const { data: user } = await api.get<User>('/auth/me', {
        headers: { Authorization: `Bearer ${accessToken}` },
      });
      set({ user, accessToken, refreshToken, isAuthenticated: true, isLoading: false });
    } catch {
      await clearTokens();
      set({ isLoading: false });
    }
  },

  login: async (data: LoginRequest) => {
    const { data: res } = await api.post<{ user: User } & AuthTokens>('/auth/login', data);
    await get().setTokens({ access_token: res.access_token, refresh_token: res.refresh_token });
    set({ user: res.user, isAuthenticated: true });
  },

  register: async (data: RegisterRequest) => {
    const { data: res } = await api.post<{ user: User } & AuthTokens>('/auth/register', data);
    await get().setTokens({ access_token: res.access_token, refresh_token: res.refresh_token });
    set({ user: res.user, isAuthenticated: true });
  },

  logout: async () => {
    try { await api.post('/auth/logout'); } catch { /* 서버 오류 무시 */ }
    await get().clearAuth();
  },

  setTokens: async ({ access_token, refresh_token }: AuthTokens) => {
    await saveTokens(access_token, refresh_token);
    set({ accessToken: access_token, refreshToken: refresh_token });
  },

  clearAuth: async () => {
    await clearTokens();
    set({ user: null, accessToken: null, refreshToken: null, isAuthenticated: false });
  },
}));
```

---

## 패턴 3 — Axios 401 Queue 인터셉터

401 발생 시 refresh를 1회만 실행. 대기 중인 요청은 Queue에 쌓아 토큰 갱신 후 일괄 재시도.

```ts
// lib/api.ts
import axios, { AxiosError, InternalAxiosRequestConfig } from 'axios';
import { getTokens, saveTokens, clearTokens } from './secureStore';

// EXPO_PUBLIC_* 변수는 constants/index.ts에서 중앙 관리 (process.env.EXPO_PUBLIC_API_URL ?? '')
import { API_BASE_URL } from '@/constants';

export const api = axios.create({
  baseURL: API_BASE_URL,
  timeout: 10_000,
  headers: { 'Content-Type': 'application/json' },
});

// 요청 인터셉터 — Bearer 토큰 자동 첨부
api.interceptors.request.use(
  async (config) => {
    const { accessToken } = await getTokens();
    if (accessToken) config.headers.Authorization = `Bearer ${accessToken}`;
    return config;
  },
  (error) => Promise.reject(error),
);

// Queue 타입
type FailedQueueItem = {
  resolve: (token: string) => void;
  reject: (error: AxiosError) => void;
};

let isRefreshing = false;
let failedQueue: FailedQueueItem[] = [];

function processQueue(error: AxiosError | null, token: string | null): void {
  failedQueue.forEach(({ resolve, reject }) =>
    error ? reject(error) : resolve(token!)
  );
  failedQueue = [];
}

// 응답 인터셉터 — 401 → refresh → 재시도
api.interceptors.response.use(
  (response) => response,
  async (error: AxiosError) => {
    const original = error.config as InternalAxiosRequestConfig & { _retry?: boolean };

    if (error.response?.status !== 401 || original._retry) {
      return Promise.reject(error);
    }

    // refresh 진행 중이면 queue 대기
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

      // api 인스턴스 대신 axios 직접 사용 — 인터셉터 재진입(무한 루프) 방지
      const { data } = await axios.post<{ access_token: string; refresh_token: string }>(
        `${API_BASE_URL}/auth/refresh`,
        { refresh_token: refreshToken },
      );

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
```

---

## 패턴 4 — SecureStore 토큰 관리

`AsyncStorage` 대신 `expo-secure-store` 사용. Promise.all로 병렬 읽기/쓰기.

```ts
// lib/secureStore.ts
import * as SecureStore from 'expo-secure-store';

const KEYS = {
  ACCESS_TOKEN: 'access_token',
  REFRESH_TOKEN: 'refresh_token',
} as const;

export async function saveTokens(accessToken: string, refreshToken: string) {
  await Promise.all([
    SecureStore.setItemAsync(KEYS.ACCESS_TOKEN, accessToken),
    SecureStore.setItemAsync(KEYS.REFRESH_TOKEN, refreshToken),
  ]);
}

export async function getTokens(): Promise<{
  accessToken: string | null;
  refreshToken: string | null;
}> {
  const [accessToken, refreshToken] = await Promise.all([
    SecureStore.getItemAsync(KEYS.ACCESS_TOKEN),
    SecureStore.getItemAsync(KEYS.REFRESH_TOKEN),
  ]);
  return { accessToken, refreshToken };
}

export async function clearTokens() {
  await Promise.all([
    SecureStore.deleteItemAsync(KEYS.ACCESS_TOKEN),
    SecureStore.deleteItemAsync(KEYS.REFRESH_TOKEN),
  ]);
}
```

---

## 패턴 5 — useAuth() Selector 훅

컴포넌트에서 `useAuthStore` 직접 구독 대신 selector 훅으로 리렌더링 범위 최소화.

```ts
// hooks/useAuth.ts
import { useAuthStore } from '@/store/authStore';

export function useAuth() {
  const user = useAuthStore((s) => s.user);
  const isAuthenticated = useAuthStore((s) => s.isAuthenticated);
  const isLoading = useAuthStore((s) => s.isLoading);
  const login = useAuthStore((s) => s.login);
  const logout = useAuthStore((s) => s.logout);
  const register = useAuthStore((s) => s.register);

  return { user, isAuthenticated, isLoading, login, logout, register };
}

// 사용 예시
function ProfileScreen() {
  const { user, logout } = useAuth();
  return <Text>{user?.email}</Text>;
}
```

---

## 공통 타입 정의

```ts
// types/index.ts
export interface User {
  id: number;
  email: string;
  username: string;
  created_at: string;
}

export interface AuthTokens {
  access_token: string;
  refresh_token: string;
}

export interface LoginRequest {
  email: string;
  password: string;
}

export interface RegisterRequest {
  email: string;
  username: string;
  password: string;
}
```

---

## 백엔드 API 스펙

| Method | Path | 설명 | 응답 |
|---|---|---|---|
| POST | /auth/login | 로그인 | `{ user, access_token, refresh_token }` |
| POST | /auth/register | 회원가입 | `{ user, access_token, refresh_token }` |
| GET | /auth/me | 현재 사용자 | `User` |
| POST | /auth/refresh | 토큰 갱신 | `{ access_token, refresh_token }` |
| POST | /auth/logout | 로그아웃 | — |

`base/fastapi/`, `base/django/`, `base/express/`, `base/nestjs/` 와 연동 가능.

---

## 안티패턴

### ❌ AsyncStorage로 토큰 저장

```ts
// 잘못된 예 — 토큰이 평문 저장됨 (탈취 위험)
import AsyncStorage from '@react-native-async-storage/async-storage';
await AsyncStorage.setItem('token', accessToken);
```

```ts
// 올바른 예 — 암호화 저장소 사용
import * as SecureStore from 'expo-secure-store';
await SecureStore.setItemAsync('access_token', accessToken);
```

### ❌ Zustand store 전체 구독

```ts
// 잘못된 예 — state 변경 시 항상 리렌더링
const authStore = useAuthStore();
```

```ts
// 올바른 예 — 필요한 필드만 selector로 구독
const user = useAuthStore((s) => s.user);
const isAuthenticated = useAuthStore((s) => s.isAuthenticated);
```

### ❌ _retry 플래그 없이 401 재시도

```ts
// 잘못된 예 — refresh 실패 시에도 무한 루프
if (error.response?.status === 401) {
  await refreshToken();
  return api(error.config);  // 또 401이면 무한 반복
}
```

```ts
// 올바른 예 — _retry 플래그로 1회 제한
if (error.response?.status !== 401 || original._retry) {
  return Promise.reject(error);
}
original._retry = true;
```

### ❌ 컴포넌트에서 직접 API 호출

```ts
// 잘못된 예 — 인증 상태와 분리된 API 호출
function LoginScreen() {
  const handleLogin = async () => {
    const res = await axios.post('/auth/login', credentials);
    // 토큰 저장을 직접 처리하는 로직...
  };
}
```

```ts
// 올바른 예 — store 액션을 통해 인증 처리
function LoginScreen() {
  const { login } = useAuth();
  const handleLogin = async () => {
    await login({ email, password });
    // 토큰 저장 + 상태 갱신은 store가 처리
  };
}
```

### ❌ navigate 직접 호출로 인증 분기

```ts
// 잘못된 예 — 비동기 타이밍 이슈 발생 가능
useEffect(() => {
  if (!isAuthenticated) router.push('/login');
}, [isAuthenticated]);
```

```tsx
// 올바른 예 — _layout.tsx에서 Redirect 컴포넌트로 선언적 처리
if (!isAuthenticated) {
  return <Redirect href="/(auth)/login" />;
}
```
