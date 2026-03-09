#!/usr/bin/env node
import { readFileSync, existsSync, mkdirSync, writeFileSync } from 'fs';
import { join, dirname } from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

interface HookInput {
    session_id: string;
    cwd: string;
    prompt: string;
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

        // Already loaded this session → skip silently
        if (existsSync(loadedFlagPath)) {
            process.exit(0);
        }

        // First call — ensure cache dir exists
        if (!existsSync(sessionCacheDir)) {
            mkdirSync(sessionCacheDir, { recursive: true });
        }

        // Read docs/history.md for phase/date summary
        const historyPath = join(projectDir, 'docs', 'history.md');
        let phaseSummary = '(history.md 읽기 실패)';
        if (existsSync(historyPath)) {
            const historyContent = readFileSync(historyPath, 'utf-8');
            const phaseMatch = historyContent.match(/현재 Phase\S*\s+([^\n]+)/);
            const dateMatch = historyContent.match(/Last updated[:\s]+([^\n]+)/);
            const phase = phaseMatch ? phaseMatch[1].trim() : '파악 필요';
            const date = dateMatch ? dateMatch[1].trim().replace(/\*+$/, '') : '날짜 미상';
            phaseSummary = `현재 Phase: ${phase} | ${date}`;
        }

        // Output to stdout → injected into Claude context
        const output = [
            '━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━',
            '📋 SESSION START — 4문서 자동 로드',
            '━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━',
            '',
            '세션이 시작되었습니다. 작업 전 아래 3개 문서를 먼저 읽으세요:',
            '  → docs/history.md  (이전 맥락 파악)',
            '  → docs/task.md     (현재 작업 범위)',
            '  → docs/checklist.md (완료 기준)',
            '',
            `[${phaseSummary}]`,
            '━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━',
        ].join('\n');

        console.log(output);

        // Create flag file (mtime = session start time)
        writeFileSync(loadedFlagPath, new Date().toISOString(), 'utf-8');

        process.exit(0);
    } catch (err) {
        console.error('Error in session-start-docs-loader hook:', err);
        process.exit(1);
    }
}

main();
