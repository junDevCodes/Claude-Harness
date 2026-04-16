---
name: data-pipeline-guidelines
description: 데이터 파이프라인 설계 가이드라인. Batch(ETL/ELT) vs Stream 선택, 오케스트레이션 도구(Airflow/Prefect/Dagster) 비교, 데이터 품질 검증(Great Expectations), idempotency·재처리 전략, 스키마 진화, feature store 패턴. 데이터 수집·변환·적재 파이프라인 설계·구축 작업 시 사용.
triggers:
  - data pipeline
  - 데이터 파이프라인
  - ETL
  - ELT
  - airflow
  - prefect
  - dagster
  - kafka
  - streaming
  - Great Expectations
  - feature store
  - DAG
---

# Data Pipeline Guidelines

## Purpose

데이터 수집 → 변환 → 적재(및 피처 서빙)까지의 파이프라인을 **재현 가능·관측 가능·복구 가능**하게 설계하기 위한 표준 패턴을 제공한다.

## When to Use

- 새 데이터 소스 통합 시
- 배치·스트리밍 파이프라인 설계
- 데이터 품질 검증 게이트 추가
- 재처리·백필(backfill) 전략 수립
- 피처 스토어 설계

---

## 1. Batch vs Stream 선택 기준

| 기준 | Batch (ETL/ELT) | Stream |
|---|---|---|
| **지연 허용** | 분 ~ 일 단위 | 초 이하 |
| **복잡도** | 낮음 | 높음 |
| **실패 복구** | 재실행 쉬움 | 체크포인트·DLQ 필수 |
| **도구** | Airflow / Prefect / Dagster / dbt | Kafka / Flink / Spark Streaming |
| **HERE:O 사용처** | 일별 CSI 세션 집계·리포트 | 실시간 변화 탐지 → WebSocket |

**혼합(Lambda 아키텍처):** HERE:O는 실시간(스트림) + 야간 재학습(배치) 모두 필요 → **두 계층 공존**.

---

## 2. 오케스트레이터 비교

| 항목 | Airflow | Prefect 2 | Dagster |
|---|---|---|---|
| **성숙도** | 최고 (업계 표준) | 중~상 | 상승 중 |
| **학습 곡선** | 가파름 | 완만 | 중간 |
| **동적 DAG** | 제한적 | 우수 | 우수 |
| **데이터 계보(lineage)** | 플러그인 | 부분 | **네이티브(Asset)** |
| **로컬 개발 경험** | 무거움 | 가벼움 | 가벼움 |
| **타입 안정성** | 낮음 | 중간 | **높음 (Pydantic)** |
| **권장 상황** | 기존 생태계 호환 필요 | 경량·Pythonic | 데이터 자산 중심 설계 |

**HERE:O 기본값:** **Prefect 2** (FastAPI와 Python 통합 쉽고 경량). 규모 확장 시 Dagster 전환 고려.

---

## 3. 표준 파이프라인 레이어

```
┌─────────────┐   ┌──────────┐   ┌────────────┐   ┌──────────┐   ┌─────────┐
│   Source    │──▶│  Ingest  │──▶│  Transform │──▶│   Load   │──▶│  Serve  │
│ (ESP32/API) │   │ (raw/bronze) │   │ (silver/gold)│   │ (warehouse) │   │ (FE/ML) │
└─────────────┘   └──────────┘   └────────────┘   └──────────┘   └─────────┘
                        │               │                │
                        ▼               ▼                ▼
                   [Quality Gate] [Quality Gate]   [Quality Gate]
```

### 3.1 메달리온 아키텍처 (Bronze / Silver / Gold)

| 레이어 | 역할 | 형식 |
|---|---|---|
| **Bronze (raw)** | 원본 그대로 저장, idempotent 수집 | Parquet/JSON, 날짜 파티션 |
| **Silver (cleaned)** | 정제·조인·표준화 | Parquet, 스키마 강제 |
| **Gold (served)** | 분석·ML·대시보드용 집계 | Parquet/DB Table, 비정규화 |

---

## 4. Idempotency & 재처리

**핵심 원칙:** 같은 입력에 대해 몇 번 실행해도 같은 결과.

### 4.1 기법

```python
# (1) 날짜 파티션 + UPSERT (dedup)
def load_daily(date: str):
    df = read_source(date)
    target_path = f"s3://bronze/csi/date={date}/"
    df.write_parquet(target_path, mode="overwrite")  # overwrite 파티션만

# (2) 해시 기반 중복 제거
df = df.with_columns(
    pl.concat_str(["session_id", "timestamp"]).hash().alias("row_hash")
).unique("row_hash")

# (3) 워터마크 (스트림)
# Kafka offset, Flink event-time watermark
```

### 4.2 백필 전략

```bash
# Prefect 예시
prefect deployment run "csi-daily/prod" \
  --param "start_date=2026-03-01" \
  --param "end_date=2026-03-31"
```

---

## 5. 데이터 품질 검증 (Great Expectations 패턴)

각 레이어 사이에 **품질 게이트**를 둔다. 실패 시 다음 레이어로 진행 금지.

```python
import great_expectations as gx

context = gx.get_context()
suite = context.add_or_update_expectation_suite("csi_silver_suite")

validator = context.sources.pandas_default.read_parquet("silver/csi.parquet")

# 필수 검증
validator.expect_column_to_exist("session_id")
validator.expect_column_values_to_not_be_null("timestamp")
validator.expect_column_values_to_be_between("amplitude", -100, 100)
validator.expect_column_values_to_match_regex("session_id", r"^sess_[a-z0-9]+$")
validator.expect_column_pair_values_a_to_be_greater_than_b(
    "end_time", "start_time"
)

result = validator.validate()
if not result.success:
    raise ValueError(f"Data quality check failed: {result.results}")
```

### 5.1 최소 품질 체크리스트

모든 테이블에 대해:
- [ ] PK 유일성
- [ ] 필수 컬럼 not null
- [ ] 값 범위 (음수/상한)
- [ ] 타입 일치
- [ ] 레코드 수 ±N% 이내 (baseline 대비)
- [ ] 신선도(freshness) — 최신 타임스탬프 vs 현재 시각

---

## 6. 스키마 진화 & 계약

### 6.1 Schema-on-Write vs Schema-on-Read

| | Schema-on-Write | Schema-on-Read |
|---|---|---|
| 예시 | Parquet, Avro + Schema Registry | JSON lake |
| 장점 | 품질 보장, 쿼리 빠름 | 유연, 저장 싸게 |
| 권장 | Silver/Gold | Bronze |

### 6.2 스키마 변경 룰 (backward compatible)

- ✅ 옵셔널 컬럼 추가
- ✅ 기본값이 있는 컬럼 추가
- ❌ 필수 컬럼 삭제·타입 변경
- ❌ 컬럼 의미 변경 (이름 같고 의미 다름 — **최악**)

파괴적 변경은 **새 버전 테이블**(`csi_silver_v2`)로 분리하고 Consumer 이전 후 구버전 제거.

---

## 7. Prefect 2 DAG 패턴

```python
from prefect import flow, task
from prefect.logging import get_run_logger

@task(retries=3, retry_delay_seconds=30, tags=["io"])
def extract_csi(date: str) -> pl.DataFrame:
    return pl.read_parquet(f"s3://raw/csi/date={date}/")

@task(tags=["transform"])
def clean(df: pl.DataFrame) -> pl.DataFrame:
    return (
        df.filter(pl.col("amplitude").is_not_null())
          .unique(["session_id", "timestamp"])
    )

@task
def validate(df: pl.DataFrame) -> pl.DataFrame:
    run_great_expectations(df, "csi_silver_suite")  # raise on fail
    return df

@task(retries=2, tags=["io"])
def load(df: pl.DataFrame, date: str):
    df.write_parquet(f"s3://silver/csi/date={date}/")

@flow(name="csi-daily", log_prints=True)
def csi_daily(date: str):
    logger = get_run_logger()
    logger.info(f"processing {date}")
    raw = extract_csi(date)
    clean_df = clean(raw)
    validated = validate(clean_df)
    load(validated, date)
```

---

## 8. 스트리밍 파이프라인 (Kafka → 처리 → 싱크)

### 8.1 HERE:O 실시간 흐름

```
ESP32 (TCP/UDP)
  ↓
Spring Boot Producer → Kafka topic: csi.raw
  ↓
Python Consumer (csi/) → FFT + baseline 처리
  ↓
  ├── Kafka topic: csi.events → Spring Boot → WebSocket
  └── Postgres/TimescaleDB 장기 저장 (배치에서 집계)
```

### 8.2 Python Consumer 예시

```python
from aiokafka import AIOKafkaConsumer, AIOKafkaProducer

async def consume_loop():
    consumer = AIOKafkaConsumer(
        "csi.raw",
        bootstrap_servers="kafka:9092",
        group_id="csi-fft-processor",
        enable_auto_commit=False,  # 처리 후 수동 커밋 (at-least-once)
    )
    producer = AIOKafkaProducer(bootstrap_servers="kafka:9092")
    await consumer.start(); await producer.start()
    try:
        async for msg in consumer:
            features = process_fft(msg.value)
            if features.is_anomaly:
                await producer.send("csi.events", features.to_bytes())
            await consumer.commit()
    finally:
        await consumer.stop(); await producer.stop()
```

**핵심 포인트:**
- `enable_auto_commit=False` + 처리 후 수동 커밋 = at-least-once
- DLQ (dead-letter queue) 토픽: `csi.raw.dlq` 구성 — 처리 실패 메시지
- 백프레셔: consumer lag 모니터링 (Grafana)

---

## 9. 피처 스토어 (Feast 간단 패턴)

ML 학습·추론에 동일한 피처 정의를 재사용.

```python
from feast import FeatureStore, Entity, FeatureView, Field
from feast.types import Float32, Int64

session = Entity(name="session_id", join_keys=["session_id"])

csi_features = FeatureView(
    name="csi_features",
    entities=[session],
    ttl=timedelta(days=7),
    schema=[
        Field(name="amp_mean", dtype=Float32),
        Field(name="amp_std", dtype=Float32),
        Field(name="dominant_freq", dtype=Float32),
        Field(name="n_peaks", dtype=Int64),
    ],
    source=parquet_source,
)
```

**HERE:O 단계별 도입:**
- Phase 초기: 미도입 — Parquet 파일 직접 사용
- Phase 중기: Feast + Redis (온라인) + Parquet (오프라인)
- Phase 후기: 운영 안정화 후 전사 도입

---

## 10. 관측성 (Observability)

모든 파이프라인은 다음 메트릭을 기본 노출한다.

| 메트릭 | 의미 |
|---|---|
| `records_in / records_out` | 입·출력 건수 (drop 비율 확인) |
| `duration_seconds` | 단계별 소요 시간 |
| `error_count` | 실패 횟수 |
| `data_freshness_seconds` | 최신 레코드 vs 현재 시각 |
| `schema_violations` | GE 검증 실패 수 |

Prometheus + Grafana 또는 Prefect UI · Dagster Asset View 활용.

---

## 11. 디렉토리 구조 (csi/ 적용안)

```
csi/
├── backend/
│   └── pipelines/
│       ├── flows/                # Prefect @flow
│       │   ├── csi_daily.py
│       │   └── csi_realtime.py
│       ├── tasks/                # @task 재사용 블록
│       │   ├── extract.py
│       │   ├── transform.py
│       │   ├── validate.py
│       │   └── load.py
│       ├── expectations/         # Great Expectations suite
│       │   └── csi_silver_suite.json
│       ├── schemas/              # Pydantic/Avro 스키마
│       └── streaming/
│           ├── consumer.py
│           └── producer.py
├── tests/
│   └── pipelines/
└── deployments/                  # Prefect deployment 설정
```

---

## 12. 안티패턴

- ❌ 한 함수에 Extract + Transform + Load 모두
- ❌ 품질 검증 없이 Silver→Gold 진행
- ❌ idempotent 하지 않은 append-only (중복 누적)
- ❌ 스키마 문서화 없이 암묵적 계약
- ❌ 실패 시 알람 없음 (또는 알람 피로)
- ❌ 피처 정의를 학습·추론 코드에서 각각 중복 구현

---

## 13. 참고

- Airflow: https://airflow.apache.org/docs/
- Prefect: https://docs.prefect.io/
- Dagster: https://docs.dagster.io/
- Great Expectations: https://docs.greatexpectations.io/
- Feast: https://docs.feast.dev/
- Medallion Architecture: https://www.databricks.com/glossary/medallion-architecture
