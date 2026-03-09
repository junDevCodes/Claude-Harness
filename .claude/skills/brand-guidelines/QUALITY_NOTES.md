# Quality Notes — brand-guidelines

> Phase 2-C 품질 점검 결과 (2026-03-05)

## 점검 결과 요약

| 항목 | 상태 | 비고 |
|---|---|---|
| 500줄 이하 | ✅ **73줄** | |
| YAML frontmatter `triggers` | ❌ 누락 | `name`, `description`, `license`만 있음 |
| 핵심 패턴 3개+ | ⚠️ | 색상 팔레트, 타이포그래피, 폰트 적용 3개 — 코드 예시 부족 |
| 코드 예시 | ⚠️ | 코드 블록 없음 — CSS/HTML 적용 예시 필요 |
| 트리거 키워드 5개+ | ⚠️ | skill-rules.json에 5개 — frontmatter 반영 필요 |
| 안티패턴 섹션 | ❌ 없음 | |

## 수정 필요 사항

### P0 — 반드시 수정

1. **YAML frontmatter에 `triggers` 필드 추가**
   ```yaml
   triggers:
     - Anthropic brand
     - brand color
     - brand guidelines
     - brand style
     - Anthropic design
     - corporate identity
     - brand typography
   ```

### P1 — 권장 수정

2. **CSS/HTML 코드 예시 추가**
   - CSS 변수로 브랜드 색상 정의하는 코드
   - Poppins/Lora 폰트 로드 및 적용 HTML/CSS 예시
   - 다크/라이트 배경별 색상 조합 예시

3. **안티패턴 섹션 추가**
   - 브랜드 색상 임의 변형 금지
   - 로고 색상/비율 변경 금지
   - 비브랜드 폰트 사용 시 주의사항

### 내용 특화 이슈

- `python-pptx` 전용 내용이 "Technical Details"에 포함 — 범용성 위해 CSS/HTML 예시도 추가 필요
- Anthropic 브랜드 전용 — 범용 하네스 자산으로서 다른 브랜드에도 적용 가능한 구조 검토

### skill-rules.json 관련 (Phase 2-D 반영)

- 현재 등록 상태: ✅ 등록됨
- "brand" 단독 키워드 없음 — false positive 방지에 적절
