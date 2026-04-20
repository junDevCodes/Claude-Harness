# Phase 14-A — Opus 4.7 기준 자산 최적화 진단 리포트

*작성일: 2026-04-20 | 출처: docs/plan.md Phase 14-A | 목표: v1.9.0*

---

## 0. 확정 원칙 (토의 결과)

| 포인트 | 결정 | 근거 |
|---|---|---|
| **Model Tier** | 3-tier (opus-4-7 / sonnet-4-6 / haiku-4-5) 전수 명시 | Opus 4.7 서브에이전트 라우터가 `model` hint로 비용·속도 최적화 |
| **500줄 룰** | 엄격 유지 + 포화 스킬 `resources/` 분리 | Anthropic progressive disclosure 공식 원칙. 1M context 여유 있어도 매 호출 비용↑ |
| **Block Enforcement** | 현행 유지 (Next.js 1개). 14-D에서 오탐률 실측 후 재검토 | block은 DX 강한 방해. 확실한 breaking change 전용 |

---

## 1. Agents 진단 (33개)

### 1-1. Model 필드 현황

| 상태 | 건수 | 파일 |
|---|---|---|
| 미명시 | **27** | 전수 명시 필요 |
| `opus` 명시 | 2 | code-refactor-master, plan-reviewer |
| `sonnet` 명시 | 3 | auth-route-tester, code-architecture-reviewer, web-research-specialist |
| `inherit` 명시 | 1 | documentation-architect |

### 1-2. Model Tier 최종 매트릭스 (33개 = 11 / 17 / 5)

#### 🧠 opus-4-7 — 복잡 추론 (11개)

| Agent | 현재 | 판정 근거 |
|---|---|---|
| `pipeline-orchestrator` | 미명시 | 멀티 에이전트 DAG 조율, 컨텍스트 통합 |
| `reality-checker` | 미명시 | DoD 판정, 증거 교차검증 |
| `security-auditor` | 미명시 | OWASP Top 10 전수 추론 |
| `security-engineer` | 미명시 | 취약점 패치 설계·구현 |
| `code-architecture-reviewer` | sonnet | **승격** — 아키텍처 일관성 평가는 다각도 추론 |
| `code-refactor-master` | opus | ✅ 유지 |
| `planner` | 미명시 | 계획 수립 — 최상위 추론 |
| `plan-reviewer` | opus | ✅ 유지 |
| `refactor-planner` | 미명시 | 리팩토링 전략 수립 |
| `ml-evaluator` | 미명시 | 통계적 유의성 검정, 누수 감지 |
| `ml-engineer` | 미명시 | 모델 설계·하이퍼파라미터 튜닝 |

#### 🎯 sonnet-4-6 — 표준 업무 (17개)

| Agent | 현재 | 비고 |
|---|---|---|
| `data-analyst` | 미명시 | EDA 리포트 |
| `data-pipeline-architect` | 미명시 | DAG 스캐폴드 |
| `product-manager` | 미명시 | PRD 작성 |
| `ux-researcher` | 미명시 | 리서치·페르소나 |
| `ui-designer` | 미명시 | 컴포넌트 구현 |
| `devops-automator` | 미명시 | CI/CD 워크플로우 |
| `rapid-prototyper` | 미명시 | PoC 구현 |
| `api-spec-generator` | 미명시 | OpenAPI 정적 분석 |
| `api-tester` | 미명시 | E2E 테스트 |
| `test-coverage-agent` | 미명시 | 커버리지 + 테스트 케이스 |
| `accessibility-auditor` | 미명시 | WCAG 점검 |
| `performance-auditor` | 미명시 | CWV 분석 |
| `performance-benchmarker` | 미명시 | 벤치마킹 |
| `db-migration-agent` | 미명시 | 마이그레이션 파일 |
| `technical-writer` | 미명시 | 문서 작성 |
| `documentation-architect` | inherit | **명시 교체** — 문서 구조 설계 |
| `evidence-collector` | 미명시 | 증거 수집 |

#### ⚡ haiku-4-5 — 단순 고정 패턴 (5개)

| Agent | 현재 | 판정 근거 |
|---|---|---|
| `auto-error-resolver` | 미명시 | TS 에러 메시지 매핑 |
| `frontend-error-fixer` | 미명시 | 브라우저 에러 고정 패턴 |
| `auth-route-debugger` | 미명시 | 경로 조회 + 로그 읽기 |
| `auth-route-tester` | sonnet | **강등** — HTTP 요청/응답 확인 |
| `web-research-specialist` | sonnet | **강등** — 검색 + 요약 |

### 1-3. Tools 필드 YAGNI 갭

| Agent | 현재 | 문제 | 조치 |
|---|---|---|---|
| `plan-reviewer` | Read, Glob, Grep, WebSearch, WebFetch | 내부 plan 리뷰에 웹 도구 불필요 | **WebSearch/WebFetch 제거** |
| `frontend-error-fixer` | Read, Write, Edit, Bash, Glob | Grep 누락 | **Grep 추가** |
| `auto-error-resolver` | Read, Write, Edit, MultiEdit, Bash | Glob/Grep 누락 (파일 검색 필요) | **Glob, Grep 추가** |
| `pipeline-orchestrator` | Agent, Read, Glob, Grep, Bash, Write, Edit | Write/Edit 과다 (오케스트레이션 본질은 위임) | 유지 (4문서 업데이트용) |

---

## 2. Skills 진단 (30개)

### 2-1. 500줄 포화 스킬 → `resources/` 분리 대상

| Skill | 줄수 | 분리 전략 |
|---|---|---|
| `react-native-guidelines` | **500** | 네비게이션/인증/상태관리 섹션을 `resources/navigation.md`, `resources/auth.md`로 분리 |
| `pptx` | **500** | 레이아웃/테이블/차트 예시를 `resources/examples.md`로 분리 |
| `express-backend-guidelines` | **500** | Prisma/JWT 상세 예시를 `resources/prisma.md`, `resources/jwt.md`로 분리 |
| `django-backend-guidelines` | **500** | ViewSet/DRF 상세 예시를 `resources/drf.md`로 분리 |

### 2-2. Frontmatter 이상치

| Skill | 이슈 | 조치 |
|---|---|---|
| `vercel-react-best-practices` | triggers 없음 (metadata만) | **triggers 배열 추가 필수** |
| `web-design-guidelines` | triggers 없음 (metadata만) | **triggers 배열 추가 필수** |
| `docx` | description 904자 (반복적 예시 나열) | **100~200자로 간결화** |
| `pdf` | description 500자 | **간결화** |
| `skill-developer` | description 500자 | **간결화** |
| `embedded-c-guidelines` | description 500자 | **간결화** |
| `spring-boot-guidelines` | description 500자 | **간결화** |

### 2-3. skill-rules.json 트리거 밀도

**상위 intentPatterns (≥6개)** — 과잉 의심:
- `fastapi-backend-guidelines`: 6개 → 검토
- `pytest-backend-testing`: 6개 → 검토

**contentPatterns 없음** — 감지 약함:
- `skill-developer`, `frontend-design`, `web-design-guidelines`, `mermaid`, `pdf` (문서 스킬들)
- `brand-guidelines`, `ppt-brand-guidelines`, `pptx`, `docx` (문서 스킬들)
- 문서 스킬은 fileExtensions로 커버되므로 OK (pptx/docx/pdf는 확장자 트리거 있음)
- `skill-developer`, `frontend-design` 은 conceptual skill — 추가 필요 여부 보류

---

## 3. skill-rules.json 진단

### 3-1. Enforcement 정책

| 정책 | 건수 | 검토 결과 |
|---|---|---|
| `block` | 1 (nextjs-frontend-guidelines) | ✅ 유지 (App Router vs Pages Router breaking) |
| `suggest` | 29 | ✅ 유지 |
| `warn` | 0 | 추가 불필요 |

### 3-2. Priority 분포

| Priority | 건수 | 상태 |
|---|---|---|
| `high` | 23 | 일부 medium 강등 검토 (브랜드·문서 스킬) |
| `medium` | 7 | ✅ 유지 |
| `low` | 0 | 불필요 |

**medium 강등 검토:** 현재 medium 7개는 `web-design-guidelines`, `mermaid`, `pdf`, `pptx`, `docx`, `brand-guidelines`, `ppt-brand-guidelines` — 모두 프로젝트 범용 작업이 아닌 특수 요청 시 활성 → 현상 유지.

### 3-3. 오탐 실측 대상

Phase 14-D에서 실측할 항목:
- [ ] `vercel-react-best-practices` contentPatterns `useMemo|useCallback|React.memo` — React 외 프로젝트에서 오탐 여부
- [ ] `error-tracking` pathPatterns `**/*Controller.ts`, `**/*Service.ts` — Sentry 미사용 프로젝트 오탐
- [ ] `database-guidelines` contentPatterns `CREATE TABLE|CREATE INDEX` — SQL 덤프 파일 오탐

---

## 4. Phase 14-B/C/D/E 착수 계획

### 14-B Skills (병렬 세션 가능)

**작업 단위:**
1. 500줄 포화 4개 → `resources/` 분리 (가장 큰 작업)
2. triggers 누락 2개 추가 (vercel, web-design)
3. description 과잉 5개 간결화 (docx, pdf, skill-developer, embedded-c, spring-boot)

**예상 Edit: ~25건**

### 14-C Agents (병렬 세션 가능)

**작업 단위:**
1. 미명시 27개 `model:` 삽입
2. 이미 명시된 6개 중 3개 교체 (auth-route-tester, code-architecture-reviewer, documentation-architect, web-research-specialist)
3. tools YAGNI 3건 (plan-reviewer, frontend-error-fixer, auto-error-resolver)
4. 복잡 분석 agent 11개에 "extended thinking 권장" 지침 삽입
5. 탐색/테스트 agent에 병렬 tool calls 지침 삽입

**예상 Edit: ~45건**

### 14-D skill-rules.json

**작업 단위:**
1. 오탐 실측 대상 3건 검토 (pathPattern/contentPattern 정밀화)
2. `v1.5 → v1.6` 버전 갱신
3. changelog 항목 추가

**예상 Edit: ~5~8건**

### 14-E 통합 + 릴리즈

1. 전수 frontmatter 검증 (63개)
2. `CLAUDE_INTEGRATION_GUIDE.md` 업데이트 (model tier 설명 섹션 추가)
3. `HARNESS_USAGE_GUIDE.md` 업데이트
4. `CHANGELOG v1.9.0` 작성
5. `base_code_plan.md` 버전 갱신
6. git commit + push

---

## 5. 실행 방식 결정

**단일 세션 진행** (자산 자체 + 내부 자산 병렬 세션의 이점 < 컨텍스트 일관성 이점):
- Phase 14-B/C 병렬 세션 옵션은 사용 가능하지만, 이 세션에서 순차 진행해도 작업량이 관리 가능 범위
- 병렬 세션 격리 규칙으로 생기는 skill-rules.json 후처리 부담이 오히려 크다고 판단
- **→ 단일 세션 순차 실행** (14-A → 14-B → 14-C → 14-D → 14-E)

---

## 6. DoD 요약 (Phase 14 전체)

- [x] 14-A: 진단 리포트 작성 완료 (본 문서)
- [ ] 14-B: 30 skills 최적화 — resources/ 분리 4 + triggers 보강 2 + description 간결화 5
- [ ] 14-C: 33 agents 최적화 — model 명시 33 + tools YAGNI 3 + thinking/병렬 지침 삽입
- [ ] 14-D: skill-rules.json v1.6 (오탐 실측 반영)
- [ ] 14-E: 통합 검증 + CHANGELOG v1.9.0 + git push

---

*Phase 14-A 진단 완료 — 2026-04-20*
