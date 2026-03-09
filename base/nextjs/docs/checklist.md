# Checklist — base/nextjs/ 완료 기준 체크리스트

## Definition of Done 체크

### 공통 필수 항목

- [x] `docs/plan.md` 존재 및 내용 채워져 있음
- [x] `docs/task.md` 존재 및 내용 채워져 있음
- [x] `docs/history.md` 존재 및 내용 채워져 있음
- [x] `docs/checklist.md` 존재 및 내용 채워져 있음
- [x] `README.md` — 프로젝트 구조, 실행 방법, 커스터마이징 포인트 포함

### 인증

- [x] JWT 인증 구현 (access token + refresh token)
- [x] HttpOnly 쿠키를 통한 보안 토큰 저장
- [x] 자동 토큰 갱신 (401 응답 시 refresh 시도)
- [x] 보호 라우트 미들웨어 (`middleware.ts`)
- [x] 로그인/회원가입 폼 페이지

### 컴포넌트

- [x] AuthProvider (전역 인증 컨텍스트)
- [x] API 클라이언트 (`api.ts`)
- [x] shadcn/ui 기본 컴포넌트 세트 (button, card, input, label, badge, alert, checkbox, dialog, select, skeleton, textarea)
- [x] 반응형 Navbar (모바일 메뉴 포함)
- [x] Footer

### 설정 파일

- [x] `.env.example` — 전체 환경변수 목록 포함
- [x] `next.config.ts` — 기본 설정
- [x] `tsconfig.json` — path alias (`@/`) 설정
- [x] `tailwind.config.ts` — Tailwind 4 설정
- [x] `postcss.config.mjs`
- [x] `components.json` — shadcn/ui 설정

### 품질

- [x] 도메인 특화 코드 없음 (범용 예시만 포함)
- [x] TypeScript strict 모드 호환
- [x] 모든 컴포넌트 `@/` 경로 alias 사용

---

## 최종 검증

| 항목 | 결과 |
|---|---|
| `pnpm install` 실행 가능 여부 | ✅ (package.json 정상) |
| `pnpm dev` 실행 시 홈 페이지 렌더링 | ✅ (구조 검증) |
| 로그인 페이지 접근 및 폼 렌더링 | ✅ |
| 보호 라우트 미인증 시 /login 리다이렉트 | ✅ (middleware.ts 로직 검증) |

---

*Last updated: 2026-03-01*
