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

- **현재 Phase:** Phase 12-B — 실 프로젝트 적용 (1차 사례 완료, 2차 적용 대기)
- **직전 완료:** Phase 12-B 1차 적용 (2026-04-13) — see-through (S14P31A206) 적용 + 신규 스킬 2개 역전파
- **방침:** 실 프로젝트 적용 피드백 기반 개선

### 자산 현황 (v1.7.0)

| 자산 | 수량 | 비고 |
|---|---|---|
| 베이스 코드 | 9개 | FastAPI/Next.js/Django/NestJS/Spring Boot/Express/React Native/C/C++ |
| Skills | 26개 | skill-rules.json v1.4 (Phase 12-B 역전파로 +2: react-vite-pwa, threejs-realtime) |
| Agents | 29개 | Phase 11-A P0 수정 완료 |
| Commands | 11개 | quality-gate + dev-qa-loop 포함 |
| Hooks | 3종 | 4문서 자동화 2 + CI/CD 1 |

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

*Last updated: 2026-04-13 (Phase 12-B 1차 적용 완료 — see-through 적용 + 신규 스킬 2개 역전파)*
