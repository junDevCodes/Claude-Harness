---
name: data-analysis-guidelines
description: 데이터 분석(EDA) 가이드라인. pandas/polars/numpy로 탐색적 분석, 결측·이상치 처리, 기술통계, 분포·상관 분석, 시각화(matplotlib/seaborn/plotly) 패턴. 노트북·스크립트 기반 데이터 탐색과 리포트 생성 작업 시 사용.
triggers:
  - pandas
  - polars
  - numpy
  - jupyter
  - EDA
  - 데이터 분석
  - exploratory data analysis
  - DataFrame
  - describe
  - value_counts
---

# Data Analysis Guidelines

## Purpose

탐색적 데이터 분석(EDA)을 체계적으로 수행하기 위한 표준 절차와 코드 관용구를 제공한다. HERE:O 맥락에서는 CSI 세션 데이터, 센서 로그, 이벤트 테이블을 대상으로 한다.

## When to Use

- 새 데이터셋 투입 시 초기 탐색
- 모델링 전 특징 설계 근거 확보
- 이상 현상·품질 문제 진단
- 통계 요약 리포트 작성

---

## 1. EDA 표준 절차 (8단계)

```
1. 로드 & 스키마 확인      (dtypes, shape, memory_usage)
2. 결측/중복 확인          (isna, duplicated)
3. 기술통계               (describe, unique, nunique)
4. 분포 확인              (hist, boxplot, violin)
5. 이상치 탐지            (IQR, Z-score, Isolation Forest)
6. 상관/연관성 분석        (corr, mutual_info, chi2)
7. 시간·그룹 분석         (groupby, resample, rolling)
8. 가설 수립 및 시각화 리포트
```

각 단계의 결과는 `reports/eda_YYYYMMDD.md`에 누적 기록한다.

---

## 2. pandas vs polars 선택 기준

| 상황 | 권장 |
|---|---|
| 데이터 < 1GB, 노트북 탐색 | **pandas** (생태계 풍부) |
| 데이터 > 1GB, 속도 중요 | **polars** (Rust 기반 5~10배 빠름) |
| 스트리밍·청크 처리 | polars `scan_csv` lazy API |
| 기존 sklearn/scipy 호환 | pandas (numpy 변환 쉬움) |

**HERE:O 기본값:** CSI 세션 로그는 `polars`, 분석 리포트 생성은 `pandas` (matplotlib 호환).

---

## 3. 표준 EDA 코드 관용구

### 3.1 로드 & 스키마 확인

```python
import polars as pl
import pandas as pd
import numpy as np

# polars (권장 - 속도)
df = pl.read_parquet("data/csi_sessions.parquet")
print(df.schema)
print(f"shape: {df.shape}, memory: {df.estimated_size('mb'):.1f}MB")

# pandas
df = pd.read_parquet("data/csi_sessions.parquet")
df.info(memory_usage='deep')
```

### 3.2 결측·중복

```python
# 결측 비율
missing = df.null_count().to_pandas().T / len(df)
missing[missing[0] > 0].sort_values(0, ascending=False)

# 중복
n_dup = df.is_duplicated().sum()
print(f"duplicates: {n_dup} ({n_dup/len(df)*100:.2f}%)")
```

### 3.3 기술통계

```python
# 수치형
df.select(pl.col(pl.NUMERIC_DTYPES)).describe()

# 범주형
for col in df.select(pl.col(pl.Utf8)).columns:
    print(df[col].value_counts().head(10))
```

### 3.4 이상치 탐지

```python
# IQR 방식 (범용)
def iqr_outliers(s: pl.Series, k: float = 1.5) -> pl.Series:
    q1, q3 = s.quantile(0.25), s.quantile(0.75)
    iqr = q3 - q1
    return (s < q1 - k*iqr) | (s > q3 + k*iqr)

# Z-score (정규분포 가정)
z = (s - s.mean()) / s.std()
outliers = z.abs() > 3

# 신호 데이터는 IsolationForest 권장
from sklearn.ensemble import IsolationForest
iso = IsolationForest(contamination=0.01, random_state=42)
outlier_flag = iso.fit_predict(X) == -1
```

### 3.5 상관 분석

```python
# 수치형 상관 (Pearson)
corr = df.select(pl.col(pl.NUMERIC_DTYPES)).corr()

# 비선형 관계 — Spearman 또는 mutual_info
from sklearn.feature_selection import mutual_info_regression
mi = mutual_info_regression(X, y, random_state=42)
```

### 3.6 시간·그룹 집계

```python
# 세션별 5초 윈도우 평균 (CSI 분석 패턴)
windowed = (
    df.sort("timestamp")
      .group_by_dynamic("timestamp", every="5s", group_by="session_id")
      .agg([
          pl.col("amplitude").mean().alias("amp_mean"),
          pl.col("amplitude").std().alias("amp_std"),
          pl.col("phase").mean().alias("phase_mean"),
      ])
)
```

---

## 4. 시각화 표준

### 4.1 라이브러리 선택

| 용도 | 라이브러리 |
|---|---|
| 정적 리포트·논문 | matplotlib + seaborn |
| 인터랙티브 탐색 | plotly |
| 대시보드 | plotly + dash (또는 React/Three.js) |

### 4.2 시각화 치트시트

```python
import matplotlib.pyplot as plt
import seaborn as sns

# 분포
sns.histplot(df_pd["amplitude"], kde=True, bins=50)

# 다변량 분포
sns.pairplot(df_pd.sample(1000), hue="label", diag_kind="kde")

# 상관 히트맵
sns.heatmap(corr.to_pandas(), annot=True, cmap="coolwarm", center=0)

# 시계열
fig, ax = plt.subplots(figsize=(12, 4))
df_pd.plot(x="timestamp", y="amp_mean", ax=ax)
ax.set_title("Amplitude over time")

# 박스플롯 (그룹 비교)
sns.boxplot(data=df_pd, x="label", y="amplitude")

# 저장 규칙
plt.tight_layout()
plt.savefig(f"reports/figures/{name}_{date}.png", dpi=120, bbox_inches="tight")
plt.close()
```

---

## 5. EDA 리포트 포맷

분석 결과는 `reports/eda_{dataset}_{YYYYMMDD}.md`에 아래 구조로 기록한다.

```markdown
# EDA Report — {dataset} ({date})

## 1. 데이터 개요
- 출처, shape, 기간, 스키마

## 2. 품질 체크
- 결측·중복·이상치 요약 테이블

## 3. 변수별 분포
- 수치형: describe + 히스토그램
- 범주형: value_counts 상위 N

## 4. 관계 분석
- 상관 히트맵
- 타깃 대비 그룹 차이

## 5. 발견 사항 (Findings)
- F1. ...
- F2. ...

## 6. 모델링 제언
- 특징 후보
- 전처리 필요 항목
- 주의사항 (데이터 누수 가능 지점)
```

---

## 6. 데이터 누수 방지 체크리스트

EDA 단계에서 이미 누수 가능성을 검토한다.

- [ ] 타깃 변수를 만들 때 미래 정보가 들어가지 않는가?
- [ ] 동일 세션/사용자가 train·test에 동시에 존재하지 않는가?
- [ ] 이상치 제거 기준이 test 데이터를 포함하지 않는가?
- [ ] 정규화·인코더가 train 전용으로 fit 되는가?
- [ ] 시간 기반 데이터는 시간순 분할을 사용하는가?

---

## 7. HERE:O 맥락 적용

| 데이터 | 권장 분석 |
|---|---|
| CSI 원시 프레임 (470B × 20Hz) | `csi-signal-processing-guidelines` 참조 → FFT 후 EDA |
| 세션 메타 (시작/종료/라벨) | 세션 지속 시간 분포, 라벨 불균형 확인 |
| 노드 상태 로그 | 결측률·지연 분포로 품질 평가 |
| 이벤트 (변화 탐지 결과) | 이벤트 간격 분포, false positive 비율 |

---

## 8. 안티패턴

- ❌ `df.describe()`만 보고 EDA 끝내기
- ❌ 이상치를 기계적으로 제거 (도메인 맥락 확인 필수)
- ❌ test 데이터로 EDA하기 (train만 사용)
- ❌ 결측치 `0`이나 `mean`으로 일괄 채우기 (의미 변형)
- ❌ 노트북을 리포트 산출물로 제출 (PDF/MD로 정리)

---

## 9. 참고 링크

- pandas: https://pandas.pydata.org/docs/user_guide/
- polars: https://docs.pola.rs/user-guide/
- seaborn 갤러리: https://seaborn.pydata.org/examples/
- EDA 체크리스트 (Chris Albon): https://chrisalbon.com/
