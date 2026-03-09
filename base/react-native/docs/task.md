# Task — base/react-native/ 세부 작업 항목

> **문서 역할:** plan.md에서 할당된 작업의 상세 실행 계획서.

---

## 현재 Phase: Phase 1-B (신규 베이스 코드 생성)

### 작업 목록

| # | 항목 | 상태 |
|---|---|---|
| 1 | docs/ 4문서 작성 | ✅ |
| 2 | package.json + app.json + tsconfig.json + babel.config.js | ✅ |
| 3 | types/index.ts — 공통 타입 정의 | ✅ |
| 4 | constants/index.ts — 공통 상수 | ✅ |
| 5 | lib/secureStore.ts — SecureStore 래퍼 | ✅ |
| 6 | lib/api.ts — Axios 클라이언트 (JWT intercept) | ✅ |
| 7 | store/authStore.ts — Zustand auth 상태 관리 | ✅ |
| 8 | hooks/useAuth.ts | ✅ |
| 9 | components/ui/Button.tsx | ✅ |
| 10 | components/ui/Input.tsx | ✅ |
| 11 | components/ui/LoadingScreen.tsx | ✅ |
| 12 | app/_layout.tsx — Root layout | ✅ |
| 13 | app/+not-found.tsx | ✅ |
| 14 | app/(auth)/_layout.tsx | ✅ |
| 15 | app/(auth)/login.tsx | ✅ |
| 16 | app/(auth)/register.tsx | ✅ |
| 17 | app/(tabs)/_layout.tsx | ✅ |
| 18 | app/(tabs)/index.tsx | ✅ |
| 19 | app/(tabs)/profile.tsx | ✅ |
| 20 | tests/authStore.test.ts | ✅ |
| 21 | .env.example | ✅ |
| 22 | README.md | ✅ |

---

---

## Phase 2-B: react-native-guidelines 스킬 작성 (현재 작업)

> **병렬 세션 작업 — 담당 파일: `.claude/skills/react-native-guidelines/`**
> `skill-rules.json`, `README.md`, 루트 `docs/`는 수정 금지.

| # | 항목 | 상태 |
|---|---|---|
| 1 | base/react-native/ 전체 코드 분석 (api.ts, authStore.ts, 레이아웃 패턴) | ✅ |
| 2 | base/react-native/docs/ 작업 계획 3문서 업데이트 | ✅ |
| 3 | `.claude/skills/react-native-guidelines/SKILL.md` 작성 | ✅ |
| 4 | SKILL.md DoD 검증 (500줄 이하, frontmatter, 패턴 3개+, 트리거 5개+, 안티패턴) | ✅ |

---

*Last updated: 2026-03-05 (Phase 2-B 스킬 작성 시작)*
