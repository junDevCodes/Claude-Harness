# Bootstrap — git-workflow-guidelines 첫 실행 세팅 상세

SKILL.md의 "🚀 Bootstrap" 섹션 요약본을 상세 명령·판정 로직과 함께 확장.
`git-workflow-expert` 에이전트가 `.claude/config/git.yaml` 부재 감지 시 이 절차를 실행.

---

## Step 1 — git remote / repo 검증

```bash
# 레포 내부인가?
git rev-parse --is-inside-work-tree 2>/dev/null || { echo "NOT_A_REPO"; exit 1; }

# 원격 존재?
git remote -v | head -1 | grep -q . || echo "NO_REMOTE"
```

실패 처리:
- `NOT_A_REPO` → 사용자에게 `git init` 실행 안내 + 재시도
- `NO_REMOTE` → `git remote add origin <url>` 안내 (또는 로컬 전용 모드 확인 질문)

---

## Step 2 — base branch 자동 감지

**우선순위:**

1. 원격 HEAD 심볼릭 레퍼런스:
   ```bash
   git symbolic-ref refs/remotes/origin/HEAD 2>/dev/null | sed 's@^refs/remotes/origin/@@'
   ```

2. 주요 브랜치 존재 확인 (순서대로 첫 발견 우승):
   ```bash
   for b in main master dev develop; do
     git ls-remote --heads origin "$b" 2>/dev/null | grep -q "$b" && echo "$b" && break
   done
   ```

3. 현재 로컬 브랜치:
   ```bash
   git branch --show-current
   ```

**사용자 확정 템플릿:**
```
감지된 base branch: <value>  (source: symbolic-ref | ls-remote | local)
이 값으로 사용할까요? (Y / 다른 값 직접 입력)
```

---

## Step 3 — Conventional Commits 준수율 분석

```bash
git log --oneline -50 --pretty=format:"%s" 2>/dev/null | \
  awk '
    /^(feat|fix|refactor|perf|docs|style|test|chore|env|plan|design|hotfix|release)(\([^)]+\))?(!)?:/ { c++ }
    END {
      total = NR
      if (total == 0) { print "NO_COMMITS"; exit }
      ratio = c / total
      printf "%d/%d (%.0f%%)\n", c, total, ratio * 100
    }
  '
```

**판정 → `commit.conventional` 값:**

| 준수율 | conventional | 효과 |
|---|---|---|
| ≥ 80% | `strict` | 위반 시 커밋 차단 (commitlint 권고) |
| 40% ~ 80% | `warn` | 경고 표시, 커밋 허용 |
| < 40% | `off` | 컨벤션 미도입 환경 — 도입 제안만, 강제 없음 |
| NO_COMMITS | `strict` | 신규 레포는 strict로 시작 |

---

## Step 4 — 브랜치 네이밍 패턴 수집

```bash
git branch -r --format='%(refname:short)' 2>/dev/null | \
  sed 's@^origin/@@' | grep -v "HEAD" | \
  awk -F/ 'NF > 1 {print $1}' | sort | uniq -c | sort -rn | head -10
```

출력 예시:
```
     12 feat
      8 fix
      3 chore
      2 refactor
      1 hotfix
```

→ 상위 prefix를 `types_allowed` 초기값으로 제안 + 표준 리스트에서 누락된 항목 추가 제안.

---

## Step 5 — 전략·Issue Tracker 질문

**순서대로 제시:**

```
Q1. 브랜치 전략?
   1: GitHub Flow (main + 단명 feature)  [default]
   2: GitFlow (main + develop + release/hotfix)
   3: Trunk-based (main 중심, feature flag 전제)
   4: Feature Branch (main + 장기 feature)
   선택:

Q2. Issue Tracker 연계?
   1: Jira
   2: GitHub Issue
   3: 없음
   선택:
   → Jira 선택 시 jira-workflow-guidelines Bootstrap 자동 체이닝

Q3. 기본 머지 정책?
   - squash  (각 PR 1 커밋 — 깔끔, 히스토리 단순)  [default]
   - merge   (머지 커밋 보존 — 기능 그룹 추적 가능)
   - rebase  (linear history 강제)
   선택:

Q4. main 브랜치 force-push 차단? (Y/N) [default: Y]
```

---

## Step 6 — `.claude/config/git.yaml` 생성

SKILL.md §Bootstrap의 템플릿 그대로 사용. Step 1~5 결과를 주석 블록 헤더에 기록:

```yaml
# .claude/config/git.yaml
# Generated: <ISO date>
# Detected: base_branch=<v>, conventional=<strict|warn|off>, types=<v>
# User choices: strategy=<v>, tracker=<v>, merge=<v>
```

---

## Step 7 — 시험 호출

**유효성 검증:**

```bash
# 1. base_branch 실존?
git ls-remote --heads origin "$(yq '.base_branch' .claude/config/git.yaml)" | grep -q .

# 2. YAML 파싱 OK?
python -c "import yaml; yaml.safe_load(open('.claude/config/git.yaml'))" 2>&1

# 3. strategy 허용값?
case "$(yq '.strategy' .claude/config/git.yaml)" in
  github-flow|gitflow|trunk-based|feature-branch) echo OK ;;
  *) echo "INVALID_STRATEGY" ;;
esac
```

모두 통과 시 Bootstrap 완료 보고 + 원래 요청 재개.

---

## Idempotency

- `.claude/config/git.yaml`이 이미 존재하면 Bootstrap 건너뛰기 (재실행 방지)
- `--force` 또는 사용자가 "재세팅" 명시적 요청 시에만 덮어쓰기
- 덮어쓰기 전에 기존 파일 `.claude/config/git.yaml.bak` 으로 백업

---

## 실패 시 대응

| 실패 | 대응 |
|---|---|
| git 미설치 | 에이전트 중단 + git 설치 안내 |
| 원격 없음 | Q: 로컬 전용 모드로 진행할지 질문 → YES면 skip Step 2 1·2순위, 3순위만 사용 |
| 커밋 50개 미만 | 있는 만큼 분석, NO_COMMITS면 `strict` 기본 |
| yq 미설치 | Python으로 대체: `python -c "import yaml; print(yaml.safe_load(open('...'))['base_branch'])"` |
