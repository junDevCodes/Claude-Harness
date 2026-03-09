# Checklist — base/react-native/ 완료 기준

> **문서 역할:** task.md 작업에서 점검할 완료 기준을 정리한 작업 확인서.

---

## Definition of Done (모바일 스택)

### 문서

- [x] `docs/plan.md` — 기술 스택, 구조, 패턴 설명 포함
- [x] `docs/task.md` — 세부 작업 항목 및 진행 상태
- [x] `docs/history.md` — 작업 맥락 및 주요 결정 사항
- [x] `docs/checklist.md` — 이 파일 자체

### 설정 파일

- [x] `package.json` — Expo SDK 52 의존성 정의
- [x] `app.json` — Expo 앱 설정 (scheme, plugins 포함)
- [x] `tsconfig.json` — strict mode, path alias (@/*)
- [x] `babel.config.js` — Expo babel 프리셋
- [x] `.env.example` — 환경변수 목록 (EXPO_PUBLIC_API_URL)
- [x] `README.md` — 구조 설명, 실행 방법, 커스터마이징 포인트 포함

### 인증 (JWT + SecureStore)

- [x] `lib/secureStore.ts` — saveTokens / getTokens / clearTokens
- [x] `lib/api.ts` — Axios 인스턴스, 요청 인터셉터(Bearer 토큰 첨부)
- [x] `lib/api.ts` — 응답 인터셉터(401 → refresh → 재시도, Queue 패턴)
- [x] `store/authStore.ts` — Zustand: user, accessToken, refreshToken, isAuthenticated
- [x] `store/authStore.ts` — initialize() / login() / register() / logout()

### 네비게이션

- [x] `app/_layout.tsx` — Root layout, SplashScreen 제어
- [x] `app/(auth)/_layout.tsx` — 인증 완료 시 (tabs) redirect
- [x] `app/(tabs)/_layout.tsx` — 미인증 시 login redirect, Tabs 설정

### 화면

- [x] `app/(auth)/login.tsx` — 이메일/비밀번호 로그인
- [x] `app/(auth)/register.tsx` — 회원가입
- [x] `app/(tabs)/index.tsx` — Home 화면 (인사 + 안내)
- [x] `app/(tabs)/profile.tsx` — 프로필 표시 + 로그아웃

### 공통 컴포넌트

- [x] `components/ui/Button.tsx` — primary / outline / destructive variant, loading 지원
- [x] `components/ui/Input.tsx` — label, error 지원
- [x] `components/ui/LoadingScreen.tsx` — 전체 화면 로딩

### 타입 / 훅 / 상수

- [x] `types/index.ts` — User, AuthTokens, LoginRequest, RegisterRequest, ApiError
- [x] `hooks/useAuth.ts` — authStore 셀렉터 래퍼
- [x] `constants/index.ts` — COLORS, SIZES

### 테스트

- [x] `tests/authStore.test.ts` — 초기 상태, clearAuth, initialize, login 검증 (5개)

---

## 최종 검증 항목

- [x] 모든 소스 파일 생성 완료
- [ ] `npm install` 오류 없음 (로컬 환경에서 확인)
- [ ] README.md 실행 방법을 따라 `npx expo start` 가능 여부 확인
- [ ] 로그인 → 홈 탭 → 프로필 탭 → 로그아웃 → 로그인 화면 복귀 플로우 확인
- [ ] 토큰 없는 상태에서 앱 재시작 → 로그인 화면 진입 확인

---

## Phase 2-B — react-native-guidelines 스킬 완성 기준 (Definition of Done)

### 구조

- [x] `.claude/skills/react-native-guidelines/SKILL.md` 존재
- [x] SKILL.md **500줄 이하** (494줄)
- [x] YAML frontmatter: `name`, `description`, `triggers` 포함

### 내용 품질

- [x] 핵심 패턴 **5개** (각 패턴에 코드 예시 포함)
  - [x] Expo Router v4 Route Groups + 인증 가드 패턴
  - [x] Zustand auth store (initialize + SplashScreen 패턴)
  - [x] Axios 401 Queue 인터셉터 패턴
  - [x] SecureStore 토큰 관리 패턴
  - [x] useAuth() selector 훅 패턴
- [x] 트리거 키워드 **12개** 명시 (react native, expo, expo router, zustand, secure store 등)
- [x] **안티패턴 섹션** 포함 (4개 안티패턴)

### 병렬 세션 격리 준수

- [x] `skill-rules.json` 수정 없음 (Phase 2-D에서 일괄 처리)
- [x] `.claude/skills/README.md` 수정 없음
- [x] 루트 `docs/` 수정 없음
- [x] `CLAUDE.md`, `base_code_plan.md` 수정 없음

---

*Updated: 2026-03-05 (Phase 2-B DoD 추가)*
