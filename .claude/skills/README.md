# Skills

Production-tested skills for Claude Code that auto-activate based on context.

---

## What Are Skills?

Skills are modular knowledge bases that Claude loads when needed. They provide:
- Domain-specific guidelines
- Best practices
- Code examples
- Anti-patterns to avoid

**Problem:** Skills don't activate automatically by default.

**Solution:** This showcase includes the hooks + configuration to make them activate.

---

## Available Skills (32개)

### 웹 백엔드 스킬 (6개)

| 스킬 | 대상 스택 | 줄수 |
|---|---|---|
| `fastapi-backend-guidelines` | FastAPI + SQLModel + async | 436 |
| `express-backend-guidelines` | Express.js + TypeScript + Prisma | 500 |
| `django-backend-guidelines` | Django 5 + DRF + SimpleJWT | 500 |
| `nestjs-backend-guidelines` | NestJS 10 + TypeScript + Prisma | 486 |
| `spring-boot-guidelines` | Spring Boot 3 + Java 21 + JPA | 499 |
| `pytest-backend-testing` | pytest + FastAPI TestClient | 403 |

### 프론트엔드 스킬 (6개)

| 스킬 | 대상 스택 | 줄수 |
|---|---|---|
| `nextjs-frontend-guidelines` | Next.js 15 + React 19 + App Router | 415 |
| `react-vite-pwa-guidelines` | React 19 + Vite + PWA + React Router v6 (T3 스켈레톤) | ~340 |
| `threejs-realtime-guidelines` | Three.js + R3F 실시간 3D 시각화 (T3 스켈레톤) | ~360 |
| `vercel-react-best-practices` | React/Next.js 성능 최적화 | 365 |
| `web-design-guidelines` | UI/UX 접근성 감사 | 295 |
| `frontend-design` | 프로덕션 UI 컴포넌트 제작 | 292 |

### 모바일 + 임베디드 스킬 (3개)

| 스킬 | 대상 스택 | 줄수 |
|---|---|---|
| `react-native-guidelines` | Expo SDK 52 + Expo Router + Zustand | 500 |
| `embedded-c-guidelines` | C99 + HAL Ops Table + FreeRTOS + Unity | 497 |
| `embedded-cpp-guidelines` | C++17 + 순수 가상 HAL + RAII + Google Test | 439 |

### 공통 인프라 스킬 (3개)

| 스킬 | 목적 | 줄수 |
|---|---|---|
| `docker-guidelines` | 멀티 스테이지 빌드, non-root, docker-compose | 456 |
| `aws-guidelines` | S3 presigned URL, ECS, ECR, IAM, CloudWatch | 459 |
| `database-guidelines` | ERD, 정규화, 인덱스, 읽기/쓰기 분리, 마이그레이션 | 441 |

### 문서/다이어그램 스킬 (4개)

| 스킬 | 목적 | 줄수 |
|---|---|---|
| `mermaid` | 다이어그램 / 플로우차트 / ERD 생성 | 212 |
| `pdf` | PDF 생성/편집/추출/변환 | 347 |
| `pptx` | PowerPoint 프레젠테이션 제작 | 500 |
| `docx` | Word 문서 제작/편집 | 486 |

### 브랜드 + 에러 추적 스킬 (3개)

| 스킬 | 목적 | 줄수 |
|---|---|---|
| `brand-guidelines` | Anthropic 브랜드 색상/타이포그래피 | 243 |
| `ppt-brand-guidelines` | VRL PPT 브랜드 가이드라인 | 267 |
| `error-tracking` | Sentry v8 에러 추적/성능 모니터링 | 404 |

### 데이터 / AI 스킬 (4개) — v1.5 신규

| 스킬 | 목적 | 줄수 |
|---|---|---|
| `data-analysis-guidelines` | pandas/polars EDA, 결측/이상치, 기술통계, 상관 분석, 시각화 | ~260 |
| `data-pipeline-guidelines` | Batch/Stream, Airflow/Prefect/Dagster, 메달리온(Bronze/Silver/Gold), Great Expectations, idempotency | ~330 |
| `ml-training-guidelines` | sklearn/LightGBM/PyTorch 단계별, MLflow, Optuna, GroupKFold, 재현성 | ~370 |
| `ml-evaluation-guidelines` | 분류/회귀/시계열 지표, 누수 방지, threshold 최적화, McNemar, baseline 회귀 감지 | ~370 |

### 🤝 협업 스킬 (2개) — v1.7 신규 (Phase 15-A)

| 스킬 | 목적 | 줄수 |
|---|---|---|
| `git-workflow-guidelines` | Conventional Commits, GitFlow vs Trunk-based vs Feature Branch 선택, 브랜치 네이밍 + Jira 이슈 키 연계, Rebase/Merge/Squash 전략, Destructive 작업 안전 게이트 | ~390 |
| `jira-workflow-guidelines` | Epic/Task 생성 컨벤션 검증, 시안 사전 승인 정책, 프로젝트별 커스터마이징(PROJECT_KEY/ASSIGNEE/COMPONENT) 가이드, Jira CLI/MCP/REST 4-tier fallback 설계 (agents의 SSOT) | ~330 |

### 메타 스킬 (1개)

| 스킬 | 목적 | 줄수 |
|---|---|---|
| `skill-developer` | 스킬 생성/관리/트리거 설계 | 498 |

---

## How to Add a Skill to Your Project

### Quick Integration

**For Claude Code:**
```
User: "Add the nestjs-backend-guidelines skill to my project"

Claude should:
1. Ask about project structure
2. Copy skill directory
3. Update skill-rules.json with their paths
4. Verify integration
```

See [CLAUDE_INTEGRATION_GUIDE.md](../../CLAUDE_INTEGRATION_GUIDE.md) for complete instructions.

### Manual Integration

**Step 1: Copy the skill directory**
```bash
cp -r advanced-harness-window/.claude/skills/nestjs-backend-guidelines \
      your-project/.claude/skills/
```

**Step 2: Update skill-rules.json**

If you don't have one, create it:
```bash
cp advanced-harness-window/.claude/skills/skill-rules.json \
   your-project/.claude/skills/
```

Then customize the `pathPatterns` for your project:
```json
{
  "skills": {
    "nestjs-backend-guidelines": {
      "fileTriggers": {
        "pathPatterns": [
          "src/**/*.ts"  // ← Update this!
        ]
      }
    }
  }
}
```

**Step 3: Test**
- Edit a file in your backend directory
- The skill should activate automatically

---

## skill-rules.json Configuration

### What It Does

Defines when skills should activate based on:
- **Keywords** in user prompts ("nestjs", "docker", "s3 upload")
- **Intent patterns** (regex matching user intent)
- **File path patterns** (editing backend files)
- **Content patterns** (code contains specific imports)

### Configuration Format

```json
{
  "skill-name": {
    "type": "domain" | "guardrail",
    "enforcement": "suggest" | "block",
    "priority": "high" | "medium" | "low",
    "promptTriggers": {
      "keywords": ["list", "of", "keywords"],
      "intentPatterns": ["regex patterns"]
    },
    "fileTriggers": {
      "pathPatterns": ["path/to/files/**/*.ts"],
      "contentPatterns": ["import.*Prisma"]
    }
  }
}
```

### Enforcement Levels

- **suggest**: Skill appears as suggestion, doesn't block
- **block**: Must use skill before proceeding (guardrail)

**Use "block" for:**
- Preventing breaking changes (Next.js App Router vs Pages Router)
- Critical security-sensitive code patterns

**Use "suggest" for:**
- General best practices
- Domain guidance
- Code organization

---

## Creating Your Own Skills

See the **skill-developer** skill for complete guide on:
- Skill YAML frontmatter structure
- Resource file organization
- Trigger pattern design
- Testing skill activation

**Quick template:**
```markdown
---
name: my-skill
description: What this skill does
triggers:
  - keyword1
  - keyword2
---

# My Skill Title

## Purpose
[Why this skill exists]

## When to Use This Skill
[Auto-activation scenarios]

## Core Patterns
[Key patterns with code examples]

## Anti-Patterns
[❌ What NOT to do]
```

---

## Troubleshooting

### Skill isn't activating

**Check:**
1. Is skill directory in `.claude/skills/`?
2. Is skill listed in `skill-rules.json`?
3. Do `pathPatterns` match your files?
4. Are hooks installed and working?

**Debug:**
```bash
# Check skills exist
ls -la .claude/skills/

# Validate skill-rules.json
cat .claude/skills/skill-rules.json | python -m json.tool

# Check hooks are executable
ls -la .claude/hooks/
```

### Skill activates too often

Update skill-rules.json:
- Make keywords more specific
- Narrow `pathPatterns`
- Increase specificity of `intentPatterns`

### Skill never activates

Update skill-rules.json:
- Add more keywords
- Broaden `pathPatterns`
- Add more `intentPatterns`

---

## For Claude Code

**When integrating a skill for a user:**

1. **Read [CLAUDE_INTEGRATION_GUIDE.md](../../CLAUDE_INTEGRATION_GUIDE.md)** first
2. Ask about their project structure
3. Customize `pathPatterns` in skill-rules.json
4. Verify the skill file has no hardcoded paths
5. Test activation after integration

**Common mistakes:**
- Keeping example paths (base/nestjs/, base/express/)
- Not asking about monorepo vs single-app
- Copying skill-rules.json without customization

---

*Last updated: 2026-04-13 (Phase 12-B 1차 적용 — react-vite-pwa / threejs-realtime 2개 역전파, 총 26개, skill-rules.json v1.4)*
