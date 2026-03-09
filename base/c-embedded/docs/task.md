# Task — base/c-embedded/ 세부 실행 계획

> **문서 역할:** `plan.md`에서 정의한 구성 계획을 파일 단위로 분해한 실행 계획.

---

## Phase A — 문서 및 빌드 시스템

| # | 파일 | 상태 |
|---|---|---|
| A-1 | `docs/plan.md` | ✅ |
| A-2 | `docs/task.md` | ✅ |
| A-3 | `docs/history.md` | ✅ |
| A-4 | `docs/checklist.md` | ✅ |
| A-5 | `README.md` | ✅ |
| A-6 | `CMakeLists.txt` (최상위) | ✅ |
| A-7 | `Makefile` (편의 래퍼) | ✅ |

---

## Phase B — 공통 설정 및 HAL 인터페이스

| # | 파일 | 내용 | 상태 |
|---|---|---|---|
| B-1 | `include/app_config.h` | 버퍼 크기, 스택 크기 등 전역 설정 | ✅ |
| B-2 | `src/hal/hal_types.h` | 공통 타입 (`hal_err_t`, GPIO/UART 열거형) | ✅ |
| B-3 | `src/hal/hal_gpio.h` | GPIO 인터페이스 헤더 | ✅ |
| B-4 | `src/hal/hal_gpio.c` | GPIO HAL 디스패처 구현 | ✅ |
| B-5 | `src/hal/hal_uart.h` | UART 인터페이스 헤더 | ✅ |
| B-6 | `src/hal/hal_uart.c` | UART HAL 디스패처 구현 | ✅ |
| B-7 | `src/hal/hal_timer.h` | Timer 인터페이스 헤더 | ✅ |
| B-8 | `src/hal/hal_timer.c` | Timer HAL 디스패처 구현 | ✅ |

---

## Phase C — 플랫폼 드라이버 구현체

| # | 파일 | 내용 | 상태 |
|---|---|---|---|
| C-1 | `src/drivers/mock/gpio_mock.c` | Host 테스트용 GPIO Mock | ✅ |
| C-2 | `src/drivers/mock/uart_mock.c` | Host 테스트용 UART Mock | ✅ |
| C-3 | `src/drivers/stm32/gpio_stm32.c` | STM32 GPIO 레퍼런스 stub | ✅ |
| C-4 | `src/drivers/stm32/uart_stm32.c` | STM32 UART 레퍼런스 stub | ✅ |

---

## Phase D — FreeRTOS 태스크 패턴

| # | 파일 | 내용 | 상태 |
|---|---|---|---|
| D-1 | `src/tasks/freertos_types.h` | FreeRTOS 타입 정의 (real/mock 공용) | ✅ |
| D-2 | `src/tasks/task_led.h` | LED 블링크 태스크 헤더 | ✅ |
| D-3 | `src/tasks/task_led.c` | LED 블링크 태스크 구현 (기본 패턴) | ✅ |
| D-4 | `src/tasks/task_uart_monitor.h` | UART 모니터링 태스크 헤더 | ✅ |
| D-5 | `src/tasks/task_uart_monitor.c` | UART 모니터링 태스크 구현 (Queue 패턴) | ✅ |

---

## Phase E — 유틸리티

| # | 파일 | 내용 | 상태 |
|---|---|---|---|
| E-1 | `src/utils/ring_buffer.h` | 정적 Ring Buffer 헤더 | ✅ |
| E-2 | `src/utils/ring_buffer.c` | Ring Buffer 구현 (no malloc) | ✅ |
| E-3 | `src/utils/logger.h` | UART 디버그 로거 헤더 | ✅ |
| E-4 | `src/utils/logger.c` | 로거 구현 | ✅ |

---

## Phase F — 진입점

| # | 파일 | 내용 | 상태 |
|---|---|---|---|
| F-1 | `src/main.c` | 진입점 + HAL 등록 + FreeRTOS 태스크 생성 | ✅ |

---

## Phase G — 테스트

| # | 파일 | 내용 | 상태 |
|---|---|---|---|
| G-1 | `tests/unity/unity.h` | Unity 테스트 프레임워크 헤더 | ✅ |
| G-2 | `tests/unity/unity_internals.h` | Unity 내부 타입 | ✅ |
| G-3 | `tests/unity/unity.c` | Unity 구현체 | ✅ |
| G-4 | `tests/test_hal_gpio.c` | GPIO HAL 단위 테스트 | ✅ |
| G-5 | `tests/test_ring_buffer.c` | Ring Buffer 단위 테스트 | ✅ |
| G-6 | `tests/run_tests.c` | 테스트 러너 | ✅ |
| G-7 | `tests/CMakeLists.txt` | 테스트 빌드 정의 | ✅ |

---

## 완료 기준

- [x] `cmake -B build -DBUILD_FOR_HOST=ON && cmake --build build` 성공
- [x] 전체 25개 테스트 PASS (11 GPIO + 14 RingBuf)
- [x] HAL 인터페이스 3종 (GPIO / UART / Timer) 완비
- [x] FreeRTOS 태스크 패턴 2종 완비
- [x] Unity 테스트 2종 25개 통과

---

## Phase H — Skills 라이브러리 기여 (Phase 2-B)

> 이 베이스 코드를 참조하여 `embedded-c-guidelines` 스킬을 작성하는 세션.
> 작업 범위: `.claude/skills/embedded-c-guidelines/` 디렉토리만 생성.
> `skill-rules.json`, 루트 `docs/` 등 공유 파일은 Phase 2-D 일괄 세션에서 처리.

| # | 항목 | 내용 | 상태 |
|---|---|---|---|
| H-1 | `SKILL.md` 작성 | HAL Ops Table, FreeRTOS Task/Queue, Ring Buffer, Unity Mock 패턴 | ✅ |
| H-2 | 500줄 이하 확인 | SKILL.md 라인 수 검증 | ✅ |
| H-3 | YAML frontmatter 포함 | name, description, triggers 5개 이상 | ✅ |
| H-4 | 핵심 패턴 3개 이상 + 코드 예시 | Pattern 1~5 작성 | ✅ |
| H-5 | 안티패턴 섹션 포함 | malloc 금지, ISR API, Ring Buffer 크기 등 | ✅ |

---

*Last updated: 2026-03-01 (작업 시작)*
