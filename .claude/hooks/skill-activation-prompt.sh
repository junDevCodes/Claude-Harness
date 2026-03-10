#!/bin/bash
# UserPromptSubmit Hook — 스킬 활성화 프롬프트 (set -e 제거: 실패 시 프롬프트 차단 방지)

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"
cat | npx tsx skill-activation-prompt.ts || exit 0
