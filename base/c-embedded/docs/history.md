# History — base/c-embedded/ 작업 맥락

> **문서 역할:** 작업 맥락과 주요 결정 사항을 기록. 다음 세션에서 이 문서부터 읽는다.

---

## 2026-03-01 — 초기 구성 (Phase 1-B 시작)

### 작업 배경

`docs/task.md` Phase 1-B에 따라 `base/c-embedded/` 생성 시작.
상위 `docs/plan.md`에 정의된 스택: **C99 + CMake + HAL 추상화 + FreeRTOS 태스크 패턴**.

### 주요 아키텍처 결정

1. **HAL 패턴: Function Pointer Table (Ops Table)**
   - `hal_gpio_ops_t`, `hal_uart_ops_t`, `hal_timer_ops_t` 구조체로 인터페이스 정의
   - `hal_gpio_register(ops)` 로 런타임 드라이버 교체
   - 동일 앱 코드로 STM32 / Host 양쪽 빌드 가능

2. **이중 빌드 타깃**
   - `PLATFORM_HOST`: Mock 드라이버 + Unity 테스트 (PC에서 실행)
   - `PLATFORM_STM32`: 실제 MCU 드라이버 (크로스 컴파일)
   - CMake `option(BUILD_FOR_HOST ...)` 으로 선택

3. **FreeRTOS 처리 방식**
   - 실제 FreeRTOS 소스는 MCU 별로 다르므로 베이스에 미포함
   - `src/tasks/freertos_types.h`에 FreeRTOS 공용 타입 정의
   - Host 빌드 시 `FREERTOS_MOCK` define으로 pthread 기반 stub 사용

4. **메모리 정책**
   - 애플리케이션 레이어에서 `malloc` / `free` 완전 금지
   - Ring Buffer는 정적 배열로 구현
   - FreeRTOS 태스크 스택 `StaticTask_t`로 정적 할당

### 생성된 파일 목록

- `docs/` — 4문서 (plan, task, history, checklist)
- `README.md`, `CMakeLists.txt`, `Makefile`
- `include/app_config.h`
- `src/hal/` — hal_types, hal_gpio, hal_uart, hal_timer (헤더 + 구현)
- `src/drivers/mock/` — gpio_mock, uart_mock
- `src/drivers/stm32/` — gpio_stm32, uart_stm32 (stub)
- `src/tasks/` — freertos_types, task_led, task_uart_monitor
- `src/utils/` — ring_buffer, logger
- `src/main.c`
- `tests/` — Unity 프레임워크 + test_hal_gpio + test_ring_buffer + run_tests

### 알려진 제약 및 다음 세션 주의사항

- STM32 드라이버는 stub 수준 (실제 HAL 라이브러리 연동 필요)
- FreeRTOS는 태스크 패턴만 제공; 실 환경에서는 MCU 포트 FreeRTOS 추가 필요
- Unity 프레임워크는 v2.5.2 API 기준 작성; 공식 Unity 소스 교체 권장
- Windows 환경에서 `make` 명령 사용 시 MinGW/Git Bash 필요

### 다음 세션 시작 시

- `docs/checklist.md`의 미완료 항목 확인 후 진행
- STM32 실제 구현이 필요하면 `src/drivers/stm32/` 확장
- ESP32 지원 추가 시 `src/drivers/esp32/` 디렉토리 추가

---

## 2026-03-05 — Phase 2-B: `embedded-c-guidelines` 스킬 작성 (병렬 세션)

### 작업 목표

`base/c-embedded/` 소스를 기반으로 `.claude/skills/embedded-c-guidelines/SKILL.md` 작성.
병렬 세션 격리 원칙 준수 — `.claude/skills/embedded-c-guidelines/` 디렉토리만 생성.

### 문서화한 핵심 패턴

| 패턴 | 소스 파일 |
|---|---|
| HAL Ops Table (Function Pointer Table) | `src/hal/hal_gpio.h`, `hal_gpio.c` |
| FreeRTOS Task Static Allocation | `src/tasks/task_led.c` |
| FreeRTOS Queue ISR→Task | `src/tasks/task_uart_monitor.c` |
| Static Ring Buffer (ISR-safe) | `src/utils/ring_buffer.h`, `ring_buffer.c` |
| Unity Test + Mock Driver 구조 | `tests/test_hal_gpio.c`, `src/drivers/mock/` |
| CMake 이중 빌드 (HOST/STM32) | `CMakeLists.txt` |
| FreeRTOS Mock Types | `src/tasks/freertos_types.h` |

### 완료 항목

- [x] `.claude/skills/embedded-c-guidelines/SKILL.md` 생성 (500줄 이하)
- [x] YAML frontmatter — name, description (트리거 키워드 포함), triggers
- [x] 핵심 패턴 5개 + 코드 예시
- [x] 안티패턴 섹션 (malloc, ISR API, Ring Buffer 크기, setUp 중복, FreeRTOS include)
- [x] 포팅 가이드 (신규 MCU 플랫폼 추가 절차)

### 다음 Phase

Phase 2-D 일괄 세션에서:
- `skill-rules.json`에 `embedded-c-guidelines` 트리거 규칙 추가
- `.claude/skills/README.md` 스킬 목록 갱신

---

*Last updated: 2026-03-05 (Phase 2-B embedded-c-guidelines 스킬 작성 완료)*
