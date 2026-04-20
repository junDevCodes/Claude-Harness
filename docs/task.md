# Task — 현재 작업 상세 계획서

> **현재 Phase:** Phase 14 완료 + 정합성 패치 2회 (v1.9.2, 2026-04-20) — 다음 작업 대기
> **출처:** docs/plan.md
> **선행 완료:** Phase 14 Opus 4.7 자산 최적화 (2026-04-20)
> **유보:** Phase 14-B2 (500줄 포화 스킬 resources/ 분리 4개)

---

## 다음 작업 후보

### A. Phase 14-B2 — Skills resources/ 분리 (유보 항목)

| 스킬 | 분리 대상 섹션 |
|---|---|
| `react-native-guidelines` | 네비게이션/인증/상태관리 → `resources/navigation.md`, `resources/auth.md` |
| `pptx` | 레이아웃/테이블/차트 → `resources/examples.md` |
| `express-backend-guidelines` | Prisma/JWT → `resources/prisma.md`, `resources/jwt.md` |
| `django-backend-guidelines` | ViewSet/DRF → `resources/drf.md` |

### B. 실 프로젝트 적용 2차 사이클 (Phase 12-B)

- `/harness-apply` 또는 `/harness-init` 신규 프로젝트 적용
- Opus 4.7 model tier 실적 측정 (opus 11 / sonnet 17 / haiku 5 호출 비율)
- 오탐/미탐 실측 → `/harness-backport` 역전파

### C. Phase 11 백로그 처리

| 그룹 | 건수 | 우선순위 |
|---|---|---|
| 11-B 훅/CI/CD 신뢰성 | 6 | P1 |
| 11-C 에이전트/스킬 참조 수정 | 9 | P1 |
| 11-D 문서 drift | 5 | P1 |
| 11-E 코드 품질 | 10 | P2 |

---

## 착수 조건

- 사용자가 다음 작업 방향 지정 시 task.md 교체
- 또는 `/harness-plan-sync` 실행으로 다음 Phase 자동 탐색

---

*Last updated: 2026-04-20 (Phase 14 완료 — 다음 작업 대기)*
