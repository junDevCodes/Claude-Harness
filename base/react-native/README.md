# base/react-native

> Expo SDK 52 + TypeScript + Expo Router v4 + Zustand + SecureStore 기반 React Native 스타터 킷.
> JWT 인증(access + refresh), 탭 네비게이션, Axios 자동 갱신 인터셉터 포함.

---

## 기술 스택

| 구분 | 기술 |
|---|---|
| Runtime | Expo SDK ~52.0.0 |
| Language | TypeScript (strict) |
| Router | Expo Router v4 (파일 기반) |
| State | Zustand v5 |
| HTTP | Axios (JWT 자동 갱신 인터셉터) |
| Storage | expo-secure-store |
| Test | jest-expo + @testing-library/react-native |

---

## 디렉토리 구조

```
base/react-native/
├── app/
│   ├── _layout.tsx             # Root layout (초기화, SplashScreen)
│   ├── +not-found.tsx
│   ├── (auth)/
│   │   ├── _layout.tsx         # 인증 완료 시 (tabs) redirect
│   │   ├── login.tsx
│   │   └── register.tsx
│   └── (tabs)/
│       ├── _layout.tsx         # 미인증 시 login redirect
│       ├── index.tsx           # Home
│       └── profile.tsx         # Profile + Logout
├── components/
│   └── ui/
│       ├── Button.tsx          # primary / outline / destructive
│       ├── Input.tsx           # label, error 지원
│       └── LoadingScreen.tsx
├── store/
│   └── authStore.ts            # Zustand auth 상태
├── lib/
│   ├── api.ts                  # Axios + 401 intercept
│   └── secureStore.ts          # SecureStore 래퍼
├── hooks/
│   └── useAuth.ts
├── types/index.ts
├── constants/index.ts
├── tests/
│   └── authStore.test.ts
├── docs/                       # 4문서 체계
├── app.json
├── package.json
├── tsconfig.json
├── babel.config.js
└── .env.example
```

---

## 빠른 시작

### 1. 의존성 설치

```bash
npm install
# 또는
yarn install
```

### 2. 환경 변수 설정

```bash
cp .env.example .env
# .env 파일에서 EXPO_PUBLIC_API_URL 수정
```

### 3. 개발 서버 실행

```bash
npx expo start
```

QR 코드 스캔 (Expo Go 앱) 또는 시뮬레이터로 확인.

### 4. 플랫폼별 실행

```bash
npx expo start --ios
npx expo start --android
npx expo start --web
```

---

## 테스트 실행

```bash
npm test
```

---

## 커스터마이징 포인트

### 1. API 서버 URL 변경

`.env` 파일에서 수정:

```env
EXPO_PUBLIC_API_URL=https://your-api.example.com/api/v1
```

또는 `app.json`의 `extra.apiUrl` 수정.

### 2. 앱 이름 / 번들 ID 변경

`app.json`:

```json
{
  "expo": {
    "name": "MyApp",
    "slug": "my-app",
    "scheme": "myapp",
    "ios": { "bundleIdentifier": "com.mycompany.myapp" },
    "android": { "package": "com.mycompany.myapp" }
  }
}
```

### 3. 새 탭 추가

`app/(tabs)/` 에 파일 추가 후 `app/(tabs)/_layout.tsx`의 `<Tabs.Screen>` 항목 추가:

```tsx
<Tabs.Screen
  name="settings"
  options={{
    title: '설정',
    tabBarIcon: ({ color, size }) => (
      <Ionicons name="settings-outline" size={size} color={color} />
    ),
  }}
/>
```

### 4. 새 인증 스크린 추가

`app/(auth)/` 에 파일 추가 (자동으로 Stack에 등록됨).

### 5. 색상 / 테마 변경

`constants/index.ts`의 `COLORS` 수정.

### 6. User 타입 확장

`types/index.ts`의 `User` 인터페이스에 필드 추가:

```typescript
export interface User {
  id: number;
  email: string;
  username: string;
  created_at: string;
  // 추가 필드
  avatar_url?: string;
  role?: 'user' | 'admin';
}
```

---

## 인증 흐름

```
앱 시작
  └─ useAuthStore.initialize()
       ├─ SecureStore에서 토큰 없음 → (auth)/login
       ├─ 토큰 있음 → GET /auth/me
       │    ├─ 성공 → (tabs) (인증 상태 유지)
       │    └─ 실패 (토큰 만료 등) → SecureStore 클리어 → (auth)/login
       └─ SplashScreen.hideAsync()

로그인 성공
  └─ POST /auth/login
       └─ 토큰 SecureStore 저장 → Zustand 상태 갱신 → (tabs)

API 요청 중 401
  └─ Axios interceptor
       └─ POST /auth/refresh
            ├─ 성공 → 새 토큰 저장 → 원본 요청 재시도
            └─ 실패 → clearAuth() → (auth)/login
```

---

## 백엔드 API 스펙

이 베이스 코드가 기대하는 엔드포인트:

| Method | Path | 설명 |
|---|---|---|
| POST | /auth/login | 로그인 → `{ user, access_token, refresh_token }` |
| POST | /auth/register | 회원가입 → `{ user, access_token, refresh_token }` |
| GET | /auth/me | 현재 사용자 정보 → `User` |
| POST | /auth/refresh | 토큰 갱신 → `{ access_token, refresh_token }` |
| POST | /auth/logout | 로그아웃 |

`base/fastapi/` 또는 `base/django/`와 함께 사용할 수 있습니다.

---

## EAS Build (배포)

```bash
npm install -g eas-cli
eas login
eas build --platform android
eas build --platform ios
```

`app.json`의 `extra.eas.projectId`를 실제 EAS 프로젝트 ID로 교체하세요.
