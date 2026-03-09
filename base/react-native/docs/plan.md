# Plan — base/react-native/ 베이스 코드 구성 계획

> **문서 역할:** 이 스택 베이스 코드의 전체 방향과 목표를 관리하는 기획서.

---

## 목표

Expo SDK 52 + TypeScript + Expo Router v4 + Zustand + SecureStore 기반의
범용 React Native 스타터 킷 생성.

JWT 인증(access + refresh token), 파일 기반 네비게이션, Axios 자동 갱신
인터셉터를 포함하는 최소 완성 구조.

---

## 기술 스택

| 구분 | 기술 | 버전 |
|---|---|---|
| Runtime | Expo SDK | ~52.0.0 |
| Language | TypeScript | ^5.3.0 |
| Router | Expo Router | ~4.0.0 |
| State | Zustand | ^5.0.0 |
| HTTP | Axios | ^1.7.0 |
| Storage | expo-secure-store | ~14.0.0 |
| Test | jest-expo + @testing-library/react-native | ~52.0.0 |

---

## 디렉토리 구조

```
base/react-native/
├── app/
│   ├── _layout.tsx           # Root layout (초기화, SplashScreen)
│   ├── +not-found.tsx
│   ├── (auth)/
│   │   ├── _layout.tsx       # 인증 완료 시 (tabs)로 redirect
│   │   ├── login.tsx
│   │   └── register.tsx
│   └── (tabs)/
│       ├── _layout.tsx       # 미인증 시 login으로 redirect
│       ├── index.tsx         # Home
│       └── profile.tsx       # Profile + Logout
├── components/
│   └── ui/
│       ├── Button.tsx
│       ├── Input.tsx
│       └── LoadingScreen.tsx
├── store/
│   └── authStore.ts          # Zustand auth 상태 관리
├── lib/
│   ├── api.ts                # Axios + 401 intercept + token refresh
│   └── secureStore.ts        # SecureStore 래퍼
├── hooks/
│   └── useAuth.ts
├── types/
│   └── index.ts
├── constants/
│   └── index.ts
├── tests/
│   └── authStore.test.ts
├── docs/                     # 4문서 체계
├── app.json
├── package.json
├── tsconfig.json
├── babel.config.js
├── .env.example
└── README.md
```

---

## 핵심 패턴

### 1. 인증 흐름

```
앱 시작
  → useAuthStore.initialize()
  → SecureStore에서 토큰 읽기
  → GET /auth/me 검증
  → 성공: (tabs)로 이동 / 실패: (auth)/login으로 이동
```

### 2. 토큰 갱신 (Axios Interceptor)

- Response 401 감지 → refreshToken으로 POST /auth/refresh 호출
- 갱신 성공: 새 token SecureStore 저장 + 원본 요청 재시도
- 갱신 실패: SecureStore 클리어 → 로그인 화면 이동
- 동시 다중 401: Queue 패턴으로 refresh 1회만 실행

### 3. Zustand auth store

| 액션 | 역할 |
|---|---|
| `initialize()` | 앱 시작 시 저장된 토큰 검증 |
| `login(data)` | 이메일/비밀번호 로그인 |
| `register(data)` | 회원가입 |
| `logout()` | 로그아웃 + SecureStore 클리어 |
| `setTokens(tokens)` | 토큰 저장 (store + SecureStore) |
| `clearAuth()` | 인증 상태 초기화 |

---

## 완성 기준 (Definition of Done)

- [ ] docs/ 4문서 존재
- [ ] JWT 인증 구현 (SecureStore 연동)
- [ ] 기본 화면 예시 (login, register, home, profile)
- [ ] `.env.example` 존재
- [ ] `README.md` 존재
- [ ] 테스트 템플릿 1개 이상

---

*Created: 2026-03-02*
