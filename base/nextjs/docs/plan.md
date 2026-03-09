# Plan — base/nextjs/ 베이스 코드 구성 계획

## 목표

Next.js 15 + TypeScript + Tailwind CSS 4 기반의 **범용 프론트엔드 스타터 킷**.
JWT 인증, shadcn/ui 컴포넌트, 보호 라우트 패턴을 포함한 최소한의 구조를 제공한다.

---

## 스택

| 항목 | 기술 |
|---|---|
| 프레임워크 | Next.js 15 (App Router) |
| 언어 | TypeScript 5 |
| 스타일링 | Tailwind CSS 4 |
| UI 컴포넌트 | shadcn/ui (Radix UI 기반) |
| 아이콘 | Lucide React |
| 유틸리티 | clsx, tailwind-merge |
| 인증 | JWT (HttpOnly 쿠키 기반, 서버/클라이언트 분리) |

---

## 디렉토리 구조

```
base/nextjs/
├── src/
│   ├── app/
│   │   ├── layout.tsx          # RootLayout + AuthProvider
│   │   ├── page.tsx            # 홈 페이지
│   │   ├── error.tsx           # 전역 에러 바운더리
│   │   ├── loading.tsx         # 전역 로딩 상태
│   │   ├── globals.css         # Tailwind + CSS 변수
│   │   ├── login/
│   │   │   └── page.tsx        # 로그인/회원가입 폼
│   │   └── api/
│   │       └── auth/
│   │           └── session/
│   │               └── route.ts  # HttpOnly 쿠키 관리
│   ├── components/
│   │   ├── layout/
│   │   │   ├── Navbar.tsx      # 반응형 네비게이션 바
│   │   │   └── Footer.tsx      # 푸터
│   │   └── ui/                 # shadcn/ui 컴포넌트
│   │       ├── button.tsx
│   │       ├── card.tsx
│   │       ├── input.tsx
│   │       ├── label.tsx
│   │       ├── badge.tsx
│   │       ├── alert.tsx
│   │       ├── checkbox.tsx
│   │       ├── dialog.tsx
│   │       ├── select.tsx
│   │       ├── skeleton.tsx
│   │       └── textarea.tsx
│   ├── lib/
│   │   ├── api.ts              # API 클라이언트 (자동 토큰 갱신)
│   │   └── utils.ts            # cn() 유틸리티
│   ├── providers/
│   │   └── AuthProvider.tsx    # 전역 인증 컨텍스트
│   ├── middleware.ts            # 보호 라우트 미들웨어
│   └── types/                  # 공통 타입 정의
├── docs/
│   ├── plan.md       ← 이 파일
│   ├── task.md
│   ├── history.md
│   └── checklist.md
├── .env.example
├── README.md
├── next.config.ts
├── tsconfig.json
├── tailwind.config.ts
├── postcss.config.mjs
└── components.json
```

---

## 인증 플로우

```
Client                    Next.js Server              Backend API
  |                            |                           |
  |--- POST /api/auth/login --> |                           |
  |                            |--- POST /api/v1/auth/login-->
  |                            |<-- { access_token, refresh_token }
  |                            |--- Set HttpOnly Cookie ---|
  |<-- 200 OK ----------------|                           |
  |                            |                           |
  |--- (subsequent requests) ->|                           |
  |   Cookie: app_access_token |                           |
  |                            |--- Authorization: Bearer ->|
```

---

## Phase

| Phase | 항목 | 상태 |
|---|---|---|
| 1 | 핵심 파일 생성 (app/, components/, lib/, providers/) | ✅ 완료 |
| 2 | 인증 플로우 완성 (AuthProvider, session route, middleware) | ✅ 완료 |
| 3 | UI 컴포넌트 세트 (shadcn/ui 11종) | ✅ 완료 |
| 4 | docs/ 4문서, .env.example, README.md | ✅ 완료 |

---

*Last updated: 2026-03-01*
