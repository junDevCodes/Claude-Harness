---
name: vercel-react-best-practices
description: React and Next.js performance optimization guidelines from Vercel Engineering. This skill should be used when writing, reviewing, or refactoring React/Next.js code to ensure optimal performance patterns. Triggers on tasks involving React components, Next.js pages, data fetching, bundle optimization, or performance improvements.
license: MIT
metadata:
  author: vercel
  version: "1.0.0"
triggers:
  - react performance
  - bundle size
  - lazy loading
  - React memo
  - useMemo
  - useCallback
  - re-render
  - optimization
  - next.js performance
  - code splitting
---

# Vercel React Best Practices

Comprehensive performance optimization guide for React and Next.js applications, maintained by Vercel. Contains 57 rules across 8 categories, prioritized by impact to guide automated refactoring and code generation.

## Purpose

Provide actionable, code-level performance patterns for React/Next.js applications. Each pattern includes BAD/GOOD code examples to guide both new code generation and existing code refactoring.

## When to Apply

Reference these guidelines when:
- Writing new React components or Next.js pages
- Implementing data fetching (client or server-side)
- Reviewing code for performance issues
- Refactoring existing React/Next.js code
- Optimizing bundle size or load times

---

## Key Patterns (Top 5 — Inline)

### Pattern 1: Waterfall Elimination — Promise.all Parallel Execution

Sequential awaits create request waterfalls. Use `Promise.all()` for independent operations (2-10x improvement).

**BAD — sequential execution, 3 round trips:**

```typescript
const user = await fetchUser()
const posts = await fetchPosts()
const comments = await fetchComments()
```

**GOOD — parallel execution, 1 round trip:**

```typescript
const [user, posts, comments] = await Promise.all([
  fetchUser(),
  fetchPosts(),
  fetchComments()
])
```

### Pattern 2: Barrel Import Avoidance — Bundle Size Optimization

Barrel files (`index.ts` re-exports) force loading thousands of unused modules (200-800ms import cost).

**BAD — imports entire library:**

```tsx
import { Check, X, Menu } from 'lucide-react'
// Loads 1,583 modules, ~2.8s extra in dev

import { Button, TextField } from '@mui/material'
// Loads 2,225 modules, ~4.2s extra in dev
```

**GOOD — direct imports only:**

```tsx
import Check from 'lucide-react/dist/esm/icons/check'
import X from 'lucide-react/dist/esm/icons/x'
import Menu from 'lucide-react/dist/esm/icons/menu'

import Button from '@mui/material/Button'
import TextField from '@mui/material/TextField'
```

**Alternative (Next.js 13.5+):**

```js
// next.config.js
module.exports = {
  experimental: {
    optimizePackageImports: ['lucide-react', '@mui/material']
  }
}
```

### Pattern 3: Dynamic Imports — Code Splitting

Use `next/dynamic` to lazy-load heavy components not needed on initial render (directly affects TTI and LCP).

**BAD — Monaco bundles with main chunk (~300KB):**

```tsx
import { MonacoEditor } from './monaco-editor'

function CodePanel({ code }: { code: string }) {
  return <MonacoEditor value={code} />
}
```

**GOOD — Monaco loads on demand:**

```tsx
import dynamic from 'next/dynamic'

const MonacoEditor = dynamic(
  () => import('./monaco-editor').then(m => m.MonacoEditor),
  { ssr: false }
)

function CodePanel({ code }: { code: string }) {
  return <MonacoEditor value={code} />
}
```

### Pattern 4: React.memo + useMemo Optimization

Extract expensive work into memoized components to enable early returns before computation.

**BAD — computes avatar even when loading:**

```tsx
function Profile({ user, loading }: Props) {
  const avatar = useMemo(() => {
    const id = computeAvatarId(user)
    return <Avatar id={id} />
  }, [user])

  if (loading) return <Skeleton />
  return <div>{avatar}</div>
}
```

**GOOD — skips computation when loading:**

```tsx
const UserAvatar = memo(function UserAvatar({ user }: { user: User }) {
  const id = useMemo(() => computeAvatarId(user), [user])
  return <Avatar id={id} />
})

function Profile({ user, loading }: Props) {
  if (loading) return <Skeleton />
  return (
    <div>
      <UserAvatar user={user} />
    </div>
  )
}
```

> **Note:** If React Compiler is enabled, manual `memo()`/`useMemo()` is not necessary.

### Pattern 5: Server Component Parallel Data Fetching

RSC executes sequentially within a tree. Restructure with composition to parallelize fetches.

**BAD — Sidebar waits for Page's fetch:**

```tsx
export default async function Page() {
  const header = await fetchHeader()
  return (
    <div>
      <div>{header}</div>
      <Sidebar />
    </div>
  )
}

async function Sidebar() {
  const items = await fetchSidebarItems()
  return <nav>{items.map(renderItem)}</nav>
}
```

**GOOD — both fetch simultaneously:**

```tsx
async function Header() {
  const data = await fetchHeader()
  return <div>{data}</div>
}

async function Sidebar() {
  const items = await fetchSidebarItems()
  return <nav>{items.map(renderItem)}</nav>
}

export default function Page() {
  return (
    <div>
      <Header />
      <Sidebar />
    </div>
  )
}
```

---

## Anti-Patterns

### 1. Client-side Data Waterfall in useEffect

Sequential data fetching inside `useEffect` chains creates cascading waterfalls visible to users.

- **BAD:** `useEffect(() => { fetch(A).then(() => fetch(B)) }, [])`
- **GOOD:** Server Component에서 직접 fetch, 또는 `Promise.all`로 병렬 처리

### 2. Barrel File (index.ts) Re-export All

`export * from './module'` 패턴으로 전체 모듈 그래프 로딩 → cold start 200-800ms 증가.

- **BAD:** `import { Button } from '@/components'` (barrel re-export)
- **GOOD:** `import { Button } from '@/components/ui/button'` (direct path)

### 3. Unnecessary State Lifting (Prop Drilling)

상위 컴포넌트로 상태를 끌어올려 불필요한 리렌더 전파.

- **BAD:** App → Layout → Sidebar → MenuItem 모두 리렌더
- **GOOD:** 상태를 사용하는 컴포넌트에 가깝게 배치, 또는 Context/Zustand 사용

### 4. useEffect for Data Fetching (RSC Available)

Server Component가 가능한 환경에서 `useEffect` + `useState`로 데이터 페칭.

- **BAD:** `useEffect(() => { fetch('/api/data').then(setData) }, [])`
- **GOOD:** `async function Page() { const data = await getData(); return <View data={data} /> }`

### 5. Large List Rendering Without Virtualization

수천 개 항목을 memo 없이 한 번에 렌더링 → 프레임 드랍.

- **BAD:** `{items.map(item => <Card key={item.id} {...item} />)}` (10,000 items)
- **GOOD:** `react-window` 또는 `@tanstack/react-virtual`로 가상화

---

## Rule Catalog (57 Rules by Category)

### 1. Eliminating Waterfalls (CRITICAL)

| Rule | Description |
|------|-------------|
| `async-defer-await` | Move await into branches where actually used |
| `async-parallel` | Use Promise.all() for independent operations |
| `async-dependencies` | Use better-all for partial dependencies |
| `async-api-routes` | Start promises early, await late in API routes |
| `async-suspense-boundaries` | Use Suspense to stream content |

### 2. Bundle Size Optimization (CRITICAL)

| Rule | Description |
|------|-------------|
| `bundle-barrel-imports` | Import directly, avoid barrel files |
| `bundle-dynamic-imports` | Use next/dynamic for heavy components |
| `bundle-defer-third-party` | Load analytics/logging after hydration |
| `bundle-conditional` | Load modules only when feature is activated |
| `bundle-preload` | Preload on hover/focus for perceived speed |

### 3. Server-Side Performance (HIGH)

| Rule | Description |
|------|-------------|
| `server-auth-actions` | Authenticate server actions like API routes |
| `server-cache-react` | Use React.cache() for per-request dedup |
| `server-cache-lru` | Use LRU cache for cross-request caching |
| `server-dedup-props` | Avoid duplicate serialization in RSC props |
| `server-serialization` | Minimize data passed to client components |
| `server-parallel-fetching` | Restructure components to parallelize fetches |
| `server-after-nonblocking` | Use after() for non-blocking operations |

### 4. Client-Side Data Fetching (MEDIUM-HIGH)

| Rule | Description |
|------|-------------|
| `client-swr-dedup` | Use SWR for automatic request deduplication |
| `client-event-listeners` | Deduplicate global event listeners |
| `client-passive-event-listeners` | Use passive listeners for scroll |
| `client-localstorage-schema` | Version and minimize localStorage data |

### 5. Re-render Optimization (MEDIUM)

| Rule | Description |
|------|-------------|
| `rerender-defer-reads` | Don't subscribe to state only used in callbacks |
| `rerender-memo` | Extract expensive work into memoized components |
| `rerender-memo-with-default-value` | Hoist default non-primitive props |
| `rerender-dependencies` | Use primitive dependencies in effects |
| `rerender-derived-state` | Subscribe to derived booleans, not raw values |
| `rerender-derived-state-no-effect` | Derive state during render, not effects |
| `rerender-functional-setstate` | Use functional setState for stable callbacks |
| `rerender-lazy-state-init` | Pass function to useState for expensive values |
| `rerender-simple-expression-in-memo` | Avoid memo for simple primitives |
| `rerender-move-effect-to-event` | Put interaction logic in event handlers |
| `rerender-transitions` | Use startTransition for non-urgent updates |
| `rerender-use-ref-transient-values` | Use refs for transient frequent values |

### 6. Rendering Performance (MEDIUM)

| Rule | Description |
|------|-------------|
| `rendering-animate-svg-wrapper` | Animate div wrapper, not SVG element |
| `rendering-content-visibility` | Use content-visibility for long lists |
| `rendering-hoist-jsx` | Extract static JSX outside components |
| `rendering-svg-precision` | Reduce SVG coordinate precision |
| `rendering-hydration-no-flicker` | Use inline script for client-only data |
| `rendering-hydration-suppress-warning` | Suppress expected mismatches |
| `rendering-activity` | Use Activity component for show/hide |
| `rendering-conditional-render` | Use ternary, not && for conditionals |
| `rendering-usetransition-loading` | Prefer useTransition for loading state |

### 7. JavaScript Performance (LOW-MEDIUM)

| Rule | Description |
|------|-------------|
| `js-batch-dom-css` | Group CSS changes via classes or cssText |
| `js-index-maps` | Build Map for repeated lookups |
| `js-cache-property-access` | Cache object properties in loops |
| `js-cache-function-results` | Cache function results in module-level Map |
| `js-cache-storage` | Cache localStorage/sessionStorage reads |
| `js-combine-iterations` | Combine multiple filter/map into one loop |
| `js-length-check-first` | Check array length before expensive comparison |
| `js-early-exit` | Return early from functions |
| `js-hoist-regexp` | Hoist RegExp creation outside loops |
| `js-min-max-loop` | Use loop for min/max instead of sort |
| `js-set-map-lookups` | Use Set/Map for O(1) lookups |
| `js-tosorted-immutable` | Use toSorted() for immutability |

### 8. Advanced Patterns (LOW)

| Rule | Description |
|------|-------------|
| `advanced-event-handler-refs` | Store event handlers in refs |
| `advanced-init-once` | Initialize app once per app load |
| `advanced-use-latest` | useLatest for stable callback refs |

---

## Resources

Each rule has a detailed file with full code examples in `rules/` directory:

```
rules/async-parallel.md
rules/bundle-barrel-imports.md
rules/server-parallel-fetching.md
```

For the complete compiled guide with all 57 rules expanded: `AGENTS.md`
