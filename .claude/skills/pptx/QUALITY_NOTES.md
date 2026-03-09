# Quality Notes — pptx

> Phase 2-C 품질 점검 결과 (2026-03-05)

## 점검 결과 요약

| 항목 | 상태 | 비고 |
|---|---|---|
| 500줄 이하 | ✅ **489줄** | 경계선 — 추가 콘텐츠 시 초과 위험 |
| YAML frontmatter `triggers` | ❌ 누락 | `name`, `description`, `license`만 있음 |
| 핵심 패턴 3개+ | ✅ | 읽기/분석, 신규 생성(html2pptx), 기존 편집(OOXML), 템플릿 기반 생성 4가지 워크플로우 |
| 코드 예시 | ✅ | Bash, Python, JavaScript, JSON 코드 블록 다수 |
| 트리거 키워드 5개+ | ⚠️ | skill-rules.json에 7개 — frontmatter 반영 필요 |
| 안티패턴 섹션 | ✅ | position:absolute 금지, 세로 스택 금지 등 CRITICAL 규칙 다수 |

## 수정 필요 사항

### P0 — 반드시 수정

1. **YAML frontmatter에 `triggers` 필드 추가**
   ```yaml
   triggers:
     - pptx
     - PowerPoint
     - presentation
     - slides
     - slide deck
     - pptx create
     - pptx edit
   ```

### P2 — 선택 수정

2. **줄수 경계선 관리** — 현재 489줄로 여유 11줄. 향후 콘텐츠 추가 시 resources/ 분리 검토

### skill-rules.json 관련 (Phase 2-D 반영)

- 현재 등록 상태: ✅ 등록됨
- 트리거 조정 필요: 없음
