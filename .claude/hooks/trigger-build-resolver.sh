#!/bin/bash
# Stop Hook — 세션 중 변경된 TS/JS 파일 요약 보고 (비차단)
# 에이전트 자동 실행 없음 — Claude가 출력을 보고 판단
# 등록 위치: settings.json Stop 훅

if [ -n "$SKIP_BUILD_CHECK" ]; then
    exit 0
fi

CLAUDE_PROJECT_DIR="${CLAUDE_PROJECT_DIR:-$(pwd)}"

# git status로 변경 파일 탐지 (범용 — 프로젝트 독립적)
changed_ts=$(git -C "$CLAUDE_PROJECT_DIR" status --porcelain 2>/dev/null | \
    grep -E '\.(ts|tsx|js|jsx)$' | grep -v node_modules || true)

if [ -n "$changed_ts" ]; then
    count=$(echo "$changed_ts" | wc -l | tr -d ' ')
    echo "⚡ Stop Hook: ${count}개 TS/JS 파일 변경 감지" >&2
    echo "$changed_ts" | head -5 >&2
    [ "$count" -gt 5 ] && echo "... 외 $((count - 5))개" >&2
fi

exit 0
