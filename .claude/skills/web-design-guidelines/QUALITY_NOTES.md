# Quality Notes — web-design-guidelines

> Phase 2-C 품질 점검 결과 (2026-03-05)

## 점검 결과 요약

| 항목 | 상태 | 비고 |
|---|---|---|
| 500줄 이하 | ✅ **39줄** | |
| YAML frontmatter `triggers` | ❌ 누락 | `name`, `description`, `metadata`만 있음 |
| 핵심 패턴 3개+ | ❌ **0개** | 패턴/코드 예시 전무 — 외부 URL fetch에 완전 의존 |
| 코드 예시 | ❌ 없음 | |
| 트리거 키워드 5개+ | ⚠️ | skill-rules.json에 8개 — frontmatter 반영 필요 |
| 안티패턴 섹션 | ❌ 없음 | |

## 심각도: 높음

이 스킬은 **외부 URL에서 가이드라인을 fetch하는 위임형 구조**. 자체 내용이 거의 없음.
- `https://raw.githubusercontent.com/vercel-labs/web-interface-guidelines/main/command.md` 의존
- 네트워크 불가 환경에서는 작동 불가

## 수정 필요 사항

### P0 — 반드시 수정

1. **YAML frontmatter에 `triggers` 필드 추가**
   ```yaml
   triggers:
     - accessibility
     - a11y
     - UX review
     - UI review
     - design audit
     - WCAG
     - aria
     - web interface guidelines
   ```

### P1 — 권장 수정

2. **핵심 패턴 최소 3개 + 코드 예시 인라인 추가**
   - 현재 외부 URL 100% 의존 → 오프라인/네트워크 오류 시 무용
   - 핵심 접근성 규칙(WCAG 2.1 AA), 키보드 네비게이션, 색상 대비 등
   - 인라인 패턴 추가 후에도 외부 fetch는 보조적으로 유지 가능

3. **안티패턴 섹션 추가**
   - div 남용 (시맨틱 태그 대신)
   - aria 속성 오용
   - 색상 대비 미충족
   - 키보드 접근 불가 인터랙션

### skill-rules.json 관련 (Phase 2-D 반영)

- 현재 등록 상태: ✅ 등록됨
- priority "medium" 유지 적절
