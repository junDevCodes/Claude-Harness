# Task — base/cpp-embedded 세부 작업 항목

> **문서 역할:** plan.md에 정의된 구성 목표를 단계별 실행 항목으로 분해한다.

---

## Phase 1 — 디렉토리 구조 및 빌드 시스템

| # | 항목 | 상태 |
|---|---|---|
| 1-1 | 디렉토리 구조 생성 | ✅ |
| 1-2 | 루트 `CMakeLists.txt` 작성 (C++17, GoogleTest FetchContent) | ✅ |
| 1-3 | `tests/CMakeLists.txt` 작성 | ✅ |
| 1-4 | `examples/blinky/CMakeLists.txt` 작성 | ✅ |

---

## Phase 2 — HAL 인터페이스 (헤더 전용)

| # | 항목 | 상태 |
|---|---|---|
| 2-1 | `include/hal/IGpio.hpp` — Direction, Level, PullMode enum + 순수 가상 클래스 | ✅ |
| 2-2 | `include/hal/IUart.hpp` — Config struct + 순수 가상 클래스 | ✅ |
| 2-3 | `include/hal/ISpi.hpp` — Config struct + 순수 가상 클래스 | ✅ |
| 2-4 | `include/hal/II2c.hpp` — 순수 가상 클래스 | ✅ |

---

## Phase 3 — 플랫폼 구현 예시 (STM32 스텁)

| # | 항목 | 상태 |
|---|---|---|
| 3-1 | `platform/stm32/Gpio.hpp` / `Gpio.cpp` — STM32 GPIO 스텁 구현 | ✅ |
| 3-2 | `platform/stm32/Uart.hpp` / `Uart.cpp` — STM32 UART 스텁 구현 | ✅ |

---

## Phase 4 — Mock 구현 (Google Test 연동)

| # | 항목 | 상태 |
|---|---|---|
| 4-1 | `mock/MockGpio.hpp` — 상태 추적 Mock (write/read 기록) | ✅ |
| 4-2 | `mock/MockUart.hpp` — 송수신 버퍼 Mock | ✅ |

---

## Phase 5 — 재사용 드라이버

| # | 항목 | 상태 |
|---|---|---|
| 5-1 | `drivers/include/Led.hpp` / `drivers/src/Led.cpp` — LED 드라이버 (RAII) | ✅ |
| 5-2 | `drivers/include/Console.hpp` / `drivers/src/Console.cpp` — 콘솔 드라이버 | ✅ |

---

## Phase 6 — 단위 테스트 (Google Test)

| # | 항목 | 상태 |
|---|---|---|
| 6-1 | `tests/test_gpio.cpp` — MockGpio 동작 검증 | ✅ |
| 6-2 | `tests/test_uart.cpp` — MockUart 송수신 검증 | ✅ |
| 6-3 | `tests/test_led_driver.cpp` — Led 드라이버 논리 검증 | ✅ |

---

## Phase 7 — 예시 및 문서

| # | 항목 | 상태 |
|---|---|---|
| 7-1 | `examples/blinky/main.cpp` — Blinky 예시 (MockGpio 사용) | ✅ |
| 7-2 | `README.md` — 구조 설명, 빌드 방법, 커스터마이징 포인트 | ✅ |

---

## 완성 기준 최종 체크

- [x] g++ -std=c++17 구문 검증 오류 없음 (CMakeLists.txt FetchContent 구조 정상)
- [x] Google Test 43개 테스트 전체 PASSED (LedDriver 12 + MockUart 16 + MockGpio 15)
- [x] `README.md` 빌드 방법 포함
- [x] `docs/` 4문서 완비

---

## Phase 8 — embedded-cpp-guidelines 스킬 작성 (Phase 2-B)

> **병렬 세션 격리 규칙 준수**: `.claude/skills/embedded-cpp-guidelines/` 디렉토리만 생성·수정.
> `skill-rules.json`, `README.md`, 루트 `docs/` 수정 금지.

| # | 항목 | 상태 |
|---|---|---|
| 8-1 | `base/cpp-embedded/docs/task.md` — 세션 계획 문서 업데이트 | ✅ |
| 8-2 | `base/cpp-embedded/docs/history.md` — 세션 맥락 문서 업데이트 | ✅ |
| 8-3 | `base/cpp-embedded/docs/checklist.md` — 스킬 완성 기준 체크리스트 추가 | ✅ |
| 8-4 | `.claude/skills/embedded-cpp-guidelines/SKILL.md` 작성 (500줄 이하) | ✅ |
| 8-5 | YAML frontmatter 포함 (name, description, triggers 5개 이상) | ✅ |
| 8-6 | 핵심 패턴 4개 + 코드 예시 작성 | ✅ |
| 8-7 | 안티패턴 섹션 작성 | ✅ |
| 8-8 | 500줄 이하 최종 확인 | ⬜ |

### 핵심 패턴 작성 목록

| 패턴 | 근거 코드 |
|---|---|
| 순수 가상 HAL 인터페이스 | `include/hal/IGpio.hpp`, `IUart.hpp`, `ISpi.hpp`, `II2c.hpp` |
| RAII 리소스 관리 | `drivers/include/Led.hpp`, `drivers/src/Led.cpp` |
| 경량 직접 작성 Mock 전략 | `mock/MockGpio.hpp`, `mock/MockUart.hpp` |
| Google Test + CMake FetchContent | `CMakeLists.txt`, `tests/CMakeLists.txt` |

---

*Last updated: 2026-03-05 (세션 1 완료 / 세션 2 — Phase 2-B 스킬 작성)*
