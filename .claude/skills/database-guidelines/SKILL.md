---
name: database-guidelines
description: >
  크로스 스택 데이터베이스 설계 가이드라인. ERD 설계 원칙, 정규화(1NF~3NF+BCNF),
  인덱스 전략(복합/커버링/부분), 읽기/쓰기 분리 패턴, 마이그레이션 관리,
  트랜잭션 격리 수준, 커넥션 풀링. 스키마 설계, 쿼리 최적화, DB 아키텍처 작업 시 사용.
triggers:
  - database
  - database design
  - database schema
  - ERD
  - entity relationship
  - normalization
  - index
  - index strategy
  - composite index
  - covering index
  - read write separation
  - read replica
  - migration
  - schema migration
  - query optimization
  - slow query
  - connection pool
  - transaction
  - transaction isolation
  - deadlock
  - foreign key
  - primary key
  - database performance
---

# Database Design & Architecture Guidelines

## Purpose

`base/*/` 전체 스택(FastAPI, Express, Django, NestJS, Spring Boot)에서 검증된 크로스 스택 데이터베이스 설계 표준.
ERD 설계, 정규화, 인덱스 전략, 읽기/쓰기 분리, 마이그레이션, 트랜잭션 관리 패턴을 일관되게 적용한다.

## When to Use This Skill

- 새 테이블/엔티티 스키마 설계 시
- 인덱스 추가/최적화 또는 슬로우 쿼리 분석 시
- 읽기/쓰기 DB 분리 아키텍처 설계 시
- 마이그레이션 전략 수립 또는 커넥션 풀 튜닝 시

---

## Core Patterns

### Pattern 1 — ERD 설계 원칙

#### 1-1. Primary Key 전략

| 전략 | 적합 환경 | 예시 스택 |
|---|---|---|
| Auto-increment (`BIGSERIAL`) | 단일 DB, JOIN 효율 | Spring Boot (`GenerationType.IDENTITY`) |
| CUID / ULID | 분산 시스템, URL-safe | Express/NestJS (`@default(cuid())`), FastAPI (`usr_` ULID) |
| UUIDv7 | 분산 + 시간 정렬 | 고트래픽 분산 환경 |

**선택 기준:** 단일 DB + ORM → Auto-increment. 분산/API 외부 노출 → CUID/ULID. Auto-increment를 API에 직접 노출 금지 (열거 공격).

#### 1-2. 필수 메타 컬럼

모든 테이블에 `created_at`, `updated_at`을 포함한다. Soft delete 필요 시 `deleted_at` 추가.

```sql
CREATE TABLE orders (
    id          BIGSERIAL PRIMARY KEY,
    -- ... 비즈니스 컬럼 ...
    created_at  TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at  TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    deleted_at  TIMESTAMPTZ           -- Soft delete (선택)
);

-- updated_at 자동 갱신 트리거 (PostgreSQL)
CREATE OR REPLACE FUNCTION update_updated_at()
RETURNS TRIGGER AS $$
BEGIN NEW.updated_at = NOW(); RETURN NEW; END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_orders_updated_at
    BEFORE UPDATE ON orders
    FOR EACH ROW EXECUTE FUNCTION update_updated_at();
```

**ORM별 updated_at 자동 갱신:**

| 스택 | 방법 |
|---|---|
| FastAPI (SQLModel) | `BaseRepository.update_async()`에서 수동 갱신 (`entity.updated_at = datetime.now(UTC)`) |
| Spring Boot (JPA) | `@PreUpdate` lifecycle callback |
| Prisma (Express/NestJS) | `@updatedAt` 어노테이션 |
| Django | `auto_now=True` |

#### 1-3. Soft Delete vs Hard Delete

| 방식 | 적합 케이스 | 주의사항 |
|---|---|---|
| Soft Delete (`deleted_at`) | 감사 로그, 복원 필요, 법적 보존 | 모든 쿼리에 `WHERE deleted_at IS NULL` 필수 |
| Hard Delete (`DELETE`) | GDPR 삭제 의무, 로그성 데이터 정리 | FK cascade 설정 확인 |

Soft delete 시 부분 인덱스 활용: `CREATE UNIQUE INDEX idx_users_email_active ON users (email) WHERE deleted_at IS NULL;`

#### 1-4. FK 및 Enum

```sql
-- FK: ON DELETE 동작 항상 명시
REFERENCES users(id) ON DELETE CASCADE    -- 부모 삭제 시 자식도 삭제
REFERENCES users(id) ON DELETE SET NULL   -- 부모 삭제 시 NULL
REFERENCES users(id) ON DELETE RESTRICT   -- 부모 삭제 차단 (기본)

-- Enum: 값 추가 잦으면 VARCHAR + CHECK, 고정이면 native ENUM
CREATE TYPE user_role AS ENUM ('USER', 'ADMIN');
-- 또는
ALTER TABLE users ADD COLUMN role VARCHAR(10) NOT NULL DEFAULT 'USER'
    CHECK (role IN ('USER', 'ADMIN'));
```

---

### Pattern 2 — 정규화 (Normalization)

#### 단계별 체크

| 단계 | 규칙 | 위반 징후 |
|---|---|---|
| **1NF** | 원자값, 반복 그룹 없음 | 컬럼에 CSV/JSON 배열 저장 |
| **2NF** | 부분 함수 종속 제거 | 복합 PK 일부에만 종속되는 컬럼 |
| **3NF** | 이행 함수 종속 제거 | 비 키 컬럼이 다른 비 키에 종속 |
| **BCNF** | 모든 결정자가 후보 키 | 복합 후보 키 테이블 |

```sql
-- BAD: 1NF 위반 (반복 그룹)
CREATE TABLE orders (
    id BIGSERIAL PRIMARY KEY, user_id BIGINT,
    item1 VARCHAR(100), price1 DECIMAL,
    item2 VARCHAR(100), price2 DECIMAL  -- 아이템 수 제한
);

-- GOOD: 별도 테이블로 분리
CREATE TABLE orders (
    id BIGSERIAL PRIMARY KEY,
    user_id BIGINT NOT NULL REFERENCES users(id),
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW()
);
CREATE TABLE order_items (
    id BIGSERIAL PRIMARY KEY,
    order_id BIGINT NOT NULL REFERENCES orders(id) ON DELETE CASCADE,
    product_id BIGINT NOT NULL REFERENCES products(id),
    quantity INT NOT NULL CHECK (quantity > 0),
    unit_price DECIMAL(12,2) NOT NULL
);
```

#### 의도적 비정규화

성능을 위한 비정규화 시 **반드시 `-- DENORM:` 주석으로 원본과 갱신 전략 명시.**

```sql
CREATE TABLE order_summaries (
    order_id     BIGINT PRIMARY KEY REFERENCES orders(id),
    total_amount DECIMAL(12,2) NOT NULL,  -- DENORM: order_items SUM, 트리거로 동기화
    item_count   INT NOT NULL             -- DENORM: order_items COUNT
);
```

**허용 기준:** JOIN이 p95 latency 초과 + 정합성을 트리거/서비스 레이어에서 보장 가능.

---

### Pattern 3 — 인덱스 전략

#### 3-1. 인덱스 유형

| 유형 | 적합 케이스 | 문법 |
|---|---|---|
| **B-tree** (기본) | 등호, 범위, 정렬 | `CREATE INDEX ...` |
| **복합** | 다중 조건 WHERE + 정렬 | `ON t(a, b, c)` |
| **커버링** | SELECT 컬럼 포함 (Index-Only Scan) | `ON t(a) INCLUDE (b, c)` |
| **부분** | 특정 조건 행만 | `... WHERE condition` |
| **GIN** | JSONB, 배열, 전문 검색 | `USING GIN(col)` |

#### 3-2. 복합 인덱스 — ERS 규칙

**Equal → Range → Sort 순서로 컬럼 배치.**

```sql
-- 쿼리: WHERE status = 'active' AND created_at > '2024-01-01' ORDER BY name
CREATE INDEX idx_users_status_created_name ON users (status, created_at, name);  -- GOOD
-- BAD: (created_at, status, name) — 범위 컬럼 뒤 인덱스 무효
```

#### 3-3. 커버링 & 부분 인덱스

```sql
-- 커버링: 테이블 힙 접근 없이 인덱스만으로 처리
CREATE INDEX idx_users_status_cover ON users (status) INCLUDE (email, name);

-- 부분: Soft delete 활성 레코드만 / 특정 상태만
CREATE UNIQUE INDEX idx_users_email_active ON users (email) WHERE deleted_at IS NULL;
CREATE INDEX idx_orders_pending ON orders (created_at) WHERE status = 'pending';
```

#### 3-4. 인덱스 모니터링

```sql
-- 미사용 인덱스 탐지
SELECT indexrelname, idx_scan, pg_size_pretty(pg_relation_size(indexrelid)) AS size
FROM pg_stat_user_indexes WHERE idx_scan = 0
ORDER BY pg_relation_size(indexrelid) DESC;

-- EXPLAIN으로 스캔 타입 확인
EXPLAIN (ANALYZE, BUFFERS) SELECT * FROM users WHERE email = 'test@example.com';
-- Seq Scan → 인덱스 필요 | Index Scan → 정상 | Index Only Scan → 최적
```

**판단 기준:** `idx_scan=0` + >10MB → 삭제 후보. `Seq Scan` + >10K행 → 인덱스 추가 후보. 쓰기 >80% 테이블 → 인덱스 최소화.

---

### Pattern 4 — 읽기/쓰기 분리

```
Application ──Write──► Primary DB ──Replication──► Read Replica (1~N대)
            ──Read───► Read Replica
```

**도입 기준:** 읽기 트래픽 80% 이상. 스택별 구현은 아래 참조.

| 스택 | 구현 방식 |
|---|---|
| FastAPI | `get_write_session_dependency()` / `get_read_session_dependency()` — 별도 AsyncEngine (base/fastapi/backend/db/orm.py) |
| Spring Boot | `@Transactional(readOnly = true)` → Read Replica 라우팅 |
| Express/NestJS | Prisma `datasources` 또는 별도 PrismaClient 인스턴스 |
| Django | `DATABASE_ROUTERS` 설정으로 읽기/쓰기 DB 분리 |

**Replication Lag 대응:**
- 일반 읽기: 최대 1초 lag 허용
- 쓰기 직후 자기 데이터 조회: **Primary에서 직접 읽기** (lag 0)
- 결제/재고 등 실시간 정합성: Primary only

---

### Pattern 5 — 마이그레이션 관리

#### 스택별 도구

| 스택 | 도구 | Dev → Prod |
|---|---|---|
| FastAPI | Alembic | `revision --autogenerate` → `upgrade head` |
| Express/NestJS | Prisma Migrate | `migrate dev` → `migrate deploy` |
| Django | Django Migrations | `makemigrations` → `migrate` |
| Spring Boot | Flyway/Liquibase | `flyway migrate` 또는 `ddl-auto: validate` |

#### 안전한 마이그레이션 원칙

```sql
-- MUST: 컬럼 추가 시 DEFAULT 지정 (테이블 잠금 최소화)
ALTER TABLE users ADD COLUMN phone VARCHAR(20) DEFAULT NULL;

-- MUST: 프로덕션 인덱스는 CONCURRENTLY (무중단)
CREATE INDEX CONCURRENTLY idx_users_phone ON users (phone);

-- MUST: NOT NULL 추가는 2단계
ALTER TABLE users ADD COLUMN role VARCHAR(10) DEFAULT 'USER';  -- Step 1
UPDATE users SET role = 'USER' WHERE role IS NULL;              -- Step 2: 백필
ALTER TABLE users ALTER COLUMN role SET NOT NULL;               -- Step 3: 제약

-- DANGER: 프로덕션에서 컬럼 삭제 직접 실행 금지 (롤백 불가!)
-- DANGER: 대형 테이블 NOT NULL + DEFAULT 동시 추가 (PG <11은 전체 행 재작성)
```

**롤백:** Alembic은 `downgrade()` 항상 작성. Prisma는 `migrate resolve --rolled-back`.

---

### Pattern 6 — 커넥션 풀링

| 스택 | 풀 라이브러리 | 권장 설정 |
|---|---|---|
| FastAPI | SQLAlchemy AsyncEngine | `pool_size=15, max_overflow=25, pool_pre_ping=True` |
| Express/NestJS | Prisma (내장) | `connection_limit=10` (.env) |
| Django | Django ORM | `CONN_MAX_AGE=600` |
| Spring Boot | HikariCP | `maximum-pool-size=10, minimum-idle=2` |

**풀 크기 공식:** `(CPU 코어 × 2) + 동시 디스크 I/O 수` (예: 4코어 SSD → 9).
PG `max_connections=100` 기준, 앱 서버 수 × 풀 크기 < 100 확인.

**필수 설정:** `pool_pre_ping=True` (유효성 검증), `pool_recycle=3600` (타임아웃 대응).
앱 서버 다수일 때 **PgBouncer** (`pool_mode=transaction`) 도입 검토.

---

### Pattern 7 — 트랜잭션 & 동시성 제어

#### 격리 수준

| 격리 수준 | Dirty | Non-repeatable | Phantom | 적합 케이스 |
|---|---|---|---|---|
| READ COMMITTED | X | O | O | 일반 CRUD (PG 기본) |
| REPEATABLE READ | X | X | X (PG) | 잔액 조회+차감 |
| SERIALIZABLE | X | X | X | 좌석 예약, 재고 차감 |

```python
# FastAPI — 격리 수준 지정
async with session.begin():
    await session.connection(execution_options={"isolation_level": "REPEATABLE READ"})
```

```java
// Spring Boot
@Transactional(isolation = Isolation.REPEATABLE_READ)
```

#### 낙관적 잠금 (Optimistic Locking)

```sql
-- version 컬럼으로 충돌 감지
UPDATE products SET stock = stock - 1, version = version + 1
WHERE id = 42 AND version = 5;  -- 0 rows affected → 충돌
```

JPA: `@Version private Integer version;` (Hibernate 자동 관리).
Prisma: `updateMany` + `where: { version: currentVersion }` 수동 체크.

#### 비관적 잠금 (Pessimistic Locking)

```sql
-- SELECT ... FOR UPDATE (행 잠금 — 좌석 예약 등)
BEGIN;
SELECT * FROM seats WHERE id = 100 FOR UPDATE;
UPDATE seats SET status = 'reserved', user_id = 42 WHERE id = 100;
COMMIT;

-- SKIP LOCKED (큐 패턴 — 잠긴 행 건너뛰기)
SELECT * FROM tasks WHERE status = 'pending'
ORDER BY created_at LIMIT 1 FOR UPDATE SKIP LOCKED;
```

**데드락 방지:** 테이블 접근 순서 통일, 트랜잭션 최대한 짧게, 대량 UPDATE 배치(1000행씩).

---

## Anti-Patterns

### 1. N+1 쿼리

```python
# BAD: 루프 안에서 쿼리
for user in users:
    orders = await session.execute(select(Order).where(Order.user_id == user.id))

# GOOD: Eager loading
stmt = select(User).options(selectinload(User.orders))  # SQLAlchemy
users = await prisma.user.findMany({ include: { orders: true } })  # Prisma
```

### 2. Leading Wildcard LIKE

```sql
-- BAD: 인덱스 사용 불가
WHERE name LIKE '%phone%';
-- GOOD: pg_trgm + GIN (PostgreSQL)
CREATE EXTENSION IF NOT EXISTS pg_trgm;
CREATE INDEX idx_name_trgm ON products USING GIN (name gin_trgm_ops);
-- 또는 접두사 검색 (B-tree 활용): WHERE name LIKE 'phone%';
```

### 3. 과도한 인덱스

```sql
-- BAD: 모든 컬럼에 개별 인덱스 (쓰기 성능 저하)
CREATE INDEX idx_1 ON users(email); CREATE INDEX idx_2 ON users(name);
CREATE INDEX idx_3 ON users(status); CREATE INDEX idx_4 ON users(created_at);

-- GOOD: 쿼리 패턴 기반 최소 인덱스
CREATE UNIQUE INDEX idx_users_email ON users (email);
CREATE INDEX idx_users_status_created ON users (status, created_at);
```

### 4. 금액을 FLOAT로 저장

```sql
-- BAD: 부동소수점 오차 (0.1 + 0.2 != 0.3)
CREATE TABLE payments (amount FLOAT);
-- GOOD: DECIMAL 또는 센트 정수
CREATE TABLE payments (amount DECIMAL(12,2) NOT NULL);  -- 또는 amount_cents BIGINT
```

### 5. OFFSET 기반 대량 페이지네이션

```sql
-- BAD: 깊은 페이지에서 성능 급락 (100K행 스캔 후 버림)
SELECT * FROM orders ORDER BY id LIMIT 20 OFFSET 100000;
-- GOOD: Cursor 기반 (Keyset Pagination)
SELECT * FROM orders WHERE id > :last_seen_id ORDER BY id LIMIT 20;
```

### 6. 트랜잭션 안에서 외부 API 호출

```python
# BAD: 외부 호출 실패 시 커넥션 장시간 점유
async with session.begin():
    order = await create_order(session, dto)
    await payment_gateway.charge(order.amount)  # 타임아웃 위험!

# GOOD: 외부 호출은 트랜잭션 밖에서
order = await create_order_in_tx(session, dto)  # TX 완료
payment = await payment_gateway.charge(order.amount)  # 별도 호출
```

---

## Quick Reference

### 새 테이블 설계 체크리스트

- [ ] PK 전략 결정 (Auto-increment / CUID / ULID)
- [ ] `created_at`, `updated_at` 메타 컬럼 포함
- [ ] Soft delete 여부 결정 (`deleted_at`)
- [ ] FK `ON DELETE` 동작 명시 (CASCADE / SET NULL / RESTRICT)
- [ ] UNIQUE 제약 설정 (email, username 등)
- [ ] 쿼리 패턴 기반 인덱스 설계 (ERS 순서)
- [ ] 금액은 DECIMAL 또는 정수, FLOAT 금지
- [ ] 3NF 이상 정규화, 비정규화 시 `-- DENORM:` 주석

### EXPLAIN 읽는 법

```
Seq Scan          → 풀 스캔 (인덱스 필요 검토)
Index Scan        → 인덱스 → 힙 접근 (정상)
Index Only Scan   → 인덱스만 처리 (최적)
Bitmap Index Scan → 다수 행 인덱스 후 힙 (중간)
Hash Join         → 대량 조인 (효율적)
Sort              → 인덱스 없는 ORDER BY (인덱스 검토)
```

---

*Last updated: 2026-03-05 — Cross-stack database patterns from base/{fastapi,express,django,nestjs,spring-boot}*
