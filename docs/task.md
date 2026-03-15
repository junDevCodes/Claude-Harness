# Task — 현재 작업 상세 계획서

> **현재 Phase:** Phase 12-B — 실 프로젝트 적용
> **출처:** docs/plan.md Phase 12-B
> **선행 완료:** Phase 12-A (2026-03-15) — 병렬 세션 분석 기능 + v1.7.0 릴리즈
> **방침:** 실 프로젝트에 하네스를 적용하고, 실사용 피드백을 수집한다.

---

## Phase 12-B — 실 프로젝트 적용

### B-1: 대상 프로젝트 선정

| 확인 항목 | 내용 |
|---|---|
| 기술 스택 | base/ 9개 중 해당 스택 확인 |
| `.claude/` 존재 여부 | 신규(harness-init) vs 기존(harness-apply) 판별 |
| 프로젝트 규모 | 적용 범위 결정 |

### B-2: 하네스 적용 실행

| 방법 | 명령 | 상황 |
|---|---|---|
| CLI 스크립트 | `bash scripts/harness-init.sh` | 새 프로젝트 |
| Claude 커맨드 | `/harness-apply` | 기존 프로젝트, 선택적 병합 |

**적용 대상 자산:**
- `.claude/settings.json` — 훅 등록
- `.claude/skills/` — 해당 스택 스킬 + 공통 스킬
- `.claude/agents/` — 필요 에이전트 선택
- `skill-rules.json` — 트리거 규칙
- `docs/` — 4문서 체계 초기화

### B-3: 적용 후 검증

1. **settings.json 훅 등록** — session-start-docs-loader, tsc-check, docs-update-reminder
2. **skill-rules.json 트리거** — 해당 스택 키워드로 프롬프트 입력 시 스킬 활성화 확인
3. **4문서 자동화** — 세션 시작 시 docs 읽기 지시 출력 확인
4. **에이전트 실행** — planner, test-coverage-agent, security-auditor 중 3개 테스트
5. **병렬 세션 전략** — `/harness-plan-sync` 실행 시 세션 전략 섹션 자동 생성 확인

### B-4: 이슈 수집

- 발견 이슈 → 해당 프로젝트 `docs/history.md`에 기록
- Phase 11 백로그 중 실 사용에서 문제된 항목 식별
- 범용 패턴 발견 시 → Phase 12-C에서 `/harness-backport` 실행

---

## 완료 후 처리

1. `docs/checklist.md` 항목 체크
2. `docs/history.md` 적용 결과 + 발견 이슈 기록
3. `docs/plan.md` Phase 12-B 완료 처리
4. Phase 12-C (역전파) 착수 여부 결정

---

*Last updated: 2026-03-15 (Phase 12-B 실 프로젝트 적용 계획)*
