# Quality Notes — error-tracking

> Phase 2-C 품질 점검 + P2 수정 완료 (2026-03-05)

## 점검 결과 요약

| 항목 | 상태 | 비고 |
|---|---|---|
| 500줄 이하 | **404줄** | 383줄 → 404줄 (안티패턴 추가) |
| YAML frontmatter `triggers` | OK | 7개 트리거 |
| 핵심 패턴 3개+ | OK | Controller/Route/Workflow/CronJob/DB/Span 6개 |
| 코드 예시 | OK | 각 패턴 TypeScript 코드 블록 포함 |
| 트리거 키워드 5개+ | OK | 7개 |
| 안티패턴 섹션 | OK | 6개 항목, 각 BAD/GOOD 코드 포함 |

## 수정 이력 (2026-03-05)

### P2 수정 완료

1. **deprecated API 교체**: `Sentry.startTransaction()` → `Sentry.startSpan()` (v8)
2. **deprecated Handlers 교체**: `Sentry.Handlers.requestHandler()/tracingHandler()` → `Sentry.setupExpressErrorHandler()`
3. **프로젝트 특정 콘텐츠 제거/일반화**:
   - "Current Status" 섹션 삭제 (Form Service/Email Service 진행률)
   - "189 ErrorLogger.log() calls remaining" 삭제
   - `/blog-api/`, `/notifications/` 경로 → `src/` 범용 경로
   - "Form Service" / "Email Service" → 범용 "Integration Examples"
4. **"Common Mistakes to Avoid" → `## Anti-Patterns`** 리네이밍 + BAD/GOOD 코드 6개
5. **제목 범용화**: "your project Sentry Integration Skill" → "Sentry v8 Error Tracking & Performance Monitoring"
6. **테스트 엔드포인트 범용화**: 프로젝트 특정 curl → `localhost:3000/api/sentry/...`
7. **Key Files 범용화**: 프로젝트별 분리 → 범용 구조 권장
8. **"Related Skills"/"Documentation" 섹션 제거**: 미존재 스킬 참조 정리

### 잔여 이슈

없음. DoD 전체 충족.

### skill-rules.json 관련 (Phase 2-D 반영)

- 현재 등록 상태: 등록됨
- 트리거 조정 필요: 없음
