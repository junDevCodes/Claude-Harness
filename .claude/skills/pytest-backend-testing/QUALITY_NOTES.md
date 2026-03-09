# Quality Notes — pytest-backend-testing

> Phase 2-C 품질 점검 결과 (2026-03-05)

## 점검 결과 요약

| 항목 | 상태 | 비고 |
|---|---|---|
| 500줄 이하 | ❌ **520줄** | 20줄 초과 — 소폭 축소로 해결 가능 |
| YAML frontmatter `triggers` | ❌ 누락 | `name`, `description`만 있음 |
| 핵심 패턴 3개+ | ✅ | Unit/Integration/Async/Mocking/Fixtures/Coverage/FastAPI Testing |
| 코드 예시 | ✅ | AAA 패턴, Mock, Fixture 등 코드 블록 다수 |
| 트리거 키워드 5개+ | ⚠️ | skill-rules.json에 12개 등록 — frontmatter 반영 필요 |
| 안티패턴 섹션 | ❌ 없음 | 테스트 안티패턴 명시 필요 |

## 수정 필요 사항

### P0 — 반드시 수정

1. **SKILL.md 500줄 이하로 축소**
   - Current Project Configuration 섹션 간소화 또는 제거
   - Quick Reference 중복 정리
   - 목표: 480줄 이하

2. **YAML frontmatter에 `triggers` 필드 추가**
   ```yaml
   triggers:
     - pytest
     - unit test
     - integration test
     - test coverage
     - mock
     - fixture
     - TestClient
     - async test
     - conftest
     - pytest-asyncio
   ```

### P1 — 권장 수정

3. **안티패턴 섹션 추가**
   - 테스트 간 상태 공유 금지
   - assert 없는 테스트
   - 과도한 mocking
   - DB 의존 단위 테스트
   - sleep 기반 비동기 테스트

### skill-rules.json 관련 (Phase 2-D 반영)

- 현재 등록 상태: ✅ 등록됨
- 트리거 조정 필요: 없음
