# Task — 현재 작업 상세 계획서

> **현재 Phase:** Phase 9-A — CLAUDE.md 4문서 정의 재정의 + 생명주기 규칙 추가
> **선행 완료:** Phase 8-E (2026-03-11) — 통합 검증 + CHANGELOG v1.6.1 릴리즈 완료 (Phase 8 전체 완료)
> **착수 배경:** Phase 8 완료 후 발견된 구조적 개선 사항 — 4문서 역할 정의 모호성 + 훅 파이프라인 미완성 + 생명주기 미정의

---

## Phase 9-A — CLAUDE.md 4문서 정의 재정의

### 개요

| 항목 | 내용 |
|---|---|
| **목표** | CLAUDE.md 4문서 역할 정의를 명확하게 재정의하고, 생명주기 + 범위 규칙을 추가 |
| **산출물** | `CLAUDE.md` 수정 (4문서 역할 테이블 + 생명주기 섹션 + 범위 규칙 + 완료 절차) |
| **의존성** | 없음 (독립 작업) |

### 수정 내용 상세

---

#### 9-A-1: 4문서 역할 테이블 수정

**파일:** `CLAUDE.md` (Agent 작업 문서 체계 섹션)

**변경 전:**

```
| docs/task.md    | plan.md에서 할당된 작업의 **상세 실행 계획서** |
| docs/history.md | 이전까지 진행된 작업 내용 + 다음 태스크 맥락을 기록하는 **맥락 문서** |
```

**변경 후:**

```
| docs/task.md    | plan.md에서 할당된 **바로 다음 진행할 기능 단위** 작업 상세 실행 계획서.<br>복수 기능 동시 기재 금지. 범위 초과 시 plan.md 항목 분리 후 재작성. |
| docs/history.md | 전체 작업 이력을 누적 기록하는 **영구 맥락 문서**.<br>현재 task.md 작업이 왜 이루어지는지 맥락 파악용. plan/task 완료 시 요약 흡수. |
```

---

#### 9-A-2: 4문서 생명주기 섹션 신규 추가

**삽입 위치:** "작업 완료 시 (필수)" 절차 직후

**추가 내용:**

```markdown
### 4문서 생명주기

| 문서 | 성격 | 완료 시 처리 |
|---|---|---|
| `plan.md` | 스프린트/마일스톤 계획 | 전체 완료([x]) → history.md 요약 기록 → 새 sprint로 교체 |
| `task.md` | 현재 기능 단위 작업 | checklist 전부 [x] → history.md 요약 기록 → 다음 기능으로 교체 |
| `checklist.md` | task.md 검증용 | task.md와 동시 교체 |
| `history.md` | 영구 이력 | 초기화 없음. plan/task 완료 내용 모두 흡수. |

⚠️ task.md 또는 plan.md 교체 전 반드시 history.md에 완료 내용을 기록해야 한다.
```

---

#### 9-A-3: task.md 범위 규칙 추가

**삽입 위치:** 4문서 역할 테이블 직후 (생명주기 섹션 앞)

**추가 내용:**

```markdown
### task.md 범위 기준

- checklist.md 미완료 항목이 15개를 초과하면 기능 범위 과다로 판단한다
- 기능이 복수일 경우 plan.md에서 항목을 먼저 분리한 뒤 task.md를 재작성한다
```

---

#### 9-A-4: 작업 완료 시 절차 업데이트

**현재:**

```
1. docs/checklist.md 최종 검증 항목 체크
2. docs/task.md 완료 항목 표시
3. docs/history.md 업데이트 (완료 내용 + 다음 세션 맥락)
4. docs/plan.md 해당 Phase 항목 체크
```

**변경 후:**

```
1. docs/checklist.md 최종 검증 항목 체크
2. docs/history.md 업데이트 (완료 내용 + 다음 세션 맥락)  ← history 먼저
3. docs/task.md 완료 항목 표시
4. docs/plan.md 해당 Phase 항목 체크
⚠️ checklist 전부 완료 시: task.md는 다음 기능으로 교체, checklist.md도 동시 교체
```

---

### 검증 방법

```bash
# 역할 정의 문구 변경 확인
grep "바로 다음 진행할 기능 단위" CLAUDE.md
grep "영구 맥락 문서" CLAUDE.md

# 생명주기 섹션 존재 확인
grep "4문서 생명주기" CLAUDE.md

# 범위 규칙 존재 확인
grep "15개를 초과" CLAUDE.md

# 완료 절차 순서 변경 확인 (history 먼저)
grep -A 8 "작업 완료 시 (필수)" CLAUDE.md
```

---

## Phase 9-B / 9-C (다음 작업 — 이번 세션 범위 내)

| Phase | 내용 | 파일 |
|---|---|---|
| **9-B** | 훅 파이프라인 완성 | `session-start-docs-loader.ts`, `docs-update-reminder.ts` |
| **9-C** | harness-plan-sync 강화 | `.claude/commands/harness-plan-sync.md` |

---

*Last updated: 2026-03-11 (Phase 9 착수 — 9-A CLAUDE.md 4문서 정의 재정의)*
