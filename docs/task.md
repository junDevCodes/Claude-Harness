# Task — Phase 15-A 하네스 Skill 일반화 + 신규

> **현재 Phase:** Phase 15-A 진행 중 (2026-04-24 착수)
> **출처:** docs/plan.md Phase 15
> **선행:** Phase 15-Pre(실 프로젝트 S14P31A206 repo 2건) 병행 진행
> **후속:** Phase 15-B (Foundation 에이전트 2개 병렬 세션)

---

## 작업 범위

실 프로젝트 `jira` skill을 일반화하여 하네스에 역전파 + `git-workflow-guidelines` 신규 작성.
에이전트 4개(git/jira/pr/release)의 참조 SSOT를 확보하는 단계.

---

## 세부 Task

### T-1. 실 프로젝트 Phase 15-Pre (S14P31A206 repo, 별도 커밋)

| # | 작업 | 파일 | 비고 |
|---|---|---|---|
| 1-1 | skill-rules.json 미등록 상태 확인 게이트 | `.claude/skills/skill-rules.json` | 사용자 확인 필수 (의도적 미등록 여부) |
| 1-2 | `jira` 항목 등록 (triggers + keywords + intentPatterns) | 같음 | 1-1 승인 후 |
| 1-3 | `lib.py`에 argparse CLI 엔트리 추가 | `.claude/skills/jira/lib.py` | 5 subcommand + JSON 출력 |
| 1-4 | 트리거 실측 1~2회 | — | 동작 키워드 메모 |

### T-2. 하네스 Skill 작성 (하네스 repo)

| # | 작업 | 파일 | 500줄 룰 |
|---|---|---|---|
| 2-1 | `jira-workflow-guidelines` 일반화 작성 | `.claude/skills/jira-workflow-guidelines/SKILL.md` | 엄격 |
| 2-2 | `git-workflow-guidelines` 신규 작성 | `.claude/skills/git-workflow-guidelines/SKILL.md` | 엄격 |
| 2-3 | `skill-rules.json` v1.6 → v1.7 | `.claude/skills/skill-rules.json` | 2 항목 추가 |
| 2-4 | `skills/README.md` 30 → 32 | `.claude/skills/README.md` | 협업 카테고리 |

### T-3. 4문서 정합성 (세션 종료 전)

| # | 작업 | 파일 |
|---|---|---|
| 3-1 | `docs/history.md` Phase 15-A 요약 기록 | history.md |
| 3-2 | `docs/checklist.md` Phase 15-A 항목 체크 | checklist.md |
| 3-3 | `docs/plan.md` Phase 15-A 체크 | plan.md |

---

## 세션 전략

- **단일 세션 완료 가능** — 예상 2.5시간
- **병렬 X** — 15-A는 순차 (skill 2개 내용 상호 참조)
- **차단 요소:** 사용자의 실 프로젝트 수정 확인 (1-1)

---

## 착수 전제

- Phase 14 완료 + v1.9.2 안정 상태 ✅
- 실 프로젝트 `jira` skill 존재 확인 ✅
- `git-workflow-guidelines` 부재 확인 ✅

---

*Last updated: 2026-04-24 (Phase 15-A 착수)*
