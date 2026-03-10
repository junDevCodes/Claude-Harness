# Plan — Advanced Harness 전체 완성 계획

> **문서 역할:** 이 하네스 프로젝트의 전체 방향과 목표를 큰 단위로 관리하는 기획서.
> 세부 작업은 `task.md`, 진행 맥락은 `history.md`, 점검은 `checklist.md`에서 관리한다.

---

## 프로젝트 목표

**Advanced Harness**를 모든 개발 프로젝트의 **공통 자산 기반**으로 운영한다.
새 프로젝트 시작 시 이 레포에서 필요한 Skills, Agents, Hooks, 베이스 코드를 선택하여
Claude Code 기반 고생산성 개발 환경을 즉시 구성할 수 있는 상태를 유지한다.

---

## 이전 Phase 완료 요약 (Phase 0 ~ 6)

| Phase | 내용 | 완료일 |
|---|---|---|
| **0** | 하네스 기반 정립 — 4문서 체계, base_code_plan.md, CLAUDE.md | 2026-03-01 |
| **1-A/B/C** | 9개 베이스 코드 스택 생성 + 통합 검증 | 2026-03-02 |
| **2-A~D** | Skills 라이브러리 24개 확충 + skill-rules.json v1.3 | 2026-03-08 |
| **3-A/B** | Agents 라이브러리 16개 확충 + README 통합 | 2026-03-09 |
| **4-A** | 4문서 자동화 훅 (session-start-docs-loader, docs-update-reminder) | 2026-03-09 |
| **4-B** | skill-rules.json 오탐 5건 수정 (v1.3) — 상시 운영 중 | 2026-03-09 |
| **4-C** | CI/CD 연동 훅 (pr-review-trigger + GitHub Actions) | 2026-03-09 |
| **4-D** | taskops auto run (/harness-plan-sync 커맨드) | 2026-03-09 |
| **5-A** | 하네스 적용 가이드 완성 (HARNESS_USAGE_GUIDE.md + /harness-apply) | 2026-03-09 |
| **5-B** | 멀티 프로젝트 운영 도구 (harness-init.sh + /harness-changelog + /harness-backport) | 2026-03-09 |
| **5-C** | 사용자 가이드 보완 (HARNESS_COMMANDS_REFERENCE.md 신규 생성) | 2026-03-09 |
| **6-A** | TIER 1 에이전트 7개 신규 생성 + 검증 완료 (7/7 PASS) | 2026-03-10 |
| **6-B** | TIER 2 에이전트 6개 적응 병합 + 검증 완료 (6/6 PASS) | 2026-03-10 |
| **6-C** | NEXUS 패턴 추출 — quality-gate + dev-qa-loop 커맨드 + history.md 핸드오프 | 2026-03-10 |
| **6-D** | 일괄 통합 — agents/README.md 29개 재편 + base_code_plan.md + CHANGELOG v1.6.0 + 삭제 | 2026-03-10 |
| **8-A~E** | 훅·에이전트 코드 품질 수정 (P0~P2 총 20건+) + CHANGELOG v1.6.1 릴리즈 | 2026-03-11 |

> **현재 자산 현황:** 베이스 코드 9개 / Skills 24개 / Agents 29개 / Commands 11개 / Hooks 3종 등록 / CHANGELOG v1.6.1

---

## Phase 7 — 상시 운영 (병합 이후)

### Phase 7-A — skill-rules.json 트리거 정확도 개선 (4-B 계속)

- [ ] 오탐 패턴 수집 → 키워드 구체화
- [ ] 미탐 패턴 수집 → intentPatterns 추가
- [ ] (상시 운영 — 실제 사용 중 발견된 케이스 수정)

### Phase 7-B — 역전파 운영 (5-B 이후 상시)

- [ ] 실 프로젝트에서 검증된 패턴 → `/harness-backport` 실행
- [ ] base/[stack]/ + Skill 업데이트
- [ ] `/harness-changelog`로 버전 릴리즈

---

## Phase 8 — 훅·에이전트 코드 품질 수정 (2026-03-10 프로덕션 리뷰 기반) ✅ 전체 완료 (2026-03-11)

> **배경:** 2026-03-10 프로덕션 레벨 코드 리뷰 결과, `.claude/hooks/` 디렉토리에서
> **CRITICAL 4건 / HIGH 6건 / MEDIUM 7건 / AGENT DoD위반 3건** 이슈 확인.
> v1.6.0 기반 코드 품질을 프로덕션 수준으로 끌어올리는 것이 목표.
>
> **⚠️ 중복 확인 표기:** 기존 계획 + QA 리뷰 양측에서 독립적으로 확인된 이슈는
> `⚠️ 중복 확인` 마크를 부여하여 **우선순위를 한 단계 상향** 처리한다.

---

### Phase 8-A — P0 CRITICAL 즉시 수정 (단일 세션) ✅ 완료 (2026-03-10)

> **목표:** 현재 실행 중인 훅에서 기능이 완전히 망가진 이슈 + 문서 불일치 즉시 수정.

| ID | 파일 | 문제 | 심각도 | 비고 |
|---|---|---|---|---|
| **A-0** | `base_code_plan.md` | 버전 표기 `v1.5.0` (실제 `v1.6.0`) — 1줄 수정 | 🔴 P0 | 신규 발견 (즉시 수정 가능) |
| **A-1** | `tsc-check.sh` | ⚠️ **중복 확인** — Stop 훅에 `tool_name` 없음 → TSC 체크 전체 로직 사문화 + 하드코딩 서비스 디렉토리 (`email\|exports\|form\|...`) | 🔴🔴 **P0++** | 기존 계획(B-3 하드코딩) + QA리뷰(P0-1 근본 결함) — 양측 독립 확인 |
| **A-2** | `trigger-build-resolver.sh` | ⚠️ **중복 확인** — 매 Stop마다 `/tmp/claude-hook-debug.log` 디버그 덤프 + `claude --agent` 유효하지 않은 CLI | 🔴🔴 **P0++** | 기존 계획(A-2) + QA리뷰(P0-2) — 양측 독립 확인 |
| **A-3** | `session-start-docs-loader.ts` | catch에서 `process.exit(1)` → UserPromptSubmit 훅 실패 시 프롬프트 차단 가능 | 🔴 P0 | 기존 계획(A-3) — 단독 확인 |
| **A-4** | `error-handling-reminder.ts` | `split('\t')` vs 실제 포맷 `split(':')` 불일치 → 파싱 완전 실패 (단, **미등록 파일** → 활성화 결정 필요) | 🟠 P1 강등 | 기존 계획(A-1) — 등록 전에는 무영향 |

---

### Phase 8-B — P1 HIGH 신뢰성 수정 (단일 세션) ✅ 완료 (2026-03-10)

> **착수 조건:** Phase 8-A 전체 완료 확인

| ID | 파일 | 문제 | 심각도 | 비고 |
|---|---|---|---|---|
| **B-1** | `session-start-docs-loader.sh` | ⚠️ **중복 확인** — `set -e` → npx/tsx 실패 시 UserPromptSubmit 차단 | 🟠🟠 **P1++** | 기존 계획(B-1) + QA리뷰(P2-1) — 양측 독립 확인 |
| **B-2** | `skill-activation-prompt.sh` | ⚠️ **중복 확인** — `set -e` 동일 문제 | 🟠🟠 **P1++** | 기존 계획(B-2) + QA리뷰(P2-1) — 양측 독립 확인 |
| **B-3** | Dead Hook 정리 | `error-handling-reminder.sh` / `stop-build-check-enhanced.sh` settings.json 미등록 — 활성화 또는 삭제 결정 | 🟠 P1 | 기존 계획(B-4) |
| **B-4** | `.claude/.session-cache/` | TTL 없음 → `.loaded` 파일 무한 누적 (현재 27개), 정리 로직 없음 | 🟠 P1 | 신규 발견 |

---

### Phase 8-C — P1 에이전트 DoD 위반 수정 (신규, 단일 세션) ✅ 완료 (2026-03-11)

> **배경:** 2026-03-10 QA 리뷰에서 DoD 위반 에이전트 10개 + 4문서 체계 충돌 2건 발견.
> `CLAUDE.md` 에이전트 완성 기준에 `tools:` 필드는 **필수 항목**으로 명시되어 있음.

| ID | 대상 | 문제 | 심각도 |
|---|---|---|---|
| **C-1** | 에이전트 10개 (`auth-route-debugger`, `auth-route-tester`, `code-architecture-reviewer`, `code-refactor-master`, `documentation-architect`, `frontend-error-fixer`, `plan-reviewer`, `planner`, `refactor-planner`, `web-research-specialist`) | YAML frontmatter `tools:` 필드 누락 — DoD 위반 | 🟠 P1 |
| **C-2** | `planner.md` | description + 내부 로직 모두 `dev/active/` 경로 사용 → 하네스 4문서 체계(`docs/`)와 충돌 | 🟠 P1 |
| **C-3** | `auth-route-debugger.md` | 특정 프로젝트 크리덴셜 하드코딩 (`yourRealm`, `your-app-client`, `testuser`, `testpassword`) + PM2 서비스명 특화 | 🟠 P1 |

---

### Phase 8-D — P2 MEDIUM 코드 품질 개선 (단일 세션) ✅ 완료 (2026-03-11)

> **착수 조건:** Phase 8-B 완료 확인

| ID | 파일 | 문제 | 심각도 |
|---|---|---|---|
| **D-1** | `docs-update-reminder.ts` | `async function main()` — await 없음, 불필요한 async | 🟡 P2 |
| **D-2** | `pr-review-trigger.ts` | 동일 — async 불필요 | 🟡 P2 |
| **D-3** | `skill-activation-prompt.ts` | 동일 | 🟡 P2 |
| **D-4** | `error-handling-reminder.ts` | 동일 (A-4 수정과 동시 처리 가능) | 🟡 P2 |
| **D-5** | `.claude/hooks/` | `package-lock.json` + `pnpm-lock.yaml` 이중 lock 파일 | 🟡 P2 |
| **D-6** | `settings.json` | `enableAllProjectMcpServers: true` + `enabledMcpjsonServers` 모순 | 🟡 P2 |
| **D-7** | `.github/workflows/pr-code-review.yaml` | `synchronize` 시마다 중복 댓글 생성 — update 분기 없음 | 🟡 P2 |

---

### Phase 8-E — 통합 검증 + 4문서 갱신 + CHANGELOG 릴리즈 ✅ 완료 (2026-03-11)

> **목표:** 8-A/B/C/D 수정 완료 후 전체 통합 검증 + v1.6.1 패치 릴리즈.

| ID | 항목 | 내용 | 상태 |
|---|---|---|---|
| E-1 | 훅 동작 검증 | 8개 파일 존재 + bash -n + 안전 패턴 | ✅ PASS |
| E-2 | 수동 실행 7케이스 | UserPromptSubmit/PostToolUse/Stop | ✅ PASS |
| E-3 | 에이전트 DoD 재검증 | tools: 10개 + planner.md + auth-route-debugger | ✅ PASS |
| E-4 | `CHANGELOG.md` v1.6.1 | Fixed 13건 + Changed 8건 | ✅ DONE |
| E-5 | 4문서 완료 처리 | plan/task/history/checklist/base_code_plan | ✅ DONE |

> **추가 수정 (E-2 검증 중 발견):** `tsc-check.sh` — `jq` 미설치 환경 대응 → `node` 기반 JSON 파싱으로 교체 (Fixed 항목 추가)

---

## Phase 9 — 4문서 체계 고도화 ⬜

> **배경:** Phase 8 완료(2026-03-11) 후 발견된 구조적 개선 사항 — 4문서 역할 정의 모호성 + 훅 파이프라인 미완성 + 문서 생명주기 미정의

| Phase | 내용 | 우선순위 | 상태 |
|---|---|---|---|
| **9-A** | `CLAUDE.md` 4문서 정의 재정의 + 생명주기 규칙 추가 | 🔴 P0 | ✅ |
| **9-B** | 훅 파이프라인 완성 (session-start-docs-loader.ts + docs-update-reminder.ts) | 🟠 P1 | ✅ |
| **9-C** | `harness-plan-sync.md` 강화 (history.md 사전 검증 + 차단) | 🟡 P2 | ✅ |

---

## 자산 현황 대시보드

| 자산 유형 | 현재 | 목표 | 비고 |
|---|---|---|---|
| Skills (디렉토리) | **24** | 24 | 변경 없음 |
| Skills (skill-rules.json 등록) | **24** | 24 | v1.3 |
| Agents | **29** | 29 | Phase 8-C DoD 수정 완료 |
| 베이스 코드 스택 | 9 | 9 | 변경 없음 |
| Slash Commands | **11** | 11 | 변경 없음 |
| 사용자 가이드 문서 | 2 | 2 | 변경 없음 |
| 4문서 자동화 훅 | 2 등록 | 2 | Phase 8 코드 품질 개선 완료 |
| CI/CD 연동 훅 | 1 등록 | 1 | Phase 8-D 중복 댓글 수정 완료 |
| GitHub Actions 템플릿 | 1 | 1 | Phase 8-D 수정 완료 |
| CHANGELOG 버전 | **v1.6.1** | v1.6.1 | Phase 8 전체 완료 릴리즈 |

---

## 핵심 원칙

1. 베이스 코드는 **범용성** — 특정 비즈니스 로직 배제
2. 스킬은 **독립성** — 다른 스킬에 의존하지 않음
3. 문서는 **4문서 체계** — 모든 작업에 plan/task/history/checklist 적용
4. 좋은 패턴은 **역전파** — 실 프로젝트에서 발견된 패턴은 base/에 반영
5. 자산은 **상시 안정** — main 브랜치는 항상 사용 가능한 상태
6. 병렬 작업 시 **격리 원칙** — 각 세션은 자신의 할당 파일/디렉토리만 수정
7. **일괄 통합 파일** — README.md 류는 반드시 전용 일괄 세션에서만 수정
8. **선택적 흡수** — 외부 에이전트는 하네스 포커스(코드 개발)에 맞는 것만 수용
9. **⚠️ 중복 확인 이슈 우선** — 복수 관점에서 독립 확인된 버그는 한 단계 상향 처리

---

*Last updated: 2026-03-10 (Phase 8 계획 전면 재정비 — 프로덕션 QA 리뷰 결과 반영: CRITICAL 4건 재분류, 중복 확인 이슈 3건 우선순위 상향, 에이전트 DoD 위반 Phase 8-C 신설)*
