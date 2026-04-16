---
name: ml-training-guidelines
description: AI/ML 모델 학습·구축 가이드라인. scikit-learn / LightGBM / PyTorch 단계별 선택, MLflow 실험 추적, Optuna 하이퍼파라미터 튜닝, 교차검증, 재현성(seed/requirements), 클래스 불균형 처리. 모델 학습 코드 작성, 실험 관리, 모델 서빙 준비 작업 시 사용.
triggers:
  - scikit-learn
  - sklearn
  - lightgbm
  - xgboost
  - pytorch
  - torch
  - mlflow
  - optuna
  - 모델 학습
  - 머신러닝
  - machine learning
  - train model
  - hyperparameter tuning
  - feature engineering
---

# ML Training Guidelines

## Purpose

재현 가능하고 추적 가능한 ML 모델 학습 파이프라인을 구축하기 위한 표준 패턴. HERE:O는 scikit-learn → LightGBM → PyTorch로 단계별 확장한다.

## When to Use

- 새 ML 모델 학습 시작
- 실험 추적 · 하이퍼파라미터 튜닝
- 모델 레지스트리·서빙 준비
- 재학습 파이프라인 자동화

---

## 1. 프레임워크 선택 매트릭스

| 상황 | 권장 프레임워크 |
|---|---|
| Baseline / 해석 가능 모델 필요 | **scikit-learn** (LogReg, RF, SVM) |
| Tabular 데이터, 대규모, 빠른 학습 | **LightGBM** (또는 XGBoost, CatBoost) |
| 시계열 신호 / 이미지 / NLP | **PyTorch** (또는 PyTorch Lightning) |
| 클래스 불균형 극심 | LightGBM `is_unbalance=True` 또는 focal loss (torch) |

**HERE:O 단계 적용:**
```
Phase M1 (변화 탐지)     → sklearn IsolationForest + LogReg
Phase M2 (인원수 분류)    → LightGBM on FFT features
Phase M3 (호흡 검출)     → PyTorch CNN/LSTM on CSI sequences
Phase M4 (자세 분류)     → PyTorch Transformer 또는 LightGBM
```

---

## 2. 프로젝트 구조 표준

```
csi/
├── ml/
│   ├── configs/           # Hydra YAML 설정
│   │   ├── model/lgbm.yaml
│   │   ├── data/csi_v1.yaml
│   │   └── experiment/exp_001.yaml
│   ├── datasets/          # Dataset 정의
│   ├── features/          # 특징 추출 (pipeline-guidelines와 공유)
│   ├── models/            # 모델 정의 (torch nn.Module)
│   ├── train/             # 학습 루프
│   │   ├── sklearn_trainer.py
│   │   ├── lgbm_trainer.py
│   │   └── torch_trainer.py
│   ├── tune/              # Optuna 튜닝
│   ├── serve/             # 추론 래퍼 (FastAPI 연동)
│   └── registry/          # MLflow registry 관리
├── experiments/           # MLflow runs (로컬)
└── mlruns/                # MLflow backend
```

---

## 3. 재현성 (Reproducibility) — 필수

```python
# 1) seed 고정 (numpy, torch, sklearn, random, cuda)
import os, random, numpy as np, torch

def set_seed(seed: int = 42):
    os.environ["PYTHONHASHSEED"] = str(seed)
    random.seed(seed)
    np.random.seed(seed)
    torch.manual_seed(seed)
    torch.cuda.manual_seed_all(seed)
    torch.backends.cudnn.deterministic = True
    torch.backends.cudnn.benchmark = False

# 2) requirements pinning
# pyproject.toml에 정확한 버전 고정 (uv lock 또는 pip-compile)

# 3) 데이터 버전 — DVC 또는 경로에 hash 포함
# data/processed/csi_v1_sha256-abc123.parquet

# 4) 코드 버전 — git commit hash를 MLflow tag로
import subprocess
commit = subprocess.check_output(["git", "rev-parse", "HEAD"]).strip().decode()
mlflow.set_tag("git_commit", commit)
```

---

## 4. MLflow 실험 추적 (HERE:O 기본값)

### 4.1 설정

```python
# csi/ml/train/base.py
import mlflow
import mlflow.sklearn
import mlflow.lightgbm
import mlflow.pytorch

mlflow.set_tracking_uri("http://mlflow:5000")  # Docker compose 서비스
mlflow.set_experiment("csi-occupancy")
```

### 4.2 공통 Run 포맷

```python
with mlflow.start_run(run_name=f"lgbm_{config.exp_id}") as run:
    # 파라미터
    mlflow.log_params(config.model.params)
    mlflow.log_param("data_version", config.data.version)
    mlflow.log_param("feature_count", X_train.shape[1])
    mlflow.set_tag("git_commit", get_git_commit())
    mlflow.set_tag("stage", "dev")

    # 학습
    model.fit(X_train, y_train)

    # 메트릭 (학습/검증)
    for epoch, m in enumerate(history):
        mlflow.log_metrics({"train_loss": m.train, "val_loss": m.val}, step=epoch)

    mlflow.log_metrics({
        "val_f1": f1, "val_auc": auc, "val_accuracy": acc
    })

    # 아티팩트
    mlflow.log_artifact("reports/confusion_matrix.png")
    mlflow.log_artifact("reports/feature_importance.png")

    # 모델 저장 (모델 시그니처 포함)
    from mlflow.models.signature import infer_signature
    sig = infer_signature(X_train, model.predict(X_train))
    mlflow.lightgbm.log_model(model, "model", signature=sig, input_example=X_train[:5])
```

---

## 5. scikit-learn Trainer 템플릿

```python
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import StratifiedKFold, cross_val_score

def train_sklearn(X, y, cfg):
    pipe = Pipeline([
        ("scaler", StandardScaler()),
        ("clf", LogisticRegression(
            C=cfg.C, max_iter=1000, class_weight="balanced", random_state=42
        )),
    ])
    cv = StratifiedKFold(n_splits=5, shuffle=True, random_state=42)
    scores = cross_val_score(pipe, X, y, cv=cv, scoring="f1_macro", n_jobs=-1)
    pipe.fit(X, y)
    return pipe, scores
```

---

## 6. LightGBM Trainer 템플릿

```python
import lightgbm as lgb

def train_lgbm(X_tr, y_tr, X_val, y_val, cfg):
    train_set = lgb.Dataset(X_tr, label=y_tr)
    val_set   = lgb.Dataset(X_val, label=y_val, reference=train_set)

    params = dict(
        objective="multiclass" if cfg.n_classes > 2 else "binary",
        num_class=cfg.n_classes if cfg.n_classes > 2 else 1,
        metric="multi_logloss" if cfg.n_classes > 2 else "binary_logloss",
        learning_rate=cfg.lr,
        num_leaves=cfg.num_leaves,
        min_data_in_leaf=cfg.min_data_in_leaf,
        feature_fraction=0.9,
        bagging_fraction=0.8,
        bagging_freq=5,
        is_unbalance=cfg.is_unbalance,
        seed=42,
        verbose=-1,
    )
    model = lgb.train(
        params, train_set,
        num_boost_round=5000,
        valid_sets=[train_set, val_set],
        valid_names=["train", "val"],
        callbacks=[
            lgb.early_stopping(stopping_rounds=100),
            lgb.log_evaluation(period=100),
        ],
    )
    return model
```

---

## 7. PyTorch Trainer 템플릿

```python
import torch
import torch.nn as nn
from torch.utils.data import DataLoader

class Trainer:
    def __init__(self, model, optimizer, scheduler, loss_fn, device):
        self.model = model.to(device)
        self.opt = optimizer
        self.sched = scheduler
        self.loss_fn = loss_fn
        self.device = device
        self.best_val = float("inf")

    def fit(self, train_loader, val_loader, epochs, patience=10, ckpt_path="best.pt"):
        bad = 0
        for epoch in range(epochs):
            tr_loss = self._train_epoch(train_loader)
            val_loss = self._eval_epoch(val_loader)
            self.sched.step(val_loss)
            mlflow.log_metrics({"train_loss": tr_loss, "val_loss": val_loss}, step=epoch)

            if val_loss < self.best_val:
                self.best_val = val_loss
                torch.save(self.model.state_dict(), ckpt_path)
                bad = 0
            else:
                bad += 1
                if bad >= patience:
                    break

    def _train_epoch(self, loader):
        self.model.train()
        total = 0.0
        for xb, yb in loader:
            xb, yb = xb.to(self.device), yb.to(self.device)
            self.opt.zero_grad()
            loss = self.loss_fn(self.model(xb), yb)
            loss.backward()
            torch.nn.utils.clip_grad_norm_(self.model.parameters(), 1.0)
            self.opt.step()
            total += loss.item() * xb.size(0)
        return total / len(loader.dataset)

    @torch.no_grad()
    def _eval_epoch(self, loader):
        self.model.eval()
        total = 0.0
        for xb, yb in loader:
            xb, yb = xb.to(self.device), yb.to(self.device)
            total += self.loss_fn(self.model(xb), yb).item() * xb.size(0)
        return total / len(loader.dataset)
```

### 7.1 PyTorch Lightning 대안

PyTorch Lightning은 위 보일러플레이트를 제거한다. **복잡한 학습 루프(GAN, 다중 GPU)에는 Lightning 권장**.

---

## 8. 하이퍼파라미터 튜닝 (Optuna)

```python
import optuna
from optuna.integration.mlflow import MLflowCallback

def objective(trial):
    params = {
        "learning_rate": trial.suggest_float("lr", 1e-3, 0.3, log=True),
        "num_leaves": trial.suggest_int("num_leaves", 15, 255),
        "min_data_in_leaf": trial.suggest_int("min_data_in_leaf", 5, 100),
        "feature_fraction": trial.suggest_float("ff", 0.6, 1.0),
    }
    model = train_lgbm_with(X_tr, y_tr, X_val, y_val, params)
    return model.best_score["val"]["binary_logloss"]

study = optuna.create_study(
    direction="minimize",
    sampler=optuna.samplers.TPESampler(seed=42),
    pruner=optuna.pruners.MedianPruner(),
)
study.optimize(
    objective,
    n_trials=100,
    callbacks=[MLflowCallback(tracking_uri="http://mlflow:5000", metric_name="val_logloss")],
)
```

**튜닝 예산 원칙:** 기본 100 trials로 시작, 수렴 확인 후 조정.

---

## 9. 교차검증 전략

| 데이터 특성 | 권장 CV |
|---|---|
| 독립 샘플 | `StratifiedKFold` (분류) / `KFold` (회귀) |
| 시간 의존 | `TimeSeriesSplit` |
| 그룹 누수 위험 (동일 세션 여러 샘플) | `GroupKFold` (group=session_id) |
| HERE:O CSI | **`GroupKFold(session_id)` 필수** — 동일 세션이 train·val 분산 금지 |

```python
from sklearn.model_selection import GroupKFold
cv = GroupKFold(n_splits=5)
for tr_idx, val_idx in cv.split(X, y, groups=session_ids):
    ...
```

---

## 10. 클래스 불균형 처리

| 기법 | 언제 |
|---|---|
| `class_weight="balanced"` (sklearn) | 기본값 |
| `is_unbalance=True` 또는 `scale_pos_weight` (LGBM) | 기본값 |
| Focal Loss (torch) | 극심한 불균형 (>1:100) |
| SMOTE / 언더샘플링 | 최후 수단 (데이터 누수 위험) |
| Threshold 조정 | 평가 단계에서 (precision/recall trade-off) |

**주의:** 오버샘플링은 CV fold 내부에서만 수행 (fold 외부 수행 시 누수).

---

## 11. 학습 체크리스트 (Pre-training)

- [ ] 데이터 분할 방식이 누수 없이 정의되었는가? (시간/그룹 고려)
- [ ] Target leak 검토 (feature에 타깃 파생값이 없는가?)
- [ ] 정규화·인코더가 `fit_transform` → train에서만
- [ ] 클래스 분포 확인 후 불균형 기법 선택
- [ ] baseline 모델 점수 측정 (DummyClassifier, LogReg)
- [ ] seed 고정, 재실행 시 동일 결과 확인
- [ ] MLflow run 시작, params·tags 로깅
- [ ] early stopping 설정

---

## 12. 학습 후 체크리스트

- [ ] train/val/test 메트릭 모두 기록
- [ ] Confusion matrix, ROC, PR-curve 저장
- [ ] Feature importance 저장 (해석)
- [ ] 모델 signature + input example 함께 저장
- [ ] `evaluation.py`로 holdout 평가 별도 수행 (`ml-evaluation-guidelines` 참조)
- [ ] 모델 카드 작성 (아래 10장 참조)

---

## 13. 모델 카드 (Model Card)

`models/{model_name}/MODEL_CARD.md`로 함께 저장.

```markdown
# Model Card — csi_occupancy_lgbm_v1

## 개요
- 태스크: 방 내부 인원수 분류 (0/1/2/3+)
- 학습일: 2026-04-20
- 데이터: csi_v1 (sessions=532, frames=1.2M)
- 분할: GroupKFold(session_id), n=5

## 성능
| Split | Accuracy | F1(macro) | AUC(macro) |
|---|---|---|---|
| CV mean | 0.84 | 0.81 | 0.92 |
| Holdout | 0.82 | 0.79 | 0.90 |

## 한계
- 학습 환경: 18m² 방, 1노드
- 일반화 미검증: 다른 공간 크기, 가구 배치

## 사용 주의
- 입력 피처 스키마 엄격히 일치
- 재학습 주기: 월 1회 또는 drift 발생 시
```

---

## 14. 안티패턴

- ❌ test set으로 하이퍼파라미터 튜닝
- ❌ seed 미고정 상태로 모델 비교
- ❌ Jupyter notebook 내 실험 저장 (MLflow 누락)
- ❌ 오버샘플링을 CV 외부에서 수행 (누수)
- ❌ EarlyStopping 없이 무한 학습
- ❌ 모델 저장 시 전처리 파이프라인 분리 (서빙 불가)
- ❌ GPU OOM 시 배치 크기 맹목적 감소 (gradient accumulation 고려)

---

## 15. 참고

- scikit-learn: https://scikit-learn.org/stable/user_guide.html
- LightGBM: https://lightgbm.readthedocs.io/
- PyTorch: https://pytorch.org/docs/
- MLflow: https://mlflow.org/docs/latest/
- Optuna: https://optuna.readthedocs.io/
- PyTorch Lightning: https://lightning.ai/docs/pytorch/stable/
