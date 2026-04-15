---
name: react-vite-pwa-guidelines
description: React 19 + Vite + TypeScript + PWA (Progressive Web App) 프론트엔드 가이드라인. Vite 빌드 설정, vite-plugin-pwa로 manifest/service worker 구성, 오프라인 캐싱 전략, WebSocket 클라이언트, App Router 없는 React Router v6 라우팅, Capacitor 변환 패턴. Next.js App Router를 사용하지 않는 React SPA/PWA 작업 시 사용.
triggers:
  - vite
  - Vite
  - vite.config
  - PWA
  - progressive web app
  - service worker
  - manifest.json
  - vite-plugin-pwa
  - workbox
  - react router
  - react-router-dom
  - capacitor
---

# React + Vite + PWA Frontend Guidelines

## Purpose

Next.js App Router를 사용하지 않는 React 19 + Vite + TypeScript SPA/PWA 프로젝트의 표준 패턴.
3D 시각화(Three.js), 실시간 WebSocket, 태블릿 PWA 설치 등 SPA 특성에 최적화된 빌드·라우팅·오프라인 전략을 제공한다.

## When to Use This Skill

- Vite 기반 React 프로젝트 신규 생성 또는 설정 변경
- PWA manifest + service worker 구성 (vite-plugin-pwa)
- React Router v6 기반 라우팅 (App Router 아님)
- WebSocket 클라이언트 컴포넌트 작성
- Capacitor를 통한 네이티브 앱 변환
- 오프라인 캐싱 전략 (이미지, API, 정적 자산)

## When NOT to Use This Skill

- Next.js App Router 프로젝트 → `nextjs-frontend-guidelines`
- React Native (Expo) 프로젝트 → `react-native-guidelines`
- Three.js / WebGL 렌더링 → `threejs-realtime-guidelines` (병행 사용 권장)

---

## Quick Start

### 신규 Vite + React 19 + TS 프로젝트

```bash
pnpm create vite@latest dashboard --template react-ts
cd dashboard
pnpm add -D vite-plugin-pwa workbox-window
pnpm add react-router-dom
```

### 최소 폴더 구조

```
src/
├── main.tsx              # ReactDOM 진입점 + Router
├── App.tsx               # 루트 컴포넌트
├── routes/               # 라우트 모듈
│   ├── index.tsx
│   └── dashboard.tsx
├── components/           # 재사용 컴포넌트
├── hooks/                # 커스텀 훅 (useWebSocket 등)
├── lib/                  # api 클라이언트, 유틸
└── pwa/                  # service worker 등록 코드
public/
├── icon-192.png
├── icon-512.png
└── apple-touch-icon.png  # iOS PWA
vite.config.ts
```

---

## Pattern 1: Vite 빌드 설정

### `vite.config.ts` 권장 템플릿

```typescript
import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';
import { VitePWA } from 'vite-plugin-pwa';
import path from 'path';

export default defineConfig({
  plugins: [
    react(),
    VitePWA({
      registerType: 'autoUpdate',
      manifest: {
        name: 'see-through Dashboard',
        short_name: 'SeeThrough',
        theme_color: '#000000',
        background_color: '#ffffff',
        display: 'standalone',
        orientation: 'landscape',
        icons: [
          { src: 'icon-192.png', sizes: '192x192', type: 'image/png' },
          { src: 'icon-512.png', sizes: '512x512', type: 'image/png' },
          {
            src: 'icon-512.png',
            sizes: '512x512',
            type: 'image/png',
            purpose: 'maskable',
          },
        ],
      },
      workbox: {
        globPatterns: ['**/*.{js,css,html,png,svg,woff2}'],
        runtimeCaching: [
          {
            urlPattern: ({ url }) => url.pathname.startsWith('/api/'),
            handler: 'NetworkFirst',
            options: {
              cacheName: 'api-cache',
              expiration: { maxEntries: 50, maxAgeSeconds: 60 * 5 },
            },
          },
        ],
      },
    }),
  ],
  resolve: {
    alias: { '@': path.resolve(__dirname, './src') },
  },
  server: {
    port: 3000,
    proxy: {
      '/api': { target: 'http://localhost:8080', changeOrigin: true },
      '/ws': { target: 'ws://localhost:8080', ws: true },
    },
  },
  build: {
    target: 'es2020',
    sourcemap: true,
    rollupOptions: {
      output: {
        manualChunks: {
          // 큰 라이브러리는 분리해 초기 로드 최적화
          'three-vendor': ['three'],
          'react-vendor': ['react', 'react-dom', 'react-router-dom'],
        },
      },
    },
  },
});
```

### 핵심 결정

- **manualChunks**: Three.js 같은 큰 라이브러리는 별도 청크로 분리해 초기 번들 크기 축소
- **server.proxy**: 개발 중 CORS 회피 + WebSocket 프록시 (`ws: true`)
- **registerType: 'autoUpdate'**: 새 빌드 자동 업데이트, 사용자에게 새로고침 유도 가능

---

## Pattern 2: PWA 등록 + 업데이트 알림

### `src/pwa/registerSW.ts`

```typescript
import { registerSW } from 'virtual:pwa-register';

export function setupPWA() {
  const updateSW = registerSW({
    onNeedRefresh() {
      // 새 버전 감지 시 사용자 확인
      if (confirm('새 버전이 있습니다. 업데이트할까요?')) {
        updateSW(true);
      }
    },
    onOfflineReady() {
      console.log('오프라인 모드 준비 완료');
    },
    onRegisterError(err) {
      console.error('SW 등록 실패', err);
    },
  });
}
```

### `src/main.tsx`

```typescript
import { setupPWA } from './pwa/registerSW';

setupPWA();
```

### `tsconfig.json`에 추가

```json
{
  "compilerOptions": {
    "types": ["vite/client", "vite-plugin-pwa/client"]
  }
}
```

---

## Pattern 3: React Router v6 라우팅

### `src/main.tsx`

```typescript
import { createBrowserRouter, RouterProvider } from 'react-router-dom';

const router = createBrowserRouter([
  {
    path: '/',
    element: <Layout />,
    errorElement: <ErrorPage />,
    children: [
      { index: true, element: <Home /> },
      { path: 'dashboard', element: <Dashboard /> },
      {
        path: 'sessions/:sessionId',
        element: <SessionDetail />,
        loader: sessionLoader,
      },
    ],
  },
]);

createRoot(document.getElementById('root')!).render(<RouterProvider router={router} />);
```

### 인증 가드 패턴 (Loader 활용)

```typescript
import { redirect } from 'react-router-dom';

export const protectedLoader = async ({ request }: LoaderFunctionArgs) => {
  const token = localStorage.getItem('access_token');
  if (!token) {
    const url = new URL(request.url);
    return redirect(`/login?from=${url.pathname}`);
  }
  return null;
};
```

---

## Pattern 4: WebSocket 클라이언트 훅

### `src/hooks/useWebSocket.ts`

```typescript
import { useEffect, useRef, useState, useCallback } from 'react';

type WSStatus = 'connecting' | 'open' | 'closed' | 'error';

export function useWebSocket<T = unknown>(url: string) {
  const [status, setStatus] = useState<WSStatus>('connecting');
  const [lastMessage, setLastMessage] = useState<T | null>(null);
  const wsRef = useRef<WebSocket | null>(null);
  const reconnectTimer = useRef<number | null>(null);

  const connect = useCallback(() => {
    setStatus('connecting');
    const ws = new WebSocket(url);
    wsRef.current = ws;

    ws.onopen = () => setStatus('open');
    ws.onmessage = (event) => {
      try {
        setLastMessage(JSON.parse(event.data));
      } catch {
        // 텍스트 메시지 그대로
        setLastMessage(event.data as T);
      }
    };
    ws.onerror = () => setStatus('error');
    ws.onclose = () => {
      setStatus('closed');
      // 5초 후 재연결
      reconnectTimer.current = window.setTimeout(connect, 5000);
    };
  }, [url]);

  useEffect(() => {
    connect();
    return () => {
      if (reconnectTimer.current) clearTimeout(reconnectTimer.current);
      wsRef.current?.close();
    };
  }, [connect]);

  const send = useCallback((data: unknown) => {
    if (wsRef.current?.readyState === WebSocket.OPEN) {
      wsRef.current.send(typeof data === 'string' ? data : JSON.stringify(data));
    }
  }, []);

  return { status, lastMessage, send };
}
```

### 사용 예시

```typescript
const { status, lastMessage } = useWebSocket<HeatmapFrame>('/ws/heatmap');

useEffect(() => {
  if (lastMessage) updateHeatmap(lastMessage);
}, [lastMessage]);
```

---

## Pattern 5: API 클라이언트 (JWT + 401 자동 재발급)

### `src/lib/api.ts`

```typescript
const BASE_URL = import.meta.env.VITE_API_BASE_URL ?? '/api';

class ApiClient {
  private getToken() {
    return localStorage.getItem('access_token');
  }

  private async request<T>(path: string, init: RequestInit = {}): Promise<T> {
    const token = this.getToken();
    const headers = new Headers(init.headers);
    headers.set('Content-Type', 'application/json');
    if (token) headers.set('Authorization', `Bearer ${token}`);

    const res = await fetch(`${BASE_URL}${path}`, { ...init, headers });

    if (res.status === 401) {
      const refreshed = await this.refresh();
      if (refreshed) return this.request<T>(path, init);
      window.location.href = '/login';
      throw new Error('Unauthorized');
    }

    if (!res.ok) throw new Error(`HTTP ${res.status}`);
    return res.json();
  }

  private async refresh(): Promise<boolean> {
    const refreshToken = localStorage.getItem('refresh_token');
    if (!refreshToken) return false;
    const res = await fetch(`${BASE_URL}/auth/refresh`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ refresh_token: refreshToken }),
    });
    if (!res.ok) return false;
    const data = await res.json();
    localStorage.setItem('access_token', data.access_token);
    return true;
  }

  get<T>(path: string) { return this.request<T>(path); }
  post<T>(path: string, body: unknown) {
    return this.request<T>(path, { method: 'POST', body: JSON.stringify(body) });
  }
}

export const api = new ApiClient();
```

---

## Pattern 6: Capacitor 네이티브 앱 변환 (선택)

```bash
pnpm add @capacitor/core @capacitor/cli @capacitor/android
npx cap init "see-through" "net.seethrough.app" --web-dir=dist
pnpm build
npx cap add android
npx cap sync android
npx cap open android
```

핵심:
- `webDir: dist` — Vite 빌드 출력
- `npx cap sync` — 빌드 후 매번 실행
- 카메라/Bluetooth 같은 네이티브 기능 필요 시 `@capacitor/camera` 등 추가

---

## Anti-Patterns (피해야 할 패턴)

### ❌ Next.js App Router 패턴 강제

```typescript
// ❌ Vite 프로젝트에 App Router 가져오기
import { useRouter } from 'next/navigation'; // 작동 안 함
'use client'; // Vite는 디렉티브 무시
```

→ React Router v6 `useNavigate` / `useLocation` 사용.

### ❌ Service Worker를 직접 작성

```typescript
// ❌ 직접 sw.js 작성 + 수동 캐시 관리
caches.open('v1').then(cache => cache.addAll([...]));
```

→ `vite-plugin-pwa` + Workbox 자동 생성에 맡긴다. 수동 작성은 캐시 무효화·업데이트 버그 다발.

### ❌ WebSocket을 컴포넌트에서 직접 생성

```typescript
// ❌ 컴포넌트가 마운트될 때마다 새 연결 생성
function Dashboard() {
  const ws = new WebSocket('/ws'); // 매 렌더마다 새 인스턴스
  // ...
}
```

→ `useWebSocket` 훅 또는 Context로 격리. 다중 마운트/언마운트 시 누수 방지.

### ❌ 전역 상태 라이브러리 무분별 도입

```typescript
// ❌ 단순 폼 상태에 Redux/Zustand 도입
```

→ React 19 `useActionState` + `useOptimistic`로 충분한 케이스가 많음. 진짜 필요할 때만 Zustand 도입.

---

## Trigger Keywords

- `vite` / `vite.config.ts`
- `PWA` / `manifest.json` / `service worker`
- `vite-plugin-pwa` / `workbox`
- `react-router-dom` / `useNavigate` / `createBrowserRouter`
- `WebSocket` (클라이언트 측)
- `Capacitor`

---

## Project Customization Notes

이 스킬은 **스켈레톤 버전(T3)**으로 작성되었으며, 다음 시나리오에서 보강이 필요하다:

1. **3D 시각화 통합** — Three.js 패턴은 `threejs-realtime-guidelines` 병행 적용
2. **인증 흐름 보강** — JWT refresh rotation, 소셜 로그인은 프로젝트 코드 작성 후 패턴 추출
3. **테스트 전략** — Vitest 또는 Playwright 패턴 추가 예정

실 프로젝트 적용 후 발견한 패턴은 `/harness-backport`로 역전파한다.
