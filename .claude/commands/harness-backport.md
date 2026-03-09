---
description: 실 프로젝트에서 발견한 유용한 패턴을 하네스 base/[stack]/와 관련 Skill에 역전파합니다
argument-hint: "[패턴 설명] (예: FastAPI JWT 슬라이딩 윈도우 갱신 패턴)"
---

# /harness-backport

실 프로젝트 개발 중 발견한 유용한 패턴을 하네스 `base/[stack]/` 및 관련 `Skill`에 체계적으로 반영하는 역전파 워크플로우 커맨드.

---

## 역전파 O/X 기준

역전파 여부를 판단하기 전에 아래 기준을 먼저 적용한다.

### 역전파 O (권장)

```
✅ 재사용 가능한 공통 패턴
   예: JWT 슬라이딩 윈도우 갱신 로직, 에러 핸들링 패턴, 페이지네이션 헬퍼

✅ 새로운 아키텍처 패턴
   예: 새 레이어(감사 로그), 새 추상화(멀티 스토리지), 이벤트 기반 디커플링

✅ 성능 최적화 기법
   예: N+1 해결 쿼리 패턴, Redis 캐싱 전략, 커넥션 풀 최적화

✅ 보안 강화 패턴
   예: 새 취약점 대응 (OWASP 업데이트), Rate Limiting 패턴, SSRF 방어

✅ 라이브러리 업그레이드 대응 패턴
   예: 신버전 Breaking Change 대응 코드, 마이그레이션 가이드
```

### 역전파 X (제외)

```
❌ 비즈니스 로직
   예: 특정 서비스의 주문 처리 로직, 도메인별 검증 규칙

❌ 환경 특화 설정
   예: 특정 회사 서버/DB 접속 정보, AWS 계정 특화 리소스 설정

❌ 개인정보/인증정보
   예: API 키, 비밀번호, 개인 식별 정보

❌ 미검증 실험적 코드
   예: 실 운영에서 1회만 사용, 성능 미측정, 사이드 이펙트 미파악

❌ 프로젝트 특화 도메인명
   예: Artist, Order, Concert 같은 특정 서비스 도메인 (범용화 불가 시)
```

---

## 실행 흐름

### Step 1: 역전파 패턴 파악

`$ARGUMENTS`에서 역전파할 패턴 설명을 읽는다.

인수가 없으면 아래를 질문한다:
> "어떤 패턴을 역전파하시겠습니까? 간단히 설명해 주세요."

---

### Step 2: 역전파 대상 판별

**역전파 O/X 기준 체크:**
1. 위의 기준표로 역전파 가능 여부 판별
2. 역전파 X 해당 시 → 이유 설명 후 중단

**해당 스택 확인:**
- 어떤 `base/[stack]/` 에 반영해야 하는가
- 복수 스택에 적용 가능한지 확인 (예: JWT 패턴은 웹 백엔드 전체)

**예시:**
```
패턴: "FastAPI에서 access token을 DB 조회 없이 검증하면서 revoke를 지원하는 패턴"
→ 해당 스택: base/fastapi/
→ 범용화 여부: 다른 스택에도 개념 적용 가능 (nestjs, express, django, spring-boot)
→ 역전파 O: 재사용 가능한 보안 강화 패턴
```

---

### Step 3: 역전파 범위 확정

아래 두 가지를 확정한다:

**① base/[stack]/ 파일 수정 범위:**

```
수정 대상 파일 목록 (예시):
- base/fastapi/domain/auth/service.py     ← 핵심 로직 반영
- base/fastapi/domain/auth/repository.py  ← 저장소 패턴 변경 시
- base/fastapi/utils/jwt.py               ← 유틸리티 패턴
- base/fastapi/README.md                  ← 변경 내용 반영
```

**② Skill 문서화 범위:**

```
패턴을 문서화할 Skill:
- .claude/skills/fastapi-backend-guidelines/SKILL.md
  → "JWT Revocation 패턴" 섹션 추가
```

변경 규모가 크면 `resources/` 파일로 분리 검토.

---

### Step 4: 역전파 실행 지시

아래 단계를 순서대로 안내한다.

**Step 4-1: base/[stack]/ 파일 수정**

수정 원칙:
- 비즈니스 도메인명 → 범용명으로 변환 (Artist → User, Concert → Item)
- 환경 특화 설정 제거 (하드코딩 URL, 특정 계정 정보)
- 실행 가능한 상태 유지 (import 경로, 의존성 맞춤)
- 주석으로 패턴의 목적 명시

```python
# 역전파 예시 — FastAPI JWT Revocation 패턴
# 출처: [실 프로젝트명] 2026-03-09 역전파
# 목적: access token revoke 지원 (DB jti 블랙리스트)
```

**Step 4-2: Skill SKILL.md 패턴 섹션 추가**

해당 Skill 문서 말미에 새 패턴 섹션을 추가한다:

```markdown
## [패턴명] 패턴 (역전파 — YYYY-MM-DD)

### 언제 사용하는가
[적용 상황 설명]

### 구현

\`\`\`[언어]
[범용화된 코드 예시]
\`\`\`

### 주의사항
- [안티패턴 또는 주의점]
```

**Step 4-3: CHANGELOG.md 역전파 항목 추가**

`CHANGELOG.md`의 `[Unreleased]` 섹션에 추가:

```markdown
### Changed
- Backport: `base/[stack]/[파일]` — [패턴명] 역전파 ([실 프로젝트]에서 검증)
- Backport: `.claude/skills/[skill]/SKILL.md` — [패턴명] 섹션 추가
```

**Step 4-4: base_code_plan.md 역전파 이력 갱신**

`## 역전파 프로세스` 섹션 또는 별도 이력 섹션에 추가:

```markdown
### YYYY-MM-DD — [패턴명] 역전파

- **출처:** [실 프로젝트명]
- **적용:** base/[stack]/[파일]
- **Skill:** .claude/skills/[skill]/SKILL.md
```

---

### Step 5: 완료 확인

**수정된 파일 목록 출력:**

```
역전파 완료 — 수정된 파일:
  ✓ base/[stack]/[파일1]
  ✓ base/[stack]/[파일2]
  ✓ .claude/skills/[skill]/SKILL.md
  ✓ CHANGELOG.md (Unreleased 항목 추가)
  ✓ base_code_plan.md (역전파 이력)
```

**skill-rules.json 수정 필요 여부 확인:**

새로운 트리거 키워드가 필요한 패턴이 추가된 경우:
> "skill-rules.json의 [skill] 트리거 키워드에 '[키워드]' 추가가 필요합니까? (Y/n)"

---

## 사용 예시

```
# FastAPI JWT Revocation 패턴 역전파
/harness-backport FastAPI access token jti 블랙리스트 패턴 역전파

# NestJS Redis 캐싱 전략 역전파
/harness-backport NestJS @Cacheable 데코레이터 + Redis 캐싱 전략

# 다중 스택 공통 에러 핸들링 역전파
/harness-backport 웹훅 이벤트 멱등성 보장 패턴 (Express + FastAPI 공통)
```

---

## 참고

- 역전파 전 실 프로젝트에서 3회 이상 사용 또는 성능/보안 효과 측정 권장
- 역전파 후 `/harness-changelog`로 버전 이력 반드시 기록
- 베이스 코드 전체 구조 변경 시 → Phase 승격 후 진행 (`docs/plan.md` 업데이트)
