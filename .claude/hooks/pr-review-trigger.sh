#!/bin/bash
# PostToolUse — gh pr create 감지 → code-architecture-reviewer 실행 권고
# exit 2: stdout → Claude 컨텍스트 주입 (block mode)

if [ -n "$SKIP_PR_REVIEW" ]; then
    exit 0
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"
cat | npx tsx pr-review-trigger.ts
