# base/cpp-embedded — C++17 임베디드 베이스 코드

C++17 기반 임베디드 시스템 개발을 위한 재사용 가능한 HAL 추상화 레이어와 드라이버 템플릿.

---

## 특징

- **HAL 추상화**: GPIO, UART, SPI, I2C 순수 가상 인터페이스
- **RAII**: 리소스 관리를 생성자/소멸자로 보장
- **의존성 주입**: 비즈니스 로직이 인터페이스에 의존 → 플랫폼 교체 용이
- **테스트 가능성**: Mock 구현으로 하드웨어 없이 단위 테스트 가능
- **C++17**: `enum class`, `[[nodiscard]]`, `noexcept`, `if constexpr` 활용
- **CMake 3.21+**: GoogleTest 자동 다운로드, INTERFACE 라이브러리 구조

---

## 디렉토리 구조

```
base/cpp-embedded/
├── include/hal/          ← HAL 순수 가상 인터페이스 (헤더 전용)
│   ├── IGpio.hpp
│   ├── IUart.hpp
│   ├── ISpi.hpp
│   └── II2c.hpp
├── platform/stm32/       ← STM32 구현 예시 (스텁)
│   ├── Gpio.hpp / .cpp
│   └── Uart.hpp / .cpp
├── mock/                 ← 단위 테스트용 Mock
│   ├── MockGpio.hpp
│   └── MockUart.hpp
├── drivers/              ← HAL 위의 재사용 드라이버
│   ├── include/Led.hpp
│   ├── include/Console.hpp
│   ├── src/Led.cpp
│   └── src/Console.cpp
├── tests/                ← Google Test 단위 테스트
│   ├── test_gpio.cpp
│   ├── test_uart.cpp
│   └── test_led_driver.cpp
├── examples/blinky/      ← 사용 예시 (MockGpio 기반)
│   └── main.cpp
├── docs/                 ← 4문서 체계
└── CMakeLists.txt
```

---

## 빌드 방법

### 요구사항

- CMake 3.21+
- C++17 지원 컴파일러 (GCC 10+, Clang 12+, MSVC 2022+)
- 인터넷 연결 (GoogleTest FetchContent 최초 다운로드 시)

### 빌드 및 테스트

```bash
# 빌드 디렉토리 생성 및 구성
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug

# 빌드
cmake --build build

# 테스트 실행
./build/run_tests         # Linux/macOS
.\build\Debug\run_tests   # Windows

# 또는 CTest 사용
cd build && ctest --output-on-failure
```

### 예시 실행

```bash
./build/blinky_example         # Linux/macOS
.\build\Debug\blinky_example   # Windows
```

### 테스트만 빌드 (예시 제외)

```bash
cmake -S . -B build -DBUILD_EXAMPLES=OFF
cmake --build build
```

---

## 커스터마이징 포인트

### 1. 새 MCU 플랫폼 적용

`platform/{target}/` 디렉토리를 생성하고 HAL 인터페이스를 구현한다.

```cpp
// platform/esp32/Gpio.hpp
#include "../../include/hal/IGpio.hpp"

namespace platform::esp32 {
class Gpio final : public hal::IGpio {
public:
    explicit Gpio(int pin) noexcept;
    void setDirection(Direction dir) noexcept override;
    void write(Level level)          noexcept override;
    Level read()          const      noexcept override;
    void toggle()                    noexcept override;
    void setPull(PullMode mode)      noexcept override;
private:
    int m_pin;
};
} // namespace platform::esp32
```

구현부에서 MCU SDK 함수를 호출한다:

```cpp
// platform/esp32/Gpio.cpp
#include "Gpio.hpp"
#include "driver/gpio.h"  // ESP-IDF 헤더

void platform::esp32::Gpio::write(Level level) noexcept {
    gpio_set_level(static_cast<gpio_num_t>(m_pin),
                   level == Level::High ? 1 : 0);
}
```

### 2. 의존성 주입으로 플랫폼 교체

```cpp
// 개발/테스트 환경
mock::MockGpio gpio;
drivers::Led   led{gpio};

// STM32 실제 환경
platform::stm32::Gpio gpio{0 /*GPIOA*/, 5 /*PIN5*/};
drivers::Led           led{gpio};

// ESP32 실제 환경
platform::esp32::Gpio gpio{GPIO_NUM_2};
drivers::Led           led{gpio};
```

### 3. 새 드라이버 추가

`drivers/include/`에 헤더, `drivers/src/`에 구현을 추가하고
`CMakeLists.txt`의 `hal_drivers` 소스 목록에 `.cpp` 파일을 추가한다.

```cmake
add_library(hal_drivers STATIC
    drivers/src/Led.cpp
    drivers/src/Console.cpp
    drivers/src/Sensor.cpp  # ← 추가
    ...
)
```

### 4. FreeRTOS 태스크 패턴

HAL 인터페이스는 RTOS 환경에서 그대로 사용 가능하다:

```cpp
void ledTask(void* param) {
    platform::stm32::Gpio gpio{0, 5};
    drivers::Led led{gpio};

    while (true) {
        led.toggle();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
```

---

## 아키텍처 다이어그램

```
┌─────────────────────────────────────┐
│          Application / RTOS          │
│  (led.on(), console.println(), ...)  │
└──────────────────┬──────────────────┘
                   │ 의존 (인터페이스)
┌──────────────────▼──────────────────┐
│           drivers/ 레이어            │
│     Led, Console (HAL 의존성 주입)   │
└──────────────────┬──────────────────┘
                   │ 의존 (인터페이스)
┌──────────────────▼──────────────────┐
│         include/hal/ 인터페이스      │
│   IGpio, IUart, ISpi, II2c          │
└──────────┬───────────────┬──────────┘
           │               │
┌──────────▼──────┐ ┌──────▼──────────┐
│  platform/stm32 │ │   mock/          │
│  (실제 MCU HAL) │ │   (테스트용 Mock)│
└─────────────────┘ └─────────────────┘
```

---

## 관련 문서

- `docs/plan.md` — 전체 구성 계획
- `docs/task.md` — 세부 작업 항목
- `docs/history.md` — 작업 맥락 및 결정 사항
- `docs/checklist.md` — 완료 기준 체크리스트
