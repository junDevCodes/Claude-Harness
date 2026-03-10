---
description: checklist.md DoD 항목 기반 증거 수집 → PASS / NEEDS WORK 자동 판정
argument-hint: Optional — 판정할 Phase 이름 (예: "Phase 6-B"). 생략 시 현재 Phase 자동 탐지
---

`docs/checklist.md`의 현재 Phase DoD 항목을 읽어 증거 기반으로 **PASS / NEEDS WORK**를 자동 판정합니다.

> **판정 철학:** 기본값은 항상 **NEEDS WORK**. 구체적 증거(파일 존재, 필수 키워드, 테스트 결과)가 확인된 경우에만 **PASS** 판정. `reality-checker` 에이전트와 동일한 Pessimistic Default 원칙 적용.

---

## 실행 흐름 (반드시 아래 순서대로)

### Step 1: 현재 Phase DoD 항목 추출

1. `docs/checklist.md`를 읽어 현재 Phase 섹션을 찾습니다.
2. `docs/task.md`를 읽어 현재 Phase 이름을 확인합니다.
3. **argument 지정 시:** `$ARGUMENTS`에서 Phase 이름을 읽어 해당 Phase 체크리스트 섹션으로 고정.
4. **argument 없을 때:** checklist.md 상단 `> **현재 Phase:**` 주석 또는 task.md의 `> **현재 Phase:**`를 읽어 자동 탐지.
5. 대상 Phase에서 `[ ]` (미완료) 체크박스 항목을 전부 추출합니다.

> 추출한 미완료 항목 수를 먼저 출력: `[Phase X-Y] 미완료 DoD 항목 N개 탐지`

---

### Step 2: 항목별 증거 수집

각 `[ ]` 항목에 대해 아래 증거 수집 방법 중 가장 적합한 방법으로 증거를 수집합니다.

| DoD 항목 유형 | 증거 수집 방법 | PASS 조건 |
|---|---|---|
| 파일 존재 | `Glob: [경로]` | 파일 1개 이상 반환 |
| YAML frontmatter | `Read: [파일]` → 1번째 줄 `---` 확인 | 파일 최상단이 `---` 로 시작 |
| 섹션/키워드 포함 여부 | `Grep: [키워드] in [파일]` | 1건 이상 매칭 |
| 줄수 제한 (500줄) | `Bash: wc -l [파일]` | 결과 ≤ 500 |
| 테스트 PASS | `Bash: [테스트 명령어]` | exit code 0 |
| 빌드 성공 | `Bash: [빌드 명령어]` | exit code 0 |
| 항목 수 확인 | `Grep -c: [패턴] in [파일]` | 결과 ≥ 기대값 |

**증거 수집 중 주의:**
- 파일이 존재하지 않으면 해당 파일 관련 모든 항목 → NEEDS WORK (추가 수집 중단)
- 증거 수집 실패(오류 발생) → NEEDS WORK 처리 (불확실한 것은 PASS 불가)

---

### Step 3: 항목별 PASS / NEEDS WORK 판정

수집된 증거를 바탕으로 각 항목을 판정합니다.

**판정 기준:**

```
PASS:      증거가 명확히 확인됨 (파일 존재 + 내용 조건 충족)
NEEDS WORK: 증거 없음 / 증거 불충분 / 수집 실패
```

**판정 시 필수 규칙:**
- "있을 것 같다" / "아마 될 것이다" 추론으로 PASS 판정 금지
- 실제 도구 실행 결과만 증거로 인정
- 부분 충족(조건 일부만 만족)도 NEEDS WORK

---

### Step 4: 종합 결과 출력

아래 형식으로 판정 결과를 출력합니다.

**출력 형식:**

```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
🔍 quality-gate — [Phase 이름] DoD 판정
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

| 항목 | 증거 | 판정 |
|---|---|---|
| [DoD 항목 1] | [수집된 증거 요약] | ✅ PASS |
| [DoD 항목 2] | [증거 없음 / 미확인] | ❌ NEEDS WORK |
| [DoD 항목 3] | [Grep: 1건 매칭] | ✅ PASS |
...

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
총평: PASS N개 / NEEDS WORK M개 (전체 N+M개)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

[전체 PASS인 경우]
✅ 전체 PASS — 모든 DoD 항목 충족. 다음 Phase로 진행 가능.

[NEEDS WORK 항목이 있는 경우]
❌ NEEDS WORK — 미충족 항목 목록:

1. [미충족 항목 1] — [미충족 이유]
2. [미충족 항목 2] — [미충족 이유]

→ 위 항목을 수정 후 /quality-gate를 재실행하세요.
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

---

## 연계 에이전트

| 에이전트 | 연계 방식 |
|---|---|
| `reality-checker` | 동일한 Pessimistic Default 판정 철학 — 광범위 품질 판정 시 reality-checker 에이전트 사용 권장 |
| `evidence-collector` | 증거 수집이 복잡한 경우(스크린샷, 빌드 아티팩트) evidence-collector 에이전트 선행 실행 |
| `dev-qa-loop` | quality-gate 판정 결과를 기반으로 자동 수정 루프 실행 시 `/dev-qa-loop` 커맨드 사용 |

---

## 실행 예시

### 예시: Phase 6-A 에이전트 파일 DoD 검증

**상황:** Phase 6-A에서 생성한 `pipeline-orchestrator.md` 에이전트 파일의 DoD를 검증합니다.

**checklist.md 대상 항목 (추출됨):**
```
- [ ] 파일 생성 확인 (.claude/agents/pipeline-orchestrator.md)
- [ ] YAML frontmatter 포함 (name, description, tools)
- [ ] 에이전트 호출 단계 (탐색→계획→실행→검증→완료) 명시
- [ ] 하네스 에이전트 목록 역할별 분류 포함
- [ ] 공통 DoD 6항목 전부 충족
```

**증거 수집 실행:**

```bash
# 파일 존재 확인
Glob: .claude/agents/pipeline-orchestrator.md
→ 결과: 1개 반환 → 파일 존재 확인

# YAML frontmatter 위치 확인
Read: .claude/agents/pipeline-orchestrator.md (1번째 줄)
→ 결과: "---" → YAML frontmatter 최상단 위치 확인

# 에이전트 호출 단계 키워드 확인
Grep: "탐색.*계획.*실행.*검증" in .claude/agents/pipeline-orchestrator.md
→ 결과: 2건 매칭 → 단계 명시 확인

# 에이전트 목록 확인
Grep: "에이전트.*목록" in .claude/agents/pipeline-orchestrator.md
→ 결과: 3건 매칭 → 목록 포함 확인
```

**판정 결과 출력:**

```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
🔍 quality-gate — Phase 6-A DoD 판정
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

| 항목 | 증거 | 판정 |
|---|---|---|
| 파일 생성 확인 | Glob: 1개 반환 | ✅ PASS |
| YAML frontmatter 최상단 위치 | 1번째 줄 "---" 확인 | ✅ PASS |
| 에이전트 호출 단계 명시 | Grep: 2건 매칭 | ✅ PASS |
| 에이전트 목록 분류 포함 | Grep: 3건 매칭 | ✅ PASS |
| 공통 DoD 6항목 충족 | Read: 6개 필수 섹션 확인 | ✅ PASS |

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
총평: PASS 5개 / NEEDS WORK 0개 (전체 5개)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

✅ 전체 PASS — 모든 DoD 항목 충족. 다음 Phase로 진행 가능.
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

---

## 추가 컨텍스트: $ARGUMENTS

argument가 비어있으면 `docs/task.md`와 `docs/checklist.md`에서 현재 Phase를 자동 탐지합니다.
argument에 `"Phase 6-B"`처럼 Phase 이름을 지정하면 해당 Phase 체크리스트 섹션을 고정으로 사용합니다.
