#!/usr/bin/env node
import { readFileSync } from 'fs';

interface HookInput {
    tool_name: string;
    tool_input: { command?: string };
    tool_response?: { output?: string };
    session_id: string;
}

function main() {
    try {
        const input = readFileSync(0, 'utf-8');
        const data: HookInput = JSON.parse(input);
        const { tool_name, tool_input, tool_response } = data;

        // 1. Bash 툴이고 gh pr create 실행 명령인지 확인
        // ^\s* — 선두 공백 허용
        // (?:\w+=\S+\s+)* — 선행 환경변수(KEY=VAL) 허용
        // gh\s+pr\s+create — 실제 gh 명령으로 시작해야 함 (echo/cat 안의 인용구 제외)
        if (tool_name !== 'Bash') process.exit(0);
        const command = tool_input.command ?? '';
        const isGhPrCreate = /^\s*(?:\w+=\S+\s+)*gh\s+pr\s+create/.test(command);
        if (!isGhPrCreate) process.exit(0);

        // 2. SKIP_PR_REVIEW 환경변수 체크
        if (process.env.SKIP_PR_REVIEW) process.exit(0);

        // 3. PR URL 추출
        const output = tool_response?.output ?? '';
        const urlMatch = output.match(/https:\/\/github\.com\/[^\s]+\/pull\/\d+/);
        const prUrl = urlMatch ? urlMatch[0] : '(PR URL 추출 실패)';

        // 4. 리뷰 권고 메시지 출력 → exit 2 (Claude 컨텍스트 주입)
        console.log([
            '━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━',
            '🔍 PR CREATED — code-architecture-reviewer 실행 권고',
            '━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━',
            '',
            `PR: ${prUrl}`,
            '',
            'code-architecture-reviewer 에이전트로 코드 리뷰를 실행하세요.',
            '리뷰 결과는 docs/reviews/[날짜]-[브랜치]-review.md에 저장됩니다.',
            '',
            '건너뛰려면: SKIP_PR_REVIEW=1 설정',
            '━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━',
        ].join('\n'));

        process.exit(2); // PostToolUse exit 2: stdout → Claude 컨텍스트 주입
    } catch (_err) {
        // PostToolUse hook: always exit 0 on error to avoid blocking
        process.exit(0);
    }
}

main();
