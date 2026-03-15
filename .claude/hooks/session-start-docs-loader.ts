#!/usr/bin/env node
import { readFileSync, existsSync, mkdirSync, writeFileSync, readdirSync, statSync, unlinkSync } from 'fs';
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
        const safeSessionId = session_id.replace(/[^a-zA-Z0-9_-]/g, '_');
        const loadedFlagPath = join(sessionCacheDir, `${safeSessionId}.loaded`);

        // Already loaded this session → skip silently
        if (existsSync(loadedFlagPath)) {
            process.exit(0);
        }

        // First call — ensure cache dir exists
        if (!existsSync(sessionCacheDir)) {
            mkdirSync(sessionCacheDir, { recursive: true });
        }

        // TTL 정리: 7일 이상 된 .loaded 파일 삭제
        const CACHE_TTL_DAYS = 7;
        const cutoff = new Date();
        cutoff.setDate(cutoff.getDate() - CACHE_TTL_DAYS);
        for (const entry of readdirSync(sessionCacheDir)) {
            if (!entry.endsWith('.loaded')) continue;
            const entryPath = join(sessionCacheDir, entry);
            if (statSync(entryPath).mtime < cutoff) {
                unlinkSync(entryPath);
            }
        }

        // Check next-task-pending.flag (이전 task 완료 신호)
        const flagPath = join(sessionCacheDir, 'next-task-pending.flag');
        let nextTaskPending = false;
        if (existsSync(flagPath)) {
            nextTaskPending = true;
            unlinkSync(flagPath);
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

        // task.md 구조 검증
        const warnings: string[] = [];
        const taskPath = join(projectDir, 'docs', 'task.md');
        const checklistPath = join(projectDir, 'docs', 'checklist.md');
        if (existsSync(taskPath)) {
            const taskContent = readFileSync(taskPath, 'utf-8');
            if (!/plan\.md/i.test(taskContent)) {
                warnings.push('⚠️  task.md에 plan.md 출처 참조 없음');
            }
            if (!/^##\s+\S/m.test(taskContent)) {
                warnings.push('⚠️  task.md 필수 섹션(## 헤딩) 없음');
            }
        }
        if (existsSync(checklistPath)) {
            const checklistContent = readFileSync(checklistPath, 'utf-8');
            const unchecked = (checklistContent.match(/- \[ \]/g) || []).length;
            if (unchecked > 15) {
                warnings.push(`⚠️  checklist.md 미완료 ${unchecked}개 — 기능 범위 초과 의심 (15개 초과)`);
            }
        }

        // Output to stdout → injected into Claude context
        const lines = [
            '━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━',
            '📋 SESSION START — 4문서 자동 로드',
            '━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━',
            '',
        ];

        if (nextTaskPending) {
            lines.push('✅ 이전 task 완료 감지! /harness-plan-sync 로 다음 task.md를 준비하세요.');
            lines.push('');
        }

        lines.push('세션이 시작되었습니다. 작업 전 아래 3개 문서를 먼저 읽으세요:');
        lines.push('  → docs/history.md  (이전 맥락 파악)');
        lines.push('  → docs/task.md     (현재 작업 범위)');
        lines.push('  → docs/checklist.md (완료 기준)');
        lines.push('');
        lines.push(`[${phaseSummary}]`);

        if (warnings.length > 0) {
            lines.push('');
            for (const w of warnings) {
                lines.push(w);
            }
        }

        lines.push('━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━');
        const output = lines.join('\n');

        console.log(output);

        // Create flag file (mtime = session start time)
        writeFileSync(loadedFlagPath, new Date().toISOString(), 'utf-8');

        process.exit(0);
    } catch (_err) {
        // UserPromptSubmit hook: 예외 발생해도 안전 종료 (프롬프트 차단 방지)
        // docs-update-reminder.ts 패턴과 통일
        process.exit(0);
    }
}

main();
