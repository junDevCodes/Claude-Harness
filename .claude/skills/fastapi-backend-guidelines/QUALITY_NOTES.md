# Quality Notes — fastapi-backend-guidelines

> Phase 2-C 품질 점검 결과 (2026-03-05)

## 점검 결과 요약

| 항목 | 상태 | 비고 |
|---|---|---|
| 500줄 이하 | ❌ **656줄** | 156줄 초과 — resources/ 활용 중이나 SKILL.md 자체가 초과 |
| YAML frontmatter `triggers` | ❌ 누락 | `name`, `description`만 있음 — `triggers` 필드 추가 필요 |
| 핵심 패턴 3개+ | ✅ | DDD, Repository, Service, Router, DTO, Error Handling 등 다수 |
| 코드 예시 | ✅ | 각 섹션에 코드 블록 포함 |
| 트리거 키워드 5개+ | ⚠️ | skill-rules.json에 9개 등록됨 — frontmatter에도 반영 필요 |
| 안티패턴 섹션 | ⚠️ 미약 | 명시적 "Anti-patterns" 섹션 없음 — 산발적 언급 4건만 |

## 수정 필요 사항

### P0 — 반드시 수정

1. **SKILL.md 500줄 이하로 축소**
   - Quick Reference, Common Imports 등을 더 간결하게 정리
   - Topic Guides 섹션의 인라인 설명을 resources/ 참조로 대체
   - 목표: 400~480줄

2. **YAML frontmatter에 `triggers` 필드 추가**
   ```yaml
   triggers:
     - fastapi
     - FastAPI
     - SQLModel
     - SQLAlchemy
     - pydantic
     - uvicorn
     - alembic
     - async python backend
     - fastapi router
     - fastapi service
   ```

### P1 — 권장 수정

3. **안티패턴 섹션 명시적 추가**
   - "## Anti-patterns" 섹션으로 분리
   - 동기 함수에서 await 누락, 세션 관리 실수, 순환 임포트 등

### skill-rules.json 관련 (Phase 2-D 반영)

- 현재 등록 상태: ✅ 등록됨
- 트리거 조정 필요: 없음 — 현재 트리거 적절
