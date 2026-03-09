# Checklist — 작업 확인서

> **현재 Phase:** Phase 6-A — TIER 1 에이전트 직접 추가 (agency-agents 선택적 병합)
> **선행 완료:** Phase 5-C (2026-03-09) — 사용자 가이드 보완 전체 완료

---

## Phase 6-A — TIER 1 에이전트 추가 체크리스트

> **병렬 세션 규칙:** 각 에이전트 담당 세션은 해당 `.md` 파일만 생성. README/docs/base_code_plan.md 수정 금지.

---

### 공통 DoD (모든 에이전트 공통)

- [ ] YAML frontmatter 포함 (`name`, `description`, `tools` 또는 동등 필드)
- [ ] 역할 및 목적 명시 (1-2문장, 첫 섹션)
- [ ] 실행 단계 순서 명시 (번호 목록)
- [ ] 사용 가능 도구 목록 명시
- [ ] 출력 형식 정의 (결과물 예시 포함)
- [ ] 실행 예시 1개 이상
- [ ] agency-agents 원본 출처 주석 (권장)

---

### 6-A-1: `pipeline-orchestrator.md`

**파일:** `.claude/agents/pipeline-orchestrator.md`

- [ ] 파일 생성 확인
- [ ] 파이프라인 설계 원칙 섹션 포함
- [ ] 에이전트 호출 단계 (탐색 → 계획 → 실행 → 검증 → 완료) 명시
- [ ] 하네스 에이전트 목록 (16개 기준 + 신규 7개) 역할별 분류 포함
- [ ] 품질 게이트 체크포인트 명시 (단계 간 전환 기준)
- [ ] 4문서 체계(`docs/`) 연동 방식 포함
- [ ] 실행 예시 1개 이상 (예: FastAPI + Next.js 신규 기능 파이프라인)
- [ ] 공통 DoD 6항목 전부 충족

---

### 6-A-2: `reality-checker.md`

**파일:** `.claude/agents/reality-checker.md`

- [ ] 파일 생성 확인
- [ ] "기본값 NEEDS WORK" 판정 철학 명시
- [ ] 증거 수집 단계 포함 (무엇을 확인하는가)
- [ ] PASS 기준 목록 (하네스 `checklist.md` DoD 항목 연동)
- [ ] NEEDS WORK 보고서 형식 정의 (미충족 항목 목록)
- [ ] `evidence-collector` 에이전트 연계 언급
- [ ] 공통 DoD 6항목 전부 충족

---

### 6-A-3: `accessibility-auditor.md`

**파일:** `.claude/agents/accessibility-auditor.md`

- [ ] 파일 생성 확인
- [ ] WCAG 2.2 / ARIA 점검 범위 명시 (색상 대비, 키보드 내비게이션, 스크린리더, 폼 레이블)
- [ ] 자동 점검 도구 실행 단계 포함 (axe-core, pa11y 등)
- [ ] 수동 점검 체크리스트 포함
- [ ] 심각도 분류 기준 (Critical / High / Medium / Low)
- [ ] 수정 권고 포맷 정의
- [ ] `nextjs-frontend-guidelines` 스킬 연계 언급
- [ ] 공통 DoD 6항목 전부 충족

---

### 6-A-4: `evidence-collector.md`

**파일:** `.claude/agents/evidence-collector.md`

- [ ] 파일 생성 확인
- [ ] 증거 유형 분류 (테스트 / 빌드 / 실행 / 문서) 포함
- [ ] 스택별 증거 수집 명령어 포함 (pytest, tsc, E2E 등)
- [ ] 증거 저장 경로 정의 (`docs/evidence/[YYYY-MM-DD]-[task]-evidence.md`)
- [ ] 증거 문서 포맷 정의
- [ ] `reality-checker` 에이전트 연계 방법 포함
- [ ] `test-coverage-agent`와 역할 분리 명시
- [ ] 공통 DoD 6항목 전부 충족

---

### 6-A-5: `devops-automator.md`

**파일:** `.claude/agents/devops-automator.md`

- [ ] 파일 생성 확인
- [ ] CI/CD 설계 단계 포함
- [ ] 스택별 파이프라인 패턴 (FastAPI / Next.js / NestJS) 포함
- [ ] Dockerfile 최적화 가이드 (멀티 스테이지 빌드) 포함
- [ ] GitHub Actions 워크플로우 템플릿 생성 지시 포함
- [ ] AWS ECS 배포 패턴 언급 (`aws-guidelines` 스킬 참조)
- [ ] `pr-review-trigger` 훅과 역할 분리 명시 (훅=감지, 에이전트=설계)
- [ ] 공통 DoD 6항목 전부 충족

---

### 6-A-6: `rapid-prototyper.md`

**파일:** `.claude/agents/rapid-prototyper.md`

- [ ] 파일 생성 확인
- [ ] PoC 구현 원칙 (동작 우선, 완성도 후순위) 명시
- [ ] 스택별 빠른 시작 패턴 포함 (`base/[stack]/` 활용)
- [ ] PoC → 본 코드 전환 기준 명시
- [ ] PoC 저장 경로 정의 (`poc/[feature-name]/`)
- [ ] 검증 단계 (최소 동작 확인) 포함
- [ ] `code-refactor-master`와 역할 분리 명시 (prototyper=PoC, refactor=개선)
- [ ] 공통 DoD 6항목 전부 충족

---

### 6-A-7: `technical-writer.md`

**파일:** `.claude/agents/technical-writer.md`

- [ ] 파일 생성 확인
- [ ] 문서 유형별 생성 단계 포함 (README / API 가이드 / ADR / CONTRIBUTING)
- [ ] README.md 표준 구조 정의
- [ ] API 문서 작성 가이드 (`api-spec-generator` 연계) 포함
- [ ] 아키텍처 설명 패턴 포함
- [ ] `documentation-architect`와 역할 분리 명시 (architect=구조 설계, writer=내용 작성)
- [ ] 공통 DoD 6항목 전부 충족

---

### 6-A 전체 완료 기준

- [ ] 7개 에이전트 파일 전원 생성 확인
  ```bash
  ls .claude/agents/pipeline-orchestrator.md
  ls .claude/agents/reality-checker.md
  ls .claude/agents/accessibility-auditor.md
  ls .claude/agents/evidence-collector.md
  ls .claude/agents/devops-automator.md
  ls .claude/agents/rapid-prototyper.md
  ls .claude/agents/technical-writer.md
  ```
- [ ] 7개 전원 공통 DoD 6항목 충족
- [ ] `.claude/agents/README.md` 수정되지 않음 확인 (Phase 6-D 전용)
- [ ] 루트 `docs/` 파일 수정되지 않음 확인 (Phase 6-D 전용)

**6-A 완료 → 6-B / 6-C 병렬 착수 가능 조건 충족**

---

## Phase 6-B — TIER 2 에이전트 적응 병합 (예정)

> Phase 6-A 전체 완료 후 착수

- [ ] `security-engineer.md` — security-auditor와 역할 분리 명시
- [ ] `performance-benchmarker.md` — performance-auditor와 역할 분리 명시
- [ ] `api-tester.md` — test-coverage-agent와 관점 분리 명시
- [ ] `ui-designer.md` — frontend-design 스킬과 포맷 분리 명시
- [ ] `ux-researcher.md` — design 카테고리 신설
- [ ] `product-manager.md` — planner와 역할 분리 명시

---

## Phase 6-C — NEXUS 패턴 추출 (예정)

> Phase 6-A 전체 완료 후 병렬 착수 가능

- [ ] `quality-gate.md` 커맨드 신규 생성
- [ ] Dev-QA Loop 커맨드 또는 기존 커맨드 강화 검토
- [ ] `docs/history.md` 에이전트 핸드오프 섹션 가이드 추가
- [ ] `/harness-plan-sync` Phase DoD 자동 주입 강화

---

## Phase 6-D — 일괄 통합 (예정)

> Phase 6-A + 6-B + 6-C 전체 완료 후 착수

- [ ] `.claude/agents/README.md` — 신규 에이전트 추가 및 카테고리 재편
- [ ] `base_code_plan.md` — Agents 수량 갱신
- [ ] `CHANGELOG.md` 업데이트 + `/harness-changelog` 실행
- [ ] 루트 `docs/` 4문서 일괄 업데이트

---

*Last updated: 2026-03-09 (Phase 6-A 체크리스트 신규 작성)*
