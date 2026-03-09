# nestjs-backend-guidelines — Quality Notes

## QA 결과: PASS (A등급)

**검증일:** 2026-03-08
**검증 방법:** SKILL.md ↔ `base/nestjs/src/` 코드 교차 검증 (1cycle)

---

## DoD 체크리스트

- [x] SKILL.md 존재 (486줄 — 500줄 이하)
- [x] YAML frontmatter (name, description, triggers 16개)
- [x] 핵심 패턴 5개 + 코드 예시
  - Pattern 1: Guard + Passport JWT 이중 전략 + @CurrentUser
  - Pattern 2: Controller → Service → Repository 계층
  - Pattern 3: Global Response / Exception Format
  - Pattern 4: JWT Refresh Token Rotation
  - Pattern 5: Module Dependencies + Bootstrap
- [x] 트리거 키워드 16개 (기준 5개 이상 충족)
- [x] 안티패턴 섹션 (6개)
- [x] skill-rules.json 등록 — Phase 2-D 처리 예정

---

## 교차 검증 결과

| 스킬 패턴 | base/nestjs/ 파일 | 일치 여부 |
|---|---|---|
| JwtStrategy (Pattern 1) | `src/auth/strategies/jwt.strategy.ts` | ✅ 정확히 일치 |
| JwtRefreshStrategy (Pattern 1) | `src/auth/strategies/jwt-refresh.strategy.ts` | ✅ 정확히 일치 |
| AuthService register/login/refresh (Pattern 4) | `src/auth/auth.service.ts` | ✅ 정확히 일치 |
| TransformInterceptor (Pattern 3) | `src/common/interceptors/transform.interceptor.ts` | ✅ 존재 확인 |
| HttpExceptionFilter (Pattern 3) | `src/common/filters/http-exception.filter.ts` | ✅ 존재 확인 |
| @CurrentUser() (Pattern 1) | `src/common/decorators/current-user.decorator.ts` | ✅ 존재 확인 |
| PrismaModule @Global (Pattern 5) | `src/prisma/prisma.module.ts` | ✅ 존재 확인 |
| UserModule/AuthModule exports | `src/user/user.module.ts`, `src/auth/auth.module.ts` | ✅ 구조 일치 |

---

## P1 이슈

없음.

---

## 비고

- `base/nestjs/src/` 22개 TypeScript 파일 전체 구조와 스킬 Project Structure 섹션 완전 일치
- AuthService의 `getMe()` 메서드는 스킬에 명시되지 않았으나 (base 코드에만 존재) — 생략 허용 (핵심 패턴 외 구현 세부)
- Quick Start 체크리스트, References 섹션 품질 우수
