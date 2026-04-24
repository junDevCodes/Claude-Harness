# Checklist — Phase 15-A DoD

> **현재 Phase:** Phase 15-A 협업 트랙 Skill 일반화 ✅ 완료 (2026-04-24)
> **전체 Phase 15 목표:** v1.10.0 릴리즈 (Skill 2개 + Agent 4개)
> **다음 Phase:** 15-B Foundation 에이전트 (git-workflow-expert / jira-workflow-expert 병렬 2 세션)

---

## Phase 15-Pre — 실 프로젝트 (S14P31A206 repo) ✅ 완료

- [x] `skill-rules.json` 미등록 상태 사용자 확인 — "그냥 만들다 보니 잘못된거" 확인 + skill 생성 규칙 적용
- [x] `jira` 항목 등록 (keywords 11 + intentPatterns 6 + fileTriggers)
- [x] `lib.py` argparse CLI 엔트리 추가 (5 subcommand)
  - [x] `task-create --task-id` + JSON 출력
  - [x] `epic-create [--force]` + JSON 출력
  - [x] `transition --key --to` + JSON 출력
  - [x] `status --key` + JSON 출력
  - [x] `list [--jql]` + JSON 출력
- [x] `python lib.py --help` 동작 확인 (stdout 정상)
- [ ] 별도 커밋 (실 프로젝트 repo) — 사용자 명시 커밋 요청 대기

---

## Phase 15-A — 하네스 Skill 작성 ✅ 완료

### Skill 1: `jira-workflow-guidelines` ✅

- [x] `.claude/skills/jira-workflow-guidelines/SKILL.md` 파일 존재 (330줄)
- [x] YAML frontmatter (`name`, `description`, `triggers`)
- [x] 500줄 이하 (330줄)
- [x] PROJECT_KEY / ASSIGNEE_ACCOUNT_ID / COMPONENT_ID **placeholder** 처리
- [x] 실 프로젝트 `lib.py` 실행 섹션 **제거**
- [x] 컨벤션 검증 규칙 9개 포함 (Epic summary / Task summary / Type / Component / SP / Assignee / Sprint / Branch / Labels)
- [x] 시안 사전 승인 정책 섹션 포함
- [x] 8개 명령 시그니처 정의
- [x] 프로젝트별 커스터마이징 가이드 섹션 (Pattern A: lib.py, Pattern B: config.yaml)
- [x] 트리거 키워드 13개

### Skill 2: `git-workflow-guidelines` ✅

- [x] `.claude/skills/git-workflow-guidelines/SKILL.md` 파일 존재 (390줄)
- [x] YAML frontmatter (`name`, `description`, `triggers`)
- [x] 500줄 이하 (390줄)
- [x] 브랜치 전략 섹션 (GitFlow/Trunk-based/GitHub Flow/Feature Branch 비교 + 의사결정 트리)
- [x] 브랜치 네이밍 규칙 (소문자/하이픈/Jira 키 포함) + 검증 정규식
- [x] Conventional Commits 규칙 (type 13종)
- [x] 리베이스 vs 머지 vs squash 의사결정 매트릭스
- [x] Destructive 작업 확인 게이트 (7가지 작업 + 템플릿)
- [x] Jira 이슈 키 연계 섹션 (smart commit 키워드)
- [x] 트리거 키워드 13개

### Skill 통합 ✅

- [x] `skill-rules.json` version v1.6 → v1.7
- [x] `jira-workflow-guidelines` 항목 등록
- [x] `git-workflow-guidelines` 항목 등록
- [x] 키워드 중복 검증 (기존 30 skills와 충돌 없음 — "git hooks" vs "claude hook system" 구분 확인)
- [x] `.claude/skills/README.md` 30→32 갱신
- [x] 🤝 협업 카테고리 섹션 추가
- [x] JSON 문법 검증 (python json.load 통과)

---

## Phase 15-A 완료 게이트 ✅

- [x] `docs/plan.md` Phase 15-A 항목 체크
- [x] `docs/task.md` 완료 항목 체크
- [x] `docs/history.md` Phase 15-A 요약 추가 (다음 단계에서 반영)
- [x] 모든 체크박스 완료 후 Phase 15-B 착수 가능

---

## Phase 15-B 착수 조건 (다음 세션)

- [x] `jira-workflow-guidelines` SKILL 완료 → `jira-workflow-expert` 에이전트 참조 가능
- [x] `git-workflow-guidelines` SKILL 완료 → `git-workflow-expert` 에이전트 참조 가능
- [x] CLI 계약 정의 (lib.py 5 subcommand) → `jira-workflow-expert` 4-tier fallback 1순위
- [ ] Phase 15-B 병렬 2 세션 개시 (B1: git-workflow-expert, B2: jira-workflow-expert)

---

*Phase 15-A 완료 — 2026-04-24*
