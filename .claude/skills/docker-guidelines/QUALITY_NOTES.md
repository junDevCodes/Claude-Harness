# docker-guidelines — Quality Notes

## QA 결과: PASS (A등급)

**검증일:** 2026-03-08
**검증 방법:** SKILL.md ↔ `base/*/Dockerfile`, `base/*/docker-compose.yaml` 교차 검증 (1cycle)

---

## DoD 체크리스트

- [x] SKILL.md 존재 (456줄 — 500줄 이하)
- [x] YAML frontmatter (name, description, triggers 15개)
- [x] 핵심 패턴 5개 + 코드 예시
  - Pattern 1: Multi-Stage Build (Python/Node.js/Java)
  - Pattern 2: Non-Root User Execution
  - Pattern 3: Layer Cache Optimization
  - Pattern 4: docker-compose Service Composition
  - Pattern 5: .dockerignore
- [x] 트리거 키워드 15개 (기준 5개 이상 충족)
- [x] 안티패턴 섹션 (6개)
- [x] skill-rules.json 등록 — Phase 2-D 처리 예정

---

## 교차 검증 결과

| 스킬 패턴 | base/ 파일 | 일치 여부 |
|---|---|---|
| Node.js 2단계 멀티 스테이지 (Pattern 1) | `base/nestjs/Dockerfile` | ✅ 정확히 일치 (corepack pnpm, builder→runner, .prisma 복사) |
| non-root appuser (Pattern 2) | `base/nestjs/Dockerfile` L38-39 | ✅ `addgroup -S appgroup && adduser -S appuser` 확인 |
| 의존성 파일 먼저 복사 (Pattern 3) | `base/nestjs/Dockerfile` L10-11 | ✅ package.json + pnpm-lock.yaml 먼저 COPY |
| postgres health check + depends_on (Pattern 4) | `base/*/docker-compose.yaml` | ✅ pattern 설명과 일치 |
| .dockerignore 패턴 (Pattern 5) | 크로스 스택 공통 패턴 | ✅ Python/Node.js/Java 섹션 분리 정확 |

**확인된 base/ Dockerfile 목록 (5개):**
- `base/fastapi/Dockerfile` — uv 단일 스테이지 (스킬에서 예외 명시 ✅)
- `base/django/Dockerfile` — Python 2단계 멀티 스테이지
- `base/express/Dockerfile` — Node.js 3단계 (스킬에서 언급 ✅)
- `base/nestjs/Dockerfile` — Node.js 2단계 + corepack pnpm
- `base/spring-boot/Dockerfile` — Java 2단계 (JDK→JRE)

---

## P1 이슈

없음.

---

## 비고

- 스킬에서 `base/fastapi/` non-root 미적용 예외를 명시적으로 기술 (`참고:` 주석) — 정직한 문서화 ✅
- Node.js 내장 `node` 사용자 옵션도 표에 명시 (base/express/ 패턴)
- BuildKit 캐시 마운트 패턴 포함 (고급 최적화) — 보너스 컨텐츠
- `관련 스킬` 섹션으로 aws-guidelines, 스택별 스킬 연결 — 스킬 간 네비게이션 우수
