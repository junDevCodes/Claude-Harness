# Checklist — 작업 확인서

> **현재 Phase:** Phase 9-A — CLAUDE.md 4문서 정의 재정의 + 생명주기 규칙 추가
> **착수 배경:** Phase 8 전체 완료 (2026-03-11) → Phase 9 착수
> **선행 완료:** Phase 8-E (2026-03-11) — 통합 검증 + CHANGELOG v1.6.1 릴리즈

---

## Phase 9-A — CLAUDE.md 수정 체크리스트

### 9-A-1: 4문서 역할 테이블 수정

- [x] `task.md` 역할 정의 문구 변경 확인 (`바로 다음 진행할 기능 단위` 포함)
- [x] `task.md` 역할에 `복수 기능 동시 기재 금지` 문구 포함 확인
- [x] `history.md` 역할 정의 문구 변경 확인 (`영구 맥락 문서` 포함)
- [x] `history.md` 역할에 `plan/task 완료 시 요약 흡수` 문구 포함 확인

### 9-A-2: 4문서 생명주기 섹션 신규 추가

- [x] `### 4문서 생명주기` 섹션 존재 확인
- [x] plan.md / task.md / checklist.md / history.md 4행 테이블 존재 확인
- [x] `⚠️ task.md 또는 plan.md 교체 전 반드시 history.md에 완료 내용을 기록해야 한다.` 경고 문구 확인

### 9-A-3: task.md 범위 규칙 추가

- [x] `### task.md 범위 기준` 섹션 존재 확인
- [x] 15개 초과 → 기능 범위 과다 판단 규칙 확인
- [x] 복수 기능 금지 + plan.md 분리 우선 규칙 확인

### 9-A-4: 작업 완료 시 절차 업데이트

- [x] 완료 절차에서 `history.md` 업데이트가 `task.md` 완료 항목 표시 **이전**에 위치 확인
- [x] `⚠️ checklist 전부 완료 시: task.md는 다음 기능으로 교체` 경고 문구 확인

### 9-A 전체 완료 기준

- [x] `grep "바로 다음 진행할 기능 단위" CLAUDE.md` → 1건 ✅
- [x] `grep "영구 맥락 문서" CLAUDE.md` → 1건 ✅
- [x] `grep "4문서 생명주기" CLAUDE.md` → 1건 ✅
- [x] `grep "15개를 초과" CLAUDE.md` → 1건 ✅
- [x] 완료 절차 순서: checklist → history → task → plan 순서 확인 ✅

---

## Phase 9-B — 훅 파이프라인 완성 체크리스트

### 9-B-1: session-start-docs-loader.ts 확장

- [x] `next-task-pending.flag` 감지 + 완료 안내 출력 로직 추가
- [x] task.md 구조 검증 경고 배열 (`warnings`) 추가
  - [x] plan.md 참조 없음 경고
  - [x] 필수 섹션 없음 경고
  - [x] checklist 미완료 15개 초과 경고
- [x] warnings가 있을 때 배너에 포함 출력 확인 (수동 실행 검증 ✅)
- [x] 빈 stdin 주입 → exit 0 확인 ✅

### 9-B-2: docs-update-reminder.ts 확장

- [x] checklist.md `- [ ]` 개수 카운트 로직 추가
- [x] `- [x]` 개수 카운트 로직 추가
- [x] unchecked === 0 && checked > 0 → 완료 플래그 생성 로직 추가
- [x] `next-task-pending.flag` 파일 생성 확인
- [x] 완료 안내 메시지 출력 확인
- [x] 빈 stdin 주입 → exit 0 확인

---

## Phase 9-C — harness-plan-sync 강화 체크리스트

### 9-C-1: Step 0 사전 검증 추가

- [x] Step 0 섹션이 파일 최상단에 위치 (Step 1 이전)
- [x] history.md 미갱신 시 중단 안내 문구 포함
- [x] plan.md 전체 완료 시 교체 안내 문구 포함

---

*Phase 9 착수 체크리스트 추가 — 2026-03-11*

---

> **이전 Phase (8-C) 체크리스트:** Phase 8-C — P1 에이전트 DoD 위반 수정
>
> **⚠️ 표기 규칙:** `⚠️ 중복 확인` — 기존 계획 + QA 리뷰 양측에서 독립 확인된 이슈. **우선순위 한 단계 상향**.

---

## Phase 8 전체 완료 현황

| Phase | 내용 | 상태 |
|---|---|---|
| **8-A** | P0 CRITICAL 즉시 수정 (5건) | ✅ 완료 |
| **8-B** | P1 HIGH 신뢰성 수정 (7건 — 신규 발견 3건 포함) | ✅ 완료 |
| **8-C** | P1 에이전트 DoD 위반 수정 (3건) | ✅ 완료 |
| **8-D** | P2 MEDIUM 코드 품질 개선 (7건) | ✅ 완료 |
| **8-E** | 통합 검증 + CHANGELOG v1.6.1 릴리즈 | ✅ 완료 |

---

## Phase 8-A — P0 CRITICAL 수정 체크리스트

### 공통 DoD (훅 파일 수정)

- [x] 수정 전 원본 코드 동작 이해 확인
- [x] 수정 후 파일 문법 오류 없음 (TypeScript: `npx tsc --noEmit`, sh: `bash -n`)
- [x] exit code 의도 확인 (UserPromptSubmit: 0=계속/1=에러, PostToolUse: 0=계속/2=블록, Stop: 0=계속)
- [x] 수정 후 수동 실행 테스트 (stdin으로 JSON 주입)

---

### A-0: `base_code_plan.md` 버전 표기 수정 ✅

**검증 파일:** `base_code_plan.md`

- [x] `현재 버전: v1.5.0` → `현재 버전: v1.6.0` 수정 확인
- [x] `CHANGELOG.md` 최신 릴리즈 버전과 일치 확인 (v1.6.0)

---

### A-1: `tsc-check.sh` 근본 결함 수정 ✅ ⚠️ 중복 확인 P0++

**검증 파일:** `.claude/hooks/tsc-check.sh`, `.claude/settings.json`

> **근본 문제:** Stop 훅 페이로드에는 `tool_name` 필드 없음 → TSC 체크 전체 로직이 절대 실행 안 됨
> **⚠️ 중복 확인:** 기존 계획(B-3 하드코딩)과 QA 리뷰(P0-1 근본 결함) 양측 독립 확인

- [x] `settings.json` — tsc-check.sh를 Stop 훅 배열에서 **제거**
- [x] `settings.json` — PostToolUse/Edit|MultiEdit|Write 훅 배열에 tsc-check.sh **추가**
- [x] `tsc-check.sh` — `tool_name` 기반 조건 분기로 재작성 (PostToolUse 페이로드 구조 활용)
- [x] `tsc-check.sh` — `tool_input.path` `.ts` 확장자 확인 조건 추가
- [x] `tsc-check.sh` — `get_repo_for_file()` case 문 (`email|exports|form|...`) 제거
- [x] `tsc-check.sh` — 대체: 파일 경로 첫 번째 디렉토리에서 `tsconfig.json` 탐지 로직 구현
- [x] `tsc-check.sh` — `eval "$tsc_cmd"` → `npx tsc --noEmit -p "$tsconfig_path"` 직접 호출로 교체
- [x] `CLAUDE_PROJECT_DIR` fallback — `$HOME/project` 제거, `$(pwd)` 로 교체
- [x] 수동 테스트: `bash -n` 문법 검증 PASS
- [x] 수동 테스트: `tsconfig.json` 없는 디렉토리 → 스킵 확인 (get_repo_for_file 반환 "")

---

### A-2: `trigger-build-resolver.sh` 전면 재작성 ✅ ⚠️ 중복 확인 P0++

**검증 파일:** `.claude/hooks/trigger-build-resolver.sh`

> **⚠️ 중복 확인:** 기존 계획(A-2 디버그 코드)과 QA 리뷰(P0-2 claude --agent 잘못된 CLI) 양측 독립 확인

- [x] 디버그 코드 블록 (L1-9) 완전 제거 확인 (`/tmp/claude-hook-debug.log` 미생성 — 신규 실행으로 검증)
- [x] `services_dirs=("email" "exports" ...)` 하드코딩 배열 완전 제거 확인
- [x] `claude --agent build-error-resolver` 호출 제거 확인
- [x] `claude chat` 호출 제거 확인
- [x] 대체 로직: `git status --porcelain | grep '\.ts\|\.js'` 기반 범용 변경 감지 구현
- [x] 변경 TS/JS 파일 요약 → stderr 출력 후 `exit 0` 확인
- [x] 빈 stdin 주입 테스트: `echo '{}' | bash trigger-build-resolver.sh` — 오류 없이 종료 PASS
- [x] 로그 파일 미변화 테스트: 새 스크립트 실행 후 /tmp/claude-hook-debug.log 크기 동일 PASS

---

### A-3: `session-start-docs-loader.ts` exit 코드 수정 ✅

**검증 파일:** `.claude/hooks/session-start-docs-loader.ts`

- [x] `process.exit(1)` → `process.exit(0)` 수정 확인 (catch 블록)
- [x] catch 블록 에러 메시지도 `catch (_err)` 묵음 패턴으로 통일 (docs-update-reminder.ts 패턴 통일)
- [x] 잘못된 JSON 주입 테스트: `echo 'bad json' | npx tsx session-start-docs-loader.ts` → exit 0 PASS
- [ ] 정상 JSON 주입 테스트: `.loaded` 파일 생성 + 출력 확인 (세션 훅으로 자동 실행됨)

---

### A-4: `error-handling-reminder.ts` 파싱 수정 ⬜ (P1 강등 — 미등록 파일)

**검증 파일:** `.claude/hooks/error-handling-reminder.ts`

> **P1 강등 이유:** 파일이 `settings.json`에 미등록 → 현재 완전 무영향. Phase 8-B 이후 착수 가능.
> 단, Phase 8-B의 Dead Hook 정리(B-3)에서 이 파일 등록 결정 시 즉시 수정 필요.

- [ ] L103 `line.split('\t')` → `line.split(':')` 수정 확인
- [ ] 필드 순서 정정: `[timestamp, filePath, repo]` (post-tool-use-tracker.mjs 포맷 일치)
- [ ] `file.path` → `file.filePath` 참조 전체 통일
- [ ] `editedFiles` 배열에 올바른 데이터 채워지는지 검증
- [ ] 카테고리 분류 (`getFileCategory`) 정상 동작 확인
- [ ] `settings.json` 등록 여부 결정 후 수동 테스트

---

### A 전체 완료 기준 ✅

- [x] A-0: base_code_plan.md 버전 수정 완료 (v1.5.0 → v1.6.0)
- [x] A-1: tsc-check.sh PostToolUse 이동 + 재작성 + settings.json 업데이트 완료
- [x] A-2: trigger-build-resolver.sh 전면 재작성 완료
- [x] A-3: session-start-docs-loader.ts exit(0) 수정 완료
- [x] `/tmp/claude-hook-debug.log` — 신규 실행 시 미생성 확인 PASS
- [x] 각 수정 파일 문법 검증 통과 (`bash -n` PASS)
- [x] 수동 실행 테스트 각 파일 통과

---

## Phase 8-B — P1 HIGH 신뢰성 수정 체크리스트

> **착수 조건:** Phase 8-A 전체 완료 확인 ✅ (2026-03-10 완료)
> **현재 Phase: 🔴 진행 중**
>
> ⚠️ **신규 발견 이슈 (B 착수 시 코드 분석으로 추가 확인):**
> - `post-tool-use-tracker.sh`에도 `set -e` 존재 → B-2-추가로 편입
> - `error-handling-reminder.ts` 경로 버그: `$HOME/.claude/tsc-cache/` ≠ `$CLAUDE_PROJECT_DIR/.claude/tsc-cache/`
> - `.session-cache/` 현재 **28개** `.loaded` 파일 (예상 27개보다 1개 더)
> - `.mjs` 파일 4개 (dead code 의심) → B-5 신규 추가

---

### B-1: `session-start-docs-loader.sh` `set -e` 제거 ✅ ⚠️ P1++

> **위험 경로:** `set -e` + `npx tsx` 실패 → exit non-zero → UserPromptSubmit 훅 차단 → Claude 대화 불가

**검증 파일:** `.claude/hooks/session-start-docs-loader.sh`

- [x] `set -e` 라인 제거 확인
- [x] `cat | npx tsx session-start-docs-loader.ts` 끝에 `|| exit 0` 추가 확인
- [x] 주석 추가: `# set -e 제거: 실패 시 프롬프트 차단 방지`
- [x] `bash -n` 문법 검증 PASS
- [x] `grep "^set -e" session-start-docs-loader.sh` → 0건 확인

---

### B-2: `skill-activation-prompt.sh` `set -e` 제거 ✅ ⚠️ P1++

**검증 파일:** `.claude/hooks/skill-activation-prompt.sh`

- [x] `set -e` 라인 제거 확인
- [x] `cat | npx tsx skill-activation-prompt.ts` 끝에 `|| exit 0` 추가 확인
- [x] `bash -n` 문법 검증 PASS
- [x] `grep "^set -e" skill-activation-prompt.sh` → 0건 확인

---

### B-2-추가: `post-tool-use-tracker.sh` `set -e` 제거 ✅ (신규 발견)

> **신규 발견:** PostToolUse 훅에서 `set -e` → 보조 명령(grep 등) 실패 시 비정상 종료

**검증 파일:** `.claude/hooks/post-tool-use-tracker.sh`

- [x] `set -e` 라인(L2) 제거 확인
- [x] 나머지 로직 변경 없음 확인
- [x] `bash -n` 문법 검증 PASS
- [x] `grep "^set -e" post-tool-use-tracker.sh` → 0건 확인

---

### B-3-삭제: `stop-build-check-enhanced.sh` 삭제 ✅

> **삭제 근거:** OLD tsc-cache 의존 + eval 보안 취약 + set -e + tsc-check.sh에 완전 흡수 (4중 결함)
> settings.json 미등록 확인 완료 → 추가 수정 불필요

- [x] settings.json에서 `stop-build-check-enhanced.sh` 미참조 확인
- [x] `rm .claude/hooks/stop-build-check-enhanced.sh` 실행
- [x] 파일 미존재 확인

---

### B-3-수정: `error-handling-reminder.ts` 4개 버그 수정 + Stop 훅 등록 ✅

> **결정: 수정 + 등록** — 에러 핸들링 리마인더 기능은 유용. 버그 4개 수정 후 Stop 훅 등록.

**검증 파일:** `.claude/hooks/error-handling-reminder.ts`, `.claude/settings.json`

**버그 1 — 경로 오류 수정 (신규 발견, 치명적):**
- [x] `join(process.env.HOME || '/root', ...)` → `join(projectDir, ...)` 수정 확인
- [x] `projectDir = process.env.CLAUDE_PROJECT_DIR || process.cwd()` 선언 추가

**버그 2 — 파싱 오류 수정 (A-4):**
- [x] `line.split('\t')` → `line.split(':')` 수정 확인
- [x] 실제 포맷 일치: `timestamp:filePath:repo` (post-tool-use-tracker.sh L152 참조)

**버그 3 — 필드명 불일치 수정 (A-4):**
- [x] `interface EditedFile { path: string; tool: string; }` → `{ timestamp, filePath, repo }` 수정
- [x] `file.path` → `file.filePath` 전체 교체 (2곳)

**버그 4 — async 제거 (D-4 선행 처리):**
- [x] `async function main()` → `function main()` 수정
- [x] `main().catch(() => process.exit(0))` → `main()` 수정

**Stop 훅 등록:**
- [x] `settings.json` Stop 훅 배열에 `error-handling-reminder.sh` 추가
  - 순서: `trigger-build-resolver.sh` → `error-handling-reminder.sh` → `docs-update-reminder.sh`

**검증:**
- [x] `echo '{"session_id":"test-b3",...}' | npx tsx error-handling-reminder.ts` → exit 0 (캐시 없음) PASS
- [x] `bash -n error-handling-reminder.sh` PASS

---

### B-4: `session-start-docs-loader.ts` TTL 7일 정리 로직 삽입 ✅

**검증 파일:** `.claude/hooks/session-start-docs-loader.ts`

- [x] `readdirSync`, `statSync`, `unlinkSync` import 추가
- [x] TTL 정리 블록 삽입 위치 확인: sessionCacheDir 생성 블록 직후, `historyPath` 선언 전
- [x] `CACHE_TTL_DAYS = 7` 상수 정의
- [x] `entry.endsWith('.loaded')` 필터링 확인
- [x] `stat.mtime < cutoff` 비교로 오래된 파일 삭제 확인
- [x] `.gitkeep` 파일은 `.loaded` 확장자가 아니므로 삭제되지 않음 확인

**수동 정리 (B-4 구현 전 일회성):**
- [x] TTL 로직으로 다음 세션 시작 시 자동 정리 예정 (7일 이상 된 파일 정리)

---

### B-5: `.mjs` 파일 dead code 조사 및 정리 ✅ (P2 신규)

**대상 4개 파일 (전체 삭제 완료):**
- ~~`.claude/hooks/post-tool-use-tracker.mjs`~~
- ~~`.claude/hooks/skill-activation-prompt.mjs`~~
- ~~`.claude/hooks/trigger-build-resolver.mjs`~~
- ~~`.claude/hooks/tsc-check.mjs`~~

- [x] `grep -r "\.mjs" .claude/settings.json` → 0건 확인 (settings.json 미참조)
- [x] `.claude/` 전체 참조 확인 → package-lock.json 내부 npm 의존성 + mermaid 문서 예시만 존재 (훅 .mjs 아님)
- [x] 미참조 확인 후 4개 파일 전체 삭제
- [x] 삭제 후 hooks 디렉토리 .mjs 미존재 확인

---

### B 전체 완료 기준 ✅

- [x] B-1: `session-start-docs-loader.sh` `set -e` 없음 + `|| exit 0` 패턴 확인
- [x] B-2: `skill-activation-prompt.sh` `set -e` 없음 + `|| exit 0` 패턴 확인
- [x] B-2-추가: `post-tool-use-tracker.sh` `set -e` 없음 확인
- [x] B-3-삭제: `stop-build-check-enhanced.sh` 미존재 확인
- [x] B-3-수정: `error-handling-reminder.ts` 4개 버그 수정 + settings.json 등록 확인
- [x] B-4: `session-start-docs-loader.ts` TTL 정리 로직 존재 확인
- [x] B-5: `.mjs` 파일 4개 미존재 확인
- [x] `grep "^set -e" .claude/hooks/*.sh` → 0건 확인 (전체 클린)
- [x] 모든 수정 파일 `bash -n` 통과
- [x] `error-handling-reminder.ts` 수동 실행 테스트 exit 0 확인 PASS
- [x] Dead hook 정리 결정 사항 history.md 기록 완료

---

## Phase 8-C — P1 에이전트 DoD 위반 수정 체크리스트

> **착수 조건:** Phase 8-B 완료 확인 ✅ (2026-03-10 완료)
> **현재 Phase: 🔴 진행 중**
> **배경:** 2026-03-10 QA 리뷰에서 에이전트 DoD 위반 10개 + 4문서 체계 충돌 2건 + 크리덴셜 하드코딩 1건 발견

---

### C-1: 에이전트 10개 `tools:` 필드 추가 ✅

> **DoD 위반:** CLAUDE.md 에이전트 완성 기준 — `tools:` 필드는 필수 항목

**추가 위치:** `color:` 또는 `model:` 바로 앞 라인에 삽입

| 파일 | 추가할 tools: 값 | 상태 |
|---|---|---|
| `auth-route-debugger.md` | `Read, Bash, Glob, Grep` | ✅ (C-3과 동시 처리) |
| `auth-route-tester.md` | `Read, Bash, Glob, Grep` | ✅ |
| `code-architecture-reviewer.md` | `Read, Glob, Grep, Write, Bash` | ✅ |
| `code-refactor-master.md` | `Read, Write, Edit, MultiEdit, Bash` | ✅ |
| `documentation-architect.md` | `Read, Write, Edit, Glob, Grep` | ✅ |
| `frontend-error-fixer.md` | `Read, Write, Edit, Bash, Glob` | ✅ |
| `plan-reviewer.md` | `Read, Glob, Grep, WebSearch, WebFetch` | ✅ |
| `planner.md` | `Read, Write, Glob, Grep, Bash` | ✅ (C-2와 동시 처리) |
| `refactor-planner.md` | `Read, Glob, Grep` | ✅ |
| `web-research-specialist.md` | `WebSearch, WebFetch, Read, Write` | ✅ |

- [x] 10개 파일 전체 `tools:` 필드 존재 확인
- [x] YAML frontmatter `---` 블록 유지 (문법 오류 없음) 확인
- [x] 검증: `for f in [...]; do grep "^tools:" .claude/agents/$f.md; done` — 10개 전원 출력

---

### C-2: `planner.md` 4문서 체계 정렬 ✅

> **경로 충돌:** `dev/active/` 참조 5곳 → `docs/` 체계로 전면 교체

**수정 대상 5곳:**

- [x] **수정 1** — YAML `description`: `dev/active/` → `docs/plan.md + docs/task.md` 참조로 수정
- [x] **수정 2** — Step 1 Read 목록: `/dev/README.md` → `docs/history.md`, `docs/plan.md`, `docs/task.md`
- [x] **수정 3** — Step 4: `mkdir -p dev/active/[task-name]` + 3개 파일 Write → `docs/plan.md`, `docs/task.md` Edit으로 교체
- [x] **수정 4** — Step 5 Summary: `dev/active/[task-name]/` 경로 → `docs/` 경로로 교체
- [x] **수정 5** — Quality Checklist 마지막 항목: `dev/active/[task-name]/` → `docs/plan.md`, `docs/task.md`
- [x] 검증: `grep "dev/active" .claude/agents/planner.md` → 0건 확인

---

### C-3: `auth-route-debugger.md` 범용화 ✅

> **보안 이슈:** 특정 프로젝트 크리덴셜 하드코딩 → 범용 플레이스홀더로 교체

**수정 대상 (4종류):**

- [x] **수정 1** — 본문 도입부 L7: `"for the your project application"` → `"for your project"`
- [x] **수정 2** — PM2 명령 (L30-35): `pm2 logs form` / `tail -f form/logs/form-error.log` → `[service-name]` 범용화
- [x] **수정 3** — Key Technical Details: `testuser`/`testpassword` → `$TEST_USER`/`$TEST_PASSWORD` 환경변수 참조
- [x] **수정 4** — Key Technical Details: `yourRealm` → `[your-realm]`, `your-app-client` → `[your-client-id]`, `form/config.ini` → `[service]/config.ini`
- [x] **추가** — 파일 상단(--- 블록 이후 첫 번째 라인): 사용 전 환경변수 설정 주의사항 블록 추가
- [x] 검증: `grep -E "testuser|testpassword|yourRealm|your-app-client|pm2 logs form" .claude/agents/auth-route-debugger.md` → 0건

---

### C 전체 완료 기준 ✅

- [x] C-1: 10개 에이전트 `tools:` 필드 전원 추가 확인
- [x] C-2: `planner.md` — `grep "dev/active"` → 0건, `grep "docs/"` 존재 확인
- [x] C-3: `auth-route-debugger.md` — 하드코딩 크리덴셜 전원 제거 확인 (grep 0건)
- [x] 수정된 에이전트 파일 YAML frontmatter `---` 블록 정상 (head -6으로 확인)
- [x] docs/checklist.md C-1/C-2/C-3 전항목 [x] 체크

---

## Phase 8-D — P2 MEDIUM 코드 품질 개선 체크리스트

> **착수 조건:** Phase 8-C 완료 확인 ✅ (2026-03-11 완료)
> **현재 Phase: 🔴 진행 중**

---

### D-1: `docs-update-reminder.ts` async 제거 ✅

**파일:** `.claude/hooks/docs-update-reminder.ts`

- [x] L14: `async function main()` → `function main()` 수정 확인
- [x] L68-70: `main().catch(() => { process.exit(0); });` → `main();` 수정 확인
- [x] 검증: `grep "async function main" docs-update-reminder.ts` → 0건
- [x] 검증: `grep "\.catch(" docs-update-reminder.ts` → 0건

---

### D-2: `pr-review-trigger.ts` async 제거 ✅

**파일:** `.claude/hooks/pr-review-trigger.ts`

- [x] L11: `async function main()` → `function main()` 수정 확인
- [x] L56-58: `main().catch(() => { process.exit(0); });` → `main();` 수정 확인
- [x] 검증: `grep "async function main" pr-review-trigger.ts` → 0건
- [x] 검증: `grep "\.catch(" pr-review-trigger.ts` → 0건

---

### D-3: `skill-activation-prompt.ts` async 제거 + exit(1) 수정 ✅

**파일:** `.claude/hooks/skill-activation-prompt.ts`

- [x] L40: `async function main()` → `function main()` 수정 확인
- [x] L128-130 catch 블록:
  - [x] `catch (err)` → `catch (_err)` 수정
  - [x] `console.error('Error in skill-activation-prompt hook:', err)` 라인 제거
  - [x] `process.exit(1)` → `process.exit(0)` 수정
  - [x] `// UserPromptSubmit: always exit 0 to avoid blocking the prompt` 주석 추가
- [x] L134-137: `main().catch(err => { ... process.exit(1); })` → `main();` 수정 확인
- [x] 검증: `grep "async function main" skill-activation-prompt.ts` → 0건
- [x] 검증: `grep "process.exit(1)" skill-activation-prompt.ts` → 0건
- [x] 검증: `grep "\.catch(" skill-activation-prompt.ts` → 0건

---

### D-5: `pnpm-lock.yaml` 제거 ✅

**파일:** `.claude/hooks/pnpm-lock.yaml`

- [x] `rm .claude/hooks/pnpm-lock.yaml` 실행
- [x] 검증: `ls .claude/hooks/pnpm-lock.yaml` → 파일 없음 확인
- [x] `package-lock.json` 유지 확인 (삭제 대상 아님)

---

### D-6: `settings.json` MCP 설정 수정 ✅

**파일:** `.claude/settings.json`

- [x] L2: `"enableAllProjectMcpServers": true` → `"enableAllProjectMcpServers": false` 수정
- [x] `enabledMcpjsonServers` 목록 (mysql/sequential-thinking/playwright) 유지 확인
- [x] 검증: `grep "enableAllProjectMcpServers" .claude/settings.json` → `false` 출력

---

### D-7: GitHub Actions 중복 댓글 방지 ✅

**파일:** `.github/workflows/pr-code-review.yaml`

- [x] `listComments` 호출로 봇 기존 댓글 탐색 로직 추가
- [x] `botComment` 변수: `c.user?.type === 'Bot'` + `c.body?.startsWith('## Code Review Checklist')` 조건
- [x] 기존 댓글 있음 → `updateComment(comment_id: botComment.id, body)` 분기
- [x] 기존 댓글 없음 → `createComment(issue_number, body)` 분기
- [x] 기존 `createComment` 단독 호출 제거 확인
- [x] 검증: `grep -E "listComments|updateComment|botComment" .github/workflows/pr-code-review.yaml` → 3패턴 전원 존재

---

### D 전체 완료 기준 ✅

- [x] D-1: `docs-update-reminder.ts` async 없음 + `.catch(` 없음
- [x] D-2: `pr-review-trigger.ts` async 없음 + `.catch(` 없음
- [x] D-3: `skill-activation-prompt.ts` async 없음 + `exit(1)` 없음 + `.catch(` 없음
- [x] D-5: `.claude/hooks/pnpm-lock.yaml` 미존재, `package-lock.json` 유지
- [x] D-6: `settings.json` `enableAllProjectMcpServers: false` 확인
- [x] D-7: `pr-code-review.yaml` listComments/updateComment/botComment 3패턴 존재
- [x] docs/checklist.md D-1~D-7 전항목 [x] 체크

---

## Phase 8-E — 통합 검증 + 릴리즈 체크리스트

> **착수 조건:** Phase 8-A/B/C/D 전체 완료 ✅ (2026-03-11)
> **현재 Phase: ✅ 완료 (2026-03-11)**

---

### E-1: 훅 파일 존재 + 문법 검증 ✅

> **목표:** settings.json 등록 8개 훅 파일 전원 존재 + bash 문법 PASS + 안전 패턴 확인

#### 1-A: 파일 존재 확인 (8개)

| 훅 파일 | 이벤트 | 상태 |
|---|---|---|
| `skill-activation-prompt.sh` | UserPromptSubmit | ✅ |
| `session-start-docs-loader.sh` | UserPromptSubmit | ✅ |
| `post-tool-use-tracker.sh` | PostToolUse/Edit | ✅ |
| `tsc-check.sh` | PostToolUse/Edit | ✅ |
| `pr-review-trigger.sh` | PostToolUse/Bash | ✅ |
| `trigger-build-resolver.sh` | Stop | ✅ |
| `error-handling-reminder.sh` | Stop | ✅ |
| `docs-update-reminder.sh` | Stop | ✅ |

- [x] 8개 파일 전원 존재 확인 (bash 루프로 일괄 체크)

#### 1-B: bash 문법 검증

- [x] `bash -n` — 전체 `.sh` 파일 문법 오류 없음 (SYNTAX ERROR 0건)

#### 1-C: 안전 패턴 확인

- [x] UserPromptSubmit 훅 (`skill-activation-prompt.sh`, `session-start-docs-loader.sh`) — `set -e` 없음
- [x] `.ts` 훅 파일 전체 — `process.exit(1)` 없음 (UserPromptSubmit 차단 방지)
- [x] `.ts` 훅 파일 전체 — `async function main` 없음 (D-1/D-2/D-3 완료 재검증)
- [x] `.ts` 훅 파일 전체 — `.catch(` 없음

---

### E-2: 훅 수동 실행 테스트 ✅

> **방법:** `cd .claude/hooks` 후 각 훅에 stdin JSON 주입 → exit code 확인

#### 2-A: UserPromptSubmit 훅

| 테스트 케이스 | 기대 exit | 기대 출력 | 상태 |
|---|---|---|---|
| `skill-activation-prompt.sh` — prompt="안녕하세요" (매칭 없음) | 0 | 아무 출력 없음 | ✅ |
| `session-start-docs-loader.sh` — 새 session_id | 0 | 4문서 읽기 지시 or 무출력 | ✅ |

- [x] skill-activation-prompt.sh exit 0 확인
- [x] session-start-docs-loader.sh exit 0 확인 (프롬프트 차단 없음)

#### 2-B: PostToolUse 훅

| 테스트 케이스 | 기대 exit | 기대 동작 | 상태 |
|---|---|---|---|
| `tsc-check.sh` — Edit + `.ts` 파일 | 0 | TSC 실행 (tsconfig 없는 디렉토리는 스킵) | ✅ |
| `tsc-check.sh` — Edit + `.py` 파일 | 0 | 스킵, 아무 출력 없음 | ✅ |
| `pr-review-trigger.sh` — Bash + `gh pr create` | 2 | PR 리뷰 권고 메시지 출력 | ✅ |
| `pr-review-trigger.sh` — Bash + `ls -la` | 0 | 스킵, 아무 출력 없음 | ✅ |

- [x] tsc-check.sh `.ts` → exit 0
- [x] tsc-check.sh `.py` → exit 0 (스킵)
- [x] pr-review-trigger.sh `gh pr create` → exit 2 (컨텍스트 주입 모드)
- [x] pr-review-trigger.sh 일반 명령 → exit 0 (스킵)

> **추가 발견 및 수정:** `tsc-check.sh` — `jq` 미설치(Windows/MINGW64) → `node` 기반 JSON 파싱으로 교체 완료 ✅

#### 2-C: Stop 훅

| 테스트 케이스 | 기대 exit | 기대 동작 | 상태 |
|---|---|---|---|
| `trigger-build-resolver.sh` — 빈 JSON | 0 | 디버그 로그 미생성 | ✅ |
| `docs-update-reminder.sh` — 없는 session_id | 0 | 아무 출력 없음 | ✅ |

- [x] trigger-build-resolver.sh → `/tmp/claude-hook-debug.log` 크기 미변화 확인
- [x] trigger-build-resolver.sh → exit 0
- [x] docs-update-reminder.sh (없는 session) → exit 0, 무출력

---

### E-3: 에이전트 DoD 재검증 ✅

> **목표:** Phase 8-C 수정 내역 최종 확인 (grep 기반)

#### 3-A: 10개 에이전트 tools: 필드

| 에이전트 | tools: 값 | 상태 |
|---|---|---|
| `auth-route-debugger` | `Read, Bash, Glob, Grep` | ✅ |
| `auth-route-tester` | `Read, Bash, Glob, Grep` | ✅ |
| `code-architecture-reviewer` | `Read, Glob, Grep, Write, Bash` | ✅ |
| `code-refactor-master` | `Read, Write, Edit, MultiEdit, Bash` | ✅ |
| `documentation-architect` | `Read, Write, Edit, Glob, Grep` | ✅ |
| `frontend-error-fixer` | `Read, Write, Edit, Bash, Glob` | ✅ |
| `plan-reviewer` | `Read, Glob, Grep, WebSearch, WebFetch` | ✅ |
| `planner` | `Read, Write, Glob, Grep, Bash` | ✅ |
| `refactor-planner` | `Read, Glob, Grep` | ✅ |
| `web-research-specialist` | `WebSearch, WebFetch, Read, Write` | ✅ |

- [x] 10개 전원 `grep "^tools:"` PASS
- [x] agents 파일 수 = 29개 (README 제외)

#### 3-B: planner.md 경로 확인

- [x] `grep "dev/active" .claude/agents/planner.md` → 0건
- [x] `grep "docs/" .claude/agents/planner.md` → 존재 확인

#### 3-C: auth-route-debugger.md 크리덴셜 확인

- [x] `grep -E "testuser|testpassword|yourRealm|your-app-client|pm2 logs form"` → 0건

---

### E-4: `CHANGELOG.md` v1.6.1 릴리즈 ✅

#### Fixed 항목 (13건) — E-2 추가 수정 포함

- [x] A-0: `base_code_plan.md` 버전 표기 수정 기재
- [x] A-1: `tsc-check.sh` 근본 결함 수정 기재
- [x] A-2: `trigger-build-resolver.sh` 전면 재작성 기재
- [x] A-3: `session-start-docs-loader.ts` catch exit(1)→exit(0) 기재
- [x] B-1: `session-start-docs-loader.sh` set -e 제거 기재
- [x] B-2: `skill-activation-prompt.sh` set -e 제거 기재
- [x] B-2-추가: `post-tool-use-tracker.sh` set -e 제거 기재
- [x] B-3-수정: `error-handling-reminder.ts` 4개 버그 수정 기재
- [x] C-1: 에이전트 10개 tools: 필드 추가 기재
- [x] C-2: `planner.md` dev/active → docs/ 전면 교체 기재
- [x] C-3: `auth-route-debugger.md` 크리덴셜 하드코딩 범용화 기재
- [x] D-3: `skill-activation-prompt.ts` exit(1)→exit(0) 기재
- [x] E-2 추가: `tsc-check.sh` jq → node 교체 기재

#### Changed 항목 (8건)

- [x] B-3-삭제: `stop-build-check-enhanced.sh` 삭제 기재
- [x] B-3-수정: `error-handling-reminder.sh` Stop 훅 신규 등록 기재
- [x] B-4: `session-start-docs-loader.ts` TTL 7일 정리 로직 기재
- [x] B-5: `.mjs` 파일 4개 dead code 삭제 기재
- [x] D-1/D-2: `docs-update-reminder.ts`, `pr-review-trigger.ts` async 제거 기재
- [x] D-5: `pnpm-lock.yaml` 삭제 기재
- [x] D-6: `settings.json` enableAllProjectMcpServers: false 기재
- [x] D-7: `pr-code-review.yaml` 중복 댓글 방지 기재

#### 검증

- [x] `grep "\[v1.6.1\]" CHANGELOG.md` → 존재 확인
- [x] `## [Unreleased]` 아래 `## [v1.6.1]` 순서 확인

---

### E-5: 4문서 + base_code_plan.md 완료 처리 ✅

| 파일 | 작업 | 상태 |
|---|---|---|
| `docs/plan.md` | Phase 8-C/D/E ✅ 완료 추가, 대시보드 v1.6.1 갱신 | ✅ |
| `docs/task.md` | 현재 Phase 완료 갱신 | ✅ |
| `docs/checklist.md` | Phase 8-E 전항목 [x] 체크 | ✅ |
| `docs/history.md` | Phase 8 완료 이력 기록, 다음 Phase 갱신 | ✅ |
| `base_code_plan.md` | 현재 버전 v1.6.0 → v1.6.1, Last updated 갱신 | ✅ |

- [x] `docs/plan.md` Phase 8-E ✅ 완료 처리
- [x] `docs/plan.md` 대시보드 CHANGELOG 버전 v1.6.1 갱신
- [x] `docs/task.md` 완료 상태 갱신
- [x] `docs/history.md` Phase 8 완료 이력 + 다음 Phase(상시 운영 7-A/7-B) 안내
- [x] `docs/checklist.md` Phase 8-E 전항목 [x] 체크
- [x] `base_code_plan.md` 현재 버전 v1.6.1, Last updated 갱신

---

## 하네스 상시 품질 기준 (안정 상태 유지)

신규 작업 완료 후 아래 기준 충족 여부를 확인한다.

### 자산 무결성

- [ ] `skill-rules.json` 등록 스킬 수 = `.claude/skills/` 디렉토리 수 (24개)
- [ ] `.claude/agents/README.md` 등록 에이전트 수 = `.claude/agents/*.md` 파일 수 (29개)
- [ ] `CHANGELOG.md` 최신 릴리즈 버전 = `base_code_plan.md` 현재 버전 일치
- [ ] `docs/history.md` Last updated 날짜 최신 작업 반영

### 훅 안정성 기준 (Phase 8 이후 추가)

- [ ] UserPromptSubmit 훅: `set -e` 미사용, catch에서 `exit(0)` 사용
- [ ] Stop 훅: catch에서 `exit(0)` 사용 (세션 차단 방지)
- [ ] PostToolUse 훅: `exit(2)` = Claude 컨텍스트 주입, 오류 시 `exit(0)`
- [ ] settings.json 등록 훅 파일 전원 존재 확인
- [ ] tsc-check.sh PostToolUse 훅으로 등록 확인 (Stop에서 이동)

### 신규 에이전트 추가 시 DoD

- [ ] YAML frontmatter (`name`, `description`, `tools`) 포함
- [ ] 역할 및 목적 명시 (1-2문장)
- [ ] 실행 단계 순서 명시 (번호 목록)
- [ ] 사용 가능 도구 목록 명시
- [ ] 출력 형식 정의 (결과물 예시 포함)
- [ ] 실행 예시 1개 이상
- [ ] 기존 에이전트/스킬과 역할 분리 명시 (충돌 가능성 있는 경우)
- [ ] `.claude/agents/README.md` 에이전트 카테고리에 행 추가
- [ ] `CHANGELOG.md` Added 항목 추가

### 신규 스킬 추가 시 DoD

- [ ] `SKILL.md` 500줄 이하
- [ ] YAML frontmatter + 트리거 키워드 5개 이상
- [ ] 핵심 패턴 3개 이상 + 코드 예시
- [ ] 안티패턴 섹션 포함
- [ ] `skill-rules.json` 트리거 규칙 추가
- [ ] `.claude/skills/README.md` 목록 갱신
- [ ] `CHANGELOG.md` Added 항목 추가

---

*Last updated: 2026-03-11 (Phase 8-E 완료 — E-1 훅 8개 PASS, E-2 수동 실행 7케이스 PASS + tsc-check.sh jq→node 추가 수정, E-3 에이전트 DoD 29개 PASS, E-4 CHANGELOG v1.6.1 릴리즈 Fixed 13건+Changed 8건, E-5 5문서 완료 처리)*
