---
description: 현재 프로젝트 또는 새 디렉토리에 Advanced Harness 자산을 초기화합니다
argument-hint: "[stack-name] (fastapi-nextjs|nestjs-nextjs|django-nextjs|express-nextjs|spring-nextjs|fastapi|nextjs|react-native|c-embedded|cpp-embedded|skills-only)"
---

# /harness-init

Advanced Harness 자산을 새 프로젝트에 초기화하는 커맨드.
`harness-init.sh`의 Claude 세션 내 대화형 버전.

---

## 실행 흐름

### Step 1: 스택 확인

`$ARGUMENTS`에서 스택 이름을 읽는다. 없으면 아래 목록을 제시하고 선택을 요청한다.

**스택 목록:**

| 번호 | 스택 | 설명 |
|---|---|---|
| 1 | `fastapi-nextjs` | FastAPI + Next.js (Python 백엔드 + React 프론트엔드) |
| 2 | `nestjs-nextjs` | NestJS + Next.js (TypeScript 풀스택) |
| 3 | `django-nextjs` | Django + Next.js (Python DRF + React 프론트엔드) |
| 4 | `express-nextjs` | Express + Next.js (Node.js 백엔드 + React 프론트엔드) |
| 5 | `spring-nextjs` | Spring Boot + Next.js (Java 백엔드 + React 프론트엔드) |
| 6 | `fastapi` | FastAPI only (Python 백엔드만) |
| 7 | `nextjs` | Next.js only (React 프론트엔드만) |
| 8 | `react-native` | React Native (Expo 모바일 앱) |
| 9 | `c-embedded` | C Embedded (FreeRTOS/HAL 펌웨어) |
| 10 | `cpp-embedded` | C++ Embedded (RAII/HAL 펌웨어) |
| 0 | `skills-only` | 스킬/에이전트만 (.claude/ 복사, 베이스 코드 제외) |

---

### Step 2: 현재 디렉토리 구조 탐지

아래를 확인한다:

```
- .claude/ 디렉토리 존재 여부  → 기존 프로젝트 판별
- CLAUDE.md 존재 여부          → 초기화 이력 확인
- docs/ 디렉토리 여부          → 4문서 체계 여부
- package.json / pyproject.toml 등 → 기존 스택 탐지
```

**판별 기준:**
- `.claude/` 존재 → "기존 프로젝트" → `/harness-apply` 사용을 권장하고 중단
- `.claude/` 없음 → "신규 프로젝트" → 초기화 진행

---

### Step 3: 하네스 경로 확인

하네스 루트 디렉토리를 확인한다. 일반적으로 아래 경로 중 하나:
- `~/harness/`
- `~/workspace/advanced-harness-window/`
- 사용자가 지정한 경로

하네스 경로를 알 수 없으면 사용자에게 질문한다:
> "Advanced Harness 디렉토리 경로를 알려주세요 (예: ~/harness)"

---

### Step 4: 복사 명령어 출력

선택한 스택과 대상 디렉토리를 기반으로 아래 명령어를 순서대로 출력한다.

```bash
# 1. .claude/ 자산 복사
cp -r [HARNESS_DIR]/.claude ./

# 2. 훅 실행 권한
chmod +x .claude/hooks/*.sh

# 3. 세션 캐시 초기화
mkdir -p .claude/.session-cache
touch .claude/.session-cache/.gitkeep

# 4. 훅 의존성 설치 (tsx 필요)
cd .claude/hooks && npm install && cd ../..

# 5. 베이스 코드 복사 (스택에 따라 다름)
# [스택별 명령어]

# 6. docs/ 4문서 stub 생성
mkdir -p docs
# [4개 파일 생성 명령어]
```

**스택별 베이스 코드 복사 명령어:**

- `fastapi-nextjs`: `cp -r [HARNESS_DIR]/base/fastapi ./backend && cp -r [HARNESS_DIR]/base/nextjs ./frontend`
- `nestjs-nextjs`: `cp -r [HARNESS_DIR]/base/nestjs ./backend && cp -r [HARNESS_DIR]/base/nextjs ./frontend`
- `django-nextjs`: `cp -r [HARNESS_DIR]/base/django ./backend && cp -r [HARNESS_DIR]/base/nextjs ./frontend`
- `express-nextjs`: `cp -r [HARNESS_DIR]/base/express ./backend && cp -r [HARNESS_DIR]/base/nextjs ./frontend`
- `spring-nextjs`: `cp -r [HARNESS_DIR]/base/spring-boot ./backend && cp -r [HARNESS_DIR]/base/nextjs ./frontend`
- `fastapi`: `cp -r [HARNESS_DIR]/base/fastapi ./backend`
- `nextjs`: `cp -r [HARNESS_DIR]/base/nextjs ./frontend`
- `react-native`: `cp -r [HARNESS_DIR]/base/react-native ./mobile`
- `c-embedded`: `cp -r [HARNESS_DIR]/base/c-embedded ./firmware`
- `cpp-embedded`: `cp -r [HARNESS_DIR]/base/cpp-embedded ./firmware`
- `skills-only`: (베이스 코드 복사 없음)

---

### Step 5: pathPattern 권장값 제시

선택한 스택에 맞는 pathPattern을 계산하여 제시한다.

`.claude/settings.json`의 `skills.pathPattern` 설정:

| 스택 | 권장 pathPattern |
|---|---|
| 풀스택 (1~5) | `"(backend\|frontend\|src\|app)/"` |
| FastAPI only | `"(backend\|src)/"` |
| Next.js only | `"(frontend\|src\|app)/"` |
| React Native | `"(src\|app\|components\|screens)/"` |
| C/C++ Embedded | `"(src\|hal\|drivers\|tests\|firmware)/"` |
| Skills only | `"(src\|app)/"` |

설정 방법을 안내한다:
```json
// .claude/settings.json
{
  "skills": {
    "pathPattern": "(backend|frontend|src|app)/"
  }
}
```

---

### Step 6: CLAUDE.md 초기화 지시

`CLAUDE.md`를 생성하거나 업데이트하도록 지시한다.

최소 포함 항목:
- 프로젝트 개요 (스택, 목적)
- 디렉토리 구조
- 실행 방법
- Agent 작업 문서 체계 (4문서 섹션)

---

### Step 7: 완료 체크리스트 출력

아래 체크리스트를 출력하여 사용자가 확인하도록 한다:

```
초기화 완료 체크리스트:
  [ ] .claude/ 디렉토리 존재 확인
  [ ] .claude/hooks/*.sh 실행 권한 확인 (ls -la .claude/hooks/)
  [ ] .claude/settings.json pathPattern 설정 완료
  [ ] docs/ 4문서 stub 존재 확인 (docs/plan.md 등)
  [ ] CLAUDE.md 프로젝트 정보 기입 완료
  [ ] Claude Code 세션 재시작 (claude .)
  [ ] /harness-apply 실행하여 설정 최종 검증
```

---

## 참고

- 기존 프로젝트에 적용하려면 `/harness-apply`를 사용하세요.
- 터미널 스크립트 방식: `bash scripts/harness-init.sh --help`
- 전체 가이드: `HARNESS_USAGE_GUIDE.md`
