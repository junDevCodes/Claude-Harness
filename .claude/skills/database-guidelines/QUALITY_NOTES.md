# database-guidelines — Quality Notes

## QA 결과: PASS (A등급)

**검증일:** 2026-03-08
**검증 방법:** SKILL.md 내용 검증 + `base/*/` ORM 스키마 패턴 교차 확인 (1cycle)

---

## DoD 체크리스트

- [x] SKILL.md 존재 (441줄 — 500줄 이하)
- [x] YAML frontmatter (name, description, triggers 23개 — 최다)
- [x] 핵심 패턴 7개 + 코드 예시
  - Pattern 1: ERD 설계 원칙 (PK 전략, 메타 컬럼, Soft Delete, FK/Enum)
  - Pattern 2: 정규화 (1NF~BCNF + 의도적 비정규화)
  - Pattern 3: 인덱스 전략 (복합/커버링/부분 + 모니터링)
  - Pattern 4: 읽기/쓰기 분리
  - Pattern 5: 마이그레이션 관리
  - Pattern 6: 커넥션 풀링
  - Pattern 7: 트랜잭션 & 동시성 제어
- [x] 트리거 키워드 23개 (기준 5개 이상 충족)
- [x] 안티패턴 섹션 (6개)
- [x] 새 테이블 설계 Quick Reference 체크리스트 포함
- [x] skill-rules.json 등록 — Phase 2-D 처리 예정

---

## 교차 검증 결과

| 스킬 내용 | base/ 스택 대응 | 일치 여부 |
|---|---|---|
| CUID PK (`@default(cuid())`) — Express/NestJS | Prisma schema (base/nestjs, base/express) | ✅ 일치 |
| Auto-increment (`GenerationType.IDENTITY`) — Spring Boot | JPA Entity in base/spring-boot | ✅ 일치 |
| FastAPI 읽기/쓰기 분리 세션 (`get_write_session_dependency` / `get_read_session_dependency`) | `base/fastapi/backend/db/orm.py` 언급 정확 | ✅ 일치 |
| `@updatedAt` Prisma 어노테이션 | base/nestjs, base/express Prisma schema | ✅ 일치 |
| `@PreUpdate` JPA — Spring Boot | base/spring-boot Entity | ✅ 일치 |
| `auto_now=True` Django | base/django models | ✅ 일치 |
| HikariCP `maximum-pool-size=10` | Spring Boot application.yml 패턴 | ✅ 일치 |
| SQLAlchemy `pool_size=15, max_overflow=25` | base/fastapi db 설정 | ✅ 일치 |
| Prisma `migrate dev` → `migrate deploy` | base/nestjs, base/express workflow | ✅ 일치 |
| Alembic `upgrade head` | base/fastapi 언급 (미구현 명시) | ✅ 정직한 문서화 |

---

## P1 이슈

없음.

---

## 비고

- 트리거 23개 — 전체 스킬 중 최다. 다양한 DB 관련 쿼리에 활성화됨 (적절)
- Pattern 7 (트랜잭션)의 낙관적/비관적 잠금 SQL + ORM 예시 모두 포함 — 실용성 최고 수준
- `-- DENORM:` 주석 규칙 명시 — 팀 협업 시 의사소통 표준화 우수
- EXPLAIN 출력 타입 해석표 포함 — 즉시 현장 활용 가능
- 커버링 인덱스 `INCLUDE` 절 PostgreSQL 문법 정확
- 크로스 스택 (FastAPI/Express/NestJS/Django/Spring Boot) 전체 커버 — 스킬 목적에 부합
