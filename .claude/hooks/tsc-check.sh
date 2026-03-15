#!/bin/bash
# PostToolUse Hook — TypeScript 타입 체크
# matcher: Edit|MultiEdit|Write — .ts/.tsx/.js/.jsx 파일 변경 시 TSC 체크 실행
# 등록 위치: settings.json PostToolUse → matcher: "Edit|MultiEdit|Write"

CLAUDE_PROJECT_DIR="${CLAUDE_PROJECT_DIR:-$(pwd)}"
HOOK_INPUT=$(cat)

# PostToolUse 페이로드에서 tool_name 추출 (node 기반, jq 미설치 환경 대응)
TOOL_NAME=$(node -e "let d='';process.stdin.on('data',c=>d+=c).on('end',()=>{try{process.stdout.write(JSON.parse(d).tool_name||'')}catch(e){}})" <<< "$HOOK_INPUT" 2>/dev/null || echo "")

# tool_name 기반 파일 경로 추출
case "$TOOL_NAME" in
    MultiEdit)
        FILE_PATHS=$(node -e "let d='';process.stdin.on('data',c=>d+=c).on('end',()=>{try{const o=JSON.parse(d);(o.tool_input.edits||[]).forEach(e=>e.file_path&&console.log(e.file_path))}catch(e){}})" <<< "$HOOK_INPUT" 2>/dev/null)
        ;;
    Write|Edit)
        FILE_PATHS=$(node -e "let d='';process.stdin.on('data',c=>d+=c).on('end',()=>{try{const o=JSON.parse(d);o.tool_input.file_path&&console.log(o.tool_input.file_path)}catch(e){}})" <<< "$HOOK_INPUT" 2>/dev/null)
        ;;
    *)
        exit 0
        ;;
esac

# TS/JS 파일만 필터링
TS_FILES=$(echo "$FILE_PATHS" | grep -E '\.(ts|tsx|js|jsx)$' || true)

if [ -z "$TS_FILES" ]; then
    exit 0
fi

# tsconfig.json 존재 여부로 범용 디렉토리 탐지 (하드코딩 서비스 목록 제거)
get_repo_for_file() {
    local file_path="$1"
    local relative_path="${file_path#$CLAUDE_PROJECT_DIR/}"
    local first_dir="${relative_path%%/*}"

    if [ -n "$first_dir" ] && [ "$first_dir" != "$relative_path" ] && \
       [ -f "$CLAUDE_PROJECT_DIR/$first_dir/tsconfig.json" ]; then
        echo "$first_dir"
        return 0
    fi
    echo ""
    return 1
}

# 체크할 디렉토리 목록 (중복 제거)
REPOS_TO_CHECK=$(echo "$TS_FILES" | while IFS= read -r file_path; do
    [ -z "$file_path" ] && continue
    repo=$(get_repo_for_file "$file_path")
    [ -n "$repo" ] && echo "$repo"
done | sort -u)

if [ -z "$REPOS_TO_CHECK" ]; then
    exit 0
fi

ERROR_COUNT=0
ERROR_OUTPUT=""
FAILED_REPOS=""

echo "⚡ TypeScript check on: $(echo "$REPOS_TO_CHECK" | tr '\n' ' ')" >&2

while IFS= read -r repo; do
    [ -z "$repo" ] && continue
    repo_path="$CLAUDE_PROJECT_DIR/$repo"

    echo -n "  Checking $repo... " >&2

    if [ ! -d "$repo_path" ]; then
        echo "❌ 디렉토리 없음" >&2
        continue
    fi

    # tsconfig 타입 탐지 → 서브쉘 격리 (cd 부작용 방지)
    if [ -f "$repo_path/tsconfig.app.json" ]; then
        CHECK_OUTPUT=$(cd "$repo_path" 2>/dev/null && npx tsc --project tsconfig.app.json --noEmit 2>&1)
    elif [ -f "$repo_path/tsconfig.build.json" ]; then
        CHECK_OUTPUT=$(cd "$repo_path" 2>/dev/null && npx tsc --project tsconfig.build.json --noEmit 2>&1)
    else
        CHECK_OUTPUT=$(cd "$repo_path" 2>/dev/null && npx tsc --noEmit 2>&1)
    fi
    CHECK_EXIT_CODE=$?

    if [ $CHECK_EXIT_CODE -ne 0 ] || echo "$CHECK_OUTPUT" | grep -q "error TS"; then
        echo "❌ 오류 발견" >&2
        ERROR_COUNT=$((ERROR_COUNT + 1))
        FAILED_REPOS="$FAILED_REPOS $repo"
        ERROR_OUTPUT="${ERROR_OUTPUT}
=== Errors in $repo ===
$CHECK_OUTPUT"
    else
        echo "✅ OK" >&2
    fi
done <<< "$REPOS_TO_CHECK"

if [ $ERROR_COUNT -gt 0 ]; then
    {
        echo ""
        echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
        echo "🚨 TypeScript 오류 발견 ($ERROR_COUNT개 디렉토리):$FAILED_REPOS"
        echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
        echo ""
        echo "$ERROR_OUTPUT" | grep "error TS" | head -10
        echo ""
    } >&2
    exit 1
fi

exit 0
