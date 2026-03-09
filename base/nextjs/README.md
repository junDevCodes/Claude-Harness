# base/nextjs — Next.js 15 스타터 킷

JWT 인증, shadcn/ui, Tailwind CSS 4를 포함한 Next.js 15 (App Router) 범용 스타터 킷.

## 스택

| 항목 | 기술 |
|---|---|
| 프레임워크 | Next.js 15 (App Router) |
| 언어 | TypeScript 5 |
| 스타일링 | Tailwind CSS 4 |
| UI 컴포넌트 | shadcn/ui (Radix UI 기반) |
| 인증 | JWT (HttpOnly 쿠키, 자동 갱신) |
| 패키지 매니저 | pnpm |

## 프로젝트 구조

```
src/
├── app/
│   ├── layout.tsx              # RootLayout + AuthProvider
│   ├── page.tsx                # 홈 페이지
│   ├── error.tsx               # 전역 에러 바운더리
│   ├── loading.tsx             # 전역 로딩 상태
│   ├── globals.css             # Tailwind + CSS 변수
│   ├── login/page.tsx          # 로그인/회원가입 폼
│   └── api/auth/session/route.ts  # HttpOnly 쿠키 관리
├── components/
│   ├── layout/
│   │   ├── Navbar.tsx          # 반응형 네비게이션
│   │   └── Footer.tsx
│   └── ui/                     # shadcn/ui 컴포넌트 11종
├── lib/
│   ├── api.ts                  # API 클라이언트 (자동 토큰 갱신)
│   └── utils.ts                # cn() 유틸리티
├── providers/
│   └── AuthProvider.tsx        # 전역 인증 컨텍스트
└── middleware.ts               # 보호 라우트 미들웨어
```

## 시작하기

### 1. 환경 변수 설정

```bash
cp .env.example .env.local
# .env.local 파일을 열어 값을 채워넣기
```

### 2. 의존성 설치 및 개발 서버 실행

```bash
pnpm install
pnpm dev
```

브라우저에서 [http://localhost:3000](http://localhost:3000)으로 접속.

### 3. 빌드

```bash
pnpm build
pnpm start
```

## 인증 플로우

```
1. 사용자가 /login에서 이메일/비밀번호 입력
2. AuthProvider.emailLogin() 호출
3. Backend API (POST /api/v1/auth/login) 요청
4. 응답받은 access_token/refresh_token을
   POST /api/auth/session 으로 HttpOnly 쿠키에 저장
5. 이후 모든 API 요청에 쿠키가 자동 첨부
6. 401 응답 시 api.ts가 자동으로 refresh 시도
```

## 주요 패턴

### 보호 라우트 추가

`src/middleware.ts`의 `PROTECTED_PATHS`에 경로 추가:

```typescript
const PROTECTED_PATHS: string[] = [
  "/dashboard",
  "/profile",
  "/settings",
];
```

### API 호출

```typescript
import { api } from "@/lib/api";

// GET 요청 (자동 토큰 갱신 포함)
const data = await api.get<User>("/api/v1/users/me");

// POST 요청
const result = await api.post<LoginResponse>("/api/v1/auth/login", {
  email,
  password,
});
```

### shadcn/ui 컴포넌트 추가

```bash
# 추가 컴포넌트 설치 (components.json 설정 기반)
pnpm dlx shadcn@latest add [component-name]
```

## 커스터마이징 포인트

| 항목 | 파일 |
|---|---|
| 앱 이름/메타데이터 | `src/app/layout.tsx` |
| 색상 테마 | `src/app/globals.css` (CSS 변수) |
| 네비게이션 링크 | `src/components/layout/Navbar.tsx` |
| 보호 라우트 | `src/middleware.ts` |
| API 기본 URL | `src/lib/api.ts` 또는 `.env.local` |
| 로고 텍스트 | `Navbar.tsx`, `Footer.tsx` |

## 백엔드 연동

이 스타터 킷은 `base/fastapi/`와 함께 사용하도록 설계됐다.

- FastAPI 기본 URL: `http://localhost:28080`
- 인증 엔드포인트: `POST /api/v1/auth/login`, `POST /api/v1/auth/signup`, `POST /api/v1/auth/refresh`
- 쿠키명: `app_access_token`, `app_refresh_token`
