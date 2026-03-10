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

## Available Agents (29)

---

### 🎯 오케스트레이션

---

### pipeline-orchestrator
**Purpose:** 복잡한 멀티 에이전트 작업을 탐색→계획→실행→검증→완료 5단계 파이프라인으로 조율하는 마스터 오케스트레이터. *(Phase 6-A 추가 — TIER 1)*

**When to use:**
- 복잡한 신규 기능 개발 — 여러 에이전트를 순서대로 호출해야 할 때
- 탐색(Explore) → 계획(Plan) → 실행(Implement) → 검증(Verify) 전체 사이클이 필요할 때
- 멀티 스택 작업에서 에이전트 간 컨텍스트를 전달해야 할 때

**Integration:** ✅ Copy as-is

---

### 📋 계획

---

### planner
**Purpose:** 구현 계획 수립 — 코드베이스 분석 후 단계별 구현 전략 설계

**When to use:**
- 새 기능 개발 시작 전 전체 구조 설계
- 복잡한 리팩토링 전략 수립
- 여러 파일에 걸친 변경 계획 필요 시

**Integration:** ✅ Copy as-is

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

### 🔍 코드 품질

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

### ✅ 품질 검증

---

### reality-checker
**Purpose:** 모든 작업의 최종 품질 판정자. 기본값 NEEDS WORK — 구체적 증거(테스트 결과, 빌드 로그, 스크린샷)가 있을 때만 PASS 판정. docs/checklist.md DoD 항목과 1:1 연동. *(Phase 6-A 추가 — TIER 1)*

**When to use:**
- 작업 완료 여부 확인 — "정말 다 됐는가?" 객관적 판정 시
- DoD 체크리스트 기반 최종 게이트 통과 여부 판단 시
- `evidence-collector` 증거 수집 후 판정 단계

**Integration:** ✅ Copy as-is

---

### evidence-collector
**Purpose:** 작업 완료 증거(테스트 로그, 빌드 출력, API 응답, E2E 스크린샷)를 수집·정리하여 구조화된 증거 문서를 생성. `reality-checker`와 연계하여 품질 기준 달성 여부를 증거 기반으로 판정 가능하게 한다. *(Phase 6-A 추가 — TIER 1)*

**When to use:**
- 테스트 결과, 빌드 로그, API 응답을 문서화해야 할 때
- `reality-checker`에게 넘기기 전 증거 패키지 준비 시
- DoD 달성 증거를 팀에 공유해야 할 때

**Integration:** ✅ Copy as-is

---

### accessibility-auditor
**Purpose:** WCAG 2.2 / ARIA 기준으로 프론트엔드 접근성을 자동 점검하고 Critical/High/Medium/Low 심각도로 분류하여 수정 권고를 제시. axe-core, pa11y 자동화 도구 연동. *(Phase 6-A 추가 — TIER 1)*

**When to use:**
- 신규 UI 컴포넌트 또는 페이지 접근성 점검 시
- 키보드 내비게이션, 색상 대비, ARIA 레이블 검증 시
- 스크린리더(NVDA/VoiceOver/JAWS) 호환성 확인 시

**Integration:** ✅ Copy as-is

---

### 🔒 보안

---

### security-auditor
**Purpose:** OWASP Top 10 기준으로 JWT/SQL Injection/XSS/CSRF/민감정보/권한 상승 취약점을 자동 점검하고 CRITICAL/HIGH/MEDIUM/LOW 심각도로 분류

**When to use:**
- 보안 감사 요청 또는 새 인증 기능 추가 후
- 배포 전 보안 체크리스트 검증 시
- 취약점 리포트 생성 → `security-engineer`에게 패치 인계

**Integration:** ✅ Copy as-is

---

### security-engineer
**Purpose:** 보안 취약점 수정 코드를 구현하고 인증·인가·암호화 등 보안 기능을 신규 개발. `security-auditor`의 진단 리포트를 기반으로 패치 코드 작성. *(Phase 6-B 추가 — TIER 2)*

**역할 분리:**
| 에이전트 | 담당 |
|---|---|
| `security-auditor` | OWASP Top 10 점검 → 취약점 리포트 (진단 전용) |
| `security-engineer` | 리포트 기반 보안 코드 구현, 인증·인가 강화 (구현 전용) |

**When to use:**
- `security-auditor` 리포트 수신 후 실제 패치 코드 작성 시
- JWT 구현 강화, CORS/HTTPS 설정, SQL Injection 방어 코드 구현 시
- 신규 인증·인가 기능 설계 및 구현 시

**Integration:** ✅ Copy as-is

---

### ⚡ 성능

---

### performance-auditor
**Purpose:** FE Core Web Vitals + BE API 응답시간을 종합 점검하고 P1/P2/P3 우선순위로 개선 권고

**When to use:**
- 성능 저하 신고 또는 배포 전 점검 시
- LCP/FCP/CLS/TTI 이슈 탐지 및 원인 분석 시
- N+1 쿼리·DB 인덱스 누락 등 백엔드 성능 이슈 점검 시

**Integration:** ✅ Copy as-is

---

### performance-benchmarker
**Purpose:** API 응답 시간, DB 쿼리 실행 시간, 프론트엔드 Core Web Vitals 기준값(baseline)을 측정하고 회귀를 감지. `performance-auditor` 권고 항목 개선 전후 수치 비교. *(Phase 6-B 추가 — TIER 2)*

**역할 분리:**
| 에이전트 | 담당 |
|---|---|
| `performance-auditor` | CWV 분석, N+1 탐지, DB 인덱스 분석 → P1/P2/P3 권고 |
| `performance-benchmarker` | 기준값 수치 측정, 회귀 비교, 부하 테스트 실행 |

**When to use:**
- `performance-auditor` 권고 적용 전후 수치 비교 측정 시
- API 응답 시간 부하 테스트(Locust) 실행 및 리포트 생성 시
- 성능 기준선(baseline) 수립 및 CI/CD 성능 게이트 설정 시

**Integration:** ✅ Copy as-is

---

### 🧪 테스트

---

### test-coverage-agent
**Purpose:** 커버리지 측정 후 미커버 함수에 정상/에러/경계값 테스트 케이스를 자동 작성. pytest/Jest/JUnit5/Google Test 스택 자동 감지.

**When to use:**
- 커버리지 목표 미달 시 (예: 80% 미만)
- 신규 서비스 레이어 추가 후 테스트 보강 시
- 미테스트 함수 탐지 및 자동 작성 필요 시

**Integration:** ✅ Copy as-is

---

### api-tester
**Purpose:** 실행 중인 서버를 대상으로 외부 클라이언트 관점에서 HTTP API를 E2E 테스트. 인증 플로우, CRUD 엔드포인트, 오류 응답을 블랙박스 관점에서 검증. *(Phase 6-B 추가 — TIER 2)*

**역할 분리:**
| 에이전트 | 담당 |
|---|---|
| `test-coverage-agent` | 소스 코드 기반 단위·통합 테스트 + 커버리지 분석 (내부) |
| `api-tester` | 실행 중인 서버 대상 HTTP 요청/응답 E2E 검증 (외부) |

**When to use:**
- 배포된 API의 인증 플로우, CRUD, 오류 응답을 통합 검증할 때
- `api-spec-generator` 생성 openapi.yaml 기반 테스트 케이스 도출 시
- Newman, pytest-httpx, Supertest 기반 E2E 테스트 실행 시

**Integration:** ✅ Copy as-is

---

### 🚀 CI/CD & DevOps

---

### devops-automator
**Purpose:** CI/CD 파이프라인 코드를 자동 생성하고 인프라를 설계. GitHub Actions 워크플로우, Dockerfile 멀티스테이지 최적화, ECR 이미지 빌드/푸시, AWS ECS 배포 파이프라인(Blue/Green·Rolling) 설계. *(Phase 6-A 추가 — TIER 1)*

**When to use:**
- 신규 프로젝트에 GitHub Actions CI/CD 파이프라인 구성 시
- Dockerfile 최적화 또는 멀티스테이지 빌드 설계 시
- AWS ECR/ECS 배포 자동화 워크플로우 생성 시

**Integration:** ✅ Copy as-is

---

### ⚡ 프로토타이핑

---

### rapid-prototyper
**Purpose:** 신기능 아이디어를 빠르게 PoC/MVP로 구현. "일단 돌아가는 것" 우선 원칙. FastAPI/Next.js/NestJS 모두 즉시 착수 가능. 검증 완료 후 `code-refactor-master`에게 인계. *(Phase 6-A 추가 — TIER 1)*

**When to use:**
- 새 기능 아이디어의 기술 실현 가능성 빠르게 확인 시
- 스프린트 내 동작하는 프로토타입 필요 시
- 설계 확정 전 여러 구현 방식을 빠르게 비교해볼 때

**Integration:** ✅ Copy as-is

---

### 🗄️ 데이터베이스

---

### db-migration-agent
**Purpose:** DB 스키마 변경을 Alembic/Prisma/Django/Flyway 마이그레이션 파일로 자동화

**When to use:**
- 모델/엔티티 변경 후 마이그레이션 파일 생성 시
- DROP/NOT NULL 등 위험 연산 포함 마이그레이션 검토 시
- 스택별(FastAPI/NestJS/Django/Spring Boot) 마이그레이션 자동화 필요 시

**Integration:** ✅ Copy as-is

---

### 📚 문서화

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

### api-spec-generator
**Purpose:** 백엔드 소스 코드를 정적 분석하여 OpenAPI 3.0 YAML 스펙을 자동 생성

**When to use:**
- API 문서화 최초 생성 시
- 라우터/컨트롤러 변경 후 openapi.yaml 갱신 필요 시
- 여러 서비스 스펙을 하나의 YAML로 병합 시

**Integration:** ✅ Copy as-is

---

### technical-writer
**Purpose:** 코드를 읽고 독자 관점으로 기술 문서를 자동 생성. README, API 가이드(api-spec-generator 연계), ADR, CONTRIBUTING.md 작성. *(Phase 6-A 추가 — TIER 1)*

**When to use:**
- 신규 또는 기존 서비스의 README.md 작성/업데이트 시
- API 가이드, 아키텍처 결정 기록(ADR), 기여 가이드 생성 시
- `documentation-architect`가 설계한 구조를 실제 콘텐츠로 채울 때

**Integration:** ✅ Copy as-is

---

### 🎨 UI/UX 설계

---

### ui-designer
**Purpose:** 요구사항을 분석하여 디자인 토큰 체계, CVA 컴포넌트 변형(variants), 다크/라이트 모드 전략을 설계하고 shadcn/ui + Tailwind CSS 기반 재사용 가능한 UI 컴포넌트를 구현. *(Phase 6-B 추가 — TIER 2)*

**역할 분리:**
| 자산 | 담당 |
|---|---|
| `frontend-design` (Skill) | 디자인 원칙·패턴 가이드라인 제공 (지침) |
| `ui-designer` (Agent) | 요구사항 분석 → 컴포넌트 설계 → 실제 구현 (실행) |

**When to use:**
- 신규 UI 컴포넌트 설계 및 구현 시 (디자인 토큰 + CVA 패턴 적용)
- 다크/라이트 모드 전략 수립 및 Tailwind CSS 구성 시
- `accessibility-auditor`와 연계하여 접근성 기준 충족 UI 구현 시

**Integration:** ✅ Copy as-is

---

### ux-researcher
**Purpose:** 사용자 리서치를 수행하고 페르소나를 정의하며 사용자 여정·흐름을 분석하여 UX 개선 권고를 도출. 인사이트를 `ui-designer`에게 인계. *(Phase 6-B 추가 — TIER 2)*

**역할 분리:**
| 에이전트 | 담당 |
|---|---|
| `ux-researcher` | 사용자 리서치, 페르소나, 사용성 분석, 개선 권고 도출 |
| `ui-designer` | ux-researcher 인사이트 기반 실제 UI 설계 및 구현 |
| `product-manager` | 비즈니스 요구사항 정의, 우선순위 결정 |

**When to use:**
- 신규 기능의 사용자 인터뷰, 사용성 테스트, 경쟁사 분석 시
- 페르소나 문서, 사용자 흐름(As-Is/To-Be) 다이어그램 작성 시
- `ui-designer` 착수 전 UX 인사이트 패키지 준비 시

**Integration:** ✅ Copy as-is

---

### 📊 제품 관리

---

### product-manager
**Purpose:** 비즈니스 목표를 분석하여 PRD를 작성하고 RICE/MoSCoW 프레임워크로 기능 우선순위를 결정. 완료 후 `planner` 에이전트에게 기술 구현 계획 인계. *(Phase 6-B 추가 — TIER 2)*

**역할 분리:**
| 에이전트 | 담당 |
|---|---|
| `planner` | 기술 구현 계획 수립, 아키텍처 설계, task 분해 |
| `product-manager` | 비즈니스 목표, 사용자 스토리, 기능 우선순위 정의 |

**When to use:**
- 신규 기능의 PRD(Product Requirements Document) 작성 시
- 백로그 우선순위 결정 (RICE 점수 산정, MoSCoW 분류) 시
- OKR 기반 기능 목표 설정 및 수용 기준 정의 시

**Integration:** ✅ Copy as-is

---

### 🤖 자동화

---

### auto-error-resolver
**Purpose:** Automatically fix TypeScript compilation errors

**When to use:**
- Build failures with TypeScript errors
- After refactoring that breaks types
- Systematic error resolution needed

**Integration:** ⚠️ May need path updates

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

### 🔑 인증

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

### 🔎 리서치

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

## Agent Quick Reference (29)

| Agent | Category | Complexity | Customization | Phase |
|-------|----------|-----------|---------------|-------|
| `pipeline-orchestrator` | 오케스트레이션 | Very High | ✅ None | 6-A |
| `planner` | 계획 | Medium | ✅ None | 기존 |
| `plan-reviewer` | 계획 | Low | ✅ None | 기존 |
| `refactor-planner` | 계획 | Medium | ✅ None | 기존 |
| `code-architecture-reviewer` | 코드 품질 | Medium | ✅ None | 기존 |
| `code-refactor-master` | 코드 품질 | High | ✅ None | 기존 |
| `reality-checker` | 품질 검증 | Medium | ✅ None | 6-A |
| `evidence-collector` | 품질 검증 | Medium | ✅ None | 6-A |
| `accessibility-auditor` | 품질 검증 | Medium | ✅ None | 6-A |
| `security-auditor` | 보안 | Medium | ✅ None | 기존 |
| `security-engineer` | 보안 | High | ✅ None | 6-B |
| `performance-auditor` | 성능 | Medium | ✅ None | 기존 |
| `performance-benchmarker` | 성능 | Medium | ✅ None | 6-B |
| `test-coverage-agent` | 테스트 | High | ✅ None | 기존 |
| `api-tester` | 테스트 | High | ✅ None | 6-B |
| `devops-automator` | CI/CD & DevOps | High | ✅ None | 6-A |
| `rapid-prototyper` | 프로토타이핑 | Medium | ✅ None | 6-A |
| `db-migration-agent` | 데이터베이스 | High | ✅ None | 기존 |
| `documentation-architect` | 문서화 | Medium | ✅ None | 기존 |
| `api-spec-generator` | 문서화 | Medium | ✅ None | 기존 |
| `technical-writer` | 문서화 | Medium | ✅ None | 6-A |
| `ui-designer` | UI/UX 설계 | High | ✅ None | 6-B |
| `ux-researcher` | UI/UX 설계 | Medium | ✅ None | 6-B |
| `product-manager` | 제품 관리 | Medium | ✅ None | 6-B |
| `auto-error-resolver` | 자동화 | Low | ⚠️ Paths | 기존 |
| `frontend-error-fixer` | 자동화 | Medium | ⚠️ Screenshot paths | 기존 |
| `auth-route-tester` | 인증 | Medium | ⚠️ Auth setup | 기존 |
| `auth-route-debugger` | 인증 | Medium | ⚠️ Auth setup | 기존 |
| `web-research-specialist` | 리서치 | Low | ✅ None | 기존 |

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

**기존 에이전트 활용 안내:**
- `db-migration-agent`: 스택 감지(Alembic/Prisma/Django/Flyway) 후 자동 마이그레이션 — "마이그레이션 만들어줘"
- `api-spec-generator`: 소스 정적 분석 → openapi.yaml 생성 — "API 문서 만들어줘"
- `test-coverage-agent`: 커버리지 측정 → 미커버 함수 테스트 자동 작성 — "커버리지 올려줘"
- `performance-auditor`: CWV + API 응답시간 점검 → P1/P2/P3 권고 — "성능 점검해줘"
- `security-auditor`: OWASP Top 10 취약점 점검 → 심각도 분류 — "보안 감사해줘"

**Phase 6-A 신규 에이전트 활용 안내:**
- `pipeline-orchestrator`: 복잡한 멀티 에이전트 작업 조율 — "전체 파이프라인 실행해줘"
- `reality-checker`: 품질 게이트 판정 — "작업 완료 확인해줘", "DoD 검증해줘"
- `evidence-collector`: 증거 수집 — "테스트 결과 문서화해줘", "작업 완료 증거 모아줘"
- `accessibility-auditor`: 접근성 점검 — "WCAG 점검해줘", "접근성 감사해줘"
- `devops-automator`: CI/CD 구성 — "GitHub Actions 워크플로우 만들어줘", "Dockerfile 최적화해줘"
- `rapid-prototyper`: PoC 구현 — "일단 빠르게 만들어줘", "PoC 구현해줘"
- `technical-writer`: 기술 문서 작성 — "README 작성해줘", "API 가이드 만들어줘"

**Phase 6-B 신규 에이전트 활용 안내:**
- `security-engineer`: 보안 패치 구현 — "보안 취약점 수정해줘", "JWT 강화해줘"
- `performance-benchmarker`: 성능 측정 — "API 응답시간 측정해줘", "부하 테스트 실행해줘"
- `api-tester`: API E2E 테스트 — "API 전체 E2E 테스트해줘", "인증 플로우 테스트해줘"
- `ui-designer`: UI 컴포넌트 설계 — "디자인 토큰 설계해줘", "UI 컴포넌트 구현해줘"
- `ux-researcher`: UX 리서치 — "사용자 흐름 분석해줘", "UX 개선점 찾아줘"
- `product-manager`: PRD 작성 — "요구사항 정의해줘", "기능 우선순위 정해줘"

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
