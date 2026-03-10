#!/usr/bin/env node
import { readFileSync, existsSync, statSync, writeFileSync } from 'fs';
import { join, dirname } from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

interface HookInput {
    session_id: string;
    [key: string]: unknown;
}

function main() {
    try {
        const input = readFileSync(0, 'utf-8');
        const data: HookInput = JSON.parse(input);
        const { session_id } = data;

        const projectDir = process.env.CLAUDE_PROJECT_DIR || join(__dirname, '..', '..');
        const sessionCacheDir = join(projectDir, '.claude', '.session-cache');
        const loadedFlagPath = join(sessionCacheDir, `${session_id}.loaded`);

        // Not a 4-doc session → skip
        if (!existsSync(loadedFlagPath)) {
            process.exit(0);
        }

        // Session start time = .loaded file mtime
        const sessionStartTime = statSync(loadedFlagPath).mtime;

        // docs/history.md last modified time
        const historyPath = join(projectDir, 'docs', 'history.md');
        if (!existsSync(historyPath)) {
            process.exit(0);
        }
        const historyMtime = statSync(historyPath).mtime;

        // history.md was updated during this session → OK
        if (historyMtime >= sessionStartTime) {
            process.exit(0);
        }

        // checklist.md 완료 여부 감지
        const checklistPath = join(projectDir, 'docs', 'checklist.md');
        const flagPath = join(sessionCacheDir, 'next-task-pending.flag');
        if (existsSync(checklistPath)) {
            const checklistContent = readFileSync(checklistPath, 'utf-8');
            const unchecked = (checklistContent.match(/- \[ \]/g) || []).length;
            const checked = (checklistContent.match(/- \[x\]/gi) || []).length;
            if (unchecked === 0 && checked > 0) {
                // 완료 플래그 생성 → 다음 세션에서 session-start-docs-loader가 감지
                writeFileSync(flagPath, new Date().toISOString(), 'utf-8');
                const doneOutput = [
                    '━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━',
                    '✅ checklist.md 전부 완료 확인',
                    '━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━',
                    '',
                    '다음 단계:',
                    '  1) docs/history.md에 완료 내용 기록',
                    '  2) /harness-plan-sync 실행하여 다음 task.md 준비',
                    '━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━',
                ].join('\n');
                console.log(doneOutput);
                process.exit(0);
            }
        }

        // Not updated → show reminder
        const output = [
            '━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━',
            '📝 SESSION END — docs/history.md 갱신 권고',
            '━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━',
            '',
            '이번 세션에서 docs/history.md가 갱신되지 않았습니다.',
            '다음 중 해당하면 지금 갱신하세요:',
            '  - 작업이 완료된 경우',
            '  - 세션을 종료하기 전인 경우',
            '  - 컨텍스트 컴팩션이 예상되는 경우',
            '',
            '/harness-docs-update 커맨드를 사용하면 단계별로 안내합니다.',
            '━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━',
        ].join('\n');

        console.log(output);
        process.exit(0);
    } catch (_err) {
        // Stop hook: always exit 0 to avoid blocking the session
        process.exit(0);
    }
}

main();
