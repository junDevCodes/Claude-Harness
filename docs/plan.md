# Plan — Advanced Harness 전체 완성 계획

> **문서 역할:** 이 하네스 프로젝트의 전체 방향과 목표를 큰 단위로 관리하는 기획서.
> 세부 작업은 `task.md`, 진행 맥락은 `history.md`, 점검은 `checklist.md`에서 관리한다.

---

## 프로젝트 목표

**Advanced Harness**를 모든 개발 프로젝트의 **공통 자산 기반**으로 운영한다.
새 프로젝트 시작 시 이 레포에서 필요한 Skills, Agents, Hooks, 베이스 코드를 선택하여
Claude Code 기반 고생산성 개발 환경을 즉시 구성할 수 있는 상태를 유지한다.

---

## 완료 Phase 요약 (Phase 0 ~ 11-A)

| Phase | 내용 | 완료일 |
|---|---|---|
| **0** | 하네스 기반 정립 — 4문서 체계, base_code_plan.md, CLAUDE.md | 2026-03-01 |
| **1-A/B/C** | 9개 베이스 코드 스택 생성 + 통합 검증 | 2026-03-02 |
| **2-A~D** | Skills 라이브러리 24개 확충 + skill-rules.json v1.3 | 2026-03-08 |
| **3-A/B** | Agents 라이브러리 16개→29개 확충 + README 통합 | 2026-03-09 |
| **4-A~D** | 4문서 자동화 훅 + CI/CD 연동 + taskops auto run | 2026-03-09 |
| **5-A~C** | 하네스 적용 가이드 + 운영 도구 + 커맨드 레퍼런스 | 2026-03-09 |
| **6-A~D** | 외부 에이전트 13개 선택적 흡수 + NEXUS 패턴 + v1.6.0 | 2026-03-10 |
| **8-A~E** | 1차 코드리뷰 수정 20건+ + v1.6.1 | 2026-03-11 |
| **9-A~C** | 4문서 체계 고도화 — CLAUDE.md 재정의 + 훅 파이프라인 | 2026-03-11 |
| **10-A~D** | 2차 코드리뷰 수정 13건 + v1.6.2 | 2026-03-11 |
| **11-A** | 3차 심층 리뷰 P0 보안/기능 결함 7건 수정 | 2026-03-11 |

> **현재 자산:** 베이스 코드 9개 / Skills 26개 / Agents 29개 / Commands 11개 / Hooks 3종 / v1.7.0

---

## Phase 7 — 상시 운영

- [ ] skill-rules.json 오탐/미탐 패턴 수집 → 키워드 구체화 (상시)
- [ ] 실 프로젝트 검증 패턴 → `/harness-backport` 역전파 (상시)
- [ ] `/harness-changelog`로 버전 릴리즈 (수시)

---

## Phase 11 — 잔여 백로그 (실 적용 중 필요 시 처리)

> **방침:** P0는 11-A에서 완료. 나머지는 실 프로젝트 적용 피드백과 함께 처리.

| ID | 내용 | 건수 | 우선순위 |
|---|---|---|---|
| 11-B | 훅/CI/CD 신뢰성 (ReDoS 방어, node 최적화, SHA 핀닝 등) | 6건 | 🟠 P1 |
| 11-C | 에이전트/스킬 참조 수정 (미존재 경로, 팬텀 MCP, 중복 키워드) | 9건 | 🟠 P1 |
| 11-D | 문서 drift (가이드 수치 불일치, harness-init 버전) | 5건 | 🟠 P1 |
| 11-E | 코드 품질 (Windows 경로, try/catch, 에이전트 출력 일관성) | 10건 | 🟡 P2 |

<details>
<summary>Phase 11 상세 항목 (접기)</summary>

### 11-B — 훅/CI/CD 신뢰성 (6건)

| ID | 문제 | 대상 |
|---|---|---|
| B-1 | `pr-review-trigger.sh` — `\|\| exit 0` 누락 | hooks |
| B-2 | `post-tool-use-tracker.sh` — node 4회 → 단일 호출 최적화 | hooks |
| B-3 | `skill-activation-prompt.ts` — ReDoS 방어 | hooks |
| B-4 | `.claude/tsc-cache/` — .gitignore 미등록 | config |
| B-5 | `pr-code-review.yaml` — `actions/github-script` SHA 미핀닝 | CI/CD |
| B-6 | `pr-code-review.yaml` — `permissions: read-all` → 최소 권한 | CI/CD |

### 11-C — 에이전트/스킬 참조 (9건)

| ID | 문제 | 대상 |
|---|---|---|
| C-1 | 에이전트 5개 — `/documentation/` 미존재 경로 | agents |
| C-2 | 에이전트 3개 — 팬텀 MCP 도구 (`mcp__*`) | agents |
| C-3 | 에이전트 2개 — `MultiEdit` 미지원 참조 | agents |
| C-4 | SKILL.md 3개 — trigger vs skill-rules.json 불일치 | skills |
| C-5 | `dev-docs`/`dev-docs-update` — 4문서 충돌 경로 | commands |
| C-6 | 스킬 2개 — 중복 keyword | skills |
| C-7 | `nextjs-frontend-guidelines` — block scope 변수 충돌 | skills |
| C-8 | `code-architecture-reviewer.md` — `docs/reviews/` 미존재 | agents |
| C-9 | `plan-reviewer.md` — 불필요 tools (WebSearch/WebFetch) | agents |

### 11-D — 문서 drift (5건)

| ID | 문제 | 대상 |
|---|---|---|
| D-1 | `HARNESS_USAGE_GUIDE.md` — Agents 16개/Commands 9개 (실제 29/11) | docs |
| D-2 | `HARNESS_COMMANDS_REFERENCE.md` — `/quality-gate`, `/dev-qa-loop` 누락 | docs |
| D-3 | `CLAUDE_INTEGRATION_GUIDE.md` — 미존재 스킬명 참조 | docs |
| D-4 | `harness-init.sh` — 하드코딩 `v1.5.0` | scripts |
| D-5 | `harness-init.sh` — 완료 순서 수정 | scripts |

### 11-E — 코드 품질 (10건)

| ID | 문제 |
|---|---|
| E-1 | Windows 경로 처리 불일치 |
| E-2 | try/catch 누락 부분 강화 |
| E-3 | 파일 확장자 처리 불일치 |
| E-4 | hooks `package.json` dependencies 미최소화 |
| E-5 | CI/CD 워크플로우 concurrency guard 미설정 |
| E-6 | 에이전트 예시 내 약한 비밀번호 |
| E-7 | skill-rules.json intentPatterns 과잉 |
| E-8 | `.claude/.session-cache` TTL 정리 주기 미보장 |
| E-9 | `CONFIG.md` / `README.md` 훅 문서 outdated |
| E-10 | 에이전트 출력 형식 일관성 부족 |

</details>

---

## Phase 12 — 실 프로젝트 적용 + 역전파 ← 현재

> **목표:** 하네스 자산을 실 서비스 프로젝트에 적용하고, 실사용 피드백 기반으로 개선한다.
> **원칙:** 완벽한 라이브러리보다 사용되는 라이브러리가 가치있다.

### Phase 12-A — 병렬 세션 분석 기능 + v1.7.0 릴리즈 ✅ 완료 (2026-03-15)

- [x] 4문서 축약 정리 (history.md 압축, plan.md 완료 Phase 축약)
- [x] `/harness-plan-sync` 병렬 세션 분석 Step 추가 (Step 4.5)
- [x] task.md 출력 템플릿에 "세션 전략" 섹션 추가
- [x] `planner.md` 병렬 고려사항 추가
- [x] CHANGELOG v1.7.0 반영
- [x] base_code_plan.md 버전 갱신 (v1.6.2 → v1.7.0)

### Phase 12-B — 실 프로젝트 적용

- [ ] 대상 프로젝트 선정 + 기술 스택 확인
- [ ] `/harness-apply` 또는 `harness-init.sh` 실행
- [ ] settings.json / skill-rules.json 충돌 해결
- [ ] 스킬 트리거 정확도 검증 + 훅 동작 검증
- [ ] 에이전트 최소 3개 실행 테스트

### Phase 12-C — 역전파 + 피드백 반영

- [ ] 실 적용 중 발견 이슈 수집
- [ ] `/harness-backport`로 유용 패턴 역전파
- [ ] Phase 11 백로그 중 실 사용에서 문제된 항목 우선 수정
- [ ] `/harness-changelog`로 패치 릴리즈

---

## 자산 현황 대시보드

| 자산 유형 | 현재 | 비고 |
|---|---|---|
| Skills | **26** | skill-rules.json v1.4 |
| Agents | **29** | planner.md 병렬 분석 추가 |
| 베이스 코드 스택 | **9** | FastAPI / Next.js / Django / NestJS / Spring Boot / Express / React Native / C / C++ |
| Slash Commands | **11** | quality-gate + dev-qa-loop 포함 |
| Hooks | **3종** | 4문서 자동화 2 + CI/CD 1 |
| 사용자 가이드 | **2** | USAGE_GUIDE + COMMANDS_REFERENCE |
| CHANGELOG | **v1.7.0** | Phase 12-A 릴리즈 완료 |

---

## 핵심 원칙

1. 베이스 코드는 **범용성** — 특정 비즈니스 로직 배제
2. 스킬은 **독립성** — 다른 스킬에 의존하지 않음
3. 문서는 **4문서 체계** — 모든 작업에 plan/task/history/checklist 적용
4. 좋은 패턴은 **역전파** — 실 프로젝트에서 발견된 패턴은 base/에 반영
5. 자산은 **상시 안정** — main 브랜치는 항상 사용 가능한 상태
6. **실사용 우선** — 내부 품질 개선보다 실 프로젝트 적용 피드백이 우선

---

*Last updated: 2026-03-15 (Phase 12-A 완료 — 병렬 세션 분석 + v1.7.0 릴리즈)*
