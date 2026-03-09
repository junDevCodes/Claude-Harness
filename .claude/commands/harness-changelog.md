---
description: 이번 세션 변경 자산을 분석하여 CHANGELOG.md 항목을 자동 생성합니다
argument-hint: "[version] (선택 — 버전을 명시하면 해당 버전으로 릴리즈)"
---

# /harness-changelog

이번 세션에서 추가하거나 수정한 하네스 자산을 분석하여 `CHANGELOG.md`에 항목을 자동 생성하는 커맨드.

---

## 실행 흐름

### Step 1: CHANGELOG.md 현재 내용 읽기

`CHANGELOG.md`를 읽어 현재 최신 버전을 파악한다.

```
최신 버전: v[X.Y.Z] — YYYY-MM-DD
```

---

### Step 2: 이번 세션 변경 자산 목록 파악

git status 또는 대화 컨텍스트를 바탕으로 이번 세션에서 변경된 자산을 분류한다.

**분류 기준:**

| 카테고리 | 경로 | 유형 |
|---|---|---|
| 신규 스킬 | `.claude/skills/[name]/SKILL.md` 신규 | Added |
| 수정 스킬 | `.claude/skills/[name]/SKILL.md` 수정 | Changed or Fixed |
| 신규 에이전트 | `.claude/agents/[name].md` 신규 | Added |
| 신규 커맨드 | `.claude/commands/[name].md` 신규 | Added |
| 신규 훅 | `.claude/hooks/[name].{ts,sh}` 신규 | Added |
| 신규 베이스 스택 | `base/[stack]/` 신규 | Added |
| 수정 베이스 스택 | `base/[stack]/` 수정 | Changed |
| 역전파 패턴 | `base/[stack]/` + `SKILL.md` 동시 수정 | Changed (backport) |
| 문서 업데이트 | `docs/*.md`, `CLAUDE.md` | Changed |
| 오탐 수정 | `skill-rules.json` | Fixed |

---

### Step 3: 변경 규모 판단 → 버전 번호 결정

아래 기준으로 버전 번호를 결정한다:

```
major (X+1.0.0): 하네스 디렉토리 구조 전면 개편
                 (예: .claude/ 구조 변경, base/ 구조 변경)

minor (X.Y+1.0): 새 자산 추가
                 - 새 스킬 추가
                 - 새 에이전트 추가
                 - 새 커맨드 추가
                 - 새 베이스 스택 추가
                 - 새 훅 추가

patch (X.Y.Z+1): 기존 자산 수정만
                 - 스킬 내용 개선/오탐 수정
                 - 에이전트 내용 개선
                 - 문서 업데이트
                 - 역전파 (기존 스택 패턴 추가)
```

**버전 결정 우선순위:** major > minor > patch
- 동일 세션에 신규 추가 + 수정이 함께 있으면 → minor

---

### Step 4: [Unreleased] 섹션에 항목 추가

`CHANGELOG.md`의 `[Unreleased]` 섹션 아래에 변경 내역을 추가한다.

**항목 형식:**

```markdown
## [Unreleased]

### Added
- Phase X-Y: `파일경로` — 설명 (한 줄 요약)
- Phase X-Y: `파일경로` — 설명

### Changed
- Phase X-Y: `파일경로` — 변경 내용

### Fixed
- Phase X-Y: `skill-rules.json` — 스킬명 오탐 수정 (키워드 → 수정 내용)
```

**작성 원칙:**
- 각 항목은 "파일 경로 — 역할 설명" 형태로 1줄 요약
- Phase 번호 명시 (예: Phase 5-B)
- 관련 파일이 여러 개면 각각 별도 줄

---

### Step 5: 릴리즈 여부 확인

사용자에게 릴리즈 여부를 확인한다:

> "이번 변경사항을 v[결정된 버전]으로 릴리즈하시겠습니까? (Y/n)"

**릴리즈하는 경우:**
1. `[Unreleased]` → `[v{버전}] — {오늘날짜}` 로 변경
2. 새 빈 `[Unreleased]` 섹션을 최상단에 추가
3. CHANGELOG.md 저장

**릴리즈하지 않는 경우:**
- `[Unreleased]` 상태로 유지 (항목만 추가)

---

### Step 6: base_code_plan.md 버전 업데이트

릴리즈한 경우, `base_code_plan.md`에서 현재 버전 표기를 업데이트한다.

```markdown
<!-- base_code_plan.md 상단 또는 해당 섹션 -->
현재 버전: v[새 버전] (YYYY-MM-DD)
```

---

## 사용 예시

```
# 이번 세션에 스킬 2개 추가 후 실행
/harness-changelog

→ 분석 결과: harness-init.md + harness-changelog.md + harness-backport.md 추가됨
→ 버전 결정: minor (새 커맨드 3개) → v1.4.0 → v1.5.0
→ [Unreleased] 섹션에 Added 3개 항목 작성
→ 릴리즈 확인 → Y → v1.5.0 릴리즈
```

```
# 버전을 직접 지정하여 실행
/harness-changelog v2.0.0

→ [Unreleased] 내용을 v2.0.0으로 릴리즈
```

---

## 참고

- CHANGELOG 형식: [Keep a Changelog](https://keepachangelog.com/ko/) 표준 준수
- 역전파 변경사항도 CHANGELOG에 기록할 것 (`/harness-backport` 참조)
- 버전 번호는 `base_code_plan.md`와 항상 동기화 유지
