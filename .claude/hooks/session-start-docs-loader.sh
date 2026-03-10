#!/bin/bash
# UserPromptSubmit Hook — 4문서 자동 로드 (set -e 제거: 실패 시 프롬프트 차단 방지)

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"
cat | npx tsx session-start-docs-loader.ts || exit 0
