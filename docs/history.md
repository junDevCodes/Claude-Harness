# History — 작업 맥락 문서

> **문서 역할:** 이전까지 진행된 모든 작업 내용을 기록하고,
> **다음 세션에서 task.md 작업의 맥락**을 이어받을 수 있도록 작성하는 문서.
> 컨텍스트 컴팩션 또는 세션 재시작 시 이 문서를 먼저 읽어 맥락을 복원한다.

---

## 문서 사용법 — 에이전트 핸드오프 포맷 (Optional)

> 에이전트 파이프라인 실행 시 **발신 에이전트 → 수신 에이전트** 간 컨텍스트 전달을 표준화하는 선택적 섹션.
> 단일 에이전트 작업에서는 생략 가능. `pipeline-orchestrator` 에이전트 주도 파이프라인 실행 시 작성 권장.

### 핸드오프 포맷 템플릿

```markdown
## 에이전트 핸드오프 — [작업명]
*작성 시점: [날짜] | 발신: [에이전트명] → 수신: [에이전트명]*

| 항목 | 내용 |
|---|---|
| **발신 에이전트** | [에이전트 이름] |
| **완료 작업** | [무엇을 했는가 — 1문장] |
| **산출물 경로** | `[파일 또는 디렉토리 경로]` |
| **수신 에이전트** | [다음 에이전트 이름] |
| **인계 컨텍스트** | [수신 에이전트가 반드시 알아야 할 핵심 정보] |
| **착수 조건** | [수신 에이전트가 시작 전 확인해야 할 사항] |
| **주의사항** | [특이사항 또는 Known Issue] |
```

**6개 필수 필드:**

| 필드 | 설명 |
|---|---|
| **발신 에이전트** | 작업을 완료하고 인계하는 에이전트 이름 |
| **완료 작업** | 발신 에이전트가 이번 세션에서 완료한 작업 — 1문장 요약 |
| **산출물 경로** | 생성/수정된 파일 또는 디렉토리 경로 (복수 시 쉼표 구분) |
| **수신 에이전트** | 다음 작업을 이어받을 에이전트 이름 |
| **인계 컨텍스트** | 수신 에이전트가 착수 전 반드시 알아야 할 핵심 결정 사항, 전제 조건, 설계 맥락 |
| **착수 조건** | 수신 에이전트가 시작 전 확인해야 할 사항 (파일 존재, 선행 작업 완료 여부 등) |

### 작성 예시 — Phase 6-B ux-researcher → ui-designer 핸드오프

```markdown
## 에이전트 핸드오프 — 로그인 플로우 UX 분석 → UI 설계
*작성 시점: 2026-03-10 | 발신: ux-researcher → 수신: ui-designer*

| 항목 | 내용 |
|---|---|
| **발신 에이전트** | ux-researcher |
| **완료 작업** | 로그인/회원가입 플로우 UX 리서치 완료 — 사용자 인터뷰 5건 + 경쟁사 분석 3건 |
| **산출물 경로** | `docs/ux/2026-03-10-login-flow-ux-research.md` |
| **수신 에이전트** | ui-designer |
| **인계 컨텍스트** | 주요 Pain Point: 비밀번호 재설정 플로우 3단계 → 1단계 단축 권고. 모바일 우선 설계 필요(사용자 72% 모바일 접속). 접근성: 색상 대비 WCAG AA 기준 미달 버튼 2개 수정 필요 |
| **착수 조건** | `docs/ux/2026-03-10-login-flow-ux-research.md` 파일 존재 확인. 페르소나 문서(섹션 3) 숙지 후 컴포넌트 설계 시작 |
| **주의사항** | 다크모드 대응은 이번 스코프 외. accessibility-auditor 에이전트와 UI 구현 후 반드시 연동 검증 필요 |
```

### pipeline-orchestrator 연계

`pipeline-orchestrator` 에이전트가 멀티 에이전트 파이프라인을 조율할 때 각 에이전트 완료 시점에 이 핸드오프 섹션을 history.md에 자동으로 작성하도록 설계하는 것을 권장합니다. 각 에이전트는 작업 완료 후 `/harness-docs-update`를 실행하거나 직접 history.md에 핸드오프 섹션을 추가합니다.

---

## 현재 상태 요약

- **현재 Phase:** Phase 9 전체 완료 — 4문서 체계 고도화 (9-A/B/C)
- **직전 완료:** 2026-03-11 Phase 9-A/B/C — CLAUDE.md 재정의 + 훅 파이프라인 완성 + harness-plan-sync 강화
- **다음 작업:** Phase 7 상시 운영 (7-A: skill-rules 오탐 수정 / 7-B: 역전파)

### 자산 현황 스냅샷 (2026-03-11 Phase 8-E 완료 기준 — v1.6.1)

| 자산 유형 | 완료 수 | 목표 | 비고 |
|---|---|---|---|
| 베이스 코드 스택 | 9개 | 9개 | 전체 완성 |
| Skills | 24개 | 24개 | skill-rules.json v1.3 |
| **Agents** | **29개** | **29개** | **Phase 8-C DoD 수정 완료 (tools: 10개 추가)** |
| Slash Commands | **11개** | 11개 | quality-gate + dev-qa-loop 포함 |
| 초기화 스크립트 | 1개 | 1개 | `scripts/harness-init.sh` |
| 버전 관리 | 1개 | 1개 | `CHANGELOG.md` **v1.6.1** (Phase 8-E 릴리즈 완료) |
| 사용자 가이드 | 2개 | 2개 | Phase 5-C 완료 |
| 훅 (4문서 자동화) | 2개 | 2개 | Phase 8 코드 품질 개선 완료 |
| 훅 (CI/CD 연동) | 1개 | 1개 | Phase 8-D 중복 댓글 수정 완료 |
| GitHub Actions 템플릿 | 1개 | 1개 | Phase 8-D 수정 완료 |

> **Phase 8 전체 완료 (2026-03-11):** P0/P1/P2 총 20건+ 수정. tsc-check.sh jq→node 포함. CHANGELOG v1.6.1 릴리즈.

---

## 작업 이력

### 2026-03-11 — Phase 9 착수: 4문서 체계 고도화

**배경:** Phase 8 전체 완료(2026-03-11) 후 발견된 구조적 개선 사항:
- 4문서 역할 정의 모호 → task.md가 "Phase 전체 실행 계획"으로 오용될 수 있음
- 훅 파이프라인이 "시작 전 읽기 → 종료 후 쓰기"까지만 구현됨. "다음 계획 문서 작성" 3번째 단계 없음
- task/plan 완료 후 문서 생명주기 미정의 → 덮어쓰기 시 history.md 미기록 위험

**Phase 9 구조:**

| Phase | 내용 | 우선순위 |
|---|---|---|
| **9-A** | CLAUDE.md 4문서 정의 재정의 + 생명주기 규칙 추가 | 🔴 P0 |
| **9-B** | 훅 파이프라인 3단계 완성 | 🟠 P1 |
| **9-C** | harness-plan-sync 강화 | 🟡 P2 |

**Phase 9 전체 완료 내용:**

| Phase | 파일 | 수정 내용 | 검증 |
|---|---|---|---|
| **9-A** | `CLAUDE.md` | 4문서 역할 테이블 재정의 (task.md → "바로 다음 진행할 기능 단위", history.md → "영구 맥락 문서"), 생명주기 섹션 신규 추가, task.md 범위 규칙(15개 상한), 완료 절차 순서 변경(history 먼저) | grep 4건 PASS ✅ |
| **9-B** | `session-start-docs-loader.ts` | next-task-pending.flag 감지, task.md 구조 검증(plan.md 참조/필수섹션/15개초과), warnings 배너 출력 | 수동 실행 PASS ✅ |
| **9-B** | `docs-update-reminder.ts` | checklist.md 완료 감지(unchecked===0), next-task-pending.flag 생성, 완료 안내 메시지 출력 | 로직 삽입 확인 ✅ |
| **9-C** | `harness-plan-sync.md` | Step 0 사전 검증 추가 — history.md 미갱신 시 중단/plan.md 전체 완료 시 교체 안내 | 파일 확인 ✅ |

---

### 2026-03-10 — Phase 8-B: P1 HIGH 신뢰성 수정 7건 완료

**수정 항목 (B-1 ~ B-5):**

| ID | 파일 | 수정 내용 | 검증 |
|---|---|---|---|
| **B-1** | `session-start-docs-loader.sh` | `set -e` 제거 + `\|\| exit 0` 추가 | bash -n PASS ✅ |
| **B-2** | `skill-activation-prompt.sh` | 동일 패턴 | bash -n PASS ✅ |
| **B-2-추가** | `post-tool-use-tracker.sh` | `set -e` 제거 (보조 명령 실패 시 비정상 종료 방지) | bash -n PASS ✅ |
| **B-3-삭제** | `stop-build-check-enhanced.sh` | 삭제 (4중 결함: old tsc-cache 의존 + eval + set -e + 기능 중복) | 파일 미존재 확인 ✅ |
| **B-3-수정** | `error-handling-reminder.ts` | 버그 4개 수정 (경로/split/필드명/async) + Stop 훅 등록 | exit 0 PASS ✅ |
| **B-4** | `session-start-docs-loader.ts` | TTL 7일 정리 로직 삽입 (readdirSync/statSync/unlinkSync) | 코드 삽입 확인 ✅ |
| **B-5** | `.mjs` 파일 4개 | dead code 확인 후 삭제 (settings.json 미참조) | 파일 미존재 확인 ✅ |

**핵심 변경 사항:**
- `settings.json` Stop 훅: `trigger-build-resolver.sh` → **`error-handling-reminder.sh` (신규)** → `docs-update-reminder.sh`
- 전체 `.sh` 훅 파일: `set -e` 완전 제거 (UserPromptSubmit/PostToolUse 모두 클린)
- `error-handling-reminder.ts`: `$HOME/.claude/tsc-cache/` → `$CLAUDE_PROJECT_DIR/.claude/tsc-cache/` 경로 수정
- `.mjs` 4개 파일 dead code 정리 완료

**B-3-수정 버그 상세:**
1. 경로: `process.env.HOME` → `process.env.CLAUDE_PROJECT_DIR || process.cwd()`
2. 파싱: `split('\t')` → `split(':')` (post-tool-use-tracker.sh 실제 포맷)
3. 필드명: `file.path` → `file.filePath`, `interface EditedFile` 재정의
4. async 제거: `async function main()` → `function main()`, `main().catch(...)` → `main()`

---

### 2026-03-10 — Phase 8-A: P0 CRITICAL 훅 버그 4건 수정 완료

**수정 항목 (A-0 ~ A-3):**

| ID | 파일 | 수정 내용 | 검증 |
|---|---|---|---|
| **A-0** | `base_code_plan.md` | `v1.5.0` → `v1.6.0` (1줄) | grep 확인 ✅ |
| **A-1** | `tsc-check.sh` + `settings.json` | Stop → PostToolUse 이동, 하드코딩 서비스 목록 제거 (tsconfig.json 탐지), eval 제거 → 직접 npx tsc, $HOME/project → $(pwd) | bash -n PASS ✅ |
| **A-2** | `trigger-build-resolver.sh` | 전면 재작성 — 디버그 로그 코드 제거, 하드코딩 배열 제거, claude --agent 호출 제거, git status --porcelain 기반 범용 감지로 교체 | 로그 미생성 확인 ✅ |
| **A-3** | `session-start-docs-loader.ts` | catch `process.exit(1)` → `process.exit(0)`, `catch (_err)` 묵음 패턴으로 통일 | bad json → exit 0 PASS ✅ |

**핵심 구조 변경:**
- `tsc-check.sh`: Stop 훅 → PostToolUse `Edit|MultiEdit|Write` 매처로 이동 (tool_name 페이로드 활용 가능)
- `settings.json` Stop 훅: `tsc-check.sh` 제거 → `trigger-build-resolver.sh`, `docs-update-reminder.sh` 2개만 유지
- `settings.json` PostToolUse: `post-tool-use-tracker.sh` + `tsc-check.sh` 순서로 실행

---

### 2026-03-10 — Phase 8 계획 수립: 프로덕션 레벨 코드 리뷰 완료 (전면 재정비)

**리뷰 범위:** `.claude/hooks/` 전체 + `.claude/agents/` 29개 + `.claude/settings.json` + `base_code_plan.md` + `.mcp.json` + `.github/workflows/`

**발견 이슈 요약 (재분류 후 최종):**

| 등급 | 건수 | 핵심 이슈 |
|---|---|---|
| 🔴🔴 P0++ (⚠️ 중복 확인) | **2건** | tsc-check.sh Stop훅에 tool_name 없음 (근본 결함) / trigger-build-resolver 디버그 덤프 |
| 🔴 P0 | **3건** | session-start-docs-loader exit(1) / base_code_plan.md v1.5.0 표기 / error-handling-reminder 미등록 파싱 오류 |
| 🟠🟠 P1++ (⚠️ 중복 확인) | **2건** | session-start-docs-loader.sh `set -e` / skill-activation-prompt.sh `set -e` |
| 🟠 P1 | **2건** | Dead hook 2개 / session-cache TTL 없음 (27개 누적) |
| 🟠 P1 (에이전트 DoD) | **3건** | 에이전트 10개 tools: 누락 / planner.md 경로 충돌 / auth-route-debugger 크리덴셜 하드코딩 |
| 🟡 P2 | **7건** | async 불필요 4개 / 이중 lock / MCP 설정 모순 / GH Actions 중복 댓글 |

**이슈 상세 — Phase 8-A (P0 CRITICAL):**

| ID | 파일 | 문제 | 등급 |
|---|---|---|---|
| A-0 | `base_code_plan.md` | 버전 `v1.5.0` (실제 `v1.6.0`) — 1줄 수정 | 🔴 P0 |
| **A-1** | `tsc-check.sh` | ⚠️ **중복 확인** — Stop 훅에 `tool_name` 없음 → TSC 체크 전체 로직 사문화 + 하드코딩 서비스 디렉토리 | 🔴🔴 **P0++** |
| **A-2** | `trigger-build-resolver.sh` | ⚠️ **중복 확인** — 매 Stop마다 `/tmp/claude-hook-debug.log` 디버그 덤프 + `claude --agent` 유효하지 않은 CLI | 🔴🔴 **P0++** |
| A-3 | `session-start-docs-loader.ts` | catch에서 `process.exit(1)` → UserPromptSubmit 프롬프트 차단 가능 | 🔴 P0 |
| A-4 | `error-handling-reminder.ts` | `split('\t')` vs 실제 포맷 `split(':')` 불일치 — **미등록 파일**이라 현재 무영향 | 🟠 P1 강등 |

**이슈 상세 — Phase 8-B (P1 HIGH):**

| ID | 파일 | 문제 | 등급 |
|---|---|---|---|
| **B-1** | `session-start-docs-loader.sh` | ⚠️ **중복 확인** — `set -e` → npx 실패 시 UserPromptSubmit 차단 | 🟠🟠 **P1++** |
| **B-2** | `skill-activation-prompt.sh` | ⚠️ **중복 확인** — `set -e` 동일 문제 | 🟠🟠 **P1++** |
| B-3 | Dead Hook 정리 | `error-handling-reminder.sh` / `stop-build-check-enhanced.sh` settings.json 미등록 | 🟠 P1 |
| B-4 | `.claude/.session-cache/` | TTL 없음 → `.loaded` 파일 27개 무한 누적 | 🟠 P1 |

**이슈 상세 — Phase 8-C (에이전트 DoD 위반 — 신규 발견):**

| ID | 대상 | 문제 | 등급 |
|---|---|---|---|
| C-1 | 에이전트 10개 | YAML frontmatter `tools:` 필드 누락 (`auth-route-debugger`, `auth-route-tester`, `code-architecture-reviewer`, `code-refactor-master`, `documentation-architect`, `frontend-error-fixer`, `plan-reviewer`, `planner`, `refactor-planner`, `web-research-specialist`) | 🟠 P1 |
| C-2 | `planner.md` | description + 내부 로직 모두 `dev/active/` 경로 → 하네스 4문서 체계(`docs/`)와 충돌 | 🟠 P1 |
| C-3 | `auth-route-debugger.md` | 특정 프로젝트 크리덴셜 하드코딩 (`yourRealm`, `testuser`, `testpassword`) + PM2 서비스명 특화 | 🟠 P1 |

**핵심 재분류 결정:**
- `error-handling-reminder.ts` A-1 → A-4/P1 강등: 파싱 오류 심각하나 파일이 `settings.json`에 **미등록** → 현재 완전 무영향
- `tsc-check.sh` B-3(하드코딩) + QA리뷰(Stop에 tool_name 없음 근본 결함) → A-1 P0++ 상향: 기존 계획보다 근본 결함이 더 심각
- Phase 8-C 신설: 에이전트 DoD 위반 10개 + 4문서 체계 충돌 2건 별도 Phase로 분리

**산출물:**
- `docs/plan.md` — Phase 8 전체 계획 전면 재정비 (8-A/B/C/D/E 5단계) + ⚠️ 중복 확인 마크 추가
- `docs/task.md` — Phase 8-A~E 상세 실행 계획 (A-0~A-4 코드 스니펫 포함)
- `docs/checklist.md` — Phase 8 전체 DoD 체크리스트 (⚠️ 중복 확인 마크 + Phase 8-C 신규 추가)
- `docs/history.md` — 현재 세션 맥락 기록

---

### 2026-03-10 — Phase 6-D: 일괄 통합 완료 (Phase 6 전체 완료 — v1.6.0)

**단일 세션에서 5개 산출물 처리 완료:**

| 단계 | 파일 | 작업 내용 |
|---|---|---|
| D-1 | `.claude/agents/README.md` | 16→29개 전면 재작성, 16개 카테고리 재편 (Quick Reference 29행) |
| D-2 | `base_code_plan.md` | Agents 16→29, Commands 9→11, 확장 로드맵 Phase 3 완료 처리, Last updated 갱신 |
| D-3 | `CHANGELOG.md` | v1.6.0 블록 삽입 (Added 13+2, Changed 2, Removed 1) |
| D-4 | `docs/` 4문서 | Phase 6 전체 완료 처리 (plan/task/checklist/history) |
| D-5 | `agency-agents-main/` | `rm -rf` 삭제 완료 — DELETED 확인 |

**Phase 6 전체 완료:**
- 에이전트 총 29개 (기존 16 + TIER 1 7 + TIER 2 6)
- Commands 총 11개 (+quality-gate, +dev-qa-loop)
- CHANGELOG v1.6.0 릴리즈 완료
- agency-agents 분석·흡수·정리 완료

---

### 2026-03-10 — Phase 6-C: NEXUS 패턴 추출 + 커맨드 신규 생성 완료

**단일 세션에서 3개 산출물 생성 완료:**

| 산출물 | 파일 | 핵심 기능 |
|---|---|---|
| `/quality-gate` 커맨드 | `.claude/commands/quality-gate.md` | checklist.md DoD 항목 기반 증거 수집 → PASS/NEEDS WORK 자동 판정 (4단계, Pessimistic Default) |
| `/dev-qa-loop` 커맨드 | `.claude/commands/dev-qa-loop.md` | 구현 → quality-gate → 수정 루프 최대 3회, [Loop N/3] 카운터, 에스컬레이션 안내 |
| 핸드오프 포맷 가이드 | `docs/history.md` (상단 추가) | 에이전트 간 컨텍스트 전달 6개 필드 템플릿 + 실제 작성 예시 + pipeline-orchestrator 연계 |

**NEXUS 패턴 흡수 결과:**
- Quality Gate 철학 → `reality-checker` 에이전트와 동일한 Pessimistic Default 원칙 적용
- Dev-QA Loop → 최대 3회 자동 반복 + 에스컬레이션 분기
- Handoff Template → history.md Optional 섹션으로 표준화

**자산 변경:**
- Slash Commands: 9개 → 11개 (+quality-gate, +dev-qa-loop)

---

### 2026-03-10 — Phase 6-B: TIER 2 에이전트 6개 적응 병합 + 검증 완료 (6/6 PASS)

**병렬 세션에서 6개 에이전트 파일 생성 완료 → 메인 세션 종합 검증 (6/6 PASS):**

| 에이전트 | 파일 | 줄수 | 특이사항 |
|---|---|---|---|
| `security-engineer` | `.claude/agents/security-engineer.md` | 514 | 원본 그대로 — 4단계, FastAPI/NestJS/Spring Boot 전체 |
| `performance-benchmarker` | `.claude/agents/performance-benchmarker.md` | 538 | Locust 인증 플로우 + CI/CD 성능 게이트 강화 |
| `api-tester` | `.claude/agents/api-tester.md` | 746 | Newman + Pact 계약 테스트 + JSON Schema 검증 강화 |
| `ui-designer` | `.claude/agents/ui-designer.md` | 544 | CVA/디자인 토큰 3계층/Storybook 8단계로 전면 강화 |
| `ux-researcher` | `.claude/agents/ux-researcher.md` | 521 | 인터뷰 가이드/정량조사/다중 역할분리 표(3개) 강화 |
| `product-manager` | `.claude/agents/product-manager.md` | 383 | RICE 프레임워크 + OKR + 7단계 + color:purple 강화 |

**YAML frontmatter 위치 수정 이력 (4개 파일):**
- `performance-benchmarker.md`, `ui-designer.md`, `ux-researcher.md`, `product-manager.md`: 출처 주석(`<!-- 원본 출처: ... -->`)이 YAML 블록 앞에 위치 → YAML 블록 뒤로 이동 수정 완료

**TIER 2 핵심 달성:**
- 6개 전원 기존 에이전트/스킬과 역할 분리 표 포함 (TIER 2 DoD)
- `ux-researcher`는 3중 역할 분리 표 (vs ui-designer / vs product-manager / vs accessibility-auditor)
- `security-engineer` → `security-auditor`와 진단/구현 역할 분리
- `performance-benchmarker` → `performance-auditor`와 분석/권고 vs 수치 측정/추적 분리
- `api-tester` → `test-coverage-agent`와 외부 블랙박스 E2E vs 내부 화이트박스 분리
- `ui-designer` → `frontend-design` 스킬과 지침 vs 실행 분리
- `product-manager` → `planner`와 비즈니스 요구사항 vs 기술 구현 분리

---

### 2026-03-10 — Phase 6-A: TIER 1 에이전트 7개 생성 + 검증 완료

**병렬 세션에서 7개 에이전트 파일 생성 완료 → 메인 세션 종합 검증 (7/7 PASS):**

| 에이전트 | 파일 | 특이사항 |
|---|---|---|
| `pipeline-orchestrator` | `.claude/agents/pipeline-orchestrator.md` | 5단계 DAG, 23개 에이전트 전체 목록 포함 |
| `reality-checker` | `.claude/agents/reality-checker.md` | Pessimistic Default 철학, A-E 서브카테고리 |
| `accessibility-auditor` | `.claude/agents/accessibility-auditor.md` | 10단계, WCAG 2.2 A/AA/AAA → Critical/High/Medium/Low |
| `evidence-collector` | `.claude/agents/evidence-collector.md` | 텍스트+시각 증거 분류, 스택별 명령어 |
| `devops-automator` | `.claude/agents/devops-automator.md` | 8단계, FastAPI ECR→ECS 워크플로우 |
| `rapid-prototyper` | `.claude/agents/rapid-prototyper.md` | **YAML frontmatter 수정** — `# Source:` 주석 YAML 블록 밖으로 이동 |
| `technical-writer` | `.claude/agents/technical-writer.md` | 7단계, README 7섹션+ADR+CONTRIBUTING |

**사전 수정 완료 이력 (병렬 세션 착수 전):**
- `docs/plan.md` 6개 경로 오류 수정 (agency-agents 명명 규칙: `testing/testing-*.md`, `engineering/engineering-*.md`)
- `docs/task.md` 6개 경로 오류 수정 + evidence-collector 역할 확장 노트 추가
- `.mcp.json` Windows npx 실행 경고 수정 (`cmd /c` wrapper 적용 — playwright, remotion-documentation)
- `agency-agents-main/` 분석 완료 → Phase 6-D 이후 삭제 예정

---

### 2026-03-09 — Phase 6 계획 수립: agency-agents 병합 분석 완료

**분석 대상:** `github.com/msitarzewski/agency-agents` (⭐15.3k, 61+개 에이전트)

**분석 결과:**

| 구분 | 내용 |
|---|---|
| 파일 포맷 | YAML frontmatter + Markdown — 하네스와 완전 동일 |
| 설치 경로 | `~/.claude/agents/` — 하네스와 동일 |
| 방향성 차이 | agency-agents=가상 비즈니스팀 시뮬레이션 / 하네스=코드 개발 기술 자동화 |
| 병합 결론 | **전체 병합 불가** → 선택적 에이전트 흡수 (TIER 1/2 총 13개) |

**TIER 1 — 즉시 추가 권장 (7개, Phase 6-A):**

| 에이전트 | 원본 | 하네스 기여 |
|---|---|---|
| `pipeline-orchestrator` | specialized/agents-orchestrator | 에이전트 파이프라인 조율 — **하네스에 없는 메타 레이어** |
| `reality-checker` | testing/reality-checker | 품질 게이트 — "기본값 NEEDS WORK" 철학 |
| `accessibility-auditor` | testing/accessibility-auditor | WCAG 접근성 점검 — 현재 완전 미보유 |
| `evidence-collector` | testing/evidence-collector | 테스트 증거 수집 자동화 |
| `devops-automator` | engineering/devops-automator | CI/CD 파이프라인 설계 |
| `rapid-prototyper` | engineering/rapid-prototyper | PoC/MVP 빠른 구현 |
| `technical-writer` | support/ | 기술 문서 자동 생성 |

**TIER 2 — 적응 후 추가 (6개, Phase 6-B):**
- `security-engineer` (auditor와 역할 분리), `performance-benchmarker` (auditor와 측정/분석 분리)
- `api-tester` (E2E 외부 관점), `ui-designer` (design 카테고리 신설)
- `ux-researcher` (신규), `product-manager` (비즈니스 요구사항)

**전체 병합 비권장 이유:**
1. 하네스 = Claude Code 네이티브 (hooks/skills/commands 시스템) — agency-agents는 멀티툴 범용
2. 마케팅/소셜미디어/공간컴퓨팅(visionOS, XR) 에이전트는 코드 개발 포커스와 불일치
3. NEXUS 7단계 오케스트레이션 전체는 하네스 4문서 철학과 충돌

**NEXUS 패턴 추출 (Phase 6-C):**
- Quality Gate → `/quality-gate` 커맨드 신규 생성
- Dev-QA Loop → 기존 커맨드 강화 또는 신규 커맨드
- Handoff Template → `history.md` 포맷 개선

**산출물:**
- `docs/plan.md` — Phase 6 전체 계획 신규 작성
- `docs/task.md` — Phase 6-A 상세 실행 계획 신규 작성
- `docs/checklist.md` — Phase 6-A~D 체크리스트 신규 작성

---

### 2026-03-01 — Phase 0: 하네스 기반 정립

1. 하네스 전체 구조 분석
2. `base_code_plan.md` 작성 — 전체 자산 운영 계획 확정
3. `docs/` 4문서 체계 구축 (plan/task/history/checklist)
4. `CLAUDE.md` — Agent 작업 문서 체계 섹션 추가
5. `docs/plan.md` — 멀티 스택 기준 전면 재작성

---

### 2026-03-01 — Phase 1-A: 선행 작업

**워크플로우 지침 완료:**
- `CLAUDE.md` — 병렬 세션 충돌 방지 규칙 추가
- `CLAUDE.md` — 베이스 코드 Definition of Done 명시
- `CLAUDE_INTEGRATION_GUIDE.md` — `base/[stack]/` 섹션 추가

**base/fastapi/ 생성 완료:**
- DDD 구조 (domain/user, domain/shared, api/, dtos/, middleware/, utils/)
- 읽기/쓰기 DB 분리 패턴, JWT 인증, S3 유틸리티
- docs/ 4문서, README.md, .env.example, Dockerfile, docker-compose.yaml

**base/nextjs/ 생성 완료:**
- AuthProvider (JWT + HttpOnly 쿠키 기반)
- 보호 라우트 미들웨어, shadcn/ui 11종 컴포넌트, login/page.tsx

---

### 2026-03-01~02 — Phase 1-B: 신규 베이스 코드 생성 (7개 스택)

| 스택 | 날짜 | 핵심 특징 |
|---|---|---|
| `base/express/` | 2026-03-01 | TypeScript + Prisma + JWT HttpOnly 쿠키 + Jest |
| `base/django/` | 2026-03-01 | DRF + SimpleJWT + AbstractBaseUser + pytest-django |
| `base/nestjs/` | 2026-03-02 | Passport JWT 이중 전략 + Repository 패턴 + E2E 11케이스 |
| `base/spring-boot/` | 2026-03-02 | Security JWT Filter + JPA + GlobalExceptionHandler + Mockito |
| `base/react-native/` | 2026-03-02 | Expo SDK 52 + Zustand v5 + SecureStore + 401 Queue 패턴 |
| `base/c-embedded/` | 2026-03-01 | HAL Ops Table + FreeRTOS + 정적 Ring Buffer + Unity 25테스트 |
| `base/cpp-embedded/` | 2026-03-02 | 순수 가상 HAL + RAII + 경량 Mock + Google Test 43테스트 |

---

### 2026-03-02 — Phase 1-C: 통합 검증 완료

- 9개 스택 전체 완성 기준 충족 확인
- 루트 `docs/` 4문서 일괄 업데이트
- `base_code_plan.md` — 9개 스택, Skills 14개 / Agents 11개 반영
- 역전파 프로세스 5단계 문서화

---

### 2026-03-02 — Phase 2: Task 2-0 + Task 2-A 완료

**skill-rules.json v1.1 정비:**
- 유령 항목 13개 제거, 미등록 스킬 9개 규칙 추가
- fastapi/nextjs 트리거 정밀화, changelog 섹션 추가

**CLAUDE.md 격리 규칙 추가:**
- Skills 병렬 세션 작업 규칙 (Phase 2-B/2-C)
- Agents 병렬 세션 작업 규칙 (Phase 3-A)
- 스킬 완성 기준 (Definition of Done) 추가
- 에이전트 완성 기준 (Definition of Done) 추가

---

### 2026-03-02~07 — Phase 2-B: 신규 스킬 생성 (10/10 완료)

| 스킬 | 줄수 | 등급 |
|---|---|---|
| `express-backend-guidelines` | 500 | A |
| `django-backend-guidelines` | 500 | A |
| `nestjs-backend-guidelines` | 486 | A |
| `spring-boot-guidelines` | 499 | A |
| `react-native-guidelines` | 500 | S |
| `embedded-c-guidelines` | 497 | S |
| `embedded-cpp-guidelines` | 439 | A |
| `docker-guidelines` | 456 | A |
| `aws-guidelines` | 459 | A |
| `database-guidelines` | 441 | A |

---

### 2026-03-03~07 — Phase 2-C: 기존 스킬 품질 점검 (14/14 완료)

주요 개선 사항 (P1 해결):
- `fastapi-backend-guidelines`: 667→436줄 (500줄 초과 해결)
- `pytest-backend-testing`: 531→403줄 (500줄 초과 해결)
- `ppt-brand-guidelines`: 안티패턴 섹션 6개 추가 (204→267줄)
- `vercel-react-best-practices`: 147→365줄 (내용 대폭 보강)
- 14개 전체 QUALITY_NOTES.md 작성 완료

---

### 2026-03-08 — Phase 2-D: 일괄 통합 완료

1. `skill-rules.json` v1.2 — 10개 신규 스킬 트리거 규칙 추가 (전체 24개 등록)
2. `.claude/skills/README.md` — 24개 스킬 카테고리별 목록 전면 재작성
3. `base_code_plan.md` — Skills 24개 대시보드, Phase 2 완료 처리
4. 루트 `docs/` 4문서 Phase 2 완료 처리

**Skills 라이브러리 최종 현황:**
- 전체 24개, 500줄 이하 전원 충족, skill-rules.json v1.2 전체 등록

---

### 2026-03-08 — Phase 3-A 상세 계획 수립

- docs/task.md: 5개 에이전트별 8단계 실행 계획 + 참조 스킬/스택 + 출력 형식 명세화
- docs/checklist.md: 공통 DoD 6항목 + 에이전트별 세부 체크리스트
- docs/history.md: Phase 3-A 실행 가이드 + 에이전트별 핵심 기술 사항 정리

---

### 2026-03-09 — Phase 3-A: 에이전트 5개 생성 완료

병렬 세션에서 5개 에이전트 파일 생성 완료:

| 에이전트 | 파일 | 줄수 |
|---|---|---|
| `db-migration-agent` | `.claude/agents/db-migration-agent.md` | 307 |
| `api-spec-generator` | `.claude/agents/api-spec-generator.md` | 640 |
| `test-coverage-agent` | `.claude/agents/test-coverage-agent.md` | 476 |
| `performance-auditor` | `.claude/agents/performance-auditor.md` | 408 |
| `security-auditor` | `.claude/agents/security-auditor.md` | 384 |

---

### 2026-03-09 — Phase 4-A 계획 수립

**배경:** 4문서 체계가 CLAUDE.md 수동 지침에만 의존 → 자동화 필요성 확인
**계획 내용:**
- `session-start-docs-loader` 훅 (UserPromptSubmit): 세션 첫 프롬프트 시 4문서 자동 컨텍스트 주입
- `docs-update-reminder` 훅 (Stop): 세션 종료 시 history.md 미갱신이면 리마인더
- `/harness-docs-update` 슬래시 커맨드: 이 프로젝트 전용 4문서 업데이트 지시
- 세션 캐시: `.claude/.session-cache/[session_id].loaded` 플래그 파일로 첫 번째 프롬프트 감지

**4-A 이후 Phase 4 계획:**
- 4-B: skill-rules.json 트리거 정확도 개선 (상시 운영)
- 4-C: CI/CD 연동 훅 확충 (PR 자동 리뷰)
- 4-D: taskops auto run (장기 과제)

---

### 2026-03-09 — Phase 4-A Fix: P0/P1 수정 완료 + 동작 검증 통과

**Step F-1 (P0 해결):** `.claude/settings.json` 수정

- `UserPromptSubmit` 훅 배열에 `session-start-docs-loader.sh` 추가 (skill-activation-prompt.sh 뒤)
- `Stop` 훅 배열 **마지막**에 `docs-update-reminder.sh` 추가 (tsc-check → trigger-build-resolver → docs-update-reminder 순서)

**Step F-2 (P1 해결):** `session-start-docs-loader.ts` L41 regex 수정

- `/현재 Phase[:\s]+([^\n]+)/` → `/현재 Phase\S*\s+([^\n]+)/`
- `**현재 Phase:**` 마크다운 형식(`**` 볼드 기호)을 정확히 매칭하도록 수정

**Step F-3 동작 검증 결과:**

| 테스트 케이스 | 기대 결과 | 실제 결과 |
|---|---|---|
| 첫 실행 — 4문서 읽기 지시 출력 | 출력 + .loaded 생성 | ✅ |
| Phase 요약 정확도 | `Phase 4-A Fix — 점검 후 P0/P1 이슈 수정 중` | ✅ (`파악 필요` 아님) |
| 두 번째 실행 (동일 session_id) | 출력 없음 | ✅ |
| docs-update-reminder (.loaded 있고 미수정) | 리마인더 출력 | ✅ |
| docs-update-reminder (.loaded 없는 session_id) | 출력 없음 | ✅ |

**테스트 캐시 정리:** `.session-cache/fix-test.loaded` 삭제 완료

---

### 2026-03-09 — Phase 4-A 점검: 이슈 2건 발견

**점검 범위:** Phase 4-A에서 생성된 파일 6종 전수 점검

**발견된 이슈:**

| 등급 | 파일 | 이슈 | 수정 방법 |
|---|---|---|---|
| **P0** | `.claude/settings.json` | 신규 훅 2개 미등록 (훅 파일 생성만 됨, 실행 등록 안 됨) | UserPromptSubmit에 session-start-docs-loader 추가, Stop에 docs-update-reminder 추가 |
| **P1** | `session-start-docs-loader.ts` L41 | Phase regex `/현재 Phase[:\s]+/` — `**현재 Phase:**` 마크다운 형식 매칭 실패 → 항상 `파악 필요` 출력 | `/현재 Phase\S*\s+([^\n]+)/` 로 변경 |
| **P2** | `session-start-docs-loader.ts` | `async function main()` — await 없음, 불필요한 async 선언 | 기능 영향 없음, 향후 개선 |

**정상 확인된 항목:**
- `docs-update-reminder.ts` — mtime 비교 로직, catch exit(0) 안전 처리 ✅
- `docs-update-reminder.sh` — SKIP_DOCS_REMINDER 체크, set -e 미사용 ✅
- `harness-docs-update.md` — YAML frontmatter, 5단계 지시 구조 ✅
- `.session-cache/.gitkeep` + `.gitignore` 패턴 ✅
- `CONFIG.md`, `README.md` 훅 소개 ✅

---

### 2026-03-09 — Phase 4-A: 4문서 자동화 훅 완료

Step 1~5 순서대로 실행 완료:

| 자산 | 파일 | 역할 |
|---|---|---|
| `session-start-docs-loader` 훅 | `.claude/hooks/session-start-docs-loader.{ts,sh}` | UserPromptSubmit — 세션 첫 프롬프트 시 4문서 읽기 지시 자동 주입 |
| `docs-update-reminder` 훅 | `.claude/hooks/docs-update-reminder.{ts,sh}` | Stop — 세션 종료 시 history.md 미갱신이면 리마인더 + `/harness-docs-update` 안내 |
| `/harness-docs-update` 커맨드 | `.claude/commands/harness-docs-update.md` | 이 프로젝트 전용 4문서 업데이트 5단계 지시 |

**세션 캐시 구조:**
- `.claude/.session-cache/[session_id].loaded` — 세션별 플래그 파일 (mtime = 세션 시작 시각)
- `.gitignore`에 `*.loaded` 패턴 추가 (`.gitkeep`만 추적)

**훅 등록 문서 갱신:**
- `CONFIG.md` — UserPromptSubmit + Stop 훅 settings.json 예시 추가
- `README.md` — session-start-docs-loader (Essential), docs-update-reminder (Optional) 소개 추가

**통합 문서 갱신:**
- `docs/plan.md` — Phase 4-A 상세 항목 추가 및 완료 처리, 대시보드 업데이트
- `base_code_plan.md` — Slash Commands (3개) → (4개), /harness-docs-update 행 추가

---

### 2026-03-09 — Phase 3-B: Agents 라이브러리 일괄 통합 완료

1. `.claude/agents/README.md` 전면 재작성 — Available Agents (10) → (16), 신규 6개 섹션 추가 (planner + 5개)
2. `base_code_plan.md` Agents 섹션 업데이트 — (11개) → (16개), 5개 신규 에이전트 카테고리별 행 추가
3. `docs/plan.md` Phase 3 완료 처리 — 3-A 5개 ✅, 3-B 3개 ✅, 대시보드 69% → 100%
4. 루트 `docs/` 4문서 Phase 3 완료 처리

**최종 Agents 라이브러리 현황:**
- 전체 16개, README 16개 전원 등록, DoD 충족

---

### 2026-03-09 — Task 3-A-QA: 에이전트 품질 검증 완료

**검증 방법:** DoD 1단계(6항목 전수 확인) + 2단계(base/ 코드 교차 검증)

| 에이전트 | 등급 | P1 | 교차 검증 |
|---|---|---|---|
| `db-migration-agent` | **S** | 0건 | ✅ (base/fastapi 모델 경로, Alembic 처리 로직 확인) |
| `api-spec-generator` | **S** | 0건 | ✅ (base/fastapi 라우터, base/nestjs 컨트롤러 매칭) |
| `test-coverage-agent` | **S** | 0건 | ✅ (base/fastapi/tests/, base/nestjs/test/ 존재 확인) |
| `performance-auditor` | **S** | 0건 | ✅ (CWV 기준값 vercel-skill 일치, N+1 패턴 정확) |
| `security-auditor` | **S** | 0건 | ✅ (OWASP A01~A10 전스택 대응 확인) |

Phase 3-B 착수 조건 충족 ✅

---

## 중요 결정 사항

| 결정 | 내용 | 날짜 |
|---|---|---|
| 4문서 체계 채택 | 모든 Claude Agents 작업은 docs/ 4문서 기준으로 관리 | 2026-03-01 |
| base/[stack]/docs/ 분리 | 스택별 작업은 해당 스택 디렉토리 안에 독립 docs/ 운영 | 2026-03-01 |
| 병렬 세션 전략 | Phase 1-B는 스택별, Phase 2-B/2-C는 스킬별, Phase 3-A는 에이전트별 독립 세션 | 2026-03-01 |
| 멀티 스택 라이브러리화 | 하네스를 9개 스택 자산 라이브러리로 확장 | 2026-03-01 |
| 원본 보존 | backend/, frontend/는 삭제 않고 base/에 클린 버전 별도 생성 | 2026-03-01 |
| C/C++ 용도 | 임베디드/펌웨어 — HAL 추상화, FreeRTOS/RTOS 패턴 중심 | 2026-03-01 |
| firebase 제거 | base/nextjs/는 이메일/JWT 인증만 포함 (범용성) | 2026-03-01 |
| 품질 검증 방법론 | DoD 전수 확인 + base/ 코드 교차 검증, P-level 심각도 분류 | 2026-03-03 |
| nextjs 풀스택 스킬 분류 | nextjs-frontend-guidelines는 Server+Client 양면 포함하는 풀스택 스킬 | 2026-03-05 |
| 4문서 신뢰성 원칙 | 문서와 실제 파일 불일치 시 코드 검증(Glob/Grep)을 우선으로 함 | 2026-03-08 |
| task/checklist 단순화 | 완료된 Phase는 history에만 보존, task/checklist는 현재 작업만 기록 | 2026-03-09 |

---

### 2026-03-09 — Phase 4-B: skill-rules.json v1.3 오탐 수정 (5건)

**수정 파일:** `.claude/skills/skill-rules.json` (v1.2 → v1.3)

| # | 스킬 | 오탐 키워드 | 수정 내용 |
|---|---|---|---|
| P1 | `fastapi-backend-guidelines` | `async`, `await` | 키워드 2개 제거 + `(async|await).*?pattern` intentPattern 제거 (JS/TS async/await 오탐 방지) |
| P2 | `database-guidelines` | `migration` (단독) | `database migration` + `schema migration` 구체화, intentPattern → 설계 관점 `(design|review|plan).*?migration` 으로 한정 (db-migration-agent 중복 방지) |
| P3 | `skill-developer` | `hook system` | `claude hook system` 으로 교체 (React hooks/git hooks 오탐 방지) |
| P4 | `pytest-backend-testing` | `test`, `tests`, `testing`, `mock`, `mocking`, `fixture` (일반 키워드 6개) | 제거 후 `python test`, `pytest mock`, `pytest fixture`, `MagicMock`, `AsyncMock` 으로 교체 (Jest/Vitest 오탐 방지) |
| P5 | `error-tracking` | `monitoring` | `error monitoring` 으로 구체화 (CloudWatch/Prometheus 오탐 방지) |

**task.md 명시 3건 + 추가 발견 2건 (pytest, error-tracking) 포함 일괄 수정.**

---

### 2026-03-09 — Phase 4-C: CI/CD 연동 훅 확충 완료

**C-1: pr-review-trigger 훅 생성**
- `.claude/hooks/pr-review-trigger.ts` — Bash PostToolUse 훅, `gh pr create` 감지 → exit 2 컨텍스트 주입
- `.claude/hooks/pr-review-trigger.sh` — bash wrapper, `SKIP_PR_REVIEW` 환경변수 체크
- exit 2 = PostToolUse block mode → stdout이 Claude 컨텍스트에 자동 주입

**C-2: GitHub Actions 워크플로우**
- `.github/workflows/pr-code-review.yaml` 신규 생성 (디렉토리 없어서 새로 만듦)
- PR opened/synchronize 트리거 → 체크리스트 댓글 자동 작성
- `tj-actions/changed-files@v44` + `actions/github-script@v7` 사용
- TS/PY 파일 수 자동 집계 + Architecture/Quality/Security 체크리스트

**C-3: code-architecture-reviewer.md 범용화**
- Documentation References: `PROJECT_KNOWLEDGE.md`, `BEST_PRACTICES.md` → `CLAUDE.md`, `docs/history.md`, `docs/task.md`, `base/[stack]/`
- 기술 스택: React/MUI/TanStack/WorkflowEngine 하드코딩 제거 → CLAUDE.md 기반 범용 표현
- 리뷰 저장 경로: `./dev/active/[task-name]/` → `docs/reviews/[YYYY-MM-DD]-[branch]-review.md`

**C-4: settings.json PostToolUse Bash 블록 추가**
- 기존 "Edit|MultiEdit|Write" 블록 뒤에 "Bash" 매처 블록 추가
- command: `$CLAUDE_PROJECT_DIR/.claude/hooks/pr-review-trigger.sh`

**C-5: 훅 문서 업데이트**
- `CONFIG.md`: Quick Start settings.json에 Bash 블록 추가 + pr-review-trigger 상세 섹션 추가
- `README.md`: Optional Hooks에 pr-review-trigger 소개 추가

---

### 2026-03-09 — Phase 5-C 완료: 사용자 가이드 보완

**수정 파일:**

| 파일 | 작업 |
|---|---|
| `HARNESS_USAGE_GUIDE.md` | Quick Start 경로 A → `harness-init.sh` 기반으로 교체, Part 1 Step 1~2 통합 (수동 cp는 대안 섹션), 6부 "하네스 운영 도구" 추가, 부록 커맨드 수 6→9 수정 |
| `HARNESS_COMMANDS_REFERENCE.md` | **신규 생성** — 9개 커맨드 + 스크립트 + 6종 훅 + 운영 워크플로우 전체 레퍼런스 |
| `base_code_plan.md` | 사용자 가이드 문서 섹션에 `HARNESS_COMMANDS_REFERENCE.md` 행 추가 |
| `docs/history.md` | 현재 세션 완료 이력 기록 |

**HARNESS_COMMANDS_REFERENCE.md 구조 (7섹션):**
1. Quick Reference (한눈에 보기 표)
2. 초기화 도구 (harness-init.sh + /harness-init)
3. 적용 도구 (/harness-apply + /harness-docs-update)
4. 계획 관리 도구 (/harness-plan-sync + /dev-docs + /dev-docs-update)
5. 버전 관리 도구 (/harness-changelog)
6. 역전파 도구 (/harness-backport)
7. 훅 시스템 (6종 훅 + 등록 확인) + 일반 운영 워크플로우

---

### 2026-03-09 — Phase 5-B 완료: 멀티 프로젝트 운영 도구 전체 구현

**구현 완료 산출물:**

| 파일 | 역할 |
|---|---|
| `scripts/harness-init.sh` | 터미널 단독 실행형 대화형 초기화 스크립트 (스택 11가지, `--help` 지원) |
| `.claude/commands/harness-init.md` | Claude 세션 내 /harness-init 커맨드 (Step 1~7 흐름) |
| `CHANGELOG.md` | Keep a Changelog 형식, v1.0.0~v1.5.0 초기 이력 포함 |
| `.claude/commands/harness-changelog.md` | 변경 자산 분석 → 버전 결정 → CHANGELOG 항목 자동 생성 |
| `.claude/commands/harness-backport.md` | 역전파 O/X 기준 + 4단계 실행 흐름 (base/[stack]/ + Skill + CHANGELOG + base_code_plan) |
| `base_code_plan.md` | `현재 버전: v1.5.0` 추가 + Commands 9개 업데이트 |

**핵심 설계 결정:**

| 항목 | 결정 | 이유 |
|---|---|---|
| 초기화 방식 | 쉘 스크립트 + 슬래시 커맨드 병행 | 스크립트: Claude 없이도 실행 / 커맨드: 대화형 안내 |
| 스택 메뉴 | 번호 선택 (0~10, 11가지) | 풀스택 5 + 단독 5 + 스킬만 1 |
| CHANGELOG 형식 | Keep a Changelog 표준 | 검증된 표준 형식, 도구 연동 용이 |
| 역전파 기준 | 재사용 패턴 O / 비즈니스 로직 X (명확한 표 제공) | 실 프로젝트 비즈니스 로직 유입 방지 |

### 2026-03-09 — Phase 5-B 계획 수립

**배경:** Phase 5-A 완료 후 Phase 5-B "멀티 프로젝트 운영 도구" 상세 계획 수립. plan.md의 3개 항목을 구체적 산출물로 분해.

**Phase 5-B 핵심 설계 결정:**

| 항목 | 결정 |
|---|---|
| 초기화 방식 | `harness-init.sh` (단독 실행) + `/harness-init` (Claude 세션) 병행 |
| 스택 선택 UI | 번호 선택 메뉴 — 풀스택 5개 + 단독 5개 + 스킬만 1개 (총 11가지) |
| 버전 형식 | `v[major].[minor].[patch]` — minor=자산 추가, patch=수정 |
| CHANGELOG 형식 | Keep a Changelog 표준 형식 (Unreleased/Added/Changed/Fixed) |
| 역전파 기준 | 재사용 가능 패턴 O / 비즈니스 로직 X (명확한 기준 커맨드에 명시) |

**산출물 계획:**

| 산출물 | 파일 | 역할 |
|---|---|---|
| 초기화 스크립트 | `scripts/harness-init.sh` | 터미널 단독 실행형 대화형 초기화 |
| 초기화 커맨드 | `.claude/commands/harness-init.md` | Claude 세션 내 대화형 초기화 |
| 버전 이력 | `CHANGELOG.md` | 하네스 전체 자산 버전 관리 |
| changelog 커맨드 | `.claude/commands/harness-changelog.md` | CHANGELOG 항목 자동 생성 |
| 역전파 커맨드 | `.claude/commands/harness-backport.md` | 실 프로젝트 패턴 → 하네스 역전파 |

---

### 2026-03-09 — Phase 5-A 계획 수립

**배경:** 사용자가 기존 진행 중인 프로젝트에도 하네스를 동일하게 적용하고 싶다는 요구사항 제기. 현재 `CLAUDE_INTEGRATION_GUIDE.md`는 Claude 내부 동작 안내 위주이고, 사용자가 직접 읽고 따를 수 있는 통합 가이드가 부재.

**Phase 5-A 핵심 설계 결정:**

| 항목 | 결정 |
|---|---|
| 신규/기존 구분 기준 | `.claude/` 디렉토리 존재 여부 |
| 기존 프로젝트 적용 방식 | 선택적 병합 — 기존 설정 보존, 하네스 자산 추가 |
| settings.json 충돌 해결 | 기존 훅 배열 유지 + 새 항목 추가 (덮어쓰기 금지) |
| skill-rules.json 충돌 해결 | 기존 스킬 유지 + 새 스킬 규칙 추가 + pathPattern 재설정 |
| pathPattern 설정 | 프로젝트 구조 탐지 후 사용자가 직접 설정하도록 가이드 |
| 베이스 코드 활용 | 기존 프로젝트는 참조용만, 신규 프로젝트만 복사 |
| 사용자 가이드 형식 | Quick Start 2경로 → 단계별 상세 → 검증 → 트러블슈팅 |

**산출물 계획:**
- `HARNESS_USAGE_GUIDE.md` — 사용자용 통합 가이드 (신규/기존 2경로)
- `.claude/commands/harness-apply.md` — 자동 적용 커맨드
- `CLAUDE_INTEGRATION_GUIDE.md` 업데이트 — 기존 프로젝트 병합 패턴 섹션 추가
- `base_code_plan.md` — Commands 5→6개

---

### 2026-03-09 — Phase 4-D: taskops auto run 구현 완료

**D-1: `/harness-plan-sync` 커맨드 생성**
- `.claude/commands/harness-plan-sync.md` 신규 생성
- YAML frontmatter: `description` + `argument-hint` (특정 Phase 고정 지원)
- 7단계 실행 흐름: plan.md 읽기 → 미완료 Phase 탐색 → [ ] 항목 추출 → 상세 분석 → task.md 덮어쓰기 → checklist.md 업데이트 → 완료 안내
- 미완료 Phase 탐색 기준: `✅` 없고 `[ ]` 항목 1개 이상인 Phase 중 첫 번째
- argument 지정 시 특정 Phase 고정 지원

**D-2: `/harness-docs-update` 확장**
- `.claude/commands/harness-docs-update.md`에 Step 0 추가
- Step 0: task.md 완료 항목 → plan.md `[ ]` → `[x]` 자동 동기화
- Phase 전체 완료 시 Phase 제목에 `✅ 완료` 자동 추가
- 기존 Step 1~5 순서 유지

**D-3: `base_code_plan.md` 업데이트**
- 디렉토리 트리: `commands/ ← 슬래시 커맨드 라이브러리 (4개)` → `(5개)`
- Slash Commands 섹션: `4개` → `5개`, `/harness-plan-sync` 행 추가
- Last updated 날짜 갱신

**D-4: 4문서 업데이트**
- `docs/plan.md` — Phase 4-D ✅ 완료 처리 + 대시보드 Commands 5개 반영
- `docs/task.md` — 완료 상태 표 업데이트
- `docs/checklist.md` — D-1~D-4 전체 [x] 체크 완료
- `docs/history.md` — Phase 4-D 완료 이력 기록

---

## 작업 이력

### 2026-03-11 — Phase 8-C: P1 에이전트 DoD 위반 수정 3건 완료

**수정 항목 (C-1 ~ C-3):**

| ID | 대상 | 수정 내용 | 검증 |
|---|---|---|---|
| **C-1** | 에이전트 10개 | YAML frontmatter `tools:` 필드 추가 (각 에이전트별 권장 값) | `grep "^tools:"` 10개 전원 PASS ✅ |
| **C-2** | `planner.md` | `dev/active/` → `docs/` 전면 교체 5곳 (description/Step1/Step4/Step5/Quality Checklist) | `grep "dev/active"` → 0건 ✅ |
| **C-3** | `auth-route-debugger.md` | 크리덴셜 하드코딩 4종 제거 + PM2 서비스명 범용화 + 주의사항 블록 추가 | grep 잔재 0건 ✅ |

**C-1 tools: 값 매핑:**
- `auth-route-debugger`: `Read, Bash, Glob, Grep`
- `auth-route-tester`: `Read, Bash, Glob, Grep`
- `code-architecture-reviewer`: `Read, Glob, Grep, Write, Bash`
- `code-refactor-master`: `Read, Write, Edit, MultiEdit, Bash`
- `documentation-architect`: `Read, Write, Edit, Glob, Grep`
- `frontend-error-fixer`: `Read, Write, Edit, Bash, Glob`
- `plan-reviewer`: `Read, Glob, Grep, WebSearch, WebFetch`
- `planner`: `Read, Write, Glob, Grep, Bash`
- `refactor-planner`: `Read, Glob, Grep`
- `web-research-specialist`: `WebSearch, WebFetch, Read, Write`

---

### 2026-03-11 — Phase 8-C 계획 수립 완료

**계획 수립 내용:**
- `docs/task.md` — Phase 8-C 상세 실행 계획 전면 재작성
  - C-1: 10개 에이전트별 tools: 값 매핑 테이블 + 삽입 위치 규칙
  - C-2: planner.md 수정 5곳 Before/After 코드 스니펫 포함
  - C-3: auth-route-debugger.md 수정 4종류 라인별 Before/After 상세
  - D-4(error-handling-reminder async 제거) Phase 8-B에서 선행 완료 반영
- `docs/checklist.md` — Phase 8-C 체크리스트 전면 재작성 (tools: 값 테이블 포함)
- `docs/history.md` — Phase 8-C 계획 수립 완료 기록

---

### 2026-03-11 — Phase 8-D: P2 MEDIUM 코드 품질 개선 6건 완료

**수정 항목 (D-1 ~ D-7):**

| ID | 파일 | 수정 내용 | 검증 |
|---|---|---|---|
| **D-1** | `docs-update-reminder.ts` | `async function main()` → `function main()` + `main().catch(...)` → `main()` | grep 0건 ✅ |
| **D-2** | `pr-review-trigger.ts` | 동일 async 제거 패턴 | grep 0건 ✅ |
| **D-3** | `skill-activation-prompt.ts` | async 제거 + `catch (err)` → `catch (_err)` + `exit(1)` → `exit(0)` 2곳 + `main().catch(...)` → `main()` | grep 0건 ✅ |
| **D-5** | `.claude/hooks/pnpm-lock.yaml` | 파일 삭제 (package-lock.json 유지) | 파일 미존재 확인 ✅ |
| **D-6** | `.claude/settings.json` | `enableAllProjectMcpServers: true` → `false` | grep `false` 확인 ✅ |
| **D-7** | `.github/workflows/pr-code-review.yaml` | `createComment` 단독 → `listComments` → botComment 탐색 → `updateComment` or `createComment` 분기 | 3패턴 grep 확인 ✅ |

**핵심 변경 사항:**
- UserPromptSubmit 훅 (`skill-activation-prompt.ts`): `exit(1)` 완전 제거 → 프롬프트 차단 위험 해소
- MCP 설정 일관성: `enableAllProjectMcpServers: false` + `enabledMcpjsonServers` 명시 목록 유효
- GH Actions: PR synchronize 시 댓글 중복 누적 방지 (update/create 분기)

---

### 2026-03-11 — Phase 8-E: 통합 검증 + CHANGELOG v1.6.1 릴리즈 완료

**검증 결과 요약:**

| ID | 항목 | 결과 | 비고 |
|---|---|---|---|
| **E-1** | 훅 파일 8개 존재 + bash -n + 안전 패턴 | ✅ 전부 PASS | set-e 없음, exit(1) 없음, async 없음 |
| **E-2** | 수동 실행 7케이스 | ✅ 전부 PASS | + 추가 수정: tsc-check.sh jq→node |
| **E-3** | 에이전트 DoD 재검증 (29개) | ✅ tools: 10개 PASS | planner/auth-route-debugger 확인 |
| **E-4** | CHANGELOG v1.6.1 릴리즈 | ✅ Fixed 13건 + Changed 8건 | |
| **E-5** | 5문서 완료 처리 | ✅ 전부 완료 | base_code_plan.md v1.6.1 |

**Phase 8-E 추가 발견 수정:**
- `tsc-check.sh` — `jq` 미설치(Windows/MINGW64) 환경에서 JSON 파싱 전혀 안 됨 → `node` 기반으로 교체
- CHANGELOG v1.6.1 Fixed 항목에 포함 (13번째)

**Phase 8 전체 완료 — v1.6.1 릴리즈:**
- Phase 8-A (P0 4건): base_code_plan.md 버전 + tsc-check.sh 이동 + trigger-build-resolver 재작성 + session-start-docs-loader exit(0)
- Phase 8-B (P1 7건): set -e 제거 3파일 + error-handling-reminder 버그 4개 + TTL 정리 + mjs 4개 삭제 + stop-build-check 삭제
- Phase 8-C (P1 3건): 에이전트 10개 tools: 추가 + planner.md dev/active→docs/ + auth-route-debugger 범용화
- Phase 8-D (P2 6건): async 제거 3파일 + pnpm-lock.yaml 삭제 + enableAllProjectMcpServers:false + GH Actions 중복 댓글 방지
- Phase 8-E (통합 검증): tsc-check.sh jq→node 추가 수정 포함

## 다음 세션에서 이어갈 작업

### Phase 7 — 상시 운영 (현재)

| 항목 | 내용 |
|---|---|
| **7-A** | skill-rules.json 오탐 수집 → 키워드 구체화 (상시) |
| **7-B** | 실 프로젝트 검증 패턴 → `/harness-backport` 역전파 (상시) |

> Phase 9 전체 완료(2026-03-11). 4문서 체계 고도화 완료. 다음 작업은 실제 사용 중 발견되는 오탐/미탐 케이스를 수정하는 상시 운영.

*Last updated: 2026-03-11 (Phase 9 전체 완료 — CLAUDE.md 4문서 재정의 + 훅 파이프라인 3단계 완성 + harness-plan-sync Step 0 사전 검증 추가)*
