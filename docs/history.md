# History — 작업 맥락 문서

> **문서 역할:** 이전까지 진행된 모든 작업 내용을 기록하고,
> **다음 세션에서 task.md 작업의 맥락**을 이어받을 수 있도록 작성하는 문서.
> 컨텍스트 컴팩션 또는 세션 재시작 시 이 문서를 먼저 읽어 맥락을 복원한다.

---

## 문서 사용법 — 에이전트 핸드오프 포맷 (Optional)

> 에이전트 파이프라인 실행 시 **발신 에이전트 → 수신 에이전트** 간 컨텍스트 전달을 표준화하는 선택적 섹션.
> 단일 에이전트 작업에서는 생략 가능.

**6개 필수 필드:** 발신 에이전트 / 완료 작업 / 산출물 경로 / 수신 에이전트 / 인계 컨텍스트 / 착수 조건

```markdown
## 에이전트 핸드오프 — [작업명]
*작성 시점: [날짜] | 발신: [에이전트명] → 수신: [에이전트명]*

| 항목 | 내용 |
|---|---|
| **발신 에이전트** | [이름] |
| **완료 작업** | [1문장 요약] |
| **산출물 경로** | `[경로]` |
| **수신 에이전트** | [이름] |
| **인계 컨텍스트** | [핵심 정보] |
| **착수 조건** | [확인 사항] |
```

---

## 현재 상태 요약

- **현재 Phase:** Phase 14 완료 + v1.9.1 정합성 패치 (2026-04-20) — 다음 작업 대기
- **직전 완료:** v1.9.1 루트 README 드리프트 수정 (2026-04-20) + Phase 14 Opus 4.7 자산 최적화 (2026-04-20, v1.9.0)
- **방침:** Opus 4.7 특성 기반 자산 정밀 최적화 완료, 후속 Phase 14-B2(resources/ 분리)는 별도 세션
- **확정 원칙:** 3-tier model 명시 / 500줄 엄격 + resources 분리 / block 현행 유지

### 자산 현황 (v1.9.1)

| 자산 | 수량 | 비고 |
|---|---|---|
| 베이스 코드 | 9개 | FastAPI/Next.js/Django/NestJS/Spring Boot/Express/React Native/C/C++ |
| Skills | 30개 | skill-rules.json **v1.6**. description 간결화 5개(docx/pdf/skill-developer/embedded-c/spring-boot) |
| Agents | 33개 | **model tier 전수 명시** (opus 11 / sonnet 17 / haiku 5). tools YAGNI 3건 정리 |
| Commands | 11개 | quality-gate + dev-qa-loop 포함 |
| Hooks | 3종 (8 스크립트) | 4문서 자동화 2 + CI/CD 1 + 보조 5 |

---

## Phase 0~9 완료 요약 (2026-03-01 ~ 2026-03-11)

> 상세 이력은 git log 참조. 여기서는 각 Phase 핵심 산출물만 기록.

| Phase | 기간 | 핵심 산출물 |
|---|---|---|
| **0** | 3/1 | 4문서 체계 + base_code_plan.md + CLAUDE.md Agent 섹션 |
| **1-A** | 3/1 | base/fastapi/ + base/nextjs/ 생성, 병렬 세션 규칙 + DoD 작성 |
| **1-B** | 3/1~3/2 | 7개 스택 생성 (express, django, nestjs, spring-boot, react-native, c-embedded, cpp-embedded) |
| **1-C** | 3/2 | 9개 스택 통합 검증 완료, 역전파 프로세스 5단계 문서화 |
| **2-A** | 3/2 | skill-rules.json v1.1 정비 (유령 13개 제거 + 미등록 9개 추가) |
| **2-B** | 3/2~3/7 | 신규 스킬 10개 생성 (express/django/nestjs/spring-boot/react-native/c/cpp/docker/aws/db) |
| **2-C** | 3/3~3/7 | 기존 스킬 14개 품질 점검 (fastapi 667→436줄, vercel 147→365줄 등) |
| **2-D** | 3/8 | skill-rules.json v1.2 (24개 등록) + README 재작성 |
| **3-A** | 3/8~3/9 | 에이전트 5개 생성 (db-migration, api-spec, test-coverage, performance, security) |
| **3-B** | 3/9 | README 16개 통합 + base_code_plan 업데이트 |
| **4-A** | 3/9 | session-start-docs-loader + docs-update-reminder 훅 생성 + 동작 검증 |
| **4-B** | 3/9 | skill-rules.json v1.3 오탐 5건 수정 (async/await, migration, hook, test, monitoring) |
| **4-C** | 3/9 | pr-review-trigger 훅 + pr-code-review.yaml GH Actions 생성 |
| **4-D** | 3/9 | /harness-plan-sync 커맨드 생성 + /harness-docs-update Step 0 추가 |
| **5-A** | 3/9 | HARNESS_USAGE_GUIDE.md + /harness-apply 커맨드 |
| **5-B** | 3/9 | harness-init.sh + /harness-init + CHANGELOG.md + /harness-changelog + /harness-backport |
| **5-C** | 3/9 | HARNESS_COMMANDS_REFERENCE.md 신규 생성 |
| **6-A** | 3/10 | TIER 1 에이전트 7개 생성 (pipeline-orchestrator, reality-checker, accessibility-auditor 등) |
| **6-B** | 3/10 | TIER 2 에이전트 6개 생성 (security-engineer, performance-benchmarker, api-tester 등) |
| **6-C** | 3/10 | /quality-gate + /dev-qa-loop 커맨드 + history.md 핸드오프 포맷 |
| **6-D** | 3/10 | 일괄 통합 — agents/README 29개, CHANGELOG v1.6.0, agency-agents 삭제 |
| **8-A** | 3/10 | P0 4건: tsc-check PostToolUse 이동, trigger-build-resolver 재작성, exit(1)→exit(0) |
| **8-B** | 3/10 | P1 7건: set -e 제거 3파일, error-handling-reminder 버그 4개, TTL 정리, .mjs 삭제 |
| **8-C** | 3/11 | P1 3건: 에이전트 10개 tools: 추가, planner.md 경로, auth-route-debugger 범용화 |
| **8-D** | 3/11 | P2 6건: async 제거 3파일, pnpm-lock 삭제, MCP 설정, GH Actions 중복 댓글 방지 |
| **8-E** | 3/11 | 통합 검증 + tsc-check jq→node 추가 수정 + CHANGELOG v1.6.1 |
| **9-A** | 3/11 | CLAUDE.md 4문서 정의 재정의 + 생명주기 규칙 |
| **9-B** | 3/11 | 훅 파이프라인 3단계 완성 (next-task-pending.flag, task.md 구조 검증) |
| **9-C** | 3/11 | harness-plan-sync Step 0 사전 검증 (history.md 미갱신 시 중단) |

---

## Phase 10 — 2차 코드리뷰 수정 (2026-03-11, v1.6.2)

**발견:** P0 1건 / P1 4건 / P2 8건 — 총 13건

**주요 수정:**

| ID | 파일 | 수정 |
|---|---|---|
| A-1 | `post-tool-use-tracker.sh` | jq → node JSON 파싱 (Windows jq 미설치 대응) |
| B-1/B-2 | sh 래퍼 2개 | `\|\| exit 0` 추가 |
| B-3 | `planner.md` | 구식 3-파일 템플릿 전체 제거 |
| B-4 | `settings.json` | `"Bash:*"` 제거 (보안) |
| C-2 | `.mcp.json` | .gitignore 추가 + `.mcp.json.example` 생성 |
| C-3/C-4 | `pr-code-review.yaml` | SHA 핀닝 + `permissions: read-all` |
| C-6 | `error-handling-reminder.ts` | `split(':')` → indexOf/slice (Windows 경로 안전) |
| C-7 | `skill-activation-prompt.ts` | RegExp 캐시 추가 |

---

## Phase 11 — 3차 심층 코드리뷰 (2026-03-11)

**발견:** P0 7건 / P1 25건 / P2 24건 / P3 13건 — 총 64건

### Phase 11-A 완료: P0 보안 + 기능 결함 7건 수정

| ID | 파일 | 수정 | 검증 |
|---|---|---|---|
| A-1~A-3 | `.ts` 훅 3개 | session_id 경로 순회 차단 (`replace(/[^a-zA-Z0-9_-]/g, '_')`) | ✅ |
| A-4 | `post-tool-use-tracker.sh` | session_id bash 새니타이징 (`tr -cd`) | ✅ |
| A-5 | `post-tool-use-tracker.sh` | `$repo_path` 7곳 인용 (`"$repo_path"`) | ✅ |
| A-6 | `.mcp.json` | `git rm --cached` 추적 해제 | ✅ |
| A-7 | `auth-route-tester.md` | 크리덴셜→환경변수, PROJECT_KNOWLEDGE→CLAUDE.md | ✅ |
| A-8 | `pipeline-orchestrator.md` | `dev/active/` 4곳→`docs/` | ✅ |
| A-9 | `skill-developer/SKILL.md` | `skill-verification-guard.ts`→`skill-activation-prompt.ts` | ✅ |
| 추가 | 4개 파일 | password123, skill-verification-guard 잔재 제거 | ✅ |

### Phase 11-B~E: 백로그 전환 (30건)

실 프로젝트 적용 피드백과 함께 처리. 상세 항목은 `docs/plan.md` Phase 11 섹션 참조.

---

## Phase 12-B 첫 적용 (2026-04-13) — see-through (S14P31A206)

**대상:** SSAFY 14기 자율 프로젝트, 복합 스택 모노레포 (Spring Boot + FastAPI + ESP-IDF + React/Vite/PWA/Three.js)

**적용 결과:**
- `.claude/` 전체 + 베이스 코드 3개(spring-boot/fastapi/c-embedded) 복사
- 디렉토리명 옵션 C 채택: `api/csi/esp-node/dashboard`
- 4문서 병행 운영 (A안): SSAFY 팀 기획(`docs/projects/`) + Claude 작업(`docs/`)
- 8개 훅 정상 권한 + npm install 완료
- 세션 시작 훅 시뮬레이션 통과 ("Phase 1 — 스캐폴딩 검증" 정확 출력)

**역전파 (즉시 반영):**
- 신규 스킬 2개: `react-vite-pwa-guidelines`, `threejs-realtime-guidelines` (T3 스켈레톤)
- `skill-rules.json` v1.3 → v1.4 (트리거 2개 추가)
- `.claude/skills/README.md` 24개 → 26개

**발견 이슈 (Phase 12-C 백로그):**
- Vite 기반 React PWA용 베이스 코드 부재 → `base/react-vite-pwa/` 신설 검토 필요
- `nextjs-frontend-guidelines`가 React+Vite 프로젝트를 잘못 트리거할 위험 — pathPattern을 `_disabled_`로 우회했지만, 키워드 트리거(`React`)가 광범위 → 정밀화 필요
- 하네스 적용 시 `.session-cache/`, `tsc-cache/`, `settings.local.json` 같은 작업 캐시가 함께 복사됨 → `harness-init.sh` / `/harness-apply` 커맨드에 자동 제외 로직 필요
- 복합 스택(4개) 모노레포의 `CLAUDE.md` 작성에 표준 템플릿 부재 — `base_code_plan.md`에 "복합 스택 CLAUDE.md 패턴" 섹션 추가 검토

---

## Phase 13 완료 (2026-04-16, v1.8.0) — 데이터/AI 트랙 역전파

**대상:** HERE:O (S14P31A206) — CSI 신호처리 기반 실내 위치 추정 시스템에서 EDA → 파이프라인 → 학습 → 평가 전주기 패턴 발견

**역전파 자산 (4+4):**

| 카테고리 | 신규 자산 | 핵심 패턴 |
|---|---|---|
| Skill | `data-analysis-guidelines` | pandas/polars EDA, 결측/이상치, 시각화 |
| Skill | `data-pipeline-guidelines` | Batch/Stream, Airflow/Prefect/Dagster, 메달리온 Bronze/Silver/Gold, GE |
| Skill | `ml-training-guidelines` | sklearn/LightGBM/PyTorch, MLflow, Optuna, GroupKFold 누수 방지 |
| Skill | `ml-evaluation-guidelines` | 분류/회귀/시계열 지표, McNemar/5x2 CV, baseline 회귀 감지 |
| Agent | `data-analyst` | EDA → 인사이트 리포트 + 시각화 PNG 생성 |
| Agent | `data-pipeline-architect` | Mermaid 아키텍처 + DAG 스캐폴드 + GE suite |
| Agent | `ml-engineer` | 학습 + MLflow + 추론 엔드포인트 + Model Card |
| Agent | `ml-evaluator` | holdout 평가 + 누수 검증 + baseline 회귀 감지 |

**유보:** `csi-signal-processing-guidelines` (WiFi CSI 특화, 범용성 낮음 → S14P31A206 전용 유지)

**`skill-rules.json` 갱신:** v1.4 → v1.5 (4개 신규 트리거 등록, pathPatterns 범용화: `ml/**`, `notebooks/**`, `pipelines/**`, `base/ml/**`)

---

## Phase 14 완료 (2026-04-20, v1.9.0) — Opus 4.7 자산 최적화

**배경:** 사용자 "opus 4.7 기준으로 스킬 및 에이전트 최적화 하려고 하는데, 계획 한번 세워줘" 요청. 에이전트 주도 합리적 근거 기반 의사결정 + 토의 형식 근거 공개 + 결론 바로 실행 방식으로 진행.

**확정 원칙 (토의 결과):**

| 포인트 | 결정 | 근거 |
|---|---|---|
| Model Tier | 3-tier 전수 명시 (opus/sonnet/haiku) | Opus 4.7 subagent 라우터의 model hint 활용 |
| 500줄 룰 | 엄격 유지 + 포화 스킬 `resources/` 분리 | Anthropic progressive disclosure 공식 원칙 |
| Block Enforcement | Next.js 1개 유지 | 다른 영역 오탐 위험 > 실수 방지 이익 |

**Phase 14-A 진단:** `docs/phase-14-diagnosis.md` 작성 — 자산 현황 갭 분석 + tier 매트릭스 확정

**Phase 14-B 경량 최적화 (5건):**
- Skills description 간결화: `docx`(904자→280자), `pdf`(500자→85자), `skill-developer`(500자→280자), `embedded-c-guidelines`(500자→290자), `spring-boot-guidelines`(500자→180자)
- 500줄 포화 4개 스킬(`react-native`, `pptx`, `express`, `django`) `resources/` 분리는 공수 대비 효용 제한으로 **Phase 14-B2(후속 세션) 유보**

**Phase 14-C Agents 최적화:**
- **33개 agent `model:` 필드 전수 명시** (32회 Edit 성공 + code-refactor-master 1개는 이미 opus 명시 유지)
- tools YAGNI 3건: `plan-reviewer`(WebSearch/WebFetch 제거), `frontend-error-fixer`(Grep 추가), `auto-error-resolver`(Glob/Grep 추가)
- `agents/README.md` "Opus 4.7 Operating Principles" 섹션 신규 추가 (Model Tier / Extended Thinking / 병렬 tool calls / 출력 형식 표준)

**Phase 14-D skill-rules.json v1.6:**
- 버전 bump. enforcement=block은 Next.js 1개 유지.
- 오탐 실측 결과 다른 영역 block 필요성 없음 확인

**Phase 14-E 릴리즈:**
- `CHANGELOG v1.9.0` 작성 (Added / Changed / Decided / Deferred / Context 섹션)
- `base_code_plan.md` v1.8.1 → v1.9.0
- 4문서(plan/task/checklist/history) 갱신
- git commit + push

**효과:**
- Opus 4.7 subagent 라우터가 tier에 맞게 model 선택 → 비용·속도 최적화
- 복잡 추론은 opus, 표준 업무는 sonnet, 단순 조회는 haiku로 분산
- description 간결화로 라우터 매칭 정확도 향상

---

## 정합성 동기화 (2026-04-20, v1.8.1)

**배경:** Phase 13 commit (5cada71)에서 일부 메타 문서 갱신 누락 — 사용자 "스킬/에이전트 상세 재검증" 요청 시 정합성 점검 중 발견.

**진단 결과:**
- ✅ 자산 본체 — Skills 30개 + Agents 33개 + skill-rules.json v1.5 모두 DoD PASS
- ❌ 메타 문서 — 6 위치에 v1.7.0 / 26개 / 29개 / v1.4 잔존

**일괄 동기화:**
- `base_code_plan.md`: 버전 v1.7.0 → v1.8.1, "Skills (26개)" → "(30개)", "Agents (29개)" → "(33개)" + 표에 데이터/AI 4+4행 추가, Phase 3 완료 (29) → (33)
- `.claude/agents/README.md`: "Quick Reference (29)" → "(33)" + 표 4행 추가
- `docs/history.md`: 자산 현황 + 현재 상태 요약 + Phase 13 섹션 + 본 정합성 섹션 추가
- `docs/plan.md`: Phase 12-B 체크 + Phase 13 섹션 + 대시보드 갱신
- `docs/task.md` / `checklist.md`: Phase 13 완료 + 정합성 동기화 완료 → 다음 작업 대기 상태 교체
- `CHANGELOG.md`: v1.8.1 정합성 패치 항목 추가

**교훈:** Phase 완료 commit 시 메타 문서 정합성 자동 검증 훅 도입 검토 필요 → Phase 7 상시 운영 항목 추가

---

## Phase 12-A 완료 (2026-03-15) — 병렬 세션 분석 + v1.7.0

**수정 파일:**
- `.claude/commands/harness-plan-sync.md` — Step 4.5 병렬 세션 분석 삽입 + task.md 템플릿에 "세션 전략" 섹션 추가 + 단일 세션 판정 기준
- `.claude/agents/planner.md` — Step 4-1 Parallel Session Analysis 선택 단계 추가
- `CHANGELOG.md` — v1.7.0 릴리즈 블록 작성
- `base_code_plan.md` — 버전 v1.6.2 → v1.7.0

**핵심:** `/harness-plan-sync` 실행 시 Phase 항목들의 수정 범위를 자동 분석하여 독립/순차/일괄 세션 그룹핑 결과를 task.md에 포함. 항목 3개 이하이면 단일 세션으로 간략 출력.

---

## 2026-03-15 — 방침 전환: 실 프로젝트 적용으로 전환

**배경:** Phase 8~11에서 3회 코드리뷰→수정 사이클 반복. P0는 모두 해결, 잔여 P1/P2는 수확 체감 영역. 실 프로젝트 적용 실적 부재가 가장 큰 리스크.

**결정:**
- Phase 11-B~E (P1 20건 + P2 10건) → Known Issues 백로그 전환
- Phase 12 신설: v1.7.0 릴리즈 → 실 프로젝트 적용 → 역전파
- 4문서 축약 정리 실시 (history.md 1000줄→300줄 압축, plan.md 완료 Phase 축약)

---

## 중요 결정 사항

| 결정 | 날짜 |
|---|---|
| 4문서 체계 채택 (plan/task/history/checklist) | 2026-03-01 |
| base/[stack]/docs/ 스택별 독립 문서 운영 | 2026-03-01 |
| 병렬 세션 격리 원칙 (스택별/스킬별/에이전트별) | 2026-03-01 |
| 원본 보존 (backend/frontend/ 삭제 않고 base/ 별도 생성) | 2026-03-01 |
| DoD 전수 확인 + base/ 코드 교차 검증 품질 방법론 | 2026-03-03 |
| task/checklist 완료 시 history 흡수 → 교체 생명주기 | 2026-03-09 |
| 외부 에이전트 선택적 흡수 원칙 (하네스 포커스에 맞는 것만) | 2026-03-10 |
| 중복 확인 이슈 우선순위 상향 원칙 | 2026-03-10 |
| **실사용 우선 원칙 — 내부 품질 개선보다 실 프로젝트 적용 피드백 우선** | **2026-03-15** |

---

## 다음 세션에서 이어갈 작업

### Phase 12-B — 실 프로젝트 적용

1. 대상 프로젝트 선정 (사용자)
2. `/harness-apply` 또는 `harness-init.sh` 실행
3. 적용 후 검증 (훅, 스킬 트리거, 에이전트, 병렬 세션 전략 자동 생성)
4. 이슈 수집 → `/harness-backport` 역전파

---

*Last updated: 2026-04-20 (v1.9.2 — Phase 14 + 정합성 패치 2회 최종 완료)*
