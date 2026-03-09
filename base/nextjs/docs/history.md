# History — base/nextjs/ 작업 맥락 문서

## 현재 상태

- **상태:** 완료 (Definition of Done 충족)
- **생성 시점:** 2026-03-01 (Phase 1-A 마이그레이션)
- **원본:** `frontend/` 디렉토리에서 범용화

---

## 생성 배경

Advanced Harness 하네스 구조를 멀티 스택 자산 라이브러리로 전환하면서,
기존 `frontend/` 디렉토리를 비즈니스 특화 요소를 제거하고 범용 스타터 킷으로 정제하여 `base/nextjs/`에 생성했다.

---

## 원본 대비 변경 사항

### 제거된 항목 (비즈니스 특화)
- `firebase`, `recharts`, `@emotion/react` 등 도메인 특화 패키지
- `artists/`, `explore/`, `messages/`, `search/`, `agent/`, `admin/` 페이지
- Firebase 인증 연동 코드 (firebase.ts, emailAuth.ts 등)
- 비즈니스 특화 컴포넌트 및 hooks
- Pretendard 폰트 (라이센스 없는 환경 고려)
- MUI Material 의존성

### 유지된 항목 (범용 패턴)
- JWT + HttpOnly 쿠키 인증 패턴 (`AuthProvider.tsx`, `session/route.ts`)
- 자동 토큰 갱신 API 클라이언트 (`api.ts`)
- 보호 라우트 미들웨어 (`middleware.ts`)
- shadcn/ui 11종 기본 컴포넌트
- 반응형 Navbar + Footer

### 표준화된 항목
- `nickname` → `username` (API 필드명 일관성, fastapi base와 맞춤)
- `app_auth_token` → `app_access_token` (쿠키명)
- 색상 변수 단순화 (브랜드 특화 색상 제거)

---

## 주요 결정 사항

| 결정 | 이유 |
|---|---|
| Firebase 제거 | 범용 스타터는 이메일/비밀번호 JWT 인증만 포함, Firebase는 선택적 추가 사항 |
| Pretendard 제거 | 시스템 기본 폰트 사용 — 프로젝트별 폰트 직접 추가 |
| MUI 제거 | shadcn/ui만으로 충분한 컴포넌트 세트 구성 |
| `username` 표준화 | `base/fastapi/`의 User 모델과 필드명 일치 |

---

## 다음 세션에서 이어갈 작업

없음. 완료 상태.

Phase 1-B에서 7개 신규 스택 생성 시 이 `base/nextjs/`를 참조 예시로 활용 가능.

---

*Last updated: 2026-03-01*
