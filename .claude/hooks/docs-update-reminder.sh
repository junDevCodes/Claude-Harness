#!/bin/bash

# Skip if environment variable is set
if [ -n "$SKIP_DOCS_REMINDER" ]; then
    exit 0
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"
cat | npx tsx docs-update-reminder.ts || exit 0
