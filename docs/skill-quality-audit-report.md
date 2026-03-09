# Skill Quality Audit Report & Modification Proposal

> **문서 역할:** 전체 24개 스킬에 대한 품질 감사 결과 + 수정 제안서
> **감사 기준:** DoD 6항목 (500줄 이하, YAML frontmatter, 핵심 패턴 3+, 트리거 5+, 안티패턴, 코드 정확도)
> **감사 일자:** 2026-03-05
> **감사 범위:** `.claude/skills/*/SKILL.md` 전체 24개

---

## 1. 전체 감사 결과 요약

### 등급 분포

| 등급 | 수량 | 스킬 목록 |
|---|---|---|
| **S** (DoD 완벽 충족) | 8 | nestjs, docker, aws, database, express, spring-boot, react-native, embedded-c |
| **A** (양호, 경미한 이슈) | 8 | django(A), embedded-cpp(A), nextjs(A), mermaid(A), docx(A), ppt-brand(A-), pdf(A-), pptx(A-) |
| **B** (보통, 개선 필요) | 2 | error-tracking(B), frontend-design(B-) |
| **C** (DoD 미충족, 수정 필수) | 4 | fastapi(C), pytest(C), vercel(C), web-design(C), brand(C+) |

### 이슈 총괄

| 심각도 | 건수 | 설명 |
|---|---|---|
| **P1 (Must Fix)** | **12건** | DoD 필수 항목 미충족 — 수정하지 않으면 Phase 2-D 통합 불가 |
| **P2 (Should Fix)** | **11건** | 품질/일관성 이슈 — 수정 권장 |
| **P3 (Nice to Have)** | **14건** | 개선하면 좋지만 필수는 아님 |

---

## 2. 전체 스킬 상세 감사 테이블

| # | 스킬 | 줄수 | 500줄 | FM | 패턴≥3 | 트리거≥5 | 안티패턴 | 등급 |
|---|---|---:|---|---|---|---|---|---|
| 1 | nestjs-backend-guidelines | 486 | ✅ | ✅ | 5 ✅ | 16 ✅ | 6개 ✅ | **S** |
| 2 | docker-guidelines | 454 | ✅ | ✅ | 5 ✅ | 14 ✅ | 6개 ✅ | **S** |
| 3 | aws-guidelines | 458 | ✅ | ✅ | 5 ✅ | 16 ✅ | 6개 ✅ | **S** |
| 4 | database-guidelines | 441 | ✅ | ✅ | 7 ✅ | 20 ✅ | 6개 ✅ | **S** |
| 5 | express-backend-guidelines | 500 | ✅ | ✅ | 5 ✅ | 15 ✅ | 5개 ✅ | **S** |
| 6 | spring-boot-guidelines | 499 | ✅ | ✅ | 5 ✅ | 15 ✅ | 5개+ ✅ | **S** |
| 7 | react-native-guidelines | 500 | ✅ | ✅ | 5 ✅ | 14 ✅ | 5개 ✅ | **S** |
| 8 | embedded-c-guidelines | 497 | ✅ | ✅ | 5 ✅ | 16 ✅ | 6개 ✅ | **S** |
| 9 | django-backend-guidelines | 500 | ✅ | ⚠️ | 5 ✅ | 11 ✅ | 6개 ✅ | **A** |
| 10 | embedded-cpp-guidelines | 439 | ✅ | ✅ | 4 ✅ | 14 ✅ | 10개(표) ⚠️ | **A** |
| 11 | nextjs-frontend-guidelines | 415 | ✅ | ✅ | 5 ✅ | 14 ✅ | 6개 ✅ | **A** |
| 12 | mermaid | 212 | ✅ | ✅ | 6+ ✅ | 10 ✅ | 4개 ✅ | **A** |
| 13 | docx | 489 | ✅ | ✅ | 15+ ✅ | 7 ✅ | 분산 ✅ | **A** |
| 14 | ppt-brand-guidelines | 204 | ✅ | ✅ | 4 ✅ | 6 ✅ | 12개 ✅ | **A-** |
| 15 | pdf | 325 | ✅ | ✅ | 12+ ✅ | 10 ✅ | 부분 ⚠️ | **A-** |
| 16 | pptx | 497 | ⚠️ | ✅ | 8+ ✅ | 7 ✅ | 산재 ⚠️ | **A-** |
| 17 | skill-developer | 437 | ✅ | ✅ | 4+ ✅ | 10 ✅ | 부분 ⚠️ | **A** |
| 18 | error-tracking | 383 | ✅ | ✅ | 6 ✅ | 7 ✅ | 코드無 ⚠️ | **B** |
| 19 | frontend-design | 51 | ✅ | ✅ | **0** ❌ | 8 ✅ | 인라인 ⚠️ | **B-** |
| 20 | **fastapi-backend-guidelines** | **667** | ❌ | ✅ | 8+ ✅ | 10 ✅ | **없음** ❌ | **C** |
| 21 | **pytest-backend-testing** | **531** | ❌ | ✅ | 6+ ✅ | 10 ✅ | **없음** ❌ | **C** |
| 22 | **vercel-react-best-practices** | 147 | ✅ | ✅ | **0** ❌ | 10 ✅ | **없음** ❌ | **C** |
| 23 | **web-design-guidelines** | 48 | ✅ | ✅ | **0** ❌ | 8 ✅ | **없음** ❌ | **C** |
| 24 | **brand-guidelines** | 81 | ✅ | ✅ | **0** ❌ | 7 ✅ | **없음** ❌ | **C+** |

---

## 3. P1 이슈 상세 (Must Fix — 12건)

> P1 이슈가 있는 스킬은 Phase 2-D 통합 전 반드시 수정해야 함.

### P1-01: `fastapi-backend-guidelines` — 667줄 (500줄 초과 167줄)

**현재 상태:** resources/ 디렉토리에 10개 파일 존재하나, SKILL.md 본체가 여전히 667줄
**원인 분석:**
- "New Domain Template" 섹션 (lines 559-655) — 97줄 코드 블록이 SKILL.md에 인라인
- "Common Imports Cheatsheet" (lines 140-170) — 30줄 참조표
- "Navigation Guide" 테이블 (lines 524-537) — 14줄

**수정 제안:**
1. "New Domain Template" (97줄) → `resources/domain-template.md`로 이동
2. "Common Imports Cheatsheet" (30줄) → `resources/imports-cheatsheet.md`로 이동
3. "Navigation Guide" (14줄) → `resources/` 참조로 대체
4. 예상 결과: 667 - 141 = **~526줄** → 추가 압축 26줄 필요

---

### P1-02: `fastapi-backend-guidelines` — 안티패턴 섹션 없음

**현재 상태:** 8+ 핵심 패턴이 있으나 안티패턴 섹션이 완전히 부재
**수정 제안:** 안티패턴 섹션 추가 (최소 5개, BAD/GOOD 코드 예시 포함)

```
추천 안티패턴 목록:
1. sync 블로킹 함수를 async 라우트에서 호출
2. Router에서 Repository 직접 호출 (Service 계층 우회)
3. Session 미정리 (with 블록 미사용)
4. raw SQL 사용 (SQLModel/SQLAlchemy ORM 미활용)
5. 하드코딩 ID 대신 ULID 미사용
```

---

### P1-03: `pytest-backend-testing` — 531줄 (500줄 초과 31줄)

**현재 상태:** resources/ 디렉토리에 7개 파일 존재
**원인 분석:**
- "Quick Reference: Test Template" 섹션 (lines 393-486) — 94줄 코드 블록

**수정 제안:**
1. "Quick Reference: Test Template" (94줄) → `resources/test-template.md`로 이동
2. 예상 결과: 531 - 94 = **~437줄** ✅

---

### P1-04: `pytest-backend-testing` — 안티패턴 섹션 없음

**현재 상태:** 테스트 패턴은 풍부하나 안티패턴 가이드 부재
**수정 제안:** 안티패턴 섹션 추가 (최소 5개)

```
추천 안티패턴 목록:
1. 테스트 간 공유 가변 상태 (shared mutable state)
2. 구현 세부사항 테스트 (implementation details testing)
3. 단위 테스트에서 실제 DB 사용
4. 에러 경로 미테스트
5. 과도한 모킹 (over-mocking)
```

---

### P1-05: `vercel-react-best-practices` — 코드 예시 0개

**현재 상태:** 147줄, 57개 규칙명을 나열한 인덱스/카탈로그. 코드 예시 전무.
**원인 분석:** SKILL.md가 `rules/` 디렉토리와 `AGENTS.md`(85KB)의 목차 역할만 수행
**수정 제안:**
1. `rules/` 디렉토리에서 가장 임팩트 높은 패턴 3~5개 추출하여 SKILL.md에 인라인
2. 추천 추출 대상:
   - `async-parallel` (Promise.all 병렬 처리)
   - `bundle-barrel-imports` (배럴 임포트 회피)
   - `rerender-memo` (React.memo 최적화)
   - `dynamic-import` (동적 임포트 코드 스플리팅)
3. 각 패턴에 BAD/GOOD 코드 예시 포함
4. 예상 결과: 147줄 → **~350줄**

---

### P1-06: `vercel-react-best-practices` — 안티패턴 섹션 없음

**수정 제안:** P1-05에서 추출한 패턴의 BAD 예시를 안티패턴 섹션으로 구성

---

### P1-07: `web-design-guidelines` — 코드 예시 0개

**현재 상태:** 48줄, 외부 URL(GitHub raw file)을 WebFetch하는 위임 스킬
**문제:** URL 접근 불가 시 스킬이 제공하는 가치 = 0
**수정 제안:**
1. 핵심 웹 디자인 규칙 3~5개를 SKILL.md에 인라인 (fallback 콘텐츠)
2. 추천 인라인 패턴:
   - 접근성 (ARIA, 시맨틱 HTML, 키보드 네비게이션)
   - 반응형 레이아웃 (CSS Grid/Flexbox 패턴)
   - 색상 대비 비율 (WCAG AA 4.5:1)
3. 각 패턴에 HTML/CSS 코드 예시 포함
4. 예상 결과: 48줄 → **~250줄**

---

### P1-08: `web-design-guidelines` — 안티패턴 섹션 없음

**수정 제안:** 접근성 위반 패턴, 비시맨틱 마크업, 고정 px 레이아웃 등을 안티패턴으로 추가

---

### P1-09: `frontend-design` — 코드 예시 0개

**현재 상태:** 51줄, 디자인 철학 및 원칙 설명 (산문 형태)
**문제:** 프론트엔드 디자인 스킬인데 CSS/HTML/React 코드가 전무
**수정 제안:**
1. 핵심 디자인 패턴 3개 이상을 코드 예시와 함께 추가
2. 추천 패턴:
   - 타이포그래피 스케일 (CSS custom properties)
   - 색상 팔레트 시스템 (CSS variables, Tailwind config)
   - 모션/트랜지션 (CSS animation 패턴)
   - 스페이셜 컴포지션 (Grid/Flexbox 레이아웃)
3. 기존 디자인 철학 텍스트 유지 + 코드 예시 추가
4. 예상 결과: 51줄 → **~300줄**

---

### P1-10: `frontend-design` — 안티패턴 전용 섹션 없음

**현재 상태:** Line 45-47에 인라인 "NEVER" 규칙 존재하나 전용 섹션 부재
**수정 제안:** 기존 인라인 규칙을 전용 "Anti-Patterns" 섹션으로 분리 + BAD/GOOD 코드 추가

---

### P1-11: `brand-guidelines` — 코드 예시 0개

**현재 상태:** 81줄, 브랜드 색상/타이포그래피 값만 텍스트로 나열
**문제:** Line 80에 "python-pptx's RGBColor class" 언급하면서 코드 미제공
**수정 제안:**
1. 브랜드 적용 코드 예시 3개 이상 추가
2. 추천 패턴:
   - CSS 변수 기반 색상 시스템 (`--brand-primary: #E87B35;`)
   - Python python-pptx 적용 (`RGBColor(0xE8, 0x7B, 0x35)`)
   - Tailwind CSS config 통합
3. 예상 결과: 81줄 → **~200줄**

---

### P1-12: `brand-guidelines` — 안티패턴 섹션 없음

**수정 제안:** 브랜드 오용 사례 (잘못된 색상 조합, 로고 변형, 배경 위 가독성 등) 안티패턴 추가

---

## 4. P2 이슈 상세 (Should Fix — 11건)

| # | 스킬 | 이슈 | 수정 제안 |
|---|---|---|---|
| P2-01 | `django-backend-guidelines` | description 필드에 키워드 삽입 (`Django project, DRF, DRF viewset...`) | description을 자연어 요약으로 정리, 키워드는 `triggers`로 이동 |
| P2-02 | `error-tracking` | `Sentry.startTransaction()` (line 321) — Sentry v8에서 deprecated | `Sentry.startSpan()`으로 교체 |
| P2-03 | `error-tracking` | 프로젝트 특정 콘텐츠: "Form Service", "Email Service", "189 ErrorLogger.log() calls remaining" | 범용 하네스에 맞게 일반화 또는 제거 |
| P2-04 | `error-tracking` | 안티패턴 섹션 (lines 333-341) — "NEVER" 목록만, 코드 예시 없음 | BAD/GOOD 코드 예시 추가 |
| P2-05 | `error-tracking` | "Key Files" 섹션 — `/blog-api/src/...` 등 프로젝트 특정 경로 | 범용 경로 또는 제거 |
| P2-06 | `pytest-backend-testing` | "Current Project Configuration" (lines 490-520) — "qwarty" 프로젝트 참조 | 범용화 또는 `resources/`로 이동 |
| P2-07 | `skill-developer` | 안티패턴 전용 섹션 없음 (Best Practices에 암묵적으로만 존재) | 전용 "Anti-Patterns" 섹션 추가 (SKILL.md 500줄 초과 실수, frontmatter 누락, 과도한 트리거 등) |
| P2-08 | `skill-developer` | Quick Start 템플릿 (line 127) — frontmatter에 `triggers:` 누락 | 템플릿에 `triggers:` 필드 추가 |
| P2-09 | `pdf` | 안티패턴 전용 섹션 없음 (line 182에 인라인 경고만) | 전용 섹션으로 분리 (Unicode 첨자 오용, 메모리 미해제, 대용량 PDF 미분할 처리 등) |
| P2-10 | `pptx` | 497줄 — 500줄 한계에 3줄 여유. 안티패턴 산재 | 핵심 내용 외 `resources/`로 분리하여 여유 확보, 안티패턴 통합 |
| P2-11 | `docx` | 489줄 — 500줄 한계에 11줄 여유 | 향후 추가 시 `resources/` 분리 고려 |

---

## 5. P3 이슈 상세 (Nice to Have — 14건)

| # | 스킬 | 이슈 |
|---|---|---|
| P3-01 | `docker-guidelines` | Node.js 패턴이 3-stage 빌드인데 base/nestjs는 2-stage (둘 다 유효) |
| P3-02 | `docker-guidelines` | non-root 유저 테이블: Python→`app` 표기, base/django는 `django` 사용 |
| P3-03 | `django-backend-guidelines` | 정확히 500줄 — 추가 여유 없음 |
| P3-04 | `express-backend-guidelines` | 정확히 500줄 — 추가 여유 없음 |
| P3-05 | `react-native-guidelines` | 정확히 500줄 — 추가 여유 없음 |
| P3-06 | `embedded-c-guidelines` | 497줄 — 3줄 여유 |
| P3-07 | `embedded-cpp-guidelines` | 안티패턴이 표 형식 (코드 예시 없음, 다른 스킬과 형식 불일치) |
| P3-08 | `nextjs-frontend-guidelines` | "use server" 트리거 존재하나 Server Actions 패턴 미문서화 |
| P3-09 | `mermaid` | 안티패턴 섹션명이 "Avoid" (다른 스킬과 명명 불일치) |
| P3-10 | `vercel-react-best-practices` | `AGENTS.md` 파일명이 에이전트 정의로 오해 소지 |
| P3-11 | `skill-developer` | "Philosophy Change (2025-10-27)" 프로젝트 특정 이력 참조 |
| P3-12 | `ppt-brand-guidelines` | CSS 예시만 존재 (python-pptx 코드 없음) |
| P3-13 | `fastapi-backend-guidelines` | 섹션 제목에 이모지 사용 (다른 스킬과 불일치) |
| P3-14 | `pptx` | `resources/` 파일 참조 경로 검증 필요 |

---

## 6. 수정 우선순위 및 작업 계획

### Tier 1: P1 수정 (Phase 2-D 전 필수 — 6개 스킬)

> 이 6개 스킬을 수정하지 않으면 DoD 미충족으로 Phase 2-D 일괄 통합 불가.

| 순서 | 스킬 | P1 건수 | 핵심 작업 | 예상 작업량 |
|---|---|---|---|---|
| 1 | `fastapi-backend-guidelines` | 2건 | 97줄→resources/ 이동 + 안티패턴 추가 | 중 |
| 2 | `pytest-backend-testing` | 2건 | 94줄→resources/ 이동 + 안티패턴 추가 | 중 |
| 3 | `vercel-react-best-practices` | 2건 | rules/에서 3-5개 패턴 추출 인라인 + 안티패턴 | 대 |
| 4 | `web-design-guidelines` | 2건 | 접근성/반응형/색상 패턴 인라인 + 안티패턴 | 대 |
| 5 | `frontend-design` | 2건 | CSS/HTML/React 코드 예시 추가 + 안티패턴 분리 | 대 |
| 6 | `brand-guidelines` | 2건 | CSS/Python/Tailwind 적용 코드 + 안티패턴 | 소 |

### Tier 2: P2 수정 (품질 향상 — 7개 스킬)

| 순서 | 스킬 | P2 건수 | 핵심 작업 |
|---|---|---|---|
| 7 | `error-tracking` | 4건 | deprecated API 교체 + 프로젝트 특정 제거 + 안티패턴 코드 |
| 8 | `django-backend-guidelines` | 1건 | description 정리 |
| 9 | `pytest-backend-testing` | 1건 | qwarty 참조 제거 |
| 10 | `skill-developer` | 2건 | 안티패턴 섹션 + 템플릿 triggers 추가 |
| 11 | `pdf` | 1건 | 안티패턴 전용 섹션 |
| 12 | `pptx` | 2건 | 안티패턴 통합 + resources/ 분리 |
| 13 | `docx` | 1건 | 향후 확장 대비 (현재 조치 불필요) |

### Tier 3: P3 수정 (일관성 개선 — 선택)

> Phase 2-D 이후 시간 여유 시 진행. 기능적 영향 없음.

---

## 7. 수정 후 예상 결과

### 수정 전 (현재)

```
S등급: 8개  |  A등급: 8개  |  B등급: 2개  |  C등급: 5개 (+1 C+)
DoD 미충족: 6개 스킬
P1 이슈: 12건
```

### 수정 후 (Tier 1 완료 시)

```
S등급: 8개  |  A등급: 14개  |  B등급: 1개  |  C등급: 0개
DoD 미충족: 0개
P1 이슈: 0건
```

### 수정 후 (Tier 1 + Tier 2 완료 시)

```
S등급: 10개  |  A등급: 13개  |  B등급: 0개  |  C등급: 0개
DoD 미충족: 0개
P1 이슈: 0건 / P2 이슈: 0건
```

---

## 8. 세션 전략 제안

### 병렬 세션 구성 (격리 원칙 준수)

```
[세션 1] fastapi-backend-guidelines P1 수정 (resources/ 이동 + 안티패턴)
[세션 2] pytest-backend-testing P1 수정 (resources/ 이동 + 안티패턴)
[세션 3] vercel-react-best-practices 전면 보강 (패턴 추출 + 안티패턴)
[세션 4] web-design-guidelines 전면 보강 (인라인 패턴 + 안티패턴)
[세션 5] frontend-design 보강 (코드 예시 + 안티패턴)
[세션 6] brand-guidelines 보강 (코드 예시 + 안티패턴)
```

> 각 세션은 담당 `.claude/skills/[skill-name]/` 디렉토리만 수정.
> `skill-rules.json`, `README.md`, `docs/` 수정 금지 (Phase 2-D 전용).

### P2 수정 (Tier 1 완료 후)

```
[세션 7]  error-tracking P2 수정 (deprecated API + 프로젝트 특정 제거)
[세션 8]  django/pytest/skill-developer P2 경미 수정 (병합 가능)
[세션 9]  pdf/pptx P2 안티패턴 정리
```

---

## 9. DoD 체크리스트 (수정 완료 후 검증용)

수정 완료된 각 스킬은 아래 6항목 전체 PASS 시에만 완료 인정:

- [ ] SKILL.md **500줄 이하**
- [ ] YAML frontmatter: `name`, `description`, `triggers` 3개 필드 존재
- [ ] 핵심 패턴 **3개 이상** + 각 패턴에 **코드 예시** 포함
- [ ] 트리거 키워드 **5개 이상**
- [ ] **안티패턴 전용 섹션** 존재 (최소 3개, BAD/GOOD 예시 권장)
- [ ] 프로젝트 특정 콘텐츠 없음 (범용 하네스 기준)

---

*Generated: 2026-03-05 | Auditor: Claude Opus 4.6 | Scope: 24/24 Skills*
