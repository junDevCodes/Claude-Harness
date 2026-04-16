---
name: ml-evaluation-guidelines
description: ML 모델 평가지표 검증 가이드라인. 분류(Accuracy/Precision/Recall/F1/ROC-AUC/PR-AUC)·회귀(MAE/RMSE/R²/MAPE)·시계열(sMAPE/MASE)·클러스터링(Silhouette) 지표, holdout vs CV 전략, 데이터 누수 방지, threshold 최적화, baseline 회귀 감지, 통계적 유의성 검정. 평가 코드 작성·지표 검증·모델 비교 작업 시 사용.
triggers:
  - confusion matrix
  - accuracy
  - precision
  - recall
  - f1 score
  - roc auc
  - pr auc
  - mae
  - rmse
  - 평가지표
  - evaluation metrics
  - classification report
  - model comparison
  - threshold tuning
---

# ML Evaluation Guidelines

## Purpose

모델 성능을 **과적합 없이·태스크에 적합한 지표로·통계적 유의성까지** 검증하기 위한 표준 절차.

## When to Use

- 학습 후 holdout 평가
- 모델 후보 비교 (A/B)
- 운영 중 성능 회귀 감지
- 평가 리포트 작성

---

## 1. 평가 태스크별 지표 선택

### 1.1 이진 분류

| 지표 | 언제 쓰는가 |
|---|---|
| **Accuracy** | 클래스 균형, 비용 동일 |
| **Precision** | False Positive 비용 높음 (스팸 필터) |
| **Recall (Sensitivity)** | False Negative 비용 높음 (질병 진단) |
| **F1** | P/R 균형 필요 |
| **ROC-AUC** | 랭킹 품질, 불균형 둔감 |
| **PR-AUC** | 극단 불균형 (positive 희소) |
| **Balanced Accuracy** | 불균형에서 Accuracy 대체 |
| **Matthews Correlation (MCC)** | 불균형 클래스 통합 지표 |
| **Log Loss / Brier** | 확률 보정(calibration) 평가 |

### 1.2 다중 클래스

- `accuracy`, `f1_macro` (클래스 동등), `f1_weighted` (샘플 비율)
- Confusion Matrix (시각화 필수)
- Per-class Precision/Recall

### 1.3 회귀

| 지표 | 해석 |
|---|---|
| **MAE** | 평균 절대 오차 (해석 쉬움) |
| **RMSE** | 큰 오차에 민감 |
| **R²** | 설명 분산 비율 (상대 지표) |
| **MAPE** | % 기반 (0 근처 값 위험) |
| **sMAPE** | MAPE 대칭 버전 |
| **Pinball Loss** | 분위 회귀(Quantile) |

### 1.4 시계열 예측

- **MASE** (Mean Absolute Scaled Error) — naive baseline 대비
- **sMAPE** — 대칭 MAPE
- **ND / NRMSE** — 정규화된 오차
- **WAPE** — 가중 APE (수요 예측 표준)

### 1.5 클러스터링 / 이상 탐지

- Silhouette, Davies-Bouldin, Calinski-Harabasz (라벨 없음)
- Adjusted Rand Index (ARI), NMI (라벨 있음)
- 이상 탐지: PR-AUC, **경보 정밀도/재현율**, MTTD (Mean Time To Detect)

### 1.6 HERE:O 태스크별 권장

| 태스크 | 주지표 | 보조지표 |
|---|---|---|
| 변화 탐지 (이상) | **PR-AUC, Recall@FPR=1%** | MTTD, False Alarm Rate |
| 인원수 분류 (0/1/2/3+) | **F1-macro** | Confusion Matrix, Accuracy |
| 호흡 추정 (BPM) | **MAE**, BPM 단위 | ±2 BPM tolerance accuracy |
| 자세 분류 | **F1-macro** | Per-class Precision/Recall |

---

## 2. 평가 코드 표준 템플릿 (sklearn)

```python
from sklearn.metrics import (
    classification_report, confusion_matrix,
    roc_auc_score, average_precision_score,
    precision_recall_curve, roc_curve,
    matthews_corrcoef,
)
import matplotlib.pyplot as plt
import numpy as np

def evaluate_classifier(y_true, y_prob, threshold=0.5, report_dir="reports"):
    y_pred = (y_prob >= threshold).astype(int)

    metrics = {
        "accuracy": (y_true == y_pred).mean(),
        "auc_roc": roc_auc_score(y_true, y_prob),
        "auc_pr": average_precision_score(y_true, y_prob),
        "mcc": matthews_corrcoef(y_true, y_pred),
    }

    # Classification report
    report = classification_report(y_true, y_pred, output_dict=True, digits=4)
    metrics.update({
        "precision": report["1"]["precision"],
        "recall": report["1"]["recall"],
        "f1": report["1"]["f1-score"],
    })

    # Confusion matrix
    cm = confusion_matrix(y_true, y_pred)
    _plot_cm(cm, f"{report_dir}/confusion_matrix.png")

    # ROC / PR curves
    _plot_roc(y_true, y_prob, f"{report_dir}/roc.png")
    _plot_pr(y_true, y_prob, f"{report_dir}/pr.png")

    return metrics
```

---

## 3. Threshold 최적화

확률 출력 모델은 기본 `0.5` 임계값이 최적이 아니다.

```python
def best_threshold_f1(y_true, y_prob):
    prec, rec, thr = precision_recall_curve(y_true, y_prob)
    f1 = 2 * prec * rec / (prec + rec + 1e-9)
    best_idx = f1.argmax()
    return thr[best_idx], f1[best_idx]

# FPR 제약 하 최대 Recall (이상 탐지 패턴)
def recall_at_fpr(y_true, y_prob, target_fpr=0.01):
    fpr, tpr, thr = roc_curve(y_true, y_prob)
    idx = np.where(fpr <= target_fpr)[0]
    if len(idx) == 0:
        return 0.0, 1.0
    best = idx[-1]
    return tpr[best], thr[best]
```

**주의:** Threshold는 **validation set**에서 튜닝, **test set**에서 최종 평가.

---

## 4. 교차검증 기반 평가 (분산 포함)

```python
from sklearn.model_selection import cross_validate
from sklearn.model_selection import GroupKFold

scoring = ["accuracy", "f1_macro", "roc_auc_ovr"]
cv = GroupKFold(n_splits=5)
results = cross_validate(
    model, X, y, groups=session_ids,
    cv=cv, scoring=scoring, n_jobs=-1,
    return_train_score=True,
)

for k in scoring:
    val = results[f"test_{k}"]
    print(f"{k}: {val.mean():.4f} ± {val.std():.4f}")
```

**보고 형식:** `0.82 ± 0.03 (5-fold GroupKFold)`

---

## 5. 데이터 누수 검증 체크리스트

평가 전 반드시 확인:

- [ ] **Test set 분리 시점** — 모든 전처리·피처 선택 **전**에 분리했는가?
- [ ] **그룹 누수** — 동일 session/user가 train과 test에 존재하지 않는가?
- [ ] **시간 누수** — 미래 정보가 과거 피처에 들어가지 않았는가?
- [ ] **타깃 누수** — 피처 중 타깃 파생값은 없는가?
- [ ] **정규화 누수** — scaler가 test 포함 데이터로 fit 되지 않았는가?
- [ ] **오버샘플링 누수** — SMOTE가 CV 외부에서 실행되지 않았는가?
- [ ] **중복 샘플 누수** — 동일 샘플이 train과 test에 존재하지 않는가?

자동 점검:
```python
# (예) session_id 누수 검증
assert len(set(train_sessions) & set(test_sessions)) == 0

# (예) 타임스탬프 누수 검증
assert train["timestamp"].max() < test["timestamp"].min()
```

---

## 6. 통계적 유의성 (모델 A vs B)

### 6.1 McNemar 검정 (분류, 동일 test set)

```python
from statsmodels.stats.contingency_tables import mcnemar

# a: A만 맞춤, b: A틀림·B맞춤, c: A맞춤·B틀림, d: 둘다 틀림
tbl = [[a, b], [c, d]]
res = mcnemar(tbl, exact=False, correction=True)
print(f"p-value: {res.pvalue:.4f}")
```

### 6.2 5x2 CV t-test (권장)

```python
# Dietterich 5x2 CV — 동일 데이터 5번 반복 2-fold
from mlxtend.evaluate import paired_ttest_5x2cv

t, p = paired_ttest_5x2cv(
    estimator1=model_a, estimator2=model_b,
    X=X, y=y, scoring="f1_macro", random_seed=42
)
print(f"t={t:.3f}, p={p:.4f}")  # p < 0.05 → 유의미한 차이
```

### 6.3 Bootstrap 신뢰구간

```python
from sklearn.utils import resample

def bootstrap_ci(y_true, y_pred, metric_fn, n=1000, alpha=0.05):
    scores = []
    n_samples = len(y_true)
    for _ in range(n):
        idx = resample(np.arange(n_samples))
        scores.append(metric_fn(y_true[idx], y_pred[idx]))
    low = np.percentile(scores, 100 * alpha / 2)
    high = np.percentile(scores, 100 * (1 - alpha / 2))
    return np.mean(scores), (low, high)
```

---

## 7. Calibration (확률 보정)

분류 확률을 직접 쓰는 경우(threshold 결정, 앙상블) calibration 확인.

```python
from sklearn.calibration import calibration_curve
import matplotlib.pyplot as plt

prob_true, prob_pred = calibration_curve(y_true, y_prob, n_bins=10)
plt.plot(prob_pred, prob_true, marker="o")
plt.plot([0,1],[0,1], "k--")
plt.xlabel("Predicted probability"); plt.ylabel("True frequency")
plt.savefig("reports/calibration.png")

# Brier Score
from sklearn.metrics import brier_score_loss
print("Brier:", brier_score_loss(y_true, y_prob))
```

**보정 기법:** `CalibratedClassifierCV(method="isotonic")` 또는 Platt scaling.

---

## 8. 회귀 평가 템플릿

```python
from sklearn.metrics import (
    mean_absolute_error, mean_squared_error, r2_score,
    mean_absolute_percentage_error,
)

def evaluate_regressor(y_true, y_pred):
    return {
        "mae":  mean_absolute_error(y_true, y_pred),
        "rmse": mean_squared_error(y_true, y_pred, squared=False),
        "r2":   r2_score(y_true, y_pred),
        "mape": mean_absolute_percentage_error(y_true, y_pred),
    }

# 잔차 플롯
residuals = y_true - y_pred
plt.scatter(y_pred, residuals, alpha=0.3)
plt.axhline(0, color="k", linestyle="--")
plt.xlabel("Predicted"); plt.ylabel("Residual")
```

---

## 9. Baseline & 회귀(Regression) 감지

### 9.1 반드시 측정할 Baseline

| Baseline | 의미 |
|---|---|
| **Dummy** (sklearn `DummyClassifier/Regressor`) | 무지성 전략 (최빈/평균) |
| **Simple rule** | 도메인 지식 기반 1-feature rule |
| **이전 버전 모델** | A/B 비교 |
| **Naive forecast** (시계열) | `y_{t-1}` |

### 9.2 회귀 감지 자동화

`baseline.json`을 커밋하고 CI에서 회귀 체크:

```json
{
  "model": "lgbm_v1",
  "metrics": {
    "f1_macro": {"value": 0.82, "tolerance": 0.02},
    "auc_roc":  {"value": 0.91, "tolerance": 0.01}
  }
}
```

```python
def check_regression(current, baseline):
    for k, spec in baseline["metrics"].items():
        if current[k] < spec["value"] - spec["tolerance"]:
            raise AssertionError(
                f"{k} regressed: {current[k]:.4f} < {spec['value']:.4f} - {spec['tolerance']}"
            )
```

---

## 10. 데이터 드리프트 (운영 중)

```python
from scipy.stats import ks_2samp
# 각 피처 분포 비교 (train vs prod)
for col in feature_cols:
    stat, p = ks_2samp(train[col], prod[col])
    if p < 0.01:
        print(f"DRIFT in {col}: KS={stat:.3f}")
```

**더 강력한 도구:** Evidently, WhyLabs, Alibi Detect.

---

## 11. 평가 리포트 포맷

`reports/eval_{model}_{date}.md`:

```markdown
# Evaluation Report — csi_lgbm_v1 (2026-04-20)

## 요약
- **주지표: F1-macro = 0.81 ± 0.02 (GroupKFold 5)**
- **Holdout F1-macro = 0.79**
- baseline 대비 +0.07 (DummyClassifier = 0.72)
- 이전 버전 대비 유의미한 향상 (McNemar p = 0.003)

## 분할 전략
- GroupKFold(session_id), 5 folds
- Holdout: 별도 공간에서 수집된 15% 세션

## 지표
| Metric | CV mean | CV std | Holdout |
|---|---|---|---|
| Accuracy | 0.84 | 0.02 | 0.82 |
| F1-macro | 0.81 | 0.02 | 0.79 |
| AUC-macro | 0.92 | 0.01 | 0.90 |

## 한계 및 주의
- Holdout 공간 크기 편향 가능 (18m² 고정)
- 2명 이상 상황 Recall 0.72 (개선 필요)

## 재현
- `uv run python ml/evaluate.py --run-id <mlflow_run>`
```

---

## 12. 안티패턴

- ❌ Accuracy만 보고하기 (불균형 클래스에서 무의미)
- ❌ Train set으로 threshold 튜닝
- ❌ Holdout을 여러 번 재사용 (의사결정에 반복 노출 = 누수)
- ❌ p-value만으로 모델 선택 (effect size도 고려)
- ❌ 계수가 소수점 6자리로 떨어지는 숫자 보고 (신뢰구간과 함께)
- ❌ 에러 분석 생략 (어떤 샘플이 틀렸는지 확인 필수)

---

## 13. 참고

- sklearn metrics: https://scikit-learn.org/stable/modules/model_evaluation.html
- Evidently: https://docs.evidentlyai.com/
- Forecasting metrics: Hyndman, "Forecasting: Principles and Practice"
- Calibration: https://scikit-learn.org/stable/modules/calibration.html
- mlxtend stat tests: http://rasbt.github.io/mlxtend/user_guide/evaluate/
