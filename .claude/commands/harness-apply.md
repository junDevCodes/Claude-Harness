---
description: 현재 프로젝트에 하네스 자산을 자동 분석하여 신규/기존 모드로 충돌 없이 적용
argument-hint: Optional — 하네스 레포 경로 지정 (기본값: 환경변수 또는 상위 디렉토리 탐색)
---

현재 프로젝트에 Advanced Harness 자산을 적용합니다.
`.claude/` 디렉토리 존재 여부를 기준으로 **신규 모드** 또는 **기존 모드**를 자동으로 판별합니다.

## 실행 흐름

### 1. 하네스 경로 확인

`$ARGUMENTS`에 하네스 레포 경로가 있으면 사용. 없으면:
1. `ADVANCED_HARNESS_PATH` 환경변수 확인
2. 상위 디렉토리에서 `advanced-harness-window/` 탐색
3. 탐색 실패 시 사용자에게 경로 입력 요청

### 2. 현재 프로젝트 구조 탐지

현재 작업 디렉토리에서 아래 항목을 확인합니다:

```
확인 항목:
- .claude/ 디렉토리 존재 여부
- .claude/settings.json 존재 여부
- .claude/skills/skill-rules.json 존재 여부
- .claude/hooks/ 존재 여부
- .claude/agents/ 존재 여부
- docs/ 4문서 존재 여부
- CLAUDE.md 존재 여부
- 주요 소스 디렉토리 (src/, backend/, frontend/, app/ 등)
- 기술 스택 힌트 (package.json, pyproject.toml, pom.xml, build.gradle.kts 등)
```

### 3. 모드 판별 및 선언

**신규 모드 (`.claude/` 없음):**
```
✨ 신규 프로젝트 모드
현재 프로젝트에 .claude/ 디렉토리가 없습니다.
하네스 자산을 전체 복사 방식으로 적용합니다.
```

**기존 모드 (`.claude/` 있음):**
```
🔄 기존 프로젝트 병합 모드
현재 프로젝트에 기존 .claude/ 설정이 감지되었습니다.
기존 설정을 보존하면서 하네스 자산을 추가합니다.
```

---

### 4-A: 신규 모드 실행 흐름

#### 4-A-1: 스택 감지 및 베이스 코드 안내

탐지된 기술 스택 힌트를 기반으로 권장 베이스 코드를 안내합니다:

```
감지된 스택: [탐지 결과]

권장 베이스 코드:
  → backend: [harness_path]/base/[stack]/
  → frontend: [harness_path]/base/nextjs/  (해당 시)

복사 명령어:
  cp -r [harness_path]/.claude ./
  cp -r [harness_path]/base/[stack]/ ./backend/  (선택)
  chmod +x .claude/hooks/*.sh
  cd .claude/hooks && npm install
```

#### 4-A-2: pathPattern 설정 안내

스택과 구조에 맞는 pathPattern 예시를 제시합니다:

```json
// 권장 pathPattern — skill-rules.json 수정 필요
"[스킬명]": {
  "fileTriggers": {
    "pathPatterns": ["[탐지된 소스 경로]/**/*.확장자"]
  }
}
```

#### 4-A-3: CLAUDE.md + docs/ 초기화 안내

```
다음 단계:
1. CLAUDE.md 생성 (4문서 체계 섹션 포함)
2. mkdir -p docs
3. Claude Code에서 초기 4문서 작성 요청
```

---

### 4-B: 기존 모드 실행 흐름

#### 4-B-1: 충돌 분석 보고

기존 설정을 읽어 충돌 가능성을 분석합니다:

**settings.json 분석:**
```
기존 훅 목록:
  UserPromptSubmit: [기존 훅 목록]
  PostToolUse: [기존 훅 목록]
  Stop: [기존 훅 목록]

추가될 훅 (충돌 없음):
  UserPromptSubmit: skill-activation-prompt, session-start-docs-loader
  PostToolUse: post-tool-use-tracker, pr-review-trigger
  Stop: docs-update-reminder
```

**skill-rules.json 분석:**
```
기존 스킬: [기존 스킬 목록]
추가 가능 스킬: [하네스 스킬 중 기존에 없는 것]
중복 스킬: [같은 이름이 있는 것] ← 주의 필요
```

#### 4-B-2: 병합 전략 제시

```
병합 순서:
1. Skills 복사 (기존 건드리지 않음, 새 스킬만 추가)
2. skill-rules.json 병합 (새 스킬 항목 추가)
3. Agents 복사 (전체 또는 선택)
4. Hooks 복사 + 실행 권한
5. settings.json 훅 항목 추가 (기존 배열 유지)
6. pathPattern 재설정
7. CLAUDE.md 4문서 섹션 추가
8. docs/ 4문서 처리 → 4-B-5 참조 (기존 문서 유무에 따라 분기)

각 단계별 구체적 명령어를 제시합니다.
```

#### 4-B-3: 단계별 병합 명령어 제시

각 단계마다 실행할 명령어를 구체적으로 출력합니다:

```bash
# Step 1: Skills 추가 (권장 스킬만)
cp -r [harness_path]/.claude/skills/fastapi-backend-guidelines .claude/skills/
cp -r [harness_path]/.claude/skills/nextjs-frontend-guidelines .claude/skills/
# ... (스택에 맞는 스킬)

# Step 2: skill-rules.json에 새 스킬 항목 추가
# → 하네스 skill-rules.json에서 해당 스킬 블록 추출하여 제시

# Step 3: Agents 추가
cp [harness_path]/.claude/agents/db-migration-agent.md .claude/agents/
cp [harness_path]/.claude/agents/api-spec-generator.md .claude/agents/
# ... (필요한 에이전트)

# Step 4: Hooks 추가
cp [harness_path]/.claude/hooks/skill-activation-prompt.{ts,sh} .claude/hooks/
cp [harness_path]/.claude/hooks/session-start-docs-loader.{ts,sh} .claude/hooks/
cp [harness_path]/.claude/hooks/docs-update-reminder.{ts,sh} .claude/hooks/
chmod +x .claude/hooks/*.sh
cd .claude/hooks && npm install

# Step 5: settings.json 훅 추가 (병합 방식)
# → 기존 settings.json을 보여주고 추가할 블록을 정확히 명시

# Step 6: pathPattern 재설정
# → 탐지된 소스 경로 기반 권장 설정 제시
```

#### 4-B-4: pathPattern 재설정

현재 프로젝트의 소스 파일 경로를 탐색하여 적합한 pathPattern을 제시합니다:

```
탐지된 소스 경로:
  Python: backend/app/**/*.py
  TypeScript: frontend/src/**/*.tsx

권장 pathPattern 설정:
  fastapi-backend-guidelines → ["backend/app/**/*.py", "backend/**/*.py"]
  nextjs-frontend-guidelines → ["frontend/src/**/*.tsx", "frontend/src/**/*.ts"]
```

#### 4-B-5: 기존 4문서 처리 (분석 → 질의응답 → 확정)

기존 `docs/` 디렉토리의 4문서 존재 여부에 따라 **두 가지 경로**로 분기합니다.

**경로 A — docs/ 없거나 4문서 불완전:**

빈 4문서 템플릿을 생성합니다:
```bash
mkdir -p docs
# plan.md, task.md, history.md, checklist.md 빈 템플릿 생성
```

**경로 B — 4문서가 이미 존재 (plan.md + task.md + history.md + checklist.md):**

⚠️ **기존 4문서를 절대 덮어쓰지 않습니다.** 아래 3단계를 수행합니다.

---

**B-Step 1: 기존 4문서 전수 분석**

4개 문서를 모두 읽고 아래 항목을 추출하여 사용자에게 보고합니다:

```
📋 기존 프로젝트 계획 분석 결과

■ plan.md 분석:
  - 전체 Phase 수: [N]개
  - 완료 Phase: [N]개 (✅ 또는 [x])
  - 진행중 Phase: [Phase명] — [설명]
  - 미착수 Phase: [N]개
  - 핵심 목표: [plan.md에서 추출한 프로젝트 목표 1-2문장]

■ task.md 분석:
  - 현재 작업: [Phase/Task명]
  - 완료 항목: [N]개 / 전체 [N]개
  - 남은 작업: [주요 미완료 항목 3개]

■ history.md 분석:
  - 기록 범위: [최초 날짜] ~ [최근 날짜]
  - 주요 결정 사항: [N]건
  - 최근 작업: [가장 최근 기록 1-2문장]

■ checklist.md 분석:
  - 현재 Phase 체크리스트: [N]개 항목
  - 완료율: [N]% ([완료]/[전체])

■ 하네스 호환성:
  - 4문서 체계 형식 일치: [일치/부분일치/불일치]
  - 훅 연동 가능 여부: [가능/수정필요] (파일명 plan.md/task.md/history.md/checklist.md)
  - 감지된 차이점: [형식 차이 있으면 구체적 기술]
```

---

**B-Step 2: 사용자 질의응답 (확정 전 필수)**

분석 결과를 보여준 뒤, 아래 질문들로 사용자 의도를 확인합니다:

```
🔍 기존 계획에 대해 확인이 필요합니다:

1. 현재 [Phase명]이 진행 중인데, 이 계획을 그대로 유지할까요?
   (a) 유지 — 기존 계획 그대로 이어서 진행
   (b) 수정 — 기존 계획에 하네스 패턴 반영하여 보강
   (c) 교체 — 기존 계획 폐기하고 새로 작성

2. history.md에 기존 맥락이 있습니다. 보존할까요?
   (a) 전체 보존
   (b) 요약 후 압축
   (c) 새로 시작 (기존 내용 아카이빙)

3. 하네스의 어떤 자산을 이 프로젝트에 활용하고 싶으신가요?
   (a) 4문서 자동화 훅만 (세션 시작 로드 + 종료 리마인더)
   (b) 훅 + 스킬 + 에이전트 전체
   (c) 특정 자산만 선택 (목록 제시)
```

**추가 질의 (사용자 답변에 따라):**

- (1-b) 선택 시: "plan.md에 하네스 형식(Phase 체크박스, 자산 대시보드)을 적용할까요?"
- (1-b) 선택 시: "task.md 범위 기준(checklist 15개 이하)을 적용할까요?"
- (2-b) 선택 시: "어느 시점 이전 내용을 압축할까요?"
- 프로젝트 목표/방향 불명확 시: "이 프로젝트의 핵심 목표를 한 문장으로 알려주세요."

---

**B-Step 3: 확정 + 문서 수정**

사용자 답변을 반영하여 4문서를 수정합니다.

**(1-a) 유지 모드:**
- 4문서 내용 변경 없음
- CLAUDE.md에 4문서 체계 섹션만 추가 (없는 경우)
- 훅이 파일명 기반으로 자동 연동

**(1-b) 수정 모드:**
```
수정 사항 (사용자 확인 후 적용):

□ plan.md:
  - Phase별 [ ] 체크박스 형식 통일
  - 완료 Phase에 ✅ 마크 추가
  - 자산 현황 대시보드 테이블 추가 (선택)

□ task.md:
  - 현재 작업 Phase 헤더에 출처(plan.md 참조) 추가
  - checklist.md 15개 이하 범위 기준 안내 추가
  - 검증 방법 섹션 보강 (없는 경우)

□ history.md:
  - "현재 상태 요약" 섹션 추가/갱신 (없는 경우)
  - "다음 세션에서 이어갈 작업" 섹션 추가 (없는 경우)
  - 에이전트 핸드오프 포맷 템플릿 추가 (없는 경우)

□ checklist.md:
  - Phase DoD 형식 통일 (있는 경우 유지)

위 수정 사항을 적용할까요? (Y/n)
```

**(1-c) 교체 모드:**
```
⚠️ 기존 4문서를 아카이빙합니다:
  → docs/archive/[날짜]-plan.md
  → docs/archive/[날짜]-task.md
  → docs/archive/[날짜]-history.md
  → docs/archive/[날짜]-checklist.md

새 4문서를 하네스 표준 형식으로 생성합니다.
사용자에게 프로젝트 목표와 첫 번째 Phase를 질문하여 초기 내용을 작성합니다.
```

---

### 5. CLAUDE.md 4문서 섹션 확인

현재 `CLAUDE.md`를 읽어 4문서 체계 섹션이 있는지 확인합니다.

**없는 경우:** 아래 내용을 CLAUDE.md에 추가하도록 안내합니다:

```markdown
## Agent 작업 문서 체계

모든 Claude Agents는 작업 시 `docs/` 디렉토리의 4문서 체계를 기준으로 작업한다.

| 문서 | 역할 |
|---|---|
| `docs/plan.md` | 전체 완성 계획 기획서 |
| `docs/task.md` | 현재 작업 상세 실행 계획서 |
| `docs/history.md` | 이전 작업 맥락 문서 |
| `docs/checklist.md` | 완료 기준 확인서 |

### 작업 시작 시 (필수)
1. docs/history.md 읽기
2. docs/task.md 읽기
3. docs/checklist.md 읽기
```

---

### 6. 최종 검증 체크리스트 출력

모든 단계 완료 후 최종 검증 목록을 출력합니다:

```
✅ 하네스 적용 완료 — 최종 검증 체크리스트

필수 확인:
  [ ] .claude/hooks/*.sh 실행 권한 확인 (ls -la .claude/hooks/*.sh)
  [ ] .claude/settings.json JSON 유효성 (cat .claude/settings.json | python3 -m json.tool)
  [ ] .claude/skills/skill-rules.json JSON 유효성
  [ ] pathPattern이 실제 내 프로젝트 파일 경로와 일치
  [ ] docs/ 4문서 4개 존재

동작 테스트:
  [ ] Claude Code 새 세션 시작 → 4문서 로드 메시지 확인
  [ ] "[스택] 관련 작업" 입력 → 스킬 활성화 확인
  [ ] /harness-docs-update 실행 → docs/ 업데이트 확인
```

---

## 추가 컨텍스트: $ARGUMENTS

argument에 하네스 레포 경로를 지정할 수 있습니다.
(예: `/Users/myname/workspace/advanced-harness-window`)

지정하지 않으면 자동으로 탐색합니다.
