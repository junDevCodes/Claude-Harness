# Quality Review — django-backend-guidelines

**점검일**: 2026-03-05
**점검 대상**: Phase 2-B 신규 스킬
**점검자**: Phase 2-C 품질 점검 세션

---

## DoD 체크리스트

| # | 기준 | 판정 | 비고 |
|---|------|------|------|
| 1 | SKILL.md ≤ 500줄 | ✅ PASS | 최종 500줄 (DoD 기준 충족) |
| 2 | YAML frontmatter (name/description/triggers) | ✅ PASS | 3필드, description 상세, triggers 12개 |
| 3 | 핵심 패턴 3개+ 코드 예시 | ✅ PASS | 5개 패턴 × 다수 코드 블록 |
| 4 | 트리거 키워드 5개+ | ✅ PASS | 12개 |
| 5 | 안티패턴 섹션 | ✅ PASS | 6개 안티패턴, ❌/✅ 대조 형식 |
| 6 | skill-rules.json 미등록 | ✅ N/A | Phase 2-D 일괄 처리 |

---

## 소스 코드 정확성 (base/django/ 대비)

**전체 평가: 높음** — 5개 패턴 모두 실제 코드와 핵심 구조 일치

### 패턴별 비교

| 패턴 | 정확성 | 차이점 |
|------|--------|--------|
| 1. User 모델 | 높음 | models.py 정확 일치. managers.py `is_superuser` 누락 → **수정 완료** |
| 2. ViewSet+Serializer | 높음 | 간략화 적절. pagination_class 미포함 (P2) |
| 3. SimpleJWT 인증 | 높음 | URL 구조 불일치 → **모듈화 패턴으로 수정 완료** |
| 4. settings 환경분리 | 높음 | 고급 패턴 생략 (DATABASE_URL, WhiteNoise, HSTS) — 간략화로 적절 |
| 5. 예외 핸들러 | 높음 | _error_response 헬퍼 미반영 — 간략화로 적절 |

---

## 수정 완료 항목 (P1)

### 1. create_superuser `is_superuser=True` 추가
- **문제**: PermissionsMixin 기본값 is_superuser=False → Django admin 접근 불가
- **수정**: `extra_fields.setdefault("is_superuser", True)` 추가
- **실제 코드 기반**: `base/django/apps/users/managers.py:20`

### 2. URL 구조 모듈화
- **문제**: config/urls.py에 플랫하게 나열 → DRF 모범사례 위반
- **수정**: apps/users/urls.py 분리 패턴 + config/urls.py include 패턴 제시
- **실제 코드 기반**: `base/django/apps/users/urls.py`, `base/django/config/urls.py`

---

## 미수정 항목 (P2 — 개선 권장, 필수 아님)

1. **pagination_class 패턴**: ViewSet에 `StandardResultsSetPagination` 사용 패턴 추가 고려
2. **development.py DATABASE_URL 우선 파싱**: dj_database_url 활용 패턴
3. **production.py HSTS/WhiteNoise**: 보안 강화 및 정적 파일 서빙 패턴

---

## skill-rules.json 등록 메모 (Phase 2-D용)

```json
{
  "django-backend-guidelines": {
    "type": "domain",
    "enforcement": "suggest",
    "priority": "high",
    "promptTriggers": {
      "keywords": [
        "django", "drf", "django rest framework", "simplejwt",
        "ModelViewSet", "APIView", "serializer", "AbstractBaseUser",
        "makemigrations", "pytest-django", "gunicorn"
      ],
      "intentPatterns": [
        "(create|add|build).*?django",
        "django.*?(view|model|serializer|url|setting)",
        "(setup|configure).*?drf"
      ]
    },
    "fileTriggers": {
      "paths": ["**/apps/**/views.py", "**/apps/**/serializers.py", "**/apps/**/models.py"],
      "contentPatterns": ["from rest_framework", "from django", "AbstractBaseUser", "ModelViewSet"]
    }
  }
}
```

---

## 총평

**PASS (수정 후 합격)** — DoD 기준 전항 충족. 5개 패턴 구성이 Django+DRF 핵심을 잘 커버하며, 안티패턴 섹션의 ❌/✅ 대조 형식이 가독성 우수. P1 이슈 2건 현장 수정 완료. 다른 백엔드 스킬(express, spring-boot, fastapi, nestjs)과 구조적 일관성 유지.
