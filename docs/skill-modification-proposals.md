# Skill Modification Proposals — 기술스택별 수정 제안서

> **문서 역할:** 전체 24개 스킬 품질 감사 결과에 기반한 **스킬별 상세 수정 제안서 + 체크리스트**.
> **작성 기준일:** 2026-03-05
> **감사 결과 참조:** `docs/skill-quality-audit-report.md`
> **수정 대상:** P1 6개 + P2 7개 = 총 13개 스킬

---

## 목차

1. [Python Backend](#1-python-backend)
   - 1-1. fastapi-backend-guidelines (P1)
   - 1-2. pytest-backend-testing (P1)
   - 1-3. django-backend-guidelines (P2)
2. [JavaScript/TypeScript Frontend](#2-javascripttypescript-frontend)
   - 2-1. vercel-react-best-practices (P1)
3. [Error Tracking](#3-error-tracking)
   - 3-1. error-tracking (P2)
4. [Design & UI](#4-design--ui)
   - 4-1. web-design-guidelines (P1)
   - 4-2. frontend-design (P1)
   - 4-3. brand-guidelines (P1)
5. [Document Generation](#5-document-generation)
   - 5-1. pdf (P2)
   - 5-2. pptx (P2)
   - 5-3. docx (P2)
6. [Meta / Tooling](#6-meta--tooling)
   - 6-1. skill-developer (P2)

---

## 1. Python Backend

---

### 1-1. `fastapi-backend-guidelines` — P1 수정

**현재 상태:** 667줄 | 등급 C | P1 2건

#### 문제 분석

| 이슈 | 심각도 | 상세 |
|---|---|---|
| 500줄 초과 (667줄, +167) | **P1** | resources/ 10개 파일 존재하나 SKILL.md 본체 미압축 |
| 안티패턴 섹션 없음 | **P1** | 8+ 패턴 존재, 안티패턴 0개 |
| 이모지 섹션 제목 | P3 | `### 🏗️ Layered Architecture` 등 — 다른 스킬과 불일치 |

#### 수정 제안

**A. 줄수 압축 (667줄 → ~480줄 목표)**

| 이동 대상 | 현재 위치 | 줄수 | 이동 목적지 |
|---|---|---|---|
| "Quick Reference: New Domain Template" | lines 559-655 | **97줄** | `resources/domain-template.md` (신규) |
| "Common Imports Cheatsheet" | lines 140-170 | **31줄** | `resources/imports-cheatsheet.md` (신규) |
| "Navigation Guide" 테이블 | lines 524-537 | **14줄** | 제거 (각 Topic Guide에 이미 링크 존재) |
| "Core Principles" 목록 | lines 541-555 | **15줄** | `resources/core-principles.md`로 이동 또는 5개로 압축 |
| 이모지 제거 | 전체 | ~0줄 | 각 `###` 제목에서 이모지만 제거 |

**예상 절감:** 97 + 31 + 14 + 10 = **~152줄 절감** → 667 - 152 = **~515줄**
**추가 압축:** Topic Guide 코드 예시 중 긴 블록 15줄 압축 → **~500줄**

**B. 안티패턴 섹션 추가 (~20줄)**

이동으로 확보된 공간에 안티패턴 추가. 아래 내용을 `## Anti-Patterns` 섹션으로 작성:

```markdown
## Anti-Patterns

### 1. async 라우트에서 sync 블로킹 호출
- BAD: `time.sleep()`, `requests.get()` in async def
- GOOD: `await asyncio.sleep()`, `httpx.AsyncClient()`

### 2. Router에서 Repository 직접 호출
- BAD: router → repository (Service 계층 우회)
- GOOD: router → service → repository

### 3. Session 관리 미흡
- BAD: `session = get_session()` 후 close 안 함
- GOOD: `async with get_write_session() as session:`

### 4. raw SQL 사용
- BAD: `session.exec(text("SELECT * FROM users"))`
- GOOD: `session.exec(select(User).where(User.id == id))`

### 5. 하드코딩 ID
- BAD: `id: int = Field(primary_key=True)`
- GOOD: `id: str = Field(default_factory=lambda: str(ULID()), primary_key=True)`
```

#### 체크리스트

- [ ] "New Domain Template" (lines 559-655) → `resources/domain-template.md`로 이동
- [ ] "Common Imports Cheatsheet" (lines 140-170) → `resources/imports-cheatsheet.md`로 이동
- [ ] "Navigation Guide" (lines 524-537) 제거 또는 3줄로 압축
- [ ] "Core Principles" (lines 541-555) 5개로 압축
- [ ] 이모지 제목 제거 (`🏗️`, `🛣️`, `🗄️`, `📦`, `🔄`, `💾`, `📝`, `⚡`, `🚨`, `📚`)
- [ ] `## Anti-Patterns` 섹션 추가 (5개 항목, BAD/GOOD 코드 포함)
- [ ] 최종 줄수 확인: **500줄 이하**
- [ ] YAML frontmatter 변경 없음 확인
- [ ] 기존 resources/ 10개 파일 링크 유지 확인
- [ ] 이동된 2개 신규 resources/ 파일에 SKILL.md 링크 추가

---

### 1-2. `pytest-backend-testing` — P1 수정

**현재 상태:** 531줄 | 등급 C | P1 2건 + P2 1건

#### 문제 분석

| 이슈 | 심각도 | 상세 |
|---|---|---|
| 500줄 초과 (531줄, +31) | **P1** | resources/ 7개 파일 존재하나 Test Template 인라인 |
| 안티패턴 섹션 없음 | **P1** | 6+ 패턴 존재, 안티패턴 0개 |
| "qwarty" 프로젝트 참조 | P2 | lines 490-520 "Current Project Configuration" |

#### 수정 제안

**A. 줄수 압축 (531줄 → ~440줄 목표)**

| 이동 대상 | 현재 위치 | 줄수 | 이동 목적지 |
|---|---|---|---|
| "Quick Reference: Test Template" | lines 393-486 | **94줄** | `resources/test-template.md` (신규) |
| "Current Project Configuration" | lines 490-520 | **31줄** | **삭제** (프로젝트 특정, 범용 하네스 부적합) |

**예상 절감:** 94 + 31 = **~125줄 절감** → 531 - 125 = **~406줄**

**B. 안티패턴 섹션 추가 (~25줄)**

```markdown
## Anti-Patterns

### 1. 테스트 간 공유 가변 상태
- BAD: 모듈 레벨 리스트에 테스트 데이터 누적
- GOOD: 각 테스트에서 fixture로 독립 데이터 생성

### 2. 구현 세부사항 테스트
- BAD: private 메서드 호출 횟수 assert
- GOOD: public API의 입출력 결과만 assert

### 3. 단위 테스트에서 실제 DB 사용
- BAD: 실제 PostgreSQL 세션으로 unit test
- GOOD: AsyncMock 세션 + in-memory fixture

### 4. 에러 경로 미테스트
- BAD: happy path만 테스트
- GOOD: 404, 401, 409, 500 등 에러 시나리오 별도 테스트

### 5. 과도한 모킹 (over-mocking)
- BAD: 모든 의존성을 Mock — 실제 동작 미검증
- GOOD: Repository만 Mock, Service 로직은 실제 실행
```

#### 체크리스트

- [ ] "Quick Reference: Test Template" (lines 393-486) → `resources/test-template.md`로 이동
- [ ] "Current Project Configuration" (lines 490-520) 삭제
- [ ] SKILL.md에 `resources/test-template.md` 참조 링크 추가
- [ ] `## Anti-Patterns` 섹션 추가 (5개 항목, BAD/GOOD 포함)
- [ ] 이모지 제목 제거 (`🏗️`, `🧪`, `🔗`, `⚡`, `🎭`, `📊`, `🚀`)
- [ ] "qwarty" 관련 문자열 전체 검색 후 제거/일반화
- [ ] 최종 줄수 확인: **500줄 이하** (목표 ~430줄)
- [ ] 기존 resources/ 7개 파일 링크 유지 확인
- [ ] 신규 `resources/test-template.md` 파일 생성 확인

---

### 1-3. `django-backend-guidelines` — P2 수정

**현재 상태:** 500줄 | 등급 A | P2 1건

#### 문제 분석

| 이슈 | 심각도 | 상세 |
|---|---|---|
| description 키워드 삽입 | P2 | description 끝에 `Django project, DRF, DRF viewset, serializer, APIView, SimpleJWT, token blacklist, AbstractBaseUser, UserManager, DJANGO_SETTINGS_MODULE, makemigrations, migrate, pytest-django, gunicorn, psycopg` 키워드 목록 삽입 |

#### 수정 제안

description 필드를 자연어 요약으로 정리. 키워드 중 triggers에 없는 것만 triggers에 추가.

**현재 description (1줄):**
```
Django 5 + DRF + SimpleJWT 백엔드 개발 가이드라인. AbstractBaseUser 커스텀 유저 모델, DRF ModelViewSet APIView, SimpleJWT 인증(access+refresh+blacklist), settings/ 환경 분리(base/development/production), 커스텀 예외 핸들러 패턴. Django REST Framework 뷰, 시리얼라이저, 권한 클래스, 페이지네이션 작성 시 사용. Django project, DRF, DRF viewset, serializer, APIView, SimpleJWT, token blacklist, AbstractBaseUser, UserManager, DJANGO_SETTINGS_MODULE, makemigrations, migrate, pytest-django, gunicorn, psycopg.
```

**수정안 description:**
```
Django 5 + DRF + SimpleJWT 백엔드 개발 가이드라인. AbstractBaseUser 커스텀 유저 모델, DRF ModelViewSet/APIView, SimpleJWT 인증(access+refresh+blacklist), settings/ 환경 분리, 커스텀 예외 핸들러 패턴.
```

**수정안 triggers (기존 12개 → 16개):**
```yaml
triggers:
  - django
  - drf
  - django rest framework
  - simplejwt
  - ModelViewSet
  - APIView
  - serializer
  - AbstractBaseUser
  - custom user model
  - django 백엔드
  - makemigrations
  - pytest-django
  - gunicorn          # 추가
  - UserManager       # 추가
  - token blacklist   # 추가
  - psycopg           # 추가
```

#### 체크리스트

- [ ] description 필드에서 키워드 목록 제거 → 자연어 요약만 유지
- [ ] triggers에 `gunicorn`, `UserManager`, `token blacklist`, `psycopg` 4개 추가
- [ ] 줄수 변화 확인 (500줄 유지 또는 감소)
- [ ] 기존 패턴/안티패턴/코드 예시 변경 없음 확인

---

## 2. JavaScript/TypeScript Frontend

---

### 2-1. `vercel-react-best-practices` — P1 수정

**현재 상태:** 147줄 | 등급 C | P1 2건

#### 문제 분석

| 이슈 | 심각도 | 상세 |
|---|---|---|
| 코드 예시 0개 | **P1** | 57개 규칙명 나열만, 코드 전무. SKILL.md 단독 가치 = 0 |
| 안티패턴 섹션 없음 | **P1** | 규칙 목록만 존재 |
| AGENTS.md 파일명 오해 소지 | P3 | 에이전트 정의 파일로 오인 가능 |

#### 수정 제안

SKILL.md를 **인덱스 + 핵심 패턴 인라인** 구조로 전면 재구성.
`rules/` 디렉토리에서 가장 임팩트 높은 5개 규칙을 추출하여 SKILL.md에 인라인.

**재구성 구조 (목표 ~350줄):**

```
---
(YAML frontmatter 유지)
---

# Vercel React Best Practices

## Purpose
## When to Use This Skill

## 핵심 패턴 (Top 5 — 인라인)

### Pattern 1: Waterfall 제거 — Promise.all 병렬 처리
  (BAD: 순차 await / GOOD: Promise.all 코드 예시)

### Pattern 2: 배럴 임포트 회피 — 번들 사이즈 최적화
  (BAD: import from 'components' / GOOD: import from 'components/Button')

### Pattern 3: React.memo + useMemo 최적화
  (BAD: 매 렌더 새 객체 / GOOD: useMemo로 참조 안정화)

### Pattern 4: 동적 임포트 — 코드 스플리팅
  (BAD: 정적 import 대형 컴포넌트 / GOOD: dynamic(() => import(...)))

### Pattern 5: Server Component 데이터 페칭
  (BAD: 클라이언트 useEffect / GOOD: RSC에서 직접 fetch)

## Anti-Patterns (5개, BAD/GOOD 코드)

### 1. 클라이언트 컴포넌트에서 데이터 워터폴
### 2. 배럴 파일(index.ts)에서 전체 re-export
### 3. 불필요한 상태 끌어올리기 (prop drilling)
### 4. useEffect 내 데이터 페칭 (RSC 가능 시)
### 5. memo 없이 큰 리스트 렌더링

## 전체 규칙 카탈로그 (57개 — 요약 테이블)
  (현재 lines 34-128 내용 유지, 카테고리별 규칙명 + 우선순위)

## Resources
  → rules/ 디렉토리 참조 안내
  → AGENTS.md → COMPILED_RULES.md로 파일명 변경 권장
```

#### 체크리스트

- [ ] `rules/` 디렉토리에서 Top 5 규칙 파일 읽기 및 핵심 코드 추출
- [ ] SKILL.md에 5개 핵심 패턴 인라인 (각 패턴 BAD/GOOD 코드 포함)
- [ ] `## Anti-Patterns` 섹션 추가 (5개 항목)
- [ ] 기존 규칙 카탈로그 (57개 목록) 유지
- [ ] `AGENTS.md` → `COMPILED_RULES.md`로 파일명 변경 (P3, 선택)
- [ ] 최종 줄수 확인: **500줄 이하** (목표 ~350줄)
- [ ] frontmatter 유지 (triggers 10개)
- [ ] `rules/` 디렉토리 참조 링크 정상 확인

---

## 3. Error Tracking

---

### 3-1. `error-tracking` — P2 수정

**현재 상태:** 383줄 | 등급 B | P2 4건

#### 문제 분석

| 이슈 | 심각도 | 상세 |
|---|---|---|
| deprecated API | P2 | line 321 `Sentry.startTransaction()` — v8에서 deprecated |
| 프로젝트 특정 콘텐츠 | P2 | "Form Service", "Email Service", "189 calls remaining", `/blog-api/` 경로 |
| 안티패턴 코드 없음 | P2 | "Common Mistakes to Avoid" (lines 333-340) — NEVER 목록만, BAD/GOOD 코드 없음 |
| Key Files 프로젝트 경로 | P2 | lines 357-371 `/blog-api/src/...`, `/notifications/src/...` |

#### 수정 제안

**A. deprecated API 교체**

```
현재 (line 321): Sentry.startTransaction({ name: '...', op: '...' })
수정: Sentry.startSpan({ name: '...', op: '...' }, (span) => { ... })
```

**B. 프로젝트 특정 콘텐츠 일반화**

| 현재 | 수정안 |
|---|---|
| "Form Service" / "Email Service" | "Service A" / "Service B" 또는 제거 |
| "Current Status" 섹션 (lines 31-41) | 전체 삭제 (진행률 추적 = 프로젝트 특정) |
| "189 ErrorLogger.log() calls remaining" | 삭제 |
| `/blog-api/src/...`, `/notifications/src/...` | `src/controllers/`, `src/services/` 등 범용 경로 |
| "Form Service Test Endpoints" (lines 273-287) | 범용 테스트 예시로 대체 |
| "Email Service Test Endpoints" (lines 289-300) | 범용 테스트 예시로 대체 또는 삭제 |

**C. 안티패턴 섹션 보강**

현재 "Common Mistakes to Avoid" (lines 333-340)를 `## Anti-Patterns`로 리네이밍하고 코드 추가:

```markdown
## Anti-Patterns

### 1. catch 블록에서 Sentry 미전송
- BAD: `catch (e) { console.error(e); }`
- GOOD: `catch (e) { Sentry.captureException(e); throw e; }`

### 2. 전역 try-catch로 모든 에러 뭉개기
- BAD: 최상위 catch에서 200 반환
- GOOD: 에러별 분기 + Sentry에 context 포함

### 3. PII 데이터 Sentry 전송
- BAD: `Sentry.setUser({ email, password, ... })`
- GOOD: `Sentry.setUser({ id: userId })` (최소 정보만)
```

#### 체크리스트

- [ ] `Sentry.startTransaction()` → `Sentry.startSpan()` 교체 (line 321 부근)
- [ ] "Current Status" 섹션 (lines 31-41) 삭제
- [ ] "189 ErrorLogger.log() calls remaining" 삭제
- [ ] "Form Service" / "Email Service" → 범용 서비스명으로 일반화
- [ ] "Key Files" 프로젝트 경로 → 범용 경로로 교체
- [ ] "Service-Specific Integration" 섹션 → "Integration Examples"로 리네이밍 + 일반화
- [ ] "Testing Sentry Integration" → 범용 curl 예시로 교체
- [ ] "Common Mistakes to Avoid" → `## Anti-Patterns`로 리네이밍 + BAD/GOOD 코드 추가
- [ ] 최종 줄수 확인: 500줄 이하 유지
- [ ] Sentry v8 API 전체 일관성 확인 (`startSpan`, `captureException`, `withScope`)

---

## 4. Design & UI

---

### 4-1. `web-design-guidelines` — P1 수정

**현재 상태:** 48줄 | 등급 C | P1 2건

#### 문제 분석

| 이슈 | 심각도 | 상세 |
|---|---|---|
| 코드 예시 0개 | **P1** | 외부 URL WebFetch 위임만, 자체 콘텐츠 없음 |
| 안티패턴 섹션 없음 | **P1** | 완전 부재 |
| URL 의존성 | P2 | GitHub raw URL 접근 불가 시 스킬 가치 = 0 |

#### 수정 제안

기존 URL 위임 방식 유지하되, **fallback 인라인 콘텐츠**를 추가하여 자체 가치 확보.

**재구성 구조 (목표 ~280줄):**

```
---
(YAML frontmatter 유지 — triggers에 "semantic HTML", "responsive" 추가)
---

# Web Interface Guidelines

## Purpose
## When to Use This Skill

## 핵심 패턴 (3개 — 인라인 fallback)

### Pattern 1: 시맨틱 HTML + ARIA
  - BAD: `<div onclick>` / GOOD: `<button>` + `aria-label`
  - 코드 예시: nav, main, article, aside 구조

### Pattern 2: 반응형 레이아웃
  - BAD: 고정 px width / GOOD: CSS Grid + clamp()
  - 코드 예시: 미디어 쿼리 없는 반응형 Grid

### Pattern 3: 색상 대비 + 포커스 표시
  - BAD: `outline: none` / GOOD: `:focus-visible` 스타일
  - WCAG AA 4.5:1 대비비 예시

## Anti-Patterns (5개)

### 1. div 남용 (비시맨틱 마크업)
### 2. outline: none (포커스 표시 제거)
### 3. 고정 font-size px (접근성 위반)
### 4. color만으로 정보 전달 (색맹 미고려)
### 5. 자동재생 미디어 (모션 민감 사용자)

## 전체 가이드라인 (외부 참조)
  → WebFetch URL 유지 (기존 방식)

## Quick Checklist
  - WCAG AA 체크 항목 요약
```

#### 체크리스트

- [ ] 핵심 패턴 3개 인라인 추가 (시맨틱 HTML, 반응형, 색상 대비)
- [ ] 각 패턴에 HTML/CSS 코드 예시 (BAD/GOOD) 포함
- [ ] `## Anti-Patterns` 섹션 추가 (5개 항목)
- [ ] 기존 WebFetch URL 위임 방식 유지 (삭제하지 않음)
- [ ] triggers에 `semantic HTML`, `responsive design` 추가
- [ ] 최종 줄수 확인: **500줄 이하** (목표 ~280줄)
- [ ] WCAG AA 기준 수치 정확성 확인 (4.5:1 대비비)

---

### 4-2. `frontend-design` — P1 수정

**현재 상태:** 51줄 | 등급 B- | P1 2건

#### 문제 분석

| 이슈 | 심각도 | 상세 |
|---|---|---|
| 코드 예시 0개 | **P1** | 디자인 철학만 산문, CSS/HTML/React 코드 전무 |
| 안티패턴 전용 섹션 없음 | **P1** | lines 45-47에 인라인 NEVER만 존재 |

#### 수정 제안

기존 디자인 철학 텍스트 유지 + **코드 패턴 3개 추가** + **안티패턴 분리**.

**재구성 구조 (목표 ~320줄):**

```
---
(YAML frontmatter 유지)
---

# Frontend Design Guidelines

## Design Philosophy (기존 lines 16-51 유지)

## 핵심 패턴 (3개 — 코드 포함)

### Pattern 1: 타이포그래피 스케일
  - CSS custom properties로 타입 스케일 정의
  ```css
  :root {
    --font-xs: clamp(0.75rem, 0.7rem + 0.25vw, 0.875rem);
    --font-sm: clamp(0.875rem, 0.8rem + 0.35vw, 1rem);
    --font-base: clamp(1rem, 0.9rem + 0.5vw, 1.125rem);
    --font-lg: clamp(1.25rem, 1.1rem + 0.75vw, 1.5rem);
    --font-xl: clamp(1.5rem, 1.2rem + 1.5vw, 2.25rem);
  }
  ```

### Pattern 2: 색상 팔레트 시스템
  - CSS variables + Tailwind 통합
  ```css
  :root {
    --color-surface: #fafafa;
    --color-text: #1a1a2e;
    --color-primary: #e94560;
    --color-accent: #0f3460;
  }
  ```

### Pattern 3: 모션 + 트랜지션
  - prefers-reduced-motion 존중
  ```css
  @media (prefers-reduced-motion: no-preference) {
    .card { transition: transform 0.2s ease, box-shadow 0.2s ease; }
    .card:hover { transform: translateY(-2px); box-shadow: 0 8px 24px rgba(0,0,0,0.12); }
  }
  ```

## Anti-Patterns (기존 인라인 NEVER 분리 + 확장)

### 1. 제네릭 AI 미학 사용
  - BAD: 그라디언트 카드 + 둥근 모서리 + 보라-파랑 기본 테마
  - GOOD: 프로젝트 고유 색상 + 의도적 타이포그래피

### 2. 공통 선택 수렴
  - BAD: 모든 카드에 동일한 `rounded-xl shadow-lg`
  - GOOD: 콘텐츠에 맞는 차별화된 레이아웃

### 3. prefers-reduced-motion 무시
  - BAD: 모든 사용자에게 애니메이션 강제
  - GOOD: @media query로 모션 민감 사용자 존중

### 4. 과도한 장식 요소
  - BAD: 불필요한 그라디언트, 그림자, 보더
  - GOOD: 콘텐츠 우선 디자인, 여백으로 구조 표현
```

#### 체크리스트

- [ ] 기존 디자인 철학 텍스트 (lines 16-51) 유지
- [ ] 핵심 패턴 3개 추가 (타이포그래피, 색상, 모션) — 각 CSS 코드 예시 포함
- [ ] lines 45-47 인라인 NEVER → `## Anti-Patterns` 전용 섹션으로 분리
- [ ] 안티패턴 4개 이상 (BAD/GOOD 설명 포함)
- [ ] 최종 줄수 확인: **500줄 이하** (목표 ~320줄)
- [ ] 기존 triggers 8개 유지 확인

---

### 4-3. `brand-guidelines` — P1 수정

**현재 상태:** 81줄 | 등급 C+ | P1 2건

#### 문제 분석

| 이슈 | 심각도 | 상세 |
|---|---|---|
| 코드 예시 0개 | **P1** | 색상값 텍스트 나열만, 적용 코드 없음 |
| 안티패턴 섹션 없음 | **P1** | 브랜드 오용 가이드 부재 |
| RGBColor 언급 무코드 | P2 | line 80 "python-pptx's RGBColor class" 코드 없이 언급 |

#### 수정 제안

기존 브랜드 정보 유지 + **3가지 기술 스택별 적용 코드** + **안티패턴**.

**재구성 구조 (목표 ~200줄):**

```
---
(YAML frontmatter 유지)
---

# Anthropic Brand Styling

## Brand Guidelines (기존 Colors, Typography 유지)

## 적용 패턴 (3개 — 코드 포함)

### Pattern 1: CSS Custom Properties
  ```css
  :root {
    --brand-bg: #faf9f5;
    --brand-text: #141413;
    --brand-accent: #E87B35;
    --brand-secondary: #D4A27F;
    --font-family: 'Styrene A', -apple-system, sans-serif;
  }
  .brand-heading { color: var(--brand-text); font-family: var(--font-family); }
  .brand-cta { background: var(--brand-accent); color: var(--brand-bg); }
  ```

### Pattern 2: Python python-pptx
  ```python
  from pptx.util import Pt
  from pptx.dml.color import RGBColor

  BRAND_COLORS = {
      "bg": RGBColor(0xFA, 0xF9, 0xF5),
      "text": RGBColor(0x14, 0x14, 0x13),
      "accent": RGBColor(0xE8, 0x7B, 0x35),
  }

  def apply_brand_font(run, size=12):
      run.font.name = "Styrene A"
      run.font.size = Pt(size)
      run.font.color.rgb = BRAND_COLORS["text"]
  ```

### Pattern 3: Tailwind CSS Config
  ```js
  // tailwind.config.js
  module.exports = {
    theme: {
      extend: {
        colors: {
          brand: { bg: '#faf9f5', text: '#141413', accent: '#E87B35', secondary: '#D4A27F' }
        },
        fontFamily: { brand: ['"Styrene A"', 'system-ui', 'sans-serif'] }
      }
    }
  }
  ```

## Anti-Patterns

### 1. 브랜드 색상 임의 변형
  - BAD: `#E87B35` → `#FF8C00` 유사색 사용
  - GOOD: 정확한 HEX 값 사용 (CSS variable 참조)

### 2. 로고 배경 위 가독성 무시
  - BAD: 밝은 배경 위에 밝은 로고 배치
  - GOOD: 대비 확보 또는 반전 로고 사용

### 3. 비브랜드 폰트 혼용
  - BAD: Styrene A + Arial 혼용
  - GOOD: Styrene A 단일 또는 시스템 폰트 fallback만
```

#### 체크리스트

- [ ] 기존 Colors / Typography 정보 유지
- [ ] 적용 패턴 3개 추가 (CSS, python-pptx, Tailwind)
- [ ] 각 패턴에 실행 가능한 코드 예시 포함
- [ ] `## Anti-Patterns` 섹션 추가 (3개 이상)
- [ ] line 80 "python-pptx's RGBColor class" → Pattern 2로 대체 (코드 포함)
- [ ] 최종 줄수 확인: **500줄 이하** (목표 ~200줄)
- [ ] 색상 HEX 값 정확성 검증 (`#faf9f5`, `#141413`, `#E87B35`, `#D4A27F`)

---

## 5. Document Generation

---

### 5-1. `pdf` — P2 수정

**현재 상태:** 325줄 | 등급 A- | P2 1건

#### 문제 분석

| 이슈 | 심각도 | 상세 |
|---|---|---|
| 안티패턴 전용 섹션 없음 | P2 | line 182 인라인 경고만 (Unicode 첨자) |

#### 수정 제안

기존 인라인 경고를 수집 + 추가 안티패턴으로 `## Anti-Patterns` 섹션 구성.

```markdown
## Anti-Patterns

### 1. Unicode 첨자 문자 사용
  - BAD: `"H₂O"` (Unicode subscript)
  - GOOD: ReportLab `<sub>2</sub>` 마크업

### 2. 대용량 PDF 한 번에 로드
  - BAD: `PdfReader("500page.pdf")` 전체 메모리 로드
  - GOOD: 페이지 범위 지정 또는 스트리밍 처리

### 3. 암호화 없이 민감 정보 PDF 생성
  - BAD: 개인정보 포함 PDF 미암호화 배포
  - GOOD: `writer.encrypt(password)` 적용
```

#### 체크리스트

- [ ] line 182 인라인 경고 유지 (원래 위치)
- [ ] `## Anti-Patterns` 전용 섹션 추가 (3개 항목)
- [ ] 줄수 확인: 500줄 이하 유지 (325 + ~20 = ~345줄)
- [ ] 기존 패턴/코드 예시 변경 없음

---

### 5-2. `pptx` — P2 수정

**현재 상태:** 497줄 | 등급 A- | P2 2건

#### 문제 분석

| 이슈 | 심각도 | 상세 |
|---|---|---|
| 안티패턴 산재 | P2 | lines 154-157, 162, 238-241, 482-486 등 산발적 |
| 500줄 근접 (497줄) | P2 | 3줄 여유 — 안티패턴 통합 시 초과 위험 |

#### 수정 제안

**옵션 A (권장):** 안티패턴 산재 유지 + 파일 상단에 안티패턴 인덱스 추가 (참조 링크)
- 이유: 497줄에서 통합 섹션 추가 시 500줄 초과. 현재 컨텍스트별 배치가 실용적.
- 파일 상단에 "Anti-Patterns Quick Index" 3줄 추가 (line 참조)

**옵션 B:** `resources/` 분리로 공간 확보 후 통합 섹션 추가
- Thumbnail/Image 변환 섹션 (lines 423-478, ~56줄) → `resources/image-conversion.md`
- 확보된 공간에 통합 안티패턴 섹션 배치

#### 체크리스트

- [ ] 옵션 선택 (A: 인덱스 추가 / B: resources 분리)
- [ ] 옵션 A 시: 상단에 Anti-Pattern 인덱스 3줄 추가
- [ ] 옵션 B 시: Image Conversion → resources/ 이동 + 통합 Anti-Patterns 섹션
- [ ] 최종 줄수 확인: **500줄 이하**

---

### 5-3. `docx` — P2 수정

**현재 상태:** 489줄 | 등급 A | P2 1건

#### 문제 분석

| 이슈 | 심각도 | 상세 |
|---|---|---|
| 500줄 근접 (489줄) | P2 | 11줄 여유 — 향후 확장 어려움 |

#### 수정 제안

현재 "Critical Rules" (lines 278-293)과 "Common Pitfalls" (lines 348-351)이 실질적 안티패턴 역할 수행.
**명시적 리네이밍**으로 DoD 안티패턴 요건 충족 가능.

```
현재: ## Critical Rules for docx-js
수정: ## Anti-Patterns & Critical Rules
```

추가 공간이 없으므로 섹션 제목만 변경하여 안티패턴 섹션 존재를 명확히 함.

#### 체크리스트

- [ ] "Critical Rules for docx-js" → "Anti-Patterns & Critical Rules"로 리네이밍
- [ ] "Common Pitfalls" → "Anti-Patterns" 하위 항목으로 통합 고려
- [ ] 줄수 변화 없음 확인 (489줄 유지)
- [ ] 기존 콘텐츠 변경 없음

---

## 6. Meta / Tooling

---

### 6-1. `skill-developer` — P2 수정

**현재 상태:** 437줄 | 등급 A | P2 2건

#### 문제 분석

| 이슈 | 심각도 | 상세 |
|---|---|---|
| 안티패턴 전용 섹션 없음 | P2 | Best Practices에 암묵적으로만 존재 |
| Quick Start 템플릿에 triggers 누락 | P2 | line 127 frontmatter에 `triggers:` 필드 없음 |

#### 수정 제안

**A. 안티패턴 섹션 추가 (~15줄)**

```markdown
## Anti-Patterns

### 1. SKILL.md 500줄 초과
  - BAD: 모든 내용을 SKILL.md에 인라인
  - GOOD: resources/ 디렉토리로 상세 내용 분리

### 2. frontmatter triggers 누락
  - BAD: name + description만 작성
  - GOOD: triggers 5개 이상 명시

### 3. 과도하게 넓은 트리거
  - BAD: triggers: ["code", "programming"]  (너무 넓어 오발동)
  - GOOD: triggers: ["nestjs guard", "nestjs module"]  (구체적)
```

**B. Quick Start 템플릿 수정**

```yaml
# 현재 (line 127):
---
name: my-new-skill
description: Brief description...
---

# 수정:
---
name: my-new-skill
description: Brief description...
triggers:
  - keyword1
  - keyword2
  - keyword3
  - keyword4
  - keyword5
---
```

#### 체크리스트

- [ ] `## Anti-Patterns` 섹션 추가 (3개 항목)
- [ ] Quick Start 템플릿에 `triggers:` 필드 추가 (5개 예시)
- [ ] "Philosophy Change (2025-10-27)" → 일반화 또는 제거 (P3, 선택)
- [ ] 최종 줄수 확인: 500줄 이하 (437 + ~20 = ~457줄)
- [ ] 기존 resources/ 6개 파일 참조 유지 확인

---

## 7. 수정 불필요 스킬 (S/A 등급 — 11개)

> 아래 스킬은 DoD 전체 충족. 수정 불필요.

| # | 스킬 | 줄수 | 등급 | 비고 |
|---|---|---|---|---|
| 1 | nestjs-backend-guidelines | 486 | S | 신규 생성, base 코드 완벽 일치 |
| 2 | docker-guidelines | 454 | S | 크로스 스택, 5패턴 + 6안티패턴 |
| 3 | aws-guidelines | 458 | S | 크로스 스택, resources/ 2파일 |
| 4 | database-guidelines | 441 | S | 최다 패턴(7) + 트리거(20) |
| 5 | express-backend-guidelines | 500 | S | base 코드 완벽 일치 |
| 6 | spring-boot-guidelines | 499 | S | jjwt 0.12.x 정확 |
| 7 | react-native-guidelines | 500 | S | Expo/Zustand/Axios 정확 |
| 8 | embedded-c-guidelines | 497 | S | HAL Ops Table + Unity 정확 |
| 9 | embedded-cpp-guidelines | 439 | A | 안티패턴 표 형식 (P3 only) |
| 10 | nextjs-frontend-guidelines | 415 | A | 풀스택 패턴, 재작성 완료 |
| 11 | mermaid | 212 | A | 23개 다이어그램 지원 |

---

## 8. 전체 수정 작업 요약

### 작업량 추정

| Tier | 대상 | 스킬 수 | 총 이슈 | 작업 규모 |
|---|---|---|---|---|
| **Tier 1 (P1)** | fastapi, pytest, vercel, web-design, frontend-design, brand | 6 | 12건 | 대규모 재구성 3 + 중규모 3 |
| **Tier 2 (P2)** | error-tracking, django, skill-developer, pdf, pptx, docx | 6 | 11건 | 중규모 1 + 소규모 5 |
| **합계** | — | **12개 스킬** | **23건** | — |

### 병렬 세션 구성 제안

```
[Tier 1 — 병렬 6세션]
  세션 1: fastapi-backend-guidelines (P1×2)     ← 중규모
  세션 2: pytest-backend-testing (P1×2 + P2×1)  ← 중규모
  세션 3: vercel-react-best-practices (P1×2)    ← 대규모 (전면 재구성)
  세션 4: web-design-guidelines (P1×2)          ← 대규모 (전면 재구성)
  세션 5: frontend-design (P1×2)                ← 대규모 (전면 재구성)
  세션 6: brand-guidelines (P1×2)               ← 소규모

[Tier 2 — 병렬 또는 순차]
  세션 7: error-tracking (P2×4)                 ← 중규모 (일반화 작업 다수)
  세션 8: django + skill-developer + pdf (P2 경미)  ← 소규모 (병합 가능)
  세션 9: pptx + docx (P2 경미)                 ← 소규모 (병합 가능)
```

### 수정 후 예상 등급

| 스킬 | 현재 | 수정 후 |
|---|---|---|
| fastapi-backend-guidelines | C | **A** |
| pytest-backend-testing | C | **A** |
| vercel-react-best-practices | C | **A** |
| web-design-guidelines | C | **A** |
| frontend-design | B- | **A** |
| brand-guidelines | C+ | **A** |
| error-tracking | B | **A** |
| django-backend-guidelines | A | **A+** |
| skill-developer | A | **A+** |
| pdf | A- | **A** |
| pptx | A- | **A** |
| docx | A | **A** |

---

*Generated: 2026-03-05 | 전체 24개 스킬 감사 기반 수정 제안서*
