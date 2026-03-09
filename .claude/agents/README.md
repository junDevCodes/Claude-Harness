# Agents

Specialized agents for complex, multi-step tasks.

---

## What Are Agents?

Agents are autonomous Claude instances that handle specific complex tasks. Unlike skills (which provide inline guidance), agents:
- Run as separate sub-tasks
- Work autonomously with minimal supervision
- Have specialized tool access
- Return comprehensive reports when complete

**Key advantage:** Agents are **standalone** - just copy the `.md` file and use immediately!

---

## Available Agents (16)

### planner
**Purpose:** 구현 계획 수립 — 코드베이스 분석 후 단계별 구현 전략 설계

**When to use:**
- 새 기능 개발 시작 전 전체 구조 설계
- 복잡한 리팩토링 전략 수립
- 여러 파일에 걸친 변경 계획 필요 시

**Integration:** ✅ Copy as-is

---

### code-architecture-reviewer
**Purpose:** Review code for architectural consistency and best practices

**When to use:**
- After implementing a new feature
- Before merging significant changes
- When refactoring code
- To validate architectural decisions

**Integration:** ✅ Copy as-is

---

### code-refactor-master
**Purpose:** Plan and execute comprehensive refactoring

**When to use:**
- Reorganizing file structures
- Breaking down large components
- Updating import paths after moves
- Improving code maintainability

**Integration:** ✅ Copy as-is

---

### documentation-architect
**Purpose:** Create comprehensive documentation

**When to use:**
- Documenting new features
- Creating API documentation
- Writing developer guides
- Generating architectural overviews

**Integration:** ✅ Copy as-is

---

### frontend-error-fixer
**Purpose:** Debug and fix frontend errors

**When to use:**
- Browser console errors
- TypeScript compilation errors in frontend
- React errors
- Build failures

**Integration:** ⚠️ May reference screenshot paths - update if needed

---

### plan-reviewer
**Purpose:** Review development plans before implementation

**When to use:**
- Before starting complex features
- Validating architectural plans
- Identifying potential issues early
- Getting second opinion on approach

**Integration:** ✅ Copy as-is

---

### refactor-planner
**Purpose:** Create comprehensive refactoring strategies

**When to use:**
- Planning code reorganization
- Modernizing legacy code
- Breaking down large files
- Improving code structure

**Integration:** ✅ Copy as-is

---

### web-research-specialist
**Purpose:** Research technical issues online

**When to use:**
- Debugging obscure errors
- Finding solutions to problems
- Researching best practices
- Comparing implementation approaches

**Integration:** ✅ Copy as-is

---

### auth-route-tester
**Purpose:** Test authenticated API endpoints

**When to use:**
- Testing routes with JWT cookie auth
- Validating endpoint functionality
- Debugging authentication issues

**Integration:** ⚠️ Requires JWT cookie-based auth

---

### auth-route-debugger
**Purpose:** Debug authentication issues

**When to use:**
- Auth failures
- Token issues
- Cookie problems
- Permission errors

**Integration:** ⚠️ Requires JWT cookie-based auth

---

### auto-error-resolver
**Purpose:** Automatically fix TypeScript compilation errors

**When to use:**
- Build failures with TypeScript errors
- After refactoring that breaks types
- Systematic error resolution needed

**Integration:** ⚠️ May need path updates

---

### db-migration-agent
**Purpose:** DB 스키마 변경을 Alembic/Prisma/Django/Flyway 마이그레이션 파일로 자동화

**When to use:**
- 모델/엔티티 변경 후 마이그레이션 파일 생성 시
- DROP/NOT NULL 등 위험 연산 포함 마이그레이션 검토 시
- 스택별(FastAPI/NestJS/Django/Spring Boot) 마이그레이션 자동화 필요 시

**Integration:** ✅ Copy as-is

---

### api-spec-generator
**Purpose:** 백엔드 소스 코드를 정적 분석하여 OpenAPI 3.0 YAML 스펙을 자동 생성

**When to use:**
- API 문서화 최초 생성 시
- 라우터/컨트롤러 변경 후 openapi.yaml 갱신 필요 시
- 여러 서비스 스펙을 하나의 YAML로 병합 시

**Integration:** ✅ Copy as-is

---

### test-coverage-agent
**Purpose:** 커버리지 측정 후 미커버 함수에 정상/에러/경계값 테스트 케이스를 자동 작성

**When to use:**
- 커버리지 목표 미달 시 (예: 80% 미만)
- 신규 서비스 레이어 추가 후 테스트 보강 시
- pytest/Jest/JUnit5/Google Test 스택에서 미테스트 함수 탐지 시

**Integration:** ✅ Copy as-is

---

### performance-auditor
**Purpose:** FE Core Web Vitals + BE API 응답시간을 종합 점검하고 P1/P2/P3 우선순위로 개선 권고

**When to use:**
- 성능 저하 신고 또는 배포 전 점검 시
- LCP/FCP/CLS/TTI 이슈 탐지 및 원인 분석 시
- N+1 쿼리·DB 인덱스 누락 등 백엔드 성능 이슈 점검 시

**Integration:** ✅ Copy as-is

---

### security-auditor
**Purpose:** OWASP Top 10 기준으로 JWT/SQL Injection/XSS/CSRF/민감정보/권한 상승 취약점을 자동 점검

**When to use:**
- 보안 감사 요청 또는 새 인증 기능 추가 후
- 배포 전 보안 체크리스트 검증 시
- CRITICAL/HIGH/MEDIUM/LOW 심각도 분류 및 조치 방법 필요 시

**Integration:** ✅ Copy as-is

---

## How to Integrate an Agent

### Standard Integration (Most Agents)

**Step 1: Copy the file**
```bash
cp showcase/.claude/agents/agent-name.md \
   your-project/.claude/agents/
```

**Step 2: Verify (optional)**
```bash
# Check for hardcoded paths
grep -n "~/git/\|/root/git/\|/Users/" your-project/.claude/agents/agent-name.md
```

**Step 3: Use it**
Ask Claude: "Use the [agent-name] agent to [task]"

That's it! Agents work immediately.

---

### Agents Requiring Customization

**frontend-error-fixer:**
- May reference screenshot paths
- Ask user: "Where should screenshots be saved?"
- Update paths in agent file

**auth-route-tester / auth-route-debugger:**
- Require JWT cookie authentication
- Update service URLs from examples
- Customize for user's auth setup

**auto-error-resolver:**
- May have hardcoded project paths
- Update to use `$CLAUDE_PROJECT_DIR` or relative paths

---

## When to Use Agents vs Skills

| Use Agents When... | Use Skills When... |
|-------------------|-------------------|
| Task requires multiple steps | Need inline guidance |
| Complex analysis needed | Checking best practices |
| Autonomous work preferred | Want to maintain control |
| Task has clear end goal | Ongoing development work |
| Example: "Review all controllers" | Example: "Creating a new route" |

**Both can work together:**
- Skill provides patterns during development
- Agent reviews the result when complete

---

## Agent Quick Reference

| Agent | Complexity | Customization | Auth Required |
|-------|-----------|---------------|---------------|
| planner | Medium | ✅ None | No |
| code-architecture-reviewer | Medium | ✅ None | No |
| code-refactor-master | High | ✅ None | No |
| documentation-architect | Medium | ✅ None | No |
| frontend-error-fixer | Medium | ⚠️ Screenshot paths | No |
| plan-reviewer | Low | ✅ None | No |
| refactor-planner | Medium | ✅ None | No |
| web-research-specialist | Low | ✅ None | No |
| auth-route-tester | Medium | ⚠️ Auth setup | JWT cookies |
| auth-route-debugger | Medium | ⚠️ Auth setup | JWT cookies |
| auto-error-resolver | Low | ⚠️ Paths | No |
| db-migration-agent | High | ✅ None | No |
| api-spec-generator | Medium | ✅ None | No |
| test-coverage-agent | High | ✅ None | No |
| performance-auditor | Medium | ✅ None | No |
| security-auditor | Medium | ✅ None | No |

---

## For Claude Code

**When integrating agents for a user:**

1. **Read [CLAUDE_INTEGRATION_GUIDE.md](../../CLAUDE_INTEGRATION_GUIDE.md)**
2. **Just copy the .md file** - agents are standalone
3. **Check for hardcoded paths:**
   ```bash
   grep "~/git/\|/root/" agent-name.md
   ```
4. **Update paths if found** to `$CLAUDE_PROJECT_DIR` or `.`
5. **For auth agents:** Ask if they use JWT cookie auth first

**신규 자동화 에이전트 활용 안내:**
- `db-migration-agent`: 스택 감지(Alembic/Prisma/Django/Flyway) 후 자동 마이그레이션 — "마이그레이션 만들어줘" 요청 시 사용
- `api-spec-generator`: 소스 정적 분석 → openapi.yaml 생성 — "API 문서 만들어줘" 요청 시 사용
- `test-coverage-agent`: 커버리지 측정 → 미커버 함수 테스트 자동 작성 — "커버리지 올려줘" 요청 시 사용
- `performance-auditor`: CWV + API 응답시간 점검 → P1/P2/P3 권고 — "성능 점검해줘" 요청 시 사용
- `security-auditor`: OWASP Top 10 취약점 점검 → 심각도 분류 — "보안 감사해줘" 요청 시 사용

**That's it!** Agents are the easiest components to integrate.

---

## Creating Your Own Agents

Agents are markdown files with optional YAML frontmatter:

```markdown
# Agent Name

## Purpose
What this agent does

## Instructions
Step-by-step instructions for autonomous execution

## Tools Available
List of tools this agent can use

## Expected Output
What format to return results in
```

**Tips:**
- Be very specific in instructions
- Break complex tasks into numbered steps
- Specify exactly what to return
- Include examples of good output
- List available tools explicitly

---

## Troubleshooting

### Agent not found

**Check:**
```bash
# Is agent file present?
ls -la .claude/agents/[agent-name].md
```

### Agent fails with path errors

**Check for hardcoded paths:**
```bash
grep "~/\|/root/\|/Users/" .claude/agents/[agent-name].md
```

**Fix:**
```bash
sed -i 's|~/git/.*project|$CLAUDE_PROJECT_DIR|g' .claude/agents/[agent-name].md
```

---

## Next Steps

1. **Browse agents above** - Find ones useful for your work
2. **Copy what you need** - Just the .md file
3. **Ask Claude to use them** - "Use [agent] to [task]"
4. **Create your own** - Follow the pattern for your specific needs

**Questions?** See [CLAUDE_INTEGRATION_GUIDE.md](../../CLAUDE_INTEGRATION_GUIDE.md)
