# Quality Notes — nextjs-frontend-guidelines

> Phase 2-C 품질 점검 결과 (2026-03-05)

## 점검 결과 요약

| 항목 | 상태 | 비고 |
|---|---|---|
| 500줄 이하 | ❌❌ **1073줄** | **573줄 초과 (2배 이상)** — 대폭 축소 필수 |
| YAML frontmatter `triggers` | ❌ 누락 | `name`, `description`만 있음 |
| 핵심 패턴 3개+ | ✅ | Server/Client Components, App Router, shadcn/ui, Auth, Data Fetching 등 다수 |
| 코드 예시 | ✅ | 풍부한 코드 블록 |
| 트리거 키워드 5개+ | ⚠️ | skill-rules.json에 11개 — frontmatter 반영 필요 |
| 안티패턴 섹션 | ⚠️ 미약 | 산발적 언급 3건 — 명시적 섹션 없음 |

## 수정 필요 사항 — 심각도: 높음

### P0 — 반드시 수정 (최우선)

1. **SKILL.md 500줄 이하로 대폭 축소 (필수)**
   - 현재 resources/ 에 10개 상세 파일이 이미 존재:
     - common-patterns.md, complete-examples.md, component-patterns.md
     - data-fetching.md, file-organization.md, loading-and-error-states.md
     - performance.md, routing-guide.md, styling-guide.md, typescript-standards.md
   - SKILL.md에 인라인으로 작성된 상세 내용을 resources/ 참조로 대체
   - Topic Guides 각 섹션을 "간략 설명 + 상세 → resources/[topic].md 참조" 형태로 축소
   - Import Patterns, Common Imports Cheatsheet 간소화
   - 목표: 350~450줄

2. **YAML frontmatter에 `triggers` 필드 추가**
   ```yaml
   triggers:
     - Next.js
     - nextjs
     - React
     - server component
     - client component
     - app router
     - shadcn
     - tailwind
     - use client
     - use server
   ```

### P1 — 권장 수정

3. **안티패턴 섹션 추가**
   - Server Component에서 useState/useEffect 사용 금지
   - Client Component에서 async 사용 금지
   - 불필요한 "use client" 남용
   - barrel export 사용 시 번들 사이즈 이슈

4. **YGS(영영사) 특화 내용 범용화 검토**
   - description에 "YGS (영영사)" 특화 — 범용 하네스 자산으로 적합한지 검토
   - Firebase/Kakao 인증은 선택사항임을 명시

### skill-rules.json 관련 (Phase 2-D 반영)

- 현재 등록 상태: ✅ 등록됨 (guardrail, block 레벨)
- enforcement "block"이 범용 하네스에서 적절한지 검토 필요
  - "suggest"로 하향 권장 (다른 프로젝트에서 강제 차단은 과도)
- blockMessage 내 YGS 특화 내용 범용화 필요
