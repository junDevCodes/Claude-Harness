# Checklist — base/c-embedded/ 완료 기준

> **문서 역할:** Definition of Done 체크리스트. 모든 항목 ✅ 확인 완료.

---

## 공통 필수 항목

- [x] `docs/plan.md` 존재 및 내용 채워져 있음
- [x] `docs/task.md` 존재 및 내용 채워져 있음
- [x] `docs/history.md` 존재 및 내용 채워져 있음
- [x] `docs/checklist.md` 존재 및 내용 채워져 있음
- [x] `README.md` — 구조, 빌드 방법, 커스터마이징 포인트 포함

---

## 임베디드 스택 필수 항목

### HAL 추상화 레이어

- [x] `src/hal/hal_types.h` — 공통 타입 (`hal_err_t`, GPIO/UART 열거형)
- [x] `src/hal/hal_gpio.h` / `.c` — GPIO 인터페이스 + 디스패처
- [x] `src/hal/hal_uart.h` / `.c` — UART 인터페이스 + 디스패처
- [x] `src/hal/hal_timer.h` / `.c` — Timer 인터페이스 + 디스패처
- [x] `src/drivers/mock/` — Host 테스트용 Mock 드라이버 (gpio, uart)
- [x] `src/drivers/stm32/` — 실제 MCU 드라이버 레퍼런스 stub

### FreeRTOS 태스크 패턴

- [x] `src/tasks/freertos_types.h` — FreeRTOS 타입 (real/mock 호환)
- [x] `src/tasks/task_led.*` — 기본 태스크 패턴 (vTaskDelay 루프)
- [x] `src/tasks/task_uart_monitor.*` — Queue 기반 태스크 통신 패턴

### 빌드 시스템

- [x] 최상위 `CMakeLists.txt` 존재
- [x] `-DBUILD_FOR_HOST=ON` 옵션으로 Host 빌드 가능
- [x] `Makefile` 편의 래퍼 존재
- [x] `tests/CMakeLists.txt` 존재

### 유틸리티

- [x] `src/utils/ring_buffer.*` — 정적 Ring Buffer (no malloc)
- [x] `src/utils/logger.*` — UART 디버그 로거

### 테스트

- [x] Unity 테스트 프레임워크 파일 포함 (`tests/unity/`)
- [x] `tests/test_hal_gpio.c` — GPIO Mock 기반 단위 테스트 (11개)
- [x] `tests/test_ring_buffer.c` — Ring Buffer 단위 테스트 (14개)
- [x] 빌드 성공: `cmake -B build -DBUILD_FOR_HOST=ON && cmake --build build`
- [x] 전체 25개 테스트 PASS (0 Failures)

---

## 코드 품질 기준

- [x] C99 표준만 사용
- [x] 동적 메모리 할당 없음 (no malloc/free in application code)
- [x] 모든 HAL 함수 반환값 `hal_err_t`로 통일
- [x] 헤더 가드 (`#ifndef ... #define ... #endif`) 전체 적용
- [x] 전역 변수 최소화 (static 지역 변수 우선)

---

## 최종 검증 결과

```
=== GPIO HAL Tests ===
11 Tests 0 Failures 0 Ignored — OK

=== Ring Buffer Tests ===
14 Tests 0 Failures 0 Ignored — OK

=== App Smoke Test ===
[INFO ] main: c-embedded base starting
[INFO ] main: tasks created
[main] Smoke test passed
```

**Definition of Done: ✅ 모든 기준 충족**

---

## Phase 2-B 체크리스트 — `embedded-c-guidelines` 스킬 작성

### 병렬 세션 격리 확인

- [x] `.claude/skills/embedded-c-guidelines/` 디렉토리만 생성
- [x] `skill-rules.json` 수정 금지 (Phase 2-D 일괄 세션 전용)
- [x] `.claude/skills/README.md` 수정 금지
- [x] 루트 `docs/` 4문서 수정 금지
- [x] `CLAUDE.md`, `base_code_plan.md` 수정 금지

### SKILL.md 완성 기준 (Definition of Done)

- [x] `SKILL.md` 존재 및 **500줄 이하**
- [x] YAML frontmatter (`name`, `description`, `triggers`) 포함
- [x] 핵심 패턴 **5개** + 코드 예시 (HAL / Task / Queue / RingBuf / Unity)
- [x] 트리거 키워드 **5개 이상** 명시
- [x] 안티패턴 섹션 포함 (5가지 anti-pattern)
- [x] `skill-rules.json` 등록은 Phase 2-D에서 일괄 처리 (이 세션 미처리)

---

*Last updated: 2026-03-05 (Phase 2-B 완료)*
