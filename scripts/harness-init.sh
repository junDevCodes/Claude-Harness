#!/usr/bin/env bash
# harness-init.sh — Advanced Harness 프로젝트 초기화 스크립트
# Claude 없이 터미널에서 직접 실행 가능

set -euo pipefail

# ─── 색상 ───────────────────────────────────────────────────────────
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m'

# ─── 하네스 루트 자동 감지 ───────────────────────────────────────────
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
HARNESS_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"

# ─── 도움말 ──────────────────────────────────────────────────────────
usage() {
  echo -e "${BOLD}harness-init.sh${NC} — Advanced Harness 프로젝트 초기화"
  echo ""
  echo -e "${BOLD}사용법:${NC}"
  echo "  bash scripts/harness-init.sh [OPTIONS]"
  echo ""
  echo -e "${BOLD}옵션:${NC}"
  echo "  --help, -h    이 도움말 출력"
  echo "  --stack N     스택 번호 직접 지정 (0~10)"
  echo "  --target DIR  대상 디렉토리 직접 지정"
  echo ""
  echo -e "${BOLD}스택 선택:${NC}"
  echo "  [1]  FastAPI + Next.js"
  echo "  [2]  NestJS + Next.js"
  echo "  [3]  Django + Next.js"
  echo "  [4]  Express + Next.js"
  echo "  [5]  Spring Boot + Next.js"
  echo "  [6]  FastAPI only"
  echo "  [7]  Next.js only"
  echo "  [8]  React Native"
  echo "  [9]  C Embedded"
  echo "  [10] C++ Embedded"
  echo "  [0]  스킬/에이전트만 (.claude/ 복사, 베이스 코드 제외)"
  echo ""
  echo -e "${BOLD}예시:${NC}"
  echo "  bash scripts/harness-init.sh"
  echo "  bash scripts/harness-init.sh --stack 1 --target ~/myproject"
  exit 0
}

# ─── 스택 메뉴 출력 ───────────────────────────────────────────────────
print_menu() {
  echo ""
  echo -e "${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
  echo -e "${BOLD}  Advanced Harness — 프로젝트 초기화${NC}"
  echo -e "${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
  echo ""
  echo -e "${BOLD}스택을 선택하세요:${NC}"
  echo ""
  echo -e "  ${YELLOW}풀스택${NC}"
  echo "  [1]  FastAPI + Next.js    (Python 백엔드 + React 프론트엔드)"
  echo "  [2]  NestJS + Next.js     (TypeScript 풀스택)"
  echo "  [3]  Django + Next.js     (Python DRF + React 프론트엔드)"
  echo "  [4]  Express + Next.js    (Node.js 백엔드 + React 프론트엔드)"
  echo "  [5]  Spring Boot + Next.js (Java 백엔드 + React 프론트엔드)"
  echo ""
  echo -e "  ${YELLOW}단독 스택${NC}"
  echo "  [6]  FastAPI only          (Python 백엔드만)"
  echo "  [7]  Next.js only          (React 프론트엔드만)"
  echo "  [8]  React Native          (Expo 모바일 앱)"
  echo "  [9]  C Embedded            (임베디드 펌웨어 — FreeRTOS/HAL)"
  echo "  [10] C++ Embedded          (임베디드 펌웨어 — RAII/HAL)"
  echo ""
  echo -e "  ${YELLOW}자산만${NC}"
  echo "  [0]  스킬/에이전트만       (.claude/ 복사, 베이스 코드 제외)"
  echo ""
  echo -e "${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
}

# ─── 스택 정보 반환 ──────────────────────────────────────────────────
get_stack_info() {
  local choice=$1
  case $choice in
    1)  STACK_NAME="fastapi-nextjs";   BACKEND="fastapi"; FRONTEND="nextjs" ;;
    2)  STACK_NAME="nestjs-nextjs";    BACKEND="nestjs";  FRONTEND="nextjs" ;;
    3)  STACK_NAME="django-nextjs";    BACKEND="django";  FRONTEND="nextjs" ;;
    4)  STACK_NAME="express-nextjs";   BACKEND="express"; FRONTEND="nextjs" ;;
    5)  STACK_NAME="spring-nextjs";    BACKEND="spring-boot"; FRONTEND="nextjs" ;;
    6)  STACK_NAME="fastapi-only";     BACKEND="fastapi"; FRONTEND="" ;;
    7)  STACK_NAME="nextjs-only";      BACKEND="";        FRONTEND="nextjs" ;;
    8)  STACK_NAME="react-native";     BACKEND="";        FRONTEND=""; MOBILE="react-native" ;;
    9)  STACK_NAME="c-embedded";       BACKEND="";        FRONTEND=""; FIRMWARE="c-embedded" ;;
    10) STACK_NAME="cpp-embedded";     BACKEND="";        FRONTEND=""; FIRMWARE="cpp-embedded" ;;
    0)  STACK_NAME="skills-only";      BACKEND="";        FRONTEND="" ;;
    *)  echo -e "${RED}오류: 유효하지 않은 선택입니다.${NC}"; exit 1 ;;
  esac
}

# ─── 경로 존재 검증 ──────────────────────────────────────────────────
validate_base_path() {
  local stack_dir=$1
  if [[ -n "$stack_dir" && ! -d "${HARNESS_DIR}/base/${stack_dir}" ]]; then
    echo -e "${YELLOW}경고: ${HARNESS_DIR}/base/${stack_dir} 디렉토리가 없습니다.${NC}"
    echo "  베이스 코드 복사를 건너뜁니다."
    return 1
  fi
  return 0
}

# ─── .claude/ 복사 ───────────────────────────────────────────────────
copy_claude_assets() {
  local target=$1
  echo -e "\n${CYAN}[1/5] .claude/ 자산 복사 중...${NC}"
  cp -r "${HARNESS_DIR}/.claude" "${target}/"
  # 훅 실행 권한 부여
  find "${target}/.claude/hooks" -name "*.sh" -exec chmod +x {} \; 2>/dev/null || true
  # 세션 캐시 초기화
  mkdir -p "${target}/.claude/.session-cache"
  touch "${target}/.claude/.session-cache/.gitkeep"
  echo -e "  ${GREEN}✓${NC} .claude/ 복사 완료"
  echo -e "  ${GREEN}✓${NC} 훅 실행 권한 설정 완료"
}

# ─── npm 패키지 설치 (tsx 필요) ─────────────────────────────────────
install_hook_deps() {
  local target=$1
  if [[ -f "${target}/.claude/hooks/package.json" ]]; then
    echo -e "\n${CYAN}[2/5] 훅 의존성 설치 중...${NC}"
    (cd "${target}/.claude/hooks" && npm install --silent 2>/dev/null) || \
      echo -e "  ${YELLOW}⚠${NC}  npm install 실패 — 수동으로 실행: cd .claude/hooks && npm install"
    echo -e "  ${GREEN}✓${NC} 훅 의존성 설치 완료"
  else
    echo -e "\n${CYAN}[2/5] 훅 의존성${NC}"
    echo -e "  ${YELLOW}→${NC}  .claude/hooks/package.json 없음 — 건너뜀"
  fi
}

# ─── 베이스 코드 복사 ────────────────────────────────────────────────
copy_base_code() {
  local target=$1
  local choice=$2

  echo -e "\n${CYAN}[3/5] 베이스 코드 복사 중...${NC}"

  BACKEND="${BACKEND:-}"
  FRONTEND="${FRONTEND:-}"
  MOBILE="${MOBILE:-}"
  FIRMWARE="${FIRMWARE:-}"

  if [[ $choice -eq 0 ]]; then
    echo -e "  ${YELLOW}→${NC}  스킬/에이전트만 모드 — 베이스 코드 건너뜀"
    return
  fi

  # 백엔드 복사
  if [[ -n "$BACKEND" ]]; then
    if validate_base_path "$BACKEND"; then
      cp -r "${HARNESS_DIR}/base/${BACKEND}" "${target}/backend"
      echo -e "  ${GREEN}✓${NC} backend/ 복사 완료 (base/${BACKEND})"
    fi
  fi

  # 프론트엔드 복사
  if [[ -n "$FRONTEND" ]]; then
    if validate_base_path "$FRONTEND"; then
      cp -r "${HARNESS_DIR}/base/${FRONTEND}" "${target}/frontend"
      echo -e "  ${GREEN}✓${NC} frontend/ 복사 완료 (base/${FRONTEND})"
    fi
  fi

  # 모바일 복사
  if [[ -n "$MOBILE" ]]; then
    if validate_base_path "$MOBILE"; then
      cp -r "${HARNESS_DIR}/base/${MOBILE}" "${target}/mobile"
      echo -e "  ${GREEN}✓${NC} mobile/ 복사 완료 (base/${MOBILE})"
    fi
  fi

  # 펌웨어 복사
  if [[ -n "$FIRMWARE" ]]; then
    if validate_base_path "$FIRMWARE"; then
      cp -r "${HARNESS_DIR}/base/${FIRMWARE}" "${target}/firmware"
      echo -e "  ${GREEN}✓${NC} firmware/ 복사 완료 (base/${FIRMWARE})"
    fi
  fi
}

# ─── docs/ 4문서 stub 생성 ───────────────────────────────────────────
create_docs_stubs() {
  local target=$1
  echo -e "\n${CYAN}[4/5] docs/ 4문서 stub 생성 중...${NC}"
  mkdir -p "${target}/docs"

  cat > "${target}/docs/plan.md" << 'STUB'
# Plan — 전체 작업 기획서

> **문서 역할:** 전체 완성 계획을 큰 단위(Phase)로 관리하는 기획서.

---

## Phase 1 — 초기 설정

- [ ] 환경 설정
- [ ] 기본 구조 확인

---

*Last updated: (날짜 입력)*
STUB

  cat > "${target}/docs/task.md" << 'STUB'
# Task — 현재 작업 상세 계획서

> **현재 Phase:** Phase 1 — 초기 설정

---

## Phase 1 — 초기 설정

### 단계별 실행 계획

| 단계 | 산출물 | 상태 |
|---|---|---|
| 1-1 | 환경 변수 설정 | [ ] |
| 1-2 | 데이터베이스 설정 | [ ] |

---

*Last updated: (날짜 입력)*
STUB

  cat > "${target}/docs/history.md" << 'STUB'
# History — 작업 맥락 문서

> **문서 역할:** 이전까지 진행된 모든 작업 내용을 기록하고,
> 다음 세션에서 task.md 작업의 맥락을 이어받을 수 있도록 작성하는 문서.

---

## 현재 상태 요약

- **현재 Phase:** Phase 1 — 초기 설정
- **직전 완료:** 프로젝트 초기화 (harness-init.sh)
- **다음 작업:** Phase 1-1 환경 변수 설정

---

## 작업 이력

### (날짜) — 프로젝트 초기화

- Advanced Harness 자산 적용 완료
- docs/ 4문서 체계 구축

---

*Last updated: (날짜 입력)*
STUB

  cat > "${target}/docs/checklist.md" << 'STUB'
# Checklist — 작업 확인서

> **현재 Phase:** Phase 1 — 초기 설정

---

## Phase 1 — 체크리스트

- [ ] 환경 변수 설정 (.env 파일)
- [ ] 데이터베이스 연결 확인
- [ ] 개발 서버 기동 확인
- [ ] .claude/ 훅 동작 확인

---

*Last updated: (날짜 입력)*
STUB

  echo -e "  ${GREEN}✓${NC} docs/plan.md, task.md, history.md, checklist.md 생성 완료"
}

# ─── CLAUDE.md 템플릿 생성 ───────────────────────────────────────────
create_claude_md() {
  local target=$1
  local stack_name=$2

  echo -e "\n${CYAN}[5/5] CLAUDE.md 생성 중...${NC}"

  cat > "${target}/CLAUDE.md" << CLAUDE_STUB
# CLAUDE.md

This file provides guidance to Claude Code when working with code in this repository.

## 프로젝트 개요

- **스택:** ${stack_name}
- **초기화:** Advanced Harness v1.5.0

## 프로젝트 구조

\`\`\`
(프로젝트 구조 기술)
\`\`\`

## 개발 환경

### 설정

\`\`\`bash
# (설치 명령어)
\`\`\`

### 실행

\`\`\`bash
# (실행 명령어)
\`\`\`

## Agent 작업 문서 체계

모든 Claude Agents는 작업 시 \`docs/\` 디렉토리의 **4문서 체계**를 기준으로 작업한다.

| 문서 | 역할 |
|---|---|
| \`docs/plan.md\` | 전체 완성 계획 **작업 기획서** |
| \`docs/task.md\` | 현재 작업의 **상세 실행 계획서** |
| \`docs/history.md\` | 이전 작업 내용 + 다음 태스크 맥락 **맥락 문서** |
| \`docs/checklist.md\` | 완료 기준 **작업 확인서** |

### 작업 시작 시 (필수)

\`\`\`
1. docs/history.md 읽기  → 이전 맥락 파악
2. docs/task.md 읽기     → 현재 작업 범위 확인
3. docs/checklist.md 읽기 → 완료 기준 확인
\`\`\`

### 작업 완료 시 (필수)

\`\`\`
1. docs/checklist.md 최종 검증 항목 체크
2. docs/task.md 완료 항목 표시
3. docs/history.md 업데이트
4. docs/plan.md 해당 Phase 항목 체크
\`\`\`

## pathPattern 설정

아래 settings.json의 \`pathPattern\` 값을 프로젝트 구조에 맞게 수정하세요:

\`\`\`json
{
  "skills": {
    "pathPattern": "(src|app|backend|frontend)/"
  }
}
\`\`\`

**스택별 권장 pathPattern:**
- FastAPI: \`"(backend|src)/"\`
- Next.js: \`"(frontend|src|app)/"\`
- 풀스택: \`"(backend|frontend|src|app)/"\`
- React Native: \`"(src|app|components)/"\`
- Embedded: \`"(src|hal|drivers|tests)/"\`
CLAUDE_STUB

  echo -e "  ${GREEN}✓${NC} CLAUDE.md 생성 완료"
}

# ─── pathPattern 안내 ────────────────────────────────────────────────
print_pathpattern_guide() {
  local choice=$1
  local target=$2

  local pattern=""
  case $choice in
    1|3|4) pattern="\"(backend|frontend|src|app)/\"" ;;
    2)     pattern="\"(backend|frontend|src|app)/\"" ;;
    5)     pattern="\"(src/main|frontend|src|app)/\"" ;;
    6)     pattern="\"(backend|src)/\"" ;;
    7)     pattern="\"(frontend|src|app)/\"" ;;
    8)     pattern="\"(src|app|components|screens)/\"" ;;
    9|10)  pattern="\"(src|hal|drivers|tests|firmware)/\"" ;;
    0)     pattern="\"(src|app)/\"" ;;
  esac

  echo -e "\n${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
  echo -e "${BOLD}  pathPattern 설정 안내${NC}"
  echo -e "${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
  echo ""
  echo -e "  ${YELLOW}→${NC}  ${target}/.claude/settings.json 열기"
  echo -e "  ${YELLOW}→${NC}  skills.pathPattern 값을 아래로 설정:"
  echo ""
  echo -e "       ${BOLD}${pattern}${NC}"
  echo ""
  echo "  프로젝트 구조에 따라 조정이 필요할 수 있습니다."
  echo ""
}

# ─── 완료 메시지 ─────────────────────────────────────────────────────
print_completion() {
  local target=$1
  local stack_name=$2

  echo -e "\n${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
  echo -e "${GREEN}${BOLD}  초기화 완료! ✓${NC}"
  echo -e "${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
  echo ""
  echo -e "  ${BOLD}스택:${NC}   ${stack_name}"
  echo -e "  ${BOLD}위치:${NC}   ${target}"
  echo ""
  echo -e "${BOLD}  다음 단계:${NC}"
  echo "  1. CLAUDE.md 프로젝트 정보 기입"
  echo "  2. .claude/settings.json pathPattern 설정"
  echo "  3. docs/plan.md Phase 계획 작성"
  echo "  4. Claude Code 세션 시작: claude ."
  echo "  5. /harness-apply 실행하여 설정 검증"
  echo ""
  echo -e "${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
}

# ─── 메인 ────────────────────────────────────────────────────────────
main() {
  local stack_choice=""
  local target_dir=""

  # 인수 파싱
  while [[ $# -gt 0 ]]; do
    case $1 in
      --help|-h)  usage ;;
      --stack)    stack_choice="$2"; shift 2 ;;
      --target)   target_dir="$2"; shift 2 ;;
      *)          echo -e "${RED}알 수 없는 옵션: $1${NC}"; usage ;;
    esac
  done

  # 스택 선택 (인수 없으면 대화형)
  if [[ -z "$stack_choice" ]]; then
    print_menu
    echo -n "선택 (0~10): "
    read -r stack_choice
  fi

  # 유효성 검사
  if ! [[ "$stack_choice" =~ ^[0-9]+$ ]] || [[ "$stack_choice" -gt 10 ]]; then
    echo -e "${RED}오류: 0~10 사이의 숫자를 입력하세요.${NC}"
    exit 1
  fi

  # 스택 정보 설정
  MOBILE="" FIRMWARE=""
  get_stack_info "$stack_choice"

  # 대상 디렉토리 (인수 없으면 대화형)
  if [[ -z "$target_dir" ]]; then
    echo ""
    echo -n "대상 디렉토리 경로 (엔터 = 현재 디렉토리): "
    read -r target_dir
    target_dir="${target_dir:-$(pwd)}"
  fi

  # 절대 경로 변환
  target_dir="$(realpath "$target_dir" 2>/dev/null || echo "$target_dir")"

  # 대상 디렉토리 생성
  if [[ ! -d "$target_dir" ]]; then
    echo -e "${YELLOW}디렉토리 생성: ${target_dir}${NC}"
    mkdir -p "$target_dir"
  fi

  # 기존 .claude/ 존재 확인
  if [[ -d "${target_dir}/.claude" ]]; then
    echo -e "${YELLOW}경고: ${target_dir}/.claude 이미 존재합니다.${NC}"
    echo -n "덮어쓰시겠습니까? (y/N): "
    read -r confirm
    if [[ "$confirm" != "y" && "$confirm" != "Y" ]]; then
      echo "취소됨."
      exit 0
    fi
  fi

  echo ""
  echo -e "${BOLD}스택:${NC} ${STACK_NAME}"
  echo -e "${BOLD}위치:${NC} ${target_dir}"
  echo ""

  # 실행
  copy_claude_assets "$target_dir"
  install_hook_deps "$target_dir"
  copy_base_code "$target_dir" "$stack_choice"
  create_docs_stubs "$target_dir"
  create_claude_md "$target_dir" "$STACK_NAME"

  # 안내
  print_pathpattern_guide "$stack_choice" "$target_dir"
  print_completion "$target_dir" "$STACK_NAME"
}

main "$@"
