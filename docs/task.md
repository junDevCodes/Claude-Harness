# Task — 현재 작업 상세 계획서

> **현재 Phase:** Phase 6-A — TIER 1 에이전트 직접 추가
> **선행 완료:** Phase 5-C (2026-03-09) — HARNESS_COMMANDS_REFERENCE.md 생성, Phase 5 전체 완료

---

## Phase 6-A — TIER 1 에이전트 직접 추가

### 개요

| 항목 | 내용 |
|---|---|
| **목표** | agency-agents에서 충돌 없이 즉시 추가 가능한 7개 에이전트를 하네스 형식으로 재작성하여 `.claude/agents/`에 추가 |
| **산출물** | `.claude/agents/` 신규 파일 7개 |
| **병렬 작업** | 에이전트별 독립 세션에서 병렬 작업 가능 |
| **의존성** | 없음 (6-A는 기존 에이전트와 충돌 없음) |
| **다음 단계** | 6-A 완료 후 → 6-B (TIER 2 적응 병합) 병렬 착수 가능 |

---

### 병렬 세션 격리 규칙

- 각 세션은 **담당 에이전트 파일 1개만** 생성·수정
- `.claude/agents/README.md` **수정 금지** → Phase 6-D 일괄 세션 전용
- 루트 `docs/` 4문서 **수정 금지** → Phase 6-D 일괄 세션 전용
- `base_code_plan.md` **수정 금지** → Phase 6-D 전용

---

### 에이전트별 실행 계획

---

#### 6-A-1: `pipeline-orchestrator.md`

**원본:** `specialized/agents-orchestrator.md` (agency-agents)
**파일 경로:** `.claude/agents/pipeline-orchestrator.md`

**역할:**
- 복잡한 멀티 에이전트 작업을 단계별 파이프라인으로 조율
- 하위 에이전트 호출 순서, 의존성, 품질 게이트 관리
- 현재 하네스에 없는 "에이전트가 에이전트를 조율" 메타 레이어

**하네스 적응 포인트:**
- agency-agents의 NEXUS 7단계 → 하네스 4문서 체계 기반으로 재설계
- 호출 가능 에이전트 목록을 하네스 16개 기준으로 재작성
- 출력 포맷: `docs/task.md` 기반 파이프라인 계획서

**필수 포함 섹션:**
```
1. 파이프라인 설계 원칙
2. 에이전트 호출 단계 (탐색 → 계획 → 실행 → 검증 → 완료)
3. 사용 가능한 하네스 에이전트 목록 (역할별 분류)
4. 품질 게이트 체크포인트 (단계 간 전환 기준)
5. 실행 예시 (FastAPI + Next.js 신규 기능 개발 파이프라인)
```

**검증 방법:**
```bash
ls .claude/agents/pipeline-orchestrator.md
# YAML frontmatter 확인
grep "name:\|description:\|tools:" .claude/agents/pipeline-orchestrator.md | head -5
```

---

#### 6-A-2: `reality-checker.md`

**원본:** `testing/reality-checker.md` (agency-agents)
**파일 경로:** `.claude/agents/reality-checker.md`

**역할:**
- 모든 작업의 최종 품질 판정자 — "기본값은 NEEDS WORK"
- 증거(테스트 결과, 빌드 로그, 스크린샷 등) 없이는 PASS 불가
- `checklist.md` DoD 기준과 연동하여 완료 여부 판정

**하네스 적응 포인트:**
- agency-agents의 다중 기준 → 하네스 `checklist.md` DoD 항목과 1:1 대응
- 판정 기준: 코드 품질 + 테스트 통과 + 문서 업데이트 + 빌드 성공
- 출력 포맷: PASS / NEEDS WORK + 미충족 항목 목록

**필수 포함 섹션:**
```
1. 판정 철학 ("기본값 NEEDS WORK")
2. 증거 수집 단계 (무엇을 확인하는가)
3. PASS 기준 (하네스 DoD 연동)
4. NEEDS WORK 보고서 형식
5. 실행 예시
```

**검증 방법:**
```bash
ls .claude/agents/reality-checker.md
grep "NEEDS WORK\|PASS" .claude/agents/reality-checker.md | head -5
```

---

#### 6-A-3: `accessibility-auditor.md`

**원본:** `testing/accessibility-auditor.md` (agency-agents)
**파일 경로:** `.claude/agents/accessibility-auditor.md`

**역할:**
- WCAG 2.2 / ARIA 기준 접근성 점검 자동화
- 현재 하네스에 완전 부재한 접근성 검증 영역
- `nextjs-frontend-guidelines` 스킬과 연계

**하네스 적응 포인트:**
- 대상 스택: `base/nextjs/` 컴포넌트 중심
- 자동화 도구: axe-core, pa11y, Chrome DevTools 접근성 패널
- 심각도 분류: Critical(WCAG A 위반) / High(WCAG AA) / Medium(WCAG AAA) / Low(Best Practice)

**필수 포함 섹션:**
```
1. 점검 범위 (색상 대비, 키보드 내비게이션, 스크린리더, 폼 레이블 등)
2. 자동 점검 도구 실행 단계
3. 수동 점검 체크리스트
4. 심각도 분류 기준
5. 수정 권고 포맷
6. 실행 예시
```

**검증 방법:**
```bash
ls .claude/agents/accessibility-auditor.md
grep "WCAG\|aria\|Critical\|High" .claude/agents/accessibility-auditor.md | head -5
```

---

#### 6-A-4: `evidence-collector.md`

**원본:** `testing/evidence-collector.md` (agency-agents)
**파일 경로:** `.claude/agents/evidence-collector.md`

**역할:**
- 작업 완료의 증거(테스트 로그, 빌드 출력, API 응답 등)를 수집·정리
- `reality-checker.md`와 연동 — 증거 수집 후 PASS 판정에 활용
- `docs/checklist.md` DoD 항목별 증거 파일 생성

**하네스 적응 포인트:**
- 증거 저장 경로: `docs/evidence/[YYYY-MM-DD]-[task]-evidence.md`
- 수집 대상: pytest 결과, tsc 출력, E2E 스크린샷, API 응답 샘플
- `test-coverage-agent`와 역할 분리: coverage=내부 분석, evidence=외부 증거

**필수 포함 섹션:**
```
1. 증거 유형 분류 (테스트/빌드/실행/문서)
2. 수집 단계 (스택별 명령어)
3. 증거 문서 포맷
4. reality-checker 연계 방법
5. 실행 예시
```

**검증 방법:**
```bash
ls .claude/agents/evidence-collector.md
grep "evidence\|증거\|docs/evidence" .claude/agents/evidence-collector.md | head -5
```

---

#### 6-A-5: `devops-automator.md`

**원본:** `engineering/devops-automator.md` (agency-agents)
**파일 경로:** `.claude/agents/devops-automator.md`

**역할:**
- CI/CD 파이프라인 코드 생성 및 자동화
- GitHub Actions 워크플로우 설계 + 도커 컨테이너화 지원
- 기존 `pr-review-trigger` 훅의 보완 에이전트 — 훅=감지, 에이전트=설계

**하네스 적응 포인트:**
- 기준 스택: `base/fastapi/`, `base/nextjs/`, `base/nestjs/` Dockerfile + docker-compose
- AWS ECS 배포 패턴: `aws-guidelines` 스킬 기반
- `.github/workflows/` 템플릿 생성 지원 (pr-code-review.yaml 패턴 참조)

**필수 포함 섹션:**
```
1. CI/CD 설계 단계
2. 스택별 파이프라인 패턴 (FastAPI/Next.js/NestJS)
3. Dockerfile 최적화 가이드 (멀티 스테이지)
4. GitHub Actions 워크플로우 템플릿 생성
5. 실행 예시 (FastAPI ECR 배포 파이프라인)
```

**검증 방법:**
```bash
ls .claude/agents/devops-automator.md
grep "GitHub Actions\|Dockerfile\|ECR\|CI/CD" .claude/agents/devops-automator.md | head -5
```

---

#### 6-A-6: `rapid-prototyper.md`

**원본:** `engineering/rapid-prototyper.md` (agency-agents)
**파일 경로:** `.claude/agents/rapid-prototyper.md`

**역할:**
- 신기능 아이디어를 빠르게 PoC/MVP로 구현
- 완성도보다 동작 검증 우선 — 이후 리팩토링은 `code-refactor-master.md`가 담당
- `base/[stack]/` 베이스 코드를 출발점으로 활용

**하네스 적응 포인트:**
- PoC 저장 경로: `poc/[feature-name]/` (임시, 검증 후 본 코드로 이동)
- 검증 기준: "동작 확인 가능한 최소 구현"
- 기존 에이전트와 역할 분리: rapid-prototyper=PoC, code-refactor-master=개선

**필수 포함 섹션:**
```
1. PoC 구현 원칙 (동작 우선, 완성도 후순위)
2. 스택별 빠른 시작 패턴
3. PoC → 본 코드 전환 기준
4. 검증 단계 (최소 동작 확인)
5. 실행 예시
```

**검증 방법:**
```bash
ls .claude/agents/rapid-prototyper.md
grep "PoC\|prototype\|MVP\|빠른" .claude/agents/rapid-prototyper.md | head -5
```

---

#### 6-A-7: `technical-writer.md`

**원본:** `support/` 문서 작성 에이전트 (agency-agents)
**파일 경로:** `.claude/agents/technical-writer.md`

**역할:**
- 코드 기반 기술 문서 자동 생성 (README, API 문서, 아키텍처 설명)
- 기존 `documentation-architect.md`와 역할 분리:
  - documentation-architect = 문서 구조 설계
  - technical-writer = 실제 문서 내용 작성
- `api-spec-generator`와 연계: spec 생성 → writer가 설명 보완

**하네스 적응 포인트:**
- 대상 산출물: README.md, CONTRIBUTING.md, API 가이드, 아키텍처 ADR
- 스타일 기준: 하네스의 4문서 체계 문서 스타일 참조
- 코드 분석 → 문서 생성 자동화

**필수 포함 섹션:**
```
1. 문서 유형별 생성 단계
2. README.md 표준 구조
3. API 문서 작성 가이드 (api-spec-generator 연계)
4. 아키텍처 설명 패턴
5. 실행 예시
```

**검증 방법:**
```bash
ls .claude/agents/technical-writer.md
grep "README\|문서\|documentation" .claude/agents/technical-writer.md | head -5
```

---

### 수정 대상 파일 요약

| 에이전트 | 파일 | 작업 | 병렬 여부 |
|---|---|---|---|
| `pipeline-orchestrator` | `.claude/agents/pipeline-orchestrator.md` | 신규 생성 | ✅ 가능 |
| `reality-checker` | `.claude/agents/reality-checker.md` | 신규 생성 | ✅ 가능 |
| `accessibility-auditor` | `.claude/agents/accessibility-auditor.md` | 신규 생성 | ✅ 가능 |
| `evidence-collector` | `.claude/agents/evidence-collector.md` | 신규 생성 | ✅ 가능 |
| `devops-automator` | `.claude/agents/devops-automator.md` | 신규 생성 | ✅ 가능 |
| `rapid-prototyper` | `.claude/agents/rapid-prototyper.md` | 신규 생성 | ✅ 가능 |
| `technical-writer` | `.claude/agents/technical-writer.md` | 신규 생성 | ✅ 가능 |

**병렬 세션에서 수정 금지 (6-D 전용):**
- `.claude/agents/README.md`
- `docs/` 루트 4문서
- `base_code_plan.md`
- `CLAUDE.md`

---

### 에이전트 완성 기준 (DoD)

| 항목 | 필수 여부 |
|---|---|
| 역할 및 목적 명시 (1-2문장) | ✅ 필수 |
| 실행 단계 순서 명시 (번호 목록) | ✅ 필수 |
| 사용 가능 도구 목록 | ✅ 필수 |
| 출력 형식 정의 (결과물 예시 포함) | ✅ 필수 |
| 실행 예시 1개 이상 | ✅ 필수 |
| agency-agents 원본 출처 주석 | 권장 |
| 기존 에이전트와 역할 분리 명시 | 6-A는 해당 없음 |

---

### 전체 검증 (6-A 완료 기준)

```bash
# 7개 파일 존재 확인
ls .claude/agents/pipeline-orchestrator.md
ls .claude/agents/reality-checker.md
ls .claude/agents/accessibility-auditor.md
ls .claude/agents/evidence-collector.md
ls .claude/agents/devops-automator.md
ls .claude/agents/rapid-prototyper.md
ls .claude/agents/technical-writer.md

# 각 파일 YAML frontmatter 확인
grep -l "^---" .claude/agents/*.md | wc -l  # README 제외하고 23개 이상이어야 함

# 6-B 착수 조건 확인
# → 7개 전원 DoD 충족 시 6-B 및 6-C 병렬 착수 가능
```

---

*Last updated: 2026-03-09 (Phase 6-A 상세 계획 수립 — 7개 에이전트 신규 생성 계획)*
