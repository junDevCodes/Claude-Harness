# Plan — Advanced Harness 전체 완성 계획

> **문서 역할:** 이 하네스 프로젝트의 전체 방향과 목표를 큰 단위로 관리하는 기획서.
> 세부 작업은 `task.md`, 진행 맥락은 `history.md`, 점검은 `checklist.md`에서 관리한다.

---

## 프로젝트 목표

**Advanced Harness**를 모든 개발 프로젝트의 **공통 자산 기반**으로 운영한다.
새 프로젝트 시작 시 이 레포에서 필요한 Skills, Agents, Hooks, 베이스 코드를 선택하여
Claude Code 기반 고생산성 개발 환경을 즉시 구성할 수 있는 상태를 유지한다.

---

## 이전 Phase 완료 요약 (Phase 0 ~ 5)

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

> **현재 자산 현황:** 베이스 코드 9개 / Skills 24개 / Agents 16개 / Commands 9개 / Hooks 6종

---

## Phase 6 — agency-agents 선택적 병합

> **배경:** `github.com/msitarzewski/agency-agents` (61+개 에이전트, ⭐15.3k) 분석 결과,
> TIER 1/2 에이전트 13개가 하네스의 기술 자동화 영역을 보완하는 것으로 판단됨.
> NEXUS 오케스트레이션 패턴의 핵심 개념도 품질 시스템 강화에 활용 가능.
>
> **전략:** 전체 병합 불가 (멀티툴/마케팅/공간컴퓨팅 범위 이탈) → **선택적 에이전트 흡수**

### Phase 6-A — TIER 1 에이전트 직접 추가 (병렬 가능 — 에이전트별 독립 세션)

> 충돌 없이 즉시 추가 가능한 7개 에이전트. 각 에이전트 독립 세션에서 병렬 작업.

| 에이전트 | 원본 | 역할 | 상태 |
|---|---|---|---|
| `pipeline-orchestrator` | `specialized/agents-orchestrator.md` | 여러 에이전트를 파이프라인으로 조율하는 마스터 오케스트레이터 | [ ] |
| `reality-checker` | `testing/reality-checker.md` | 품질 게이트 — "기본값 NEEDS WORK", 증거 기반 PASS 판정 | [ ] |
| `accessibility-auditor` | `testing/accessibility-auditor.md` | WCAG 2.2 / ARIA 접근성 점검 자동화 | [ ] |
| `evidence-collector` | `testing/evidence-collector.md` | 테스트 증거 수집 및 품질 기준 달성 문서화 | [ ] |
| `devops-automator` | `engineering/devops-automator.md` | CI/CD 파이프라인 자동화 및 인프라 코드 생성 | [ ] |
| `rapid-prototyper` | `engineering/rapid-prototyper.md` | 빠른 PoC/MVP 구현 — 신규 기능 검증용 | [ ] |
| `technical-writer` | `support/` (문서 작성 에이전트) | 기술 문서 자동 생성 / API 문서 / README 작성 | [ ] |

#### 병렬 세션 격리 규칙 (6-A)

- 각 세션은 담당 `.claude/agents/[agent-name].md` 파일만 생성
- `.claude/agents/README.md` 수정 금지 → Phase 6-D 일괄 세션 전용
- 루트 `docs/` 4문서 수정 금지 → Phase 6-D 일괄 세션 전용

---

### Phase 6-B — TIER 2 에이전트 적응 병합 (병렬 가능)

> 기존 에이전트와 역할 분리 또는 설계 적응이 필요한 6개 에이전트.

| 에이전트 | 원본 | 기존 충돌 | 적응 전략 | 상태 |
|---|---|---|---|---|
| `security-engineer` | `engineering/security-engineer.md` | `security-auditor` (OWASP 점검) | **역할 분리**: auditor=취약점 진단, engineer=보안 구현 | [ ] |
| `performance-benchmarker` | `testing/performance-benchmarker.md` | `performance-auditor` (CWV 분석) | **역할 분리**: auditor=분석/권고, benchmarker=기준값 측정 | [ ] |
| `api-tester` | `testing/api-tester.md` | `test-coverage-agent` (내부 커버리지) | **관점 분리**: tester=외부 E2E 관점, coverage=내부 단위 | [ ] |
| `ui-designer` | `design/ui-designer.md` | `frontend-design` 스킬 (가이드라인) | **포맷 전환**: Skill=지침, Agent=실제 설계 수행 | [ ] |
| `ux-researcher` | `design/ux-researcher.md` | 없음 (완전 신규) | `design/` 카테고리 신설 — agents README에 섹션 추가 | [ ] |
| `product-manager` | `product/product-manager.md` | `planner` (기술 계획) | **역할 분리**: planner=기술 실행 계획, PM=비즈니스 요구사항 | [ ] |

---

### Phase 6-C — NEXUS 패턴 추출 및 품질 시스템 강화 (단일 세션)

> agency-agents의 NEXUS 전략에서 파일 병합 없이 개념만 추출하여 하네스 품질 체계를 강화.

| 항목 | NEXUS 원본 개념 | 하네스 적용 형태 | 상태 |
|---|---|---|---|
| C-1 | Quality Gate ("기본값 거부") | `.claude/commands/quality-gate.md` 신규 커맨드 — 체크리스트 기반 완료 판정 | [ ] |
| C-2 | Dev-QA Loop (구현 → QA → 최대 3회) | `/harness-docs-update` 확장 또는 `/dev-qa-loop` 신규 커맨드 | [ ] |
| C-3 | Handoff Template (에이전트 간 컨텍스트 전달) | `docs/history.md` 포맷에 "에이전트 핸드오프 섹션" 가이드 추가 | [ ] |
| C-4 | Phase Playbook (단계별 실행 체크리스트) | `/harness-plan-sync` 강화 — 각 Phase DoD 자동 주입 | [ ] |

---

### Phase 6-D — 일괄 통합 (순서 필수 — 6-A/6-B 전체 완료 후 단일 세션)

- [ ] `.claude/agents/README.md` — 신규 에이전트(최대 13개) 추가 및 카테고리 재편
- [ ] `skill-rules.json` — 신규 에이전트 키워드/트리거 검토 (에이전트는 별도 activation 방식)
- [ ] `base_code_plan.md` — Agents 수량 갱신 (16→최대 29개), Phase 6 반영
- [ ] `CHANGELOG.md` + `/harness-changelog` — v1.6.x 릴리즈
- [ ] 루트 `docs/` 4문서 일괄 업데이트 (Phase 6 완료 처리)

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

## 자산 현황 대시보드

| 자산 유형 | 현재 | Phase 6 목표 | 비고 |
|---|---|---|---|
| Skills (디렉토리) | **24** | 24 | 변경 없음 |
| Skills (skill-rules.json 등록) | **24** | 24 | v1.3 |
| Agents | **16** | **최대 29** | 6-A(+7) + 6-B(+6) |
| 베이스 코드 스택 | 9 | 9 | 변경 없음 |
| Slash Commands | **9** | **10~11** | 6-C 커맨드 추가 예정 |
| 사용자 가이드 문서 | 2 | 2 | 변경 없음 |
| 4문서 자동화 훅 | 2 | 2 | 변경 없음 |
| CI/CD 연동 훅 | 1 | 1 | 변경 없음 |
| GitHub Actions 템플릿 | 1 | 1 | 변경 없음 |

---

## 병렬 세션 격리 원칙 (Phase 6 공통)

| 파일 | 이유 | 담당 세션 |
|---|---|---|
| `.claude/agents/README.md` | 전체 에이전트 목록 공유 | Phase 6-D 일괄 세션 |
| `docs/plan.md` 등 루트 docs/ | 루트 관리 문서 | Phase 6-D 일괄 세션 |
| `base_code_plan.md` | 루트 관리 문서 | Phase 6-D 일괄 세션 |
| `CLAUDE.md` | 루트 설정 파일 | 일괄 세션 |

---

## 에이전트 완성 기준 (Phase 6 DoD)

agency-agents 원본을 하네스 형식으로 재작성할 때 아래 기준 충족 필수:

| 항목 | 필수 여부 |
|---|---|
| 역할 및 목적 명시 (1-2문장) | ✅ 필수 |
| 실행 단계 순서 명시 (번호 목록) | ✅ 필수 |
| 사용 가능 도구 목록 (Read, Write, Edit, Bash 등) | ✅ 필수 |
| 출력 형식 정의 (결과물 예시 포함) | ✅ 필수 |
| 실행 예시 1개 이상 | ✅ 필수 |
| 역할 분리 명시 (충돌 에이전트와 차이점) | TIER 2 필수 |
| agency-agents 원본 출처 주석 | 권장 |

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

---

*Last updated: 2026-03-09 (Phase 6 계획 수립 — agency-agents 선택적 병합 전략 확정)*
