# base/c-embedded — C99 Embedded Base Code

C99 기반 임베디드 시스템 개발을 위한 범용 베이스 코드.
MCU 독립적인 HAL 패턴, FreeRTOS 태스크 패턴, Unity 테스트 프레임워크를 포함한다.

---

## 특징

- **HAL 추상화**: GPIO / UART / Timer 인터페이스 — 동일 코드로 STM32, ESP32, Host 실행
- **FreeRTOS 패턴**: LED Blink (기본 태스크), UART Monitor (Queue 패턴)
- **Host 빌드**: Mock 드라이버로 하드웨어 없이 PC에서 빌드·테스트 가능
- **Unity 테스트**: 25개 단위 테스트, `ctest` 한 줄로 실행
- **정적 메모리**: `malloc`/`free` 없음, Ring Buffer 포함

---

## 디렉토리 구조

```
base/c-embedded/
├── docs/                    ← 4문서 (plan/task/history/checklist)
├── README.md
├── CMakeLists.txt           ← 빌드 정의
├── Makefile                 ← 편의 래퍼
├── include/
│   └── app_config.h         ← 전역 설정 (스택 크기, 버퍼 크기, 핀 번호)
├── src/
│   ├── main.c               ← 진입점 + HAL 등록 + 태스크 생성
│   ├── hal/                 ← HAL 인터페이스 레이어
│   │   ├── hal_types.h      ← 공통 타입 (hal_err_t, GPIO/UART 열거형)
│   │   ├── hal_gpio.h/.c    ← GPIO: init/write/read/toggle
│   │   ├── hal_uart.h/.c    ← UART: init/transmit/receive/callback
│   │   └── hal_timer.h/.c   ← Timer: start/stop/tick/delay
│   ├── drivers/
│   │   ├── mock/            ← PC 테스트용 Mock 구현체
│   │   │   ├── gpio_mock.c/h
│   │   │   └── uart_mock.c/h
│   │   └── stm32/           ← STM32 레퍼런스 stub (TODO 주석 포함)
│   │       ├── gpio_stm32.c/h
│   │       └── uart_stm32.c/h
│   ├── tasks/               ← FreeRTOS 태스크 패턴
│   │   ├── freertos_types.h ← FreeRTOS 타입 (real/mock 호환)
│   │   ├── task_led.c/h     ← LED 블링크 (vTaskDelay 패턴)
│   │   └── task_uart_monitor.c/h ← UART Monitor (Queue 패턴)
│   └── utils/
│       ├── ring_buffer.c/h  ← 정적 Ring Buffer (ISR-safe)
│       └── logger.c/h       ← UART 디버그 로거
└── tests/
    ├── unity/               ← Unity 테스트 프레임워크 (v2.5.2 호환)
    ├── test_hal_gpio.c      ← GPIO HAL 테스트 (11개)
    ├── test_ring_buffer.c   ← Ring Buffer 테스트 (14개)
    └── run_tests.c          ← 테스트 러너
```

---

## 빌드 방법

### 요구사항

- CMake 3.16+
- GCC (Linux/macOS) 또는 MinGW-w64 / MSYS2 (Windows)

### Host 빌드 + 테스트 (권장 시작점)

```bash
# 방법 1: Makefile 래퍼 사용
make test

# 방법 2: CMake 직접 사용
cmake -B build -DBUILD_FOR_HOST=ON -DBUILD_TESTS=ON
cmake --build build
ctest --test-dir build -V
```

### Release 빌드

```bash
make release
# 또는
cmake -B build_release -DCMAKE_BUILD_TYPE=Release -DBUILD_FOR_HOST=ON
cmake --build build_release
```

### STM32 크로스 컴파일

```bash
cmake -B build_stm32 \
      -DBUILD_FOR_HOST=OFF \
      -DBUILD_TESTS=OFF \
      -DCMAKE_TOOLCHAIN_FILE=toolchain-arm-none-eabi.cmake
cmake --build build_stm32
```

`toolchain-arm-none-eabi.cmake` 예시:
```cmake
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_C_FLAGS "-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb")
```

---

## 커스터마이징 포인트

### 1. 핀 / 버퍼 / 스택 크기 변경

`include/app_config.h` 에서 수정:
```c
#define GPIO_LED_STATUS_PIN   13U    // 보드 LED 핀 번호
#define LED_BLINK_PERIOD_MS   500U   // 블링크 주기
#define UART_DEBUG_BAUD       115200U
#define TASK_LED_STACK_SIZE   256U   // FreeRTOS 스택 (words)
```

### 2. 새 MCU 드라이버 추가

```
src/drivers/esp32/
├── gpio_esp32.c    ← hal_gpio_ops_t 구현
└── uart_esp32.c    ← hal_uart_ops_t 구현
```

`src/main.c`에서 드라이버 선택:
```c
#elif defined(PLATFORM_ESP32)
  #include "gpio_esp32.h"
  #define GPIO_OPS   gpio_esp32_ops
```

`CMakeLists.txt`에서 드라이버 소스 추가:
```cmake
elseif(PLATFORM_ESP32)
  set(DRIVER_SRC src/drivers/esp32/gpio_esp32.c ...)
```

### 3. 새 FreeRTOS 태스크 추가

`src/tasks/task_sensor.h/.c` 생성 → `task_sensor_run()` 구현 →
`src/main.c`의 `tasks_create()`에 `xTaskCreateStatic()` 추가.

### 4. 실제 FreeRTOS 연결

```
# FreeRTOS 소스 추가 (프로젝트 루트에)
FreeRTOS/
  Source/
    tasks.c  queue.c  list.c  timers.c
    portable/GCC/ARM_CM4F/port.c
    portable/MemMang/heap_4.c
  FreeRTOSConfig.h  ← 프로젝트에 맞게 수정
```

`CMakeLists.txt`에서 `FREERTOS_MOCK` define 제거 → 실제 헤더 포함.

---

## 테스트 실행 결과 예시

```
========================================
 c-embedded base — Unit Test Suite
========================================

--- GPIO HAL Tests ---
test_hal_gpio.c:PASS (11 tests)

--- Ring Buffer Tests ---
test_ring_buffer.c:PASS (14 tests)

========================================
 ALL TESTS PASSED
========================================
```

---

## HAL 패턴 설명

```c
// 1. 플랫폼 드라이버 정의
const hal_gpio_ops_t gpio_my_ops = {
    .init   = my_gpio_init,
    .write  = my_gpio_write,
    .read   = my_gpio_read,
    .toggle = my_gpio_toggle,
    .deinit = my_gpio_deinit,
};

// 2. 시작 시 등록
hal_gpio_register(&gpio_my_ops);

// 3. 어디서든 동일하게 사용
hal_gpio_init(LED_PIN, HAL_GPIO_DIR_OUTPUT);
hal_gpio_write(LED_PIN, HAL_GPIO_LEVEL_HIGH);
hal_gpio_toggle(LED_PIN);
```

---

*C99 / CMake 3.16+ / Unity v2.5.2 / FreeRTOS 10.x 호환*
