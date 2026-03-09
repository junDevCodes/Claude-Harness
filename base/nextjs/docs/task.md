# Task — base/nextjs/ 작업 상세 계획서

## 현재 상태: 완료

모든 작업 항목이 완료됐다.

---

## 완료된 작업

| # | 항목 | 상태 |
|---|---|---|
| 1 | `package.json` — 의존성 정의 (firebase 등 비즈니스 패키지 제거) | ✅ |
| 2 | `next.config.ts` — 기본 Next.js 설정 | ✅ |
| 3 | `tsconfig.json`, `postcss.config.mjs`, `components.json` | ✅ |
| 4 | `src/lib/utils.ts` — cn() 유틸리티 | ✅ |
| 5 | `src/lib/api.ts` — API 클라이언트 (자동 토큰 갱신) | ✅ |
| 6 | `src/providers/AuthProvider.tsx` — JWT 인증 컨텍스트 | ✅ |
| 7 | `src/app/globals.css` — Tailwind 4 + CSS 변수 | ✅ |
| 8 | `src/app/layout.tsx` — RootLayout | ✅ |
| 9 | `src/app/page.tsx` — 홈 페이지 | ✅ |
| 10 | `src/app/error.tsx`, `loading.tsx` | ✅ |
| 11 | `src/middleware.ts` — 보호 라우트 패턴 | ✅ |
| 12 | `src/app/login/page.tsx` — 로그인/회원가입 폼 | ✅ |
| 13 | `src/app/api/auth/session/route.ts` — HttpOnly 쿠키 관리 | ✅ |
| 14 | `src/components/layout/Navbar.tsx` — 반응형 네비게이션 바 | ✅ |
| 15 | `src/components/layout/Footer.tsx` — 푸터 | ✅ |
| 16 | `src/components/ui/` — shadcn/ui 11종 컴포넌트 | ✅ |
| 17 | `docs/` 4문서 (plan / task / history / checklist) | ✅ |
| 18 | `.env.example`, `README.md` | ✅ |

---

## 커스터마이징 포인트

새 프로젝트에 복사 후 수정해야 할 항목:

1. **앱 이름:** `layout.tsx` metadata, `Navbar.tsx` 로고 텍스트, `Footer.tsx`
2. **환경 변수:** `.env.example` → `.env.local`로 복사 후 값 설정
3. **API 엔드포인트:** `src/lib/api.ts`의 `NEXT_PUBLIC_API_URL`
4. **보호 라우트:** `src/middleware.ts`의 `PROTECTED_PATHS`
5. **네비게이션:** `src/components/layout/Navbar.tsx`의 `navLinks`
6. **색상:** `src/app/globals.css`의 `--color-primary` 등

---

*Last updated: 2026-03-01*
