# Hooks

Claude Code hooks that enable skill auto-activation, file tracking, and validation.

---

## What Are Hooks?

Hooks are scripts that run at specific points in Claude's workflow:
- **UserPromptSubmit**: When user submits a prompt
- **PreToolUse**: Before a tool executes  
- **PostToolUse**: After a tool completes
- **Stop**: When user requests to stop

**Key insight:** Hooks can modify prompts, block actions, and track state - enabling features Claude can't do alone.

---

## Essential Hooks (Start Here)

### session-start-docs-loader (UserPromptSubmit)

**Purpose:** 세션 첫 번째 프롬프트에서 4문서(docs/) 읽기 지시를 Claude 컨텍스트에 자동 주입

**How it works:**
1. stdin에서 `{ session_id, cwd, prompt }` JSON 읽기
2. `.claude/.session-cache/[session_id].loaded` 파일 존재 여부 확인
3. 없으면 → 첫 번째 프롬프트: 4문서 읽기 지시 stdout 출력 + `.loaded` 플래그 파일 생성
4. 있으면 → 이미 로드됨: 즉시 exit(0) (무출력)

**Why it's essential:** 세션마다 수동으로 4문서를 열지 않아도 되며, 작업 맥락을 자동으로 복원한다.

**Integration:**
```bash
# Copy both files
cp session-start-docs-loader.sh your-project/.claude/hooks/
cp session-start-docs-loader.ts your-project/.claude/hooks/

# Make executable
chmod +x your-project/.claude/hooks/session-start-docs-loader.sh
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
            "command": "$CLAUDE_PROJECT_DIR/.claude/hooks/session-start-docs-loader.sh"
          }
        ]
      }
    ]
  }
}
```

**Session cache:** `.claude/.session-cache/[session_id].loaded` — `.gitignore`에 `*.loaded` 패턴 추가 필요

**Customization:** ✅ None needed — `docs/history.md`에서 Phase 정보 자동 추출

---

### skill-activation-prompt (UserPromptSubmit)

**Purpose:** Automatically suggests relevant skills based on user prompts and file context

**How it works:**
1. Reads `skill-rules.json`
2. Matches user prompt against trigger patterns
3. Checks which files user is working with
4. Injects skill suggestions into Claude's context

**Why it's essential:** This is THE hook that makes skills auto-activate.

**Integration:**
```bash
# Copy both files
cp skill-activation-prompt.sh your-project/.claude/hooks/
cp skill-activation-prompt.ts your-project/.claude/hooks/

# Make executable
chmod +x your-project/.claude/hooks/skill-activation-prompt.sh

# Install dependencies
cd your-project/.claude/hooks
npm install
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

**Customization:** ✅ None needed - reads skill-rules.json automatically

---

### post-tool-use-tracker (PostToolUse)

**Purpose:** Tracks file changes to maintain context across sessions

**How it works:**
1. Monitors Edit/Write/MultiEdit tool calls
2. Records which files were modified
3. Creates cache for context management
4. Auto-detects project structure (frontend, backend, packages, etc.)

**Why it's essential:** Helps Claude understand what parts of your codebase are active.

**Integration:**
```bash
# Copy file
cp post-tool-use-tracker.sh your-project/.claude/hooks/

# Make executable
chmod +x your-project/.claude/hooks/post-tool-use-tracker.sh
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

**Customization:** ✅ None needed - auto-detects structure

---

## Optional Hooks (Require Customization)

### docs-update-reminder (Stop)

**Purpose:** 세션 종료 시 `docs/history.md` 미갱신 감지 → 갱신 리마인더 출력

**How it works:**
1. stdin에서 `{ session_id }` JSON 읽기
2. `.session-cache/[session_id].loaded` 파일 없으면 → 4문서 기반 세션 아님, 스킵
3. `.loaded` 파일 mtime = 세션 시작 시각
4. `docs/history.md` mtime 비교
5. history.md 미갱신이면 → 리마인더 + `/harness-docs-update` 커맨드 안내
6. 오류 시 항상 exit(0) — Stop 훅 블로킹 방지

**Integration:**
```bash
cp docs-update-reminder.sh your-project/.claude/hooks/
cp docs-update-reminder.ts your-project/.claude/hooks/
chmod +x your-project/.claude/hooks/docs-update-reminder.sh
```

**Add to settings.json:**
```json
{
  "hooks": {
    "Stop": [
      {
        "hooks": [
          {
            "type": "command",
            "command": "$CLAUDE_PROJECT_DIR/.claude/hooks/docs-update-reminder.sh"
          }
        ]
      }
    ]
  }
}
```

**Skip:** `SKIP_DOCS_REMINDER=1` 환경변수로 비활성화 가능

**Customization:** ⚠️ 이 훅은 `docs/history.md`가 있는 프로젝트 전용 (4문서 체계 사용 프로젝트)

---

### pr-review-trigger (PostToolUse — Bash)

**Purpose:** `gh pr create` 실행 직후 `code-architecture-reviewer` 에이전트 실행을 Claude 컨텍스트에 권고

**How it works:**
1. PostToolUse(Bash) 이벤트로 모든 Bash 명령 완료 후 실행
2. `tool_input.command`에 `gh pr create` 포함 시에만 동작 (그 외 exit 0)
3. `tool_response.output`에서 PR URL 추출 (regex)
4. 권고 메시지 stdout 출력 + **exit 2** → Claude 컨텍스트 주입

**Integration:**
```bash
cp pr-review-trigger.sh your-project/.claude/hooks/
cp pr-review-trigger.ts your-project/.claude/hooks/
chmod +x your-project/.claude/hooks/pr-review-trigger.sh
```

**Add to settings.json:**
```json
{
  "hooks": {
    "PostToolUse": [
      {
        "matcher": "Bash",
        "hooks": [
          {
            "type": "command",
            "command": "$CLAUDE_PROJECT_DIR/.claude/hooks/pr-review-trigger.sh"
          }
        ]
      }
    ]
  }
}
```

**Skip:** `SKIP_PR_REVIEW=1` 환경변수로 비활성화 가능

**Review output path:** `docs/reviews/[YYYY-MM-DD]-[branch-or-task-name]-review.md`

**GitHub Actions 연계:** `.github/workflows/pr-code-review.yaml` — PR 체크리스트 댓글 자동 작성

**Customization:** ✅ None needed — `gh pr create` 명령 자동 감지

---

### tsc-check (Stop)

**Purpose:** TypeScript compilation check when user stops

**⚠️ WARNING:** Configured for multi-service monorepo structure

**Integration:**

**First, determine if this is right for you:**
- ✅ Use if: Multi-service TypeScript monorepo
- ❌ Skip if: Single-service project or different build setup

**If using:**
1. Copy tsc-check.sh
2. **EDIT the service detection (line ~28):**
   ```bash
   # Replace example services with YOUR services:
   case "$repo" in
       api|web|auth|payments|...)  # ← Your actual services
   ```
3. Test manually before adding to settings.json

**Customization:** ⚠️⚠️⚠️ Heavy

---

### trigger-build-resolver (Stop)

**Purpose:** Auto-launches build-error-resolver agent when compilation fails

**Depends on:** tsc-check hook working correctly

**Customization:** ✅ None (but tsc-check must work first)

---

## For Claude Code

**When setting up hooks for a user:**

1. **Read [CLAUDE_INTEGRATION_GUIDE.md](../../CLAUDE_INTEGRATION_GUIDE.md)** first
2. **Always start with the two essential hooks**
3. **Ask before adding Stop hooks** - they can block if misconfigured  
4. **Verify after setup:**
   ```bash
   ls -la .claude/hooks/*.sh | grep rwx
   ```

**Questions?** See [CLAUDE_INTEGRATION_GUIDE.md](../../CLAUDE_INTEGRATION_GUIDE.md)
