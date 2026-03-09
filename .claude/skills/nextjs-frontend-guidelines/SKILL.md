---
name: nextjs-frontend-guidelines
description: Next.js 15 + React 19 + TypeScript 프론트엔드 개발 가이드라인. App Router, Server/Client Component 분리, JWT 이메일 인증(AuthProvider), api 클라이언트(401 자동 갱신), Hydration 보호 패턴, Middleware 라우트 보호, shadcn/ui + Tailwind CSS. 컴포넌트/페이지/API 라우트/데이터 페칭/인증 작업 시 사용.
triggers:
  - Next.js
  - nextjs
  - next.js
  - React
  - server component
  - client component
  - app router
  - use client
  - use server
  - shadcn
  - tailwind
  - AuthProvider
  - useAuth
  - next middleware
---

# Next.js 15 Frontend Development Guidelines

## Purpose

`base/nextjs/` 기반 Next.js 15 + React 19 + TypeScript 범용 프론트엔드 표준.
App Router, Server/Client Component 분리, JWT 이메일 인증, api 클라이언트, Middleware 라우트 보호 패턴을 일관되게 적용한다.

## When to Use This Skill

- 컴포넌트/페이지 생성 (Server vs Client 결정)
- JWT 인증 흐름 구현 (로그인/회원가입/로그아웃)
- API 호출 추가 (`api.get/post/patch/delete`)
- Middleware 라우트 보호 설정
- Hydration 불일치 방지
- shadcn/ui + Tailwind CSS 스타일링

---

## Quick Start

### New Component Checklist

- [ ] Server Component가 기본 — `useState`/`useEffect` 필요할 때만 `'use client'`
- [ ] Props는 TypeScript interface로 정의
- [ ] import alias `@/` 사용
- [ ] shadcn/ui 컴포넌트 우선 사용
- [ ] 조건부 클래스는 `cn()` 사용

### New Page Checklist

- [ ] `src/app/{route}/page.tsx` 생성
- [ ] 인증 필요 시 `middleware.ts`의 `PROTECTED_PATHS`에 경로 추가
- [ ] 인증 상태 UI에 `mounted` 패턴 적용 (Hydration 방지)
- [ ] 로딩 상태: `src/app/{route}/loading.tsx`

---

## Project Structure

```
src/
├── app/
│   ├── layout.tsx          # RootLayout — AuthProvider 등록
│   ├── page.tsx            # 홈 페이지
│   ├── loading.tsx         # 전역 로딩 UI
│   ├── error.tsx           # 전역 에러 바운더리 ('use client')
│   ├── login/page.tsx      # 로그인/회원가입 페이지
│   └── api/
│       └── auth/
│           └── session/    # 토큰 쿠키 동기화 API route
├── components/
│   ├── layout/             # Navbar.tsx, Footer.tsx
│   └── ui/                 # shadcn/ui 컴포넌트
├── lib/
│   ├── api.ts              # API 클라이언트 + 토큰 관리
│   └── utils.ts            # cn() 유틸리티
├── providers/
│   └── AuthProvider.tsx    # JWT 인증 컨텍스트
└── middleware.ts            # 라우트 보호
```

---

## Core Patterns

### Pattern 1 — Server vs Client Component

기본값은 Server Component. `'use client'`는 최소화한다.

```typescript
// ✅ Server Component (기본) — async, native fetch 사용
// api.get()은 localStorage 의존 → Server Component에서 사용 불가
// src/app/users/page.tsx
export default async function UsersPage() {
  const res = await fetch(
    `${process.env.NEXT_PUBLIC_API_URL}/api/v1/users`,
    { cache: 'no-store' },  // 동적 데이터는 cache: 'no-store'
  );
  if (!res.ok) throw new Error('Failed to fetch users');
  const users: User[] = await res.json();
  return <UserList users={users} />;
}

// ✅ Client Component — 상태/이벤트가 있는 최소 단위만
// src/components/ui/Counter.tsx
'use client';
import { useState } from 'react';

export function Counter() {
  const [count, setCount] = useState(0);
  return <button onClick={() => setCount(c => c + 1)}>{count}</button>;
}

// ✅ 새 페이지 기본 템플릿
// src/app/{route}/page.tsx
import type { Metadata } from 'next';

export const metadata: Metadata = { title: 'Page Title' };

export default function Page() {
  return <main className="container mx-auto px-4 py-8">{/* content */}</main>;
}
```

---

### Pattern 2 — AuthProvider + useAuth()

`AuthProvider`는 `layout.tsx`에 등록. `useAuth()`로 인증 상태 및 액션 접근.

```typescript
// src/providers/AuthProvider.tsx — AuthContextType 인터페이스
interface AuthContextType {
  user: UserInfo | null;
  isLoading: boolean;
  isAuthenticated: boolean;
  emailLogin: (email: string, password: string) => Promise<void>;
  emailSignUp: (email: string, password: string, username: string) => Promise<void>;
  login: (loginResponse: LoginResponse) => void;  // 외부 로그인 응답 처리
  logout: () => Promise<void>;
  refreshUser: () => Promise<void>;
}

// 로그인 페이지에서 사용
'use client';
import { useAuth } from '@/providers/AuthProvider';

export default function LoginPage() {
  const { emailLogin, emailSignUp, isLoading } = useAuth();
  const [error, setError] = useState('');

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    try {
      await emailLogin(email, password);
      router.push('/');
    } catch (err) {
      setError(err instanceof Error ? err.message : 'An error occurred');
    }
  };
  // ...
}

// 인증 보호 컴포넌트
'use client';
export function ProtectedSection() {
  const { isAuthenticated, isLoading, user } = useAuth();
  if (isLoading) return <Skeleton />;
  if (!isAuthenticated) return <Link href="/login">Login required</Link>;
  return <div>Hello, {user?.username}</div>;
}
```

---

### Pattern 3 — API 클라이언트 (api.get/post/patch/delete)

`src/lib/api.ts`의 `api` 객체는 **Client Component 전용** (localStorage 의존).
Server Component 데이터 페칭은 native `fetch()`를 사용한다. 401 시 refresh token 자동 재시도.

```typescript
// src/lib/api.ts — 기본 제공 메서드
export const api = {
  get: <T>(endpoint: string) => apiRequest<T>(endpoint),
  post: <T>(endpoint: string, body: unknown) =>
    apiRequest<T>(endpoint, { method: 'POST', body: JSON.stringify(body) }),
  patch: <T>(endpoint: string, body: unknown) =>
    apiRequest<T>(endpoint, { method: 'PATCH', body: JSON.stringify(body) }),
  delete: <T>(endpoint: string) =>
    apiRequest<T>(endpoint, { method: 'DELETE' }),
};

// Client Component에서 사용
'use client';
import { api, ApiError } from '@/lib/api';

async function handleUpdate(id: string, data: UpdateInput) {
  try {
    const result = await api.patch<User>(`/api/v1/users/${id}`, data);
    return result;
  } catch (err) {
    if (err instanceof ApiError) {
      if (err.status === 401) router.push('/login');
      throw new Error(`Update failed: ${err.statusText}`);
    }
    throw err;
  }
}

// 도메인 전용 API 함수 추가 패턴 (api.ts 하단에 확장)
export async function getProfile(id: string): Promise<UserInfo> {
  return api.get<UserInfo>(`/api/v1/users/${id}`);
}
```

---

### Pattern 4 — Hydration 보호 + Middleware 라우트 보호

SSR/CSR 불일치 방지를 위한 `mounted` 패턴. 라우트 보호는 `middleware.ts`에서 중앙 관리.

```typescript
// Hydration 보호 — 인증 상태 의존 UI에 필수
'use client';
export function Navbar() {
  const [mounted, setMounted] = useState(false);
  const { isAuthenticated, isLoading } = useAuth();

  useEffect(() => { setMounted(true); }, []);

  return (
    <nav>
      {/* mounted 전에는 빈 자리 — SSR/CSR 불일치 방지 */}
      {mounted && !isLoading ? (
        isAuthenticated ? <LogoutButton /> : <Link href="/login">Login</Link>
      ) : (
        <div className="w-[72px] h-10" />  {/* 레이아웃 시프트 방지 */}
      )}
    </nav>
  );
}

// middleware.ts — 서버 사이드 라우트 보호
const PROTECTED_PATHS: string[] = [
  '/dashboard',
  '/profile',
  // 새 보호 경로는 여기에 추가
];

export function middleware(request: NextRequest) {
  const { pathname } = request.nextUrl;
  const accessToken = request.cookies.get('app_access_token')?.value;
  const isProtected = PROTECTED_PATHS.some(p => pathname.startsWith(p));

  if (isProtected && !accessToken) {
    const loginUrl = new URL('/login', request.url);
    loginUrl.searchParams.set('redirect', pathname);
    return NextResponse.redirect(loginUrl);
  }
  return NextResponse.next();
}
```

---

### Pattern 5 — 토큰 관리 (localStorage + Cookie 동기화)

액세스 토큰은 `localStorage` + HTTP-only 쿠키에 동기화. 쿠키는 Middleware가 사용.

```typescript
// src/lib/api.ts — 토큰 저장/삭제
const ACCESS_TOKEN_KEY = 'app_access_token';
const REFRESH_TOKEN_KEY = 'app_refresh_token';

export function setTokens(accessToken: string, refreshToken: string): void {
  localStorage.setItem(ACCESS_TOKEN_KEY, accessToken);
  localStorage.setItem(REFRESH_TOKEN_KEY, refreshToken);
  syncTokensToCookies(accessToken, refreshToken);  // POST /api/auth/session
}

export function clearTokens(): void {
  localStorage.removeItem(ACCESS_TOKEN_KEY);
  localStorage.removeItem(REFRESH_TOKEN_KEY);
  fetch('/api/auth/session', { method: 'DELETE' });  // 쿠키 삭제
}

// 브라우저 탭 간 인증 상태 동기화 (AuthProvider 내부)
useEffect(() => {
  const handleStorageChange = (e: StorageEvent) => {
    if (e.key === 'app_access_token') {
      if (!e.newValue) setUser(null);
      else refreshUser();
    }
  };
  window.addEventListener('storage', handleStorageChange);
  return () => window.removeEventListener('storage', handleStorageChange);
}, [refreshUser]);
```

---

## Anti-Patterns

### ❌ Server Component에서 useState/useEffect 사용

```typescript
// BAD — Server Component는 브라우저 API/훅 불가
export default async function Page() {
  const [data, setData] = useState(null);  // ← 런타임 에러
  useEffect(() => { fetch('/api/data'); }, []);
}

// GOOD — Server Component는 native fetch() 사용 (api.get은 Client 전용)
export default async function Page() {
  const res = await fetch(`${process.env.NEXT_PUBLIC_API_URL}/api/v1/data`, {
    cache: 'no-store',
  });
  const data: Data = await res.json();
  return <DataView data={data} />;
}
```

### ❌ 불필요한 'use client' 남용

```typescript
// BAD — 정적 콘텐츠에 'use client' → 번들 사이즈 증가
'use client';
export default function AboutPage() {
  return <div>Static content here</div>;  // 훅/이벤트 없음
}

// GOOD — 훅/이벤트가 없으면 Server Component로 유지
export default function AboutPage() {
  return <div>Static content here</div>;
}
```

### ❌ Hydration 불일치 무시 (인증 상태 직접 렌더링)

```typescript
// BAD — SSR 시 user=null, CSR 시 user 있음 → 불일치 에러
export function Navbar() {
  const { isAuthenticated } = useAuth();
  return isAuthenticated ? <LogoutBtn /> : <LoginBtn />;  // Hydration 에러
}

// GOOD — mounted 상태 확인 후 렌더링
export function Navbar() {
  const [mounted, setMounted] = useState(false);
  const { isAuthenticated } = useAuth();
  useEffect(() => { setMounted(true); }, []);
  if (!mounted) return <div className="w-[72px] h-10" />;
  return isAuthenticated ? <LogoutBtn /> : <LoginBtn />;
}
```

### ❌ Server Component에서 api.get() 사용

```typescript
// BAD — api.get()은 localStorage 의존 → Server Component에서 런타임 에러
export default async function UsersPage() {
  const users = await api.get<User[]>('/api/v1/users');  // ← window/localStorage 없음
}

// GOOD — Server Component는 native fetch() 사용
export default async function UsersPage() {
  const res = await fetch(`${process.env.NEXT_PUBLIC_API_URL}/api/v1/users`, {
    cache: 'no-store',
  });
  const users: User[] = await res.json();
  return <UserList users={users} />;
}
// api.get/post/patch/delete → Client Component ('use client') 전용
```

### ❌ useEffect에서 데이터 페칭

```typescript
// BAD — 클라이언트 사이드 fetch, 초기 로딩 지연
'use client';
export default function UsersPage() {
  const [users, setUsers] = useState([]);
  useEffect(() => { fetch('/api/v1/users').then(...); }, []);
}

// GOOD — Server Component는 native fetch() 사용 (빠름, SEO 유리)
export default async function UsersPage() {
  const res = await fetch(`${process.env.NEXT_PUBLIC_API_URL}/api/v1/users`, {
    cache: 'no-store',
  });
  const users: User[] = await res.json();
  return <UserList users={users} />;
}
```

### ❌ 토큰을 직접 fetch 헤더에 주입

```typescript
// BAD — 토큰 만료/갱신 처리 없음, 중복 코드
const token = localStorage.getItem('token');
fetch('/api/v1/data', { headers: { Authorization: `Bearer ${token}` } });

// GOOD — api 클라이언트 사용 (401 자동 갱신 포함)
const data = await api.get<Data>('/api/v1/data');
```

---

## References

- `base/nextjs/src/providers/AuthProvider.tsx` — 인증 컨텍스트 전체 구현
- `base/nextjs/src/lib/api.ts` — API 클라이언트 + 토큰 관리
- `base/nextjs/src/middleware.ts` — 라우트 보호 미들웨어
- `base/nextjs/src/components/layout/Navbar.tsx` — Hydration 보호 패턴
- `base/nextjs/src/app/login/page.tsx` — 로그인/회원가입 폼 패턴
