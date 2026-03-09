# Claude Integration Guide

**FOR CLAUDE CODE:** When a user asks you to integrate components from this showcase repository into their project, follow these instructions carefully.

---

## Overview

This repository is a **reference library** of Claude Code infrastructure components. Users will ask you to help integrate specific pieces into their projects. Your role is to:

1. **Ask clarifying questions** about their project structure
2. **Copy the appropriate files**
3. **Customize configurations** for their setup
4. **Verify the integration** works correctly

**Key Principle:** ALWAYS ask before assuming project structure. What works for one project won't work for another.

---

## 적용 시나리오 판별 (신규 vs 기존 프로젝트)

사용자가 하네스 적용을 요청하면 **먼저 현재 프로젝트 상태를 확인**합니다:

```bash
ls .claude/ 2>/dev/null && echo "기존 모드" || echo "신규 모드"
```

### 신규 프로젝트 모드 (`.claude/` 없음)

→ `.claude/` 전체 복사 후 설정. `HARNESS_USAGE_GUIDE.md` 1부를 따릅니다.

```bash
cp -r [harness_path]/.claude ./
chmod +x .claude/hooks/*.sh
cd .claude/hooks && npm install
```

→ 베이스 코드 선택 후 복사 (신규 프로젝트만 해당).

### 기존 프로젝트 모드 (`.claude/` 있음)

**절대 `.claude/`를 덮어쓰지 않습니다.** 병합 방식으로 진행합니다.

**병합 원칙:**
- `settings.json`: 기존 훅 배열 **유지** + 새 항목 추가
- `skill-rules.json`: 기존 스킬 **유지** + 새 스킬 항목 추가
- `skills/`: 기존 스킬 **유지** + 새 스킬 디렉토리 추가
- `agents/`: 기존 에이전트 **유지** + 새 에이전트 파일 추가
- `hooks/`: 기존 훅 **유지** + 새 훅 파일 추가

**병합 전 충돌 분석:**
```bash
# 기존 settings.json 훅 목록 확인
cat .claude/settings.json | python3 -c "
import json, sys
d = json.load(sys.stdin)
for event, hooks in d.get('hooks', {}).items():
    print(f'{event}:', [h.get('hooks',[{}])[0].get('command','?') for h in hooks if h.get('hooks')])
"

# 기존 스킬 목록 확인
ls .claude/skills/

# 기존 에이전트 목록 확인
ls .claude/agents/
```

**settings.json 병합 패턴:**
```json
// 기존 UserPromptSubmit 배열에 추가 (기존 항목 뒤에)
"UserPromptSubmit": [
  { /* 기존 항목 유지 */ },
  {
    "hooks": [{
      "type": "command",
      "command": "$CLAUDE_PROJECT_DIR/.claude/hooks/skill-activation-prompt.sh"
    }]
  },
  {
    "hooks": [{
      "type": "command",
      "command": "$CLAUDE_PROJECT_DIR/.claude/hooks/session-start-docs-loader.sh"
    }]
  }
]
```

**skill-rules.json 병합 패턴:**
```bash
# 하네스에서 필요한 스킬 항목만 추출하여 기존 파일에 추가
# 1. 하네스 skill-rules.json에서 원하는 스킬 블록 읽기
# 2. 기존 파일의 "skills" 객체에 새 항목 추가
# 3. JSON 문법 검증
cat .claude/skills/skill-rules.json | python3 -m json.tool > /dev/null && echo "✅ JSON 유효"
```

### pathPattern 재설정 (신규/기존 공통 중요 사항)

하네스의 기본 pathPattern은 `base/[stack]/` 경로를 참조합니다.
**내 프로젝트 구조에 맞게 반드시 재설정해야 합니다.**

```bash
# 내 프로젝트 주요 파일 경로 탐색
find . -name "*.py" -not -path "./.venv/*" | head -5
find . -name "*.ts" -not -path "./node_modules/*" | head -5
```

흔한 pathPattern 패턴:
```json
"pathPatterns": ["backend/**/*.py"]          // 백엔드 분리 구조
"pathPatterns": ["src/**/*.ts"]              // 단순 구조
"pathPatterns": ["packages/api/**/*.py"]     // 모노레포
```

### 4문서 체계 적용 (기존 프로젝트)

기존 프로젝트에 4문서 체계가 없으면:
1. `CLAUDE.md`에 4문서 체계 섹션 추가 (기존 내용 보존)
2. `docs/` 디렉토리 생성 후 초기 4문서 작성

```
Claude에게 요청: "현재 프로젝트를 분석해서 docs/ 4문서를 초기화해줘"
```

---

---

## Tech Stack Compatibility Check

**CRITICAL:** Before integrating a skill, verify the user's tech stack matches the skill requirements.

### Frontend Skills

**frontend-dev-guidelines requires:**
- React (19+)
- MUI v5
- TypeScript

**Before integrating, ask:**
"Do you use React with MUI v5?"

**If NO:**
```
The frontend-dev-guidelines skill is designed specifically for React + MUI v7. I can:
1. Help you create a similar skill adapted for [their stack] using this as a template
2. Extract the framework-agnostic patterns (file organization, performance, etc.)
3. Skip this skill if not relevant

Which would you prefer?
```

### Backend Skills

**backend-dev-guidelines requires:**
- Node.js/Express
- TypeScript
- Prisma ORM
- Sentry

**Before integrating, ask:**
"Do you use Node.js with Express and Prisma?"

**If NO:**
```
The backend-dev-guidelines skill is designed for Express/Prisma. I can:
1. Help you create similar guidelines adapted for [their stack] using this as a template
2. Extract the architecture patterns (layered architecture works for any framework)
3. Skip this skill

Which would you prefer?
```

### Skills That Are Tech-Agnostic

These work for ANY tech stack:
- ✅ **skill-developer** - Meta-skill, no tech requirements
- ✅ **route-tester** - Only requires JWT cookie auth (framework agnostic)
- ✅ **error-tracking** - Sentry works with most stacks

---

## General Integration Pattern

When user says: **"Add [component] to my project"**

1. Identify component type (skill/hook/agent/command)
2. **CHECK TECH STACK COMPATIBILITY** (for frontend/backend skills)
3. Ask about their project structure
4. Copy files OR adapt for their stack
5. Customize for their setup
6. Verify integration
7. Provide next steps

---

## Integrating Skills

### Step-by-Step Process

**When user requests a skill** (e.g., "add backend-dev-guidelines"):

#### 1. Understand Their Project

**ASK THESE QUESTIONS:**
- "What's your project structure? Single app, monorepo, or multi-service?"
- "Where is your [backend/frontend] code located?"
- "What frameworks/technologies do you use?"

#### 2. Copy the Skill

```bash
cp -r /path/to/showcase/.claude/skills/[skill-name] \\
      $CLAUDE_PROJECT_DIR/.claude/skills/
```

#### 3. Handle skill-rules.json

**Check if it exists:**
```bash
ls $CLAUDE_PROJECT_DIR/.claude/skills/skill-rules.json
```

**If NO (doesn't exist):**
- Copy the template from showcase
- Remove skills user doesn't want
- Customize for their project

**If YES (exists):**
- Read their current skill-rules.json
- Add the new skill entry
- Merge carefully to avoid breaking existing skills

#### 4. Customize Path Patterns

**CRITICAL:** Update `pathPatterns` in skill-rules.json to match THEIR structure:

**Example - User has monorepo:**
```json
{
  "backend-dev-guidelines": {
    "fileTriggers": {
      "pathPatterns": [
        "packages/api/src/**/*.ts",
        "packages/server/src/**/*.ts",
        "apps/backend/**/*.ts"
      ]
    }
  }
}
```

**Example - User has single backend:**
```json
{
  "backend-dev-guidelines": {
    "fileTriggers": {
      "pathPatterns": [
        "src/**/*.ts",
        "backend/**/*.ts"
      ]
    }
  }
}
```

**Safe Generic Patterns** (when unsure):
```json
{
  "pathPatterns": [
    "**/*.ts",          // All TypeScript files
    "src/**/*.ts",      // Common src directory
    "backend/**/*.ts"   // Common backend directory
  ]
}
```

#### 5. Verify Integration

```bash
# Check skill was copied
ls -la $CLAUDE_PROJECT_DIR/.claude/skills/[skill-name]

# Validate skill-rules.json syntax
cat $CLAUDE_PROJECT_DIR/.claude/skills/skill-rules.json | jq .
```

**Tell user:** "Try editing a file in [their-backend-path] and the skill should activate."

---

### Skill-Specific Notes

#### backend-dev-guidelines
- **Tech Requirements:** Node.js/Express, Prisma, TypeScript, Sentry
- **Ask:** "Do you use Express with Prisma?" "Where's your backend code?"
- **If different stack:** Offer to adapt using this as template
- **Customize:** pathPatterns
- **Example paths:** `api/`, `server/`, `backend/`, `services/*/src/`
- **Adaptation tip:** Architecture patterns (Routes→Controllers→Services) transfer to most frameworks

#### frontend-dev-guidelines
- **Tech Requirements:** React 18+, MUI v7, TanStack Query/Router, TypeScript
- **Ask:** "Do you use React with MUI v7?" "Where's your frontend code?"
- **If different stack:** Offer to create adapted version (Vue, Angular, etc.)
- **Customize:** pathPatterns + all framework-specific examples
- **Example paths:** `frontend/`, `client/`, `web/`, `apps/web/src/`
- **Adaptation tip:** File organization and performance patterns transfer, component code doesn't

#### route-tester
- **Tech Requirements:** JWT cookie-based authentication (framework agnostic)
- **Ask:** "Do you use JWT cookie-based authentication?"
- **If NO:** "This skill is designed for JWT cookies. Want me to adapt it for [their auth type] or skip it?"
- **Customize:** Service URLs, auth patterns
- **Works with:** Any backend framework using JWT cookies

#### error-tracking
- **Tech Requirements:** Sentry (works with most backends)
- **Ask:** "Do you use Sentry?" "Where's your backend code?"
- **If NO Sentry:** "Want to use this as template for [their error tracking]?"
- **Customize:** pathPatterns
- **Adaptation tip:** Error tracking philosophy transfers to other tools (Rollbar, Bugsnag, etc.)

#### skill-developer
- **Tech Requirements:** None!
- **Copy as-is** - meta-skill, fully generic, teaches skill creation for ANY tech stack

---

## Adapting Skills for Different Tech Stacks

When user's tech stack differs from skill requirements, you have options:

### Option 1: Adapt Existing Skill (Recommended)

**When to use:** User wants similar guidelines but for different tech

**Process:**
1. **Copy the skill as a starting point:**
   ```bash
   cp -r showcase/.claude/skills/frontend-dev-guidelines \\
         $CLAUDE_PROJECT_DIR/.claude/skills/vue-dev-guidelines
   ```

2. **Identify what needs changing:**
   - Framework-specific code examples (React → Vue)
   - Library APIs (MUI → Vuetify/PrimeVue)
   - Import statements
   - Component patterns

3. **Keep what transfers:**
   - File organization principles
   - Performance optimization strategies
   - TypeScript standards
   - General best practices

4. **Replace examples systematically:**
   - Ask user for equivalent patterns in their stack
   - Update code examples to their framework
   - Keep the overall structure and sections

5. **Update skill name and triggers:**
   - Rename skill appropriately
   - Update skill-rules.json triggers for their stack
   - Test activation

**Example - Adapting frontend-dev-guidelines for Vue:**
```
I'll create vue-dev-guidelines based on the React skill structure:
- Replace React.FC → Vue defineComponent
- Replace useSuspenseQuery → Vue composables
- Replace MUI components → [their component library]
- Keep: File organization, performance patterns, TypeScript guidelines

This will take a few minutes. Sound good?
```

### Option 2: Extract Framework-Agnostic Patterns

**When to use:** Stacks are very different, but core principles apply

**Process:**
1. Read through the existing skill
2. Identify framework-agnostic patterns:
   - Layered architecture (backend)
   - File organization strategies
   - Performance optimization principles
   - Testing strategies
   - Error handling philosophy

3. Create new skill with just those patterns
4. User can add framework-specific examples later

**Example:**
```
The backend-dev-guidelines uses Express, but the layered architecture
(Routes → Controllers → Services → Repositories) works for Django too.

I can create a skill with:
- Layered architecture pattern
- Separation of concerns principles
- Error handling best practices
- Testing strategies

Then you can add Django-specific examples as you establish patterns.
```

### Option 3: Use as Reference Only

**When to use:** Too different to adapt, but user wants inspiration

**Process:**
1. User browses the existing skill
2. You help create a new skill from scratch
3. Use existing skill's structure as a template
4. Follow modular pattern (main + resource files)

### What Usually Transfers Across Tech Stacks

**Architecture & Organization:**
- ✅ Layered architecture (Routes/Controllers/Services pattern)
- ✅ Separation of concerns
- ✅ File organization strategies (features/ pattern)
- ✅ Progressive disclosure (main + resource files)
- ✅ Repository pattern for data access

**Development Practices:**
- ✅ Error handling philosophy
- ✅ Input validation importance
- ✅ Testing strategies
- ✅ Performance optimization principles
- ✅ TypeScript best practices

**Framework-Specific Code:**
- ❌ React hooks → Don't transfer to Vue/Angular
- ❌ MUI components → Different component libraries
- ❌ Prisma queries → Different ORM syntax
- ❌ Express middleware → Different framework patterns
- ❌ Routing implementations → Framework-specific

### When to Recommend Adaptation vs Skipping

**Recommend adaptation if:**
- User wants similar guidelines for their stack
- Core patterns apply (layered architecture, etc.)
- User has time to help with framework-specific examples

**Recommend skipping if:**
- Stacks are completely different
- User doesn't need those patterns
- Would take too long to adapt
- User prefers creating from scratch

---

## Integrating Hooks

### Essential Hooks (Always Safe to Copy)

#### skill-activation-prompt (UserPromptSubmit)

**Purpose:** Auto-suggests skills based on user prompts

**Integration (NO customization needed):**

```bash
# Copy both files
cp showcase/.claude/hooks/skill-activation-prompt.sh \\
   $CLAUDE_PROJECT_DIR/.claude/hooks/
cp showcase/.claude/hooks/skill-activation-prompt.ts \\
   $CLAUDE_PROJECT_DIR/.claude/hooks/

# Make executable
chmod +x $CLAUDE_PROJECT_DIR/.claude/hooks/skill-activation-prompt.sh

# Install dependencies if needed
if [ -f "showcase/.claude/hooks/package.json" ]; then
  cp showcase/.claude/hooks/package.json \\
     $CLAUDE_PROJECT_DIR/.claude/hooks/
  cd $CLAUDE_PROJECT_DIR/.claude/hooks && npm install
fi
```

**Add to settings.json:**
```json
{
  "hooks": {
    "UserPromptSubmit": [
      {
        "hooks": [
          {
            "type": "command",
            "command": "$CLAUDE_PROJECT_DIR/.claude/hooks/skill-activation-prompt.sh"
          }
        ]
      }
    ]
  }
}
```

**This hook is FULLY GENERIC** - works anywhere, no customization needed!

#### post-tool-use-tracker (PostToolUse)

**Purpose:** Tracks file changes for context management

**Integration (NO customization needed):**

```bash
# Copy file
cp showcase/.claude/hooks/post-tool-use-tracker.sh \\
   $CLAUDE_PROJECT_DIR/.claude/hooks/

# Make executable
chmod +x $CLAUDE_PROJECT_DIR/.claude/hooks/post-tool-use-tracker.sh
```

**Add to settings.json:**
```json
{
  "hooks": {
    "PostToolUse": [
      {
        "matcher": "Edit|MultiEdit|Write",
        "hooks": [
          {
            "type": "command",
            "command": "$CLAUDE_PROJECT_DIR/.claude/hooks/post-tool-use-tracker.sh"
          }
        ]
      }
    ]
  }
}
```

**This hook is FULLY GENERIC** - auto-detects project structure!

---

### Optional Hooks (Require Heavy Customization)

#### tsc-check.sh and trigger-build-resolver.sh (Stop hooks)

⚠️ **WARNING:** These hooks are configured for a specific multi-service monorepo structure.

**Before integrating, ask:**
1. "Do you have a monorepo with multiple TypeScript services?"
2. "What are your service directory names?"
3. "Where are your tsconfig.json files located?"

**For SIMPLE projects (single service):**
- **RECOMMEND SKIPPING** these hooks
- They're overkill for single-service projects
- User can run `tsc --noEmit` manually instead

**For COMPLEX projects (multi-service monorepo):**

1. Copy the files
2. **MUST EDIT** tsc-check.sh - find this section:
```bash
case "$repo" in
    email|exports|form|frontend|projects|uploads|users|utilities|events|database)
        echo "$repo"
        return 0
        ;;
esac
```

3. Replace with USER'S actual service names:
```bash
case "$repo" in
    api|web|auth|payments|notifications)  # ← User's services
        echo "$repo"
        return 0
        ;;
esac
```

4. Test manually before adding to settings.json:
```bash
./.claude/hooks/tsc-check.sh
```

**IMPORTANT:** If this hook fails, it will block Stop events. Only add if you're sure it works for their setup.

---

## Integrating Agents

**Agents are STANDALONE** - easiest to integrate!

### Standard Agent Integration

```bash
# Copy the agent file
cp showcase/.claude/agents/[agent-name].md \\
   $CLAUDE_PROJECT_DIR/.claude/agents/
```

**That's it!** Agents work immediately, no configuration needed.

### Check for Hardcoded Paths

Some agents may reference paths. **Before copying, read the agent file and check for:**

- `~/git/old-project/` → Should be `$CLAUDE_PROJECT_DIR` or `.`
- `/root/git/project/` → Should be `$CLAUDE_PROJECT_DIR` or `.`
- Hardcoded screenshot paths → Ask user where they want screenshots

**If found, update them:**
```bash
sed -i 's|~/git/old-project/|.|g' $CLAUDE_PROJECT_DIR/.claude/agents/[agent].md
sed -i 's|/root/git/.*PROJECT.*DIR|$CLAUDE_PROJECT_DIR|g' \\
    $CLAUDE_PROJECT_DIR/.claude/agents/[agent].md
```

### Agent-Specific Notes

**auth-route-tester / auth-route-debugger:**
- Requires JWT cookie-based authentication in user's project
- Ask: "Do you use JWT cookies for auth?"
- If NO: "These agents are for JWT cookie auth. Skip them or want me to adapt?"

**frontend-error-fixer:**
- May reference screenshot paths
- Ask: "Where should screenshots be saved?"

**All other agents:**
- Copy as-is, they're fully generic

---

## Integrating Slash Commands

```bash
# Copy command file
cp showcase/.claude/commands/[command].md \\
   $CLAUDE_PROJECT_DIR/.claude/commands/
```

### Customize Paths

Commands may reference dev docs paths. **Check and update:**

**dev-docs and dev-docs-update:**
- Look for `dev/active/` path references
- Ask: "Where do you want dev documentation stored?"
- Update paths in the command files

**route-research-for-testing:**
- May reference service paths
- Ask about their API structure

---

## Common Patterns & Best Practices

### Pattern: Asking About Project Structure

**DON'T assume:**
- ❌ "I'll add this for your blog-api service"
- ❌ "Configuring for your frontend directory"

**DO ask:**
- ✅ "What's your project structure? Monorepo or single app?"
- ✅ "Where is your backend code located?"
- ✅ "Do you use workspaces or have multiple services?"

### Pattern: Customizing skill-rules.json

**User has monorepo with workspaces:**
```json
{
  "pathPatterns": [
    "packages/*/src/**/*.ts",
    "apps/*/src/**/*.tsx"
  ]
}
```

**User has Nx monorepo:**
```json
{
  "pathPatterns": [
    "apps/api/src/**/*.ts",
    "libs/*/src/**/*.ts"
  ]
}
```

**User has simple structure:**
```json
{
  "pathPatterns": [
    "src/**/*.ts",
    "backend/**/*.ts"
  ]
}
```

### Pattern: settings.json Integration

**NEVER copy the showcase settings.json directly!**

Instead, **extract and merge** the sections they need:

1. Read their existing settings.json
2. Add the hook configurations they want
3. Preserve their existing config

**Example merge:**
```json
{
  // ... their existing config ...
  "hooks": {
    // ... their existing hooks ...
    "UserPromptSubmit": [  // ← Add this section
      {
        "hooks": [
          {
            "type": "command",
            "command": "$CLAUDE_PROJECT_DIR/.claude/hooks/skill-activation-prompt.sh"
          }
        ]
      }
    ]
  }
}
```

---

## Verification Checklist

After integration, **verify these items:**

```bash
# 1. Hooks are executable
ls -la $CLAUDE_PROJECT_DIR/.claude/hooks/*.sh
# Should show: -rwxr-xr-x

# 2. skill-rules.json is valid JSON
cat $CLAUDE_PROJECT_DIR/.claude/skills/skill-rules.json | jq .
# Should parse without errors

# 3. Hook dependencies installed (if TypeScript hooks)
ls $CLAUDE_PROJECT_DIR/.claude/hooks/node_modules/
# Should show packages if package.json exists

# 4. Settings.json is valid JSON
cat $CLAUDE_PROJECT_DIR/.claude/settings.json | jq .
# Should parse without errors
```

**Then ask user to test:**
- "Try editing a file in [relevant-path] - the skill should activate"
- "Try asking me a question about [topic] - I should suggest the skill"

---

## Common Mistakes to Avoid

### ❌ DON'T: Copy settings.json as-is
**Why:** The Stop hooks reference non-existent services
**DO:** Extract only UserPromptSubmit and PostToolUse sections

### ❌ DON'T: Keep example service names
**Why:** User doesn't have blog-api, auth-service, etc.
**DO:** Ask about their actual structure and update

### ❌ DON'T: Skip making hooks executable
**Why:** Hooks won't run without execute permission
**DO:** Always `chmod +x` after copying

### ❌ DON'T: Assume monorepo structure
**Why:** Most projects are single-service
**DO:** Ask first, then customize

### ❌ DON'T: Add all skills at once
**Why:** Overwhelming and may not all be relevant
**DO:** Ask which skills are relevant to their work

### ❌ DON'T: Copy Stop hooks without testing
**Why:** They can block the Stop event if they fail
**DO:** Test manually first, only add if working

---

## Example Integration Conversations

### Example 1: Adding Backend Skill

**User:** "Add the backend-dev-guidelines skill to my project"

**Claude should respond:**
```
I'll add the backend-dev-guidelines skill. First, I need to understand
your project structure:

1. Is this a monorepo with multiple services, or a single backend application?
2. Where is your backend code located? (e.g., src/, backend/, server/)
3. Do you use Express.js?
```

**User:** "Single app, code is in src/api/, yes we use Express"

**Claude should:**
1. Copy the skill directory
2. Update skill-rules.json with pathPattern: `"src/api/**/*.ts"`
3. Verify it's copied correctly
4. Respond:
```
✅ Added backend-dev-guidelines skill!

Configuration:
- Skill triggers when editing files in src/api/
- Activated by keywords like "controller", "service", "route", "API"

Try it: Edit a file in src/api/ and you should see the skill suggestion.
```

### Example 2: Setting Up Hooks

**User:** "Set up the skill activation hooks"

**Claude should respond:**
```
I'll set up the skill auto-activation system. This requires two hooks:
1. skill-activation-prompt - Suggests skills based on your prompts
2. post-tool-use-tracker - Tracks file changes

Do you have a .claude/settings.json file already, or should I create one?
```

**User:** "I have settings.json already"

**Claude should:**
1. Read their settings.json
2. Copy both hook files
3. Run npm install in hooks directory
4. Add hook configurations to their settings.json (preserving existing config)
5. Make hooks executable
6. Respond:
```
✅ Skill activation hooks installed!

What I did:
- Copied skill-activation-prompt hook
- Copied post-tool-use-tracker hook
- Installed dependencies
- Added to your settings.json

Test it: Ask me "help with backend development" and I should suggest
the backend-dev-guidelines skill.
```

### Example 3: Adding Agent

**User:** "Add the code-architecture-reviewer agent"

**Claude should:**
```
The code-architecture-reviewer agent reviews code for architectural issues
and best practices. This agent works standalone, no configuration needed.

I'll copy it to your .claude/agents/ directory now.
```

**Then:**
1. Copy the agent file
2. Respond:
```
✅ Added code-architecture-reviewer agent!

Use it by running the Task tool with:
- subagent_type: "code-architecture-reviewer"
- prompt: Description of code to review

The agent will review your code and provide architectural feedback.
```

### Example 3: Tech Stack Mismatch

**User:** "Add the frontend-dev-guidelines skill to my Vue project"

**Claude should:**
```
I see you're using Vue. The frontend-dev-guidelines skill is specifically
designed for React + MUI v7. I have a few options:

Option 1 (Recommended): I can create a vue-dev-guidelines skill using the
React skill as a template. I'll adapt:
- Component patterns for Vue (Composition API, defineComponent)
- Replace MUI examples with your component library
- Adapt data fetching for Vue patterns
- Keep: File organization, performance principles, TypeScript standards

Option 2: Extract only the framework-agnostic patterns (file organization,
performance, testing) without React-specific code.

Option 3: Skip it and help you create Vue guidelines from scratch.

Which would you prefer?
```

**User:** "Option 1 sounds good, I use Vuetify"

**Claude should:**
1. Copy frontend-dev-guidelines as vue-dev-guidelines
2. Go through each resource file
3. Replace React patterns with Vue Composition API
4. Replace MUI components with Vuetify
5. Update skill-rules.json with Vue-specific triggers (.vue files)
6. Respond:
```
✅ Created vue-dev-guidelines skill adapted for Vue 3 + Vuetify!

What I changed:
- React.FC components → Vue defineComponent
- MUI v7 examples → Vuetify components
- TanStack Query → Your data fetching approach
- File patterns → .vue files

What I kept:
- File organization (features/ pattern)
- Performance optimization strategies
- TypeScript standards
- Loading/error state principles

Try editing a .vue file - the skill should activate.
```

---

## Base Code Integration (`base/[stack]/`)

### Overview

`base/[stack]/` 디렉토리는 각 기술스택의 **범용 스타터 킷**이다.  
새 프로젝트를 시작할 때 해당 스택 디렉토리 전체를 복사하여 즉시 사용 가능한 개발 환경을 구성한다.

### 사용 가능한 스택

| 경로 | 스택 | 핵심 구성 |
|---|---|---|
| `base/fastapi/` | Python FastAPI | DDD + JWT + S3 + PostgreSQL (읽기/쓰기 분리) |
| `base/nextjs/` | Next.js 15 | App Router + JWT + shadcn/ui + 미들웨어 |
| `base/express/` | Express.js | TypeScript + Prisma + JWT + S3 + Docker |
| `base/django/` | Django 5 | DRF + SimpleJWT + PostgreSQL + S3 + Docker |
| `base/nestjs/` | NestJS | TypeScript + Prisma + JWT Guards + Docker |
| `base/spring-boot/` | Spring Boot 3 | Java 21 + Security + JPA + S3 + Docker |
| `base/react-native/` | React Native | Expo + Router + Zustand + SecureStore |
| `base/c-embedded/` | C Embedded | C99 + CMake + HAL + FreeRTOS 패턴 |
| `base/cpp-embedded/` | C++ Embedded | C++17 + CMake + HAL 클래스 + Google Test |

### 새 프로젝트에 베이스 코드 통합하는 방법

**Step 1: 스택 선택 및 복사**
```bash
# 예시: FastAPI 스타터 킷 복사
cp -r base/fastapi/ ~/my-new-project/backend/

# 예시: Next.js 스타터 킷 복사
cp -r base/nextjs/ ~/my-new-project/frontend/
```

**Step 2: 환경변수 설정**
```bash
cp .env.example .env
# .env 파일을 열어 실제 값으로 채우기
```

**Step 3: README.md 참고하여 실행**
```bash
# 각 스택의 README.md에 실행 방법이 명시되어 있음
cat README.md
```

**Step 4: 커스터마이징**
- `README.md`의 "커스터마이징 포인트" 섹션 참고
- 도메인 엔티티 추가: `base/fastapi/backend/domain/` 아래 새 도메인 디렉토리 생성
- 도메인 페이지 추가: `base/nextjs/src/app/` 아래 새 라우트 추가

### 베이스 코드 디렉토리 구조

각 `base/[stack]/`에는 다음이 포함된다:

```
base/[stack]/
├── docs/
│   ├── plan.md       ← 이 스택 베이스 코드 구성 계획
│   ├── task.md       ← 세부 작업 항목 및 진행 상태
│   ├── history.md    ← 작업 맥락 및 주요 결정 사항
│   └── checklist.md  ← 완료 기준 체크리스트
├── README.md         ← 구조 설명 + 실행 방법 + 커스터마이징 포인트
├── .env.example      ← 전체 환경변수 목록 (웹/모바일 스택)
├── Dockerfile        ← 컨테이너 빌드 설정 (웹 백엔드 스택)
├── docker-compose.yaml ← 로컬 개발 환경 구성 (웹 백엔드 스택)
└── ... (스택 소스 코드)
```

### 베이스 코드에서 Skills/Agents 연계

베이스 코드와 함께 해당 스택에 맞는 Skill을 함께 설정하면 최적의 AI 지원 개발 환경을 구성할 수 있다.

| 스택 | 권장 Skill |
|---|---|
| `base/fastapi/` | `fastapi-backend-guidelines` |
| `base/nextjs/` | `nextjs-frontend-guidelines`, `vercel-react-best-practices` |
| `base/express/` | `express-backend-guidelines` (추가 예정) |
| `base/django/` | `django-backend-guidelines` (추가 예정) |
| `base/nestjs/` | `nestjs-backend-guidelines` (추가 예정) |

---

## Quick Reference Tables

### What Needs Customization?

| Component | Tech Requirements | Customization | What to Ask |
|-----------|------------------|--------------|-------------|
| **skill-developer** | None | ✅ None | Copy as-is |
| **backend-dev-guidelines** | Express/Prisma/Node | ⚠️ Paths + tech check | "Use Express/Prisma?" "Where's backend?" |
| **frontend-dev-guidelines** | React/MUI v7 | ⚠️⚠️ Paths + framework | "Use React/MUI v7?" "Where's frontend?" |
| **route-tester** | JWT cookies | ⚠️ Auth + paths | "JWT cookie auth?" |
| **error-tracking** | Sentry | ⚠️ Paths | "Use Sentry?" "Where's backend?" |
| **skill-activation-prompt** | ✅ None | Copy as-is |
| **post-tool-use-tracker** | ✅ None | Copy as-is |
| **tsc-check** | ⚠️⚠️⚠️ Heavy | "Monorepo or single service?" |
| **All agents** | ✅ Minimal | Check paths |
| **All commands** | ⚠️ Paths | "Where for dev docs?" |

### When to Recommend Skipping

| Component | Skip If... |
|-----------|-----------|
| **tsc-check hooks** | Single-service project or different build setup |
| **route-tester** | Not using JWT cookie authentication |
| **frontend-dev-guidelines** | Not using React + MUI |
| **auth agents** | Not using JWT cookie auth |

---

## Final Tips for Claude

**When user says "add everything":**
- Start with essentials: skill-activation hooks + 1-2 relevant skills
- Don't overwhelm them with all 5 skills + 10 agents
- Ask what they actually need

**When something doesn't work:**
- Check verification checklist
- Verify paths match their structure
- Test hooks manually
- Check for JSON syntax errors

**When user is unsure:**
- Recommend starting with just skill-activation hooks
- Add backend OR frontend skill (whichever they use)
- Add more later as needed

**Always explain what you're doing:**
- Show the commands you're running
- Explain why you're asking questions
- Provide clear next steps after integration

---

**Remember:** This is a reference library, not a working application. Your job is to help users cherry-pick and adapt components for THEIR specific project structure.
