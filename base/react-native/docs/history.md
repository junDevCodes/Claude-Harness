# History — base/react-native/ 작업 맥락

> **문서 역할:** 작업 맥락 및 주요 결정 사항을 기록하는 문서.

---

## 현재 상태

- **Phase:** Phase 1-B (신규 베이스 코드 생성)
- **시작일:** 2026-03-02
- **상태:** 작업 진행 중

---

## 주요 결정 사항

| 결정 | 내용 | 날짜 |
|---|---|---|
| Expo SDK 52 채택 | 2024년 말 안정 릴리스, expo-router v4 포함 | 2026-03-02 |
| Expo Router v4 | 파일 기반 라우팅, typed routes 활성화 | 2026-03-02 |
| Zustand v5 | Redux 없이 간결한 전역 상태 관리 | 2026-03-02 |
| Axios 인터셉터 | 401 → refresh → 재시도 패턴 (Queue 방식으로 중복 방지) | 2026-03-02 |
| SecureStore 전용 | AsyncStorage 대신 보안 저장소 사용 (토큰 전용) | 2026-03-02 |
| Path alias @/ | tsconfig paths로 절대 경로 import | 2026-03-02 |
| firebase 제외 | 범용성을 위해 이메일/JWT 인증만 포함 | 2026-03-02 |

---

## 작업 이력

### 2026-03-02 — 전체 구현 완료

**생성 파일 목록:**

- `docs/` 4문서 (plan, task, history, checklist)
- `package.json` — Expo SDK 52, Zustand v5, Axios, SecureStore
- `app.json` — scheme, typedRoutes, plugins 설정
- `tsconfig.json` — strict, @/* path alias
- `babel.config.js` — babel-preset-expo + reanimated plugin
- `types/index.ts` — User, AuthTokens, LoginRequest, RegisterRequest
- `constants/index.ts` — COLORS, SIZES, TYPOGRAPHY
- `lib/secureStore.ts` — saveTokens / getTokens / clearTokens
- `lib/api.ts` — Axios 인스턴스 + 401 Queue 인터셉터
- `store/authStore.ts` — Zustand auth store (initialize/login/register/logout)
- `hooks/useAuth.ts` — authStore 셀렉터 래퍼
- `components/ui/Button.tsx` — primary/outline/destructive, loading
- `components/ui/Input.tsx` — label, error 지원
- `components/ui/LoadingScreen.tsx`
- `app/_layout.tsx` — Root layout, SplashScreen 제어
- `app/+not-found.tsx`
- `app/(auth)/_layout.tsx` — 인증 완료 시 redirect
- `app/(auth)/login.tsx`
- `app/(auth)/register.tsx`
- `app/(tabs)/_layout.tsx` — 미인증 시 redirect
- `app/(tabs)/index.tsx` — Home
- `app/(tabs)/profile.tsx` — Profile + 로그아웃
- `tests/authStore.test.ts` — 5개 단위 테스트
- `.env.example`
- `README.md` — 구조 설명, 실행 방법, 커스터마이징 포인트, API 스펙

---

---

## Phase 2-B — 2026-03-05 스킬 작성 세션

**세션 목적:** `react-native-guidelines` 스킬 생성 (Phase 2-B 병렬 세션)

**분석 완료 파일:**
- `lib/api.ts` — Axios 인터셉터, 401 Queue 패턴
- `lib/secureStore.ts` — SecureStore saveTokens/getTokens/clearTokens
- `store/authStore.ts` — Zustand initialize/login/logout 패턴
- `hooks/useAuth.ts` — selector 래퍼 훅
- `app/_layout.tsx` — Root layout, SplashScreen 제어
- `app/(tabs)/_layout.tsx` — 인증 가드 + Tabs 구성
- `app/(auth)/_layout.tsx` — 인증 완료 시 redirect

**핵심 패턴 (스킬에 반영):**
1. Expo Router v4 Route Groups + Redirect 인증 가드
2. Zustand v5 auth store (initialize → SplashScreen 패턴)
3. Axios 401 Queue 인터셉터 (isRefreshing + failedQueue)
4. SecureStore 토큰 관리 (Promise.all 병렬 읽기/쓰기)
5. useAuth() selector 훅 (불필요한 리렌더링 방지)

**병렬 세션 격리 준수:**
- 수정 대상: `.claude/skills/react-native-guidelines/` 디렉토리만
- skill-rules.json, README.md, 루트 docs/ 수정 없음

---

**완료:** SKILL.md 494줄, DoD 전항목 통과

*Last updated: 2026-03-05 (Phase 2-B 완료 — react-native-guidelines 스킬 생성)*
