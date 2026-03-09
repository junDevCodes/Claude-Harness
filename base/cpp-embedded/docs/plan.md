# Plan — base/cpp-embedded 베이스 코드 구성 계획

> **문서 역할:** C++17 임베디드 베이스 코드의 전체 구성 목표와 아키텍처를 정의한다.

---

## 목표

C++17 기반 임베디드 시스템 개발에서 **즉시 재사용 가능한 HAL 추상화 레이어와 비즈니스 로직 구조**를 제공한다.
특정 MCU(STM32, ESP32 등)에 종속되지 않고, 인터페이스 기반 설계로 플랫폼 이식성과 단위 테스트 가능성을 동시에 확보한다.

---

## 아키텍처 원칙

1. **HAL 추상화 (Interface 기반)** — 모든 하드웨어 접근은 순수 가상 인터페이스를 통한다.
2. **RAII** — 모든 리소스(핀, 버퍼, 락)는 생성자-소멸자 쌍으로 관리한다.
3. **의존성 역전(DIP)** — 비즈니스 로직은 HAL 구체 구현이 아닌 인터페이스에 의존한다.
4. **테스트 가능성** — Mock 구현을 통해 하드웨어 없이 단위 테스트가 가능해야 한다.
5. **C++17 표준 준수** — `std::optional`, `std::variant`, `std::string_view`, `if constexpr` 활용.
6. **Zero-overhead 추상화** — 런타임 오버헤드 최소화, `constexpr` 및 템플릿 활용.

---

## 디렉토리 구조

```
base/cpp-embedded/
├── docs/                        ← 4문서 체계
│   ├── plan.md
│   ├── task.md
│   ├── history.md
│   └── checklist.md
│
├── include/                     ← 공개 헤더 (설치 가능)
│   └── hal/
│       ├── IGpio.hpp            ← GPIO 인터페이스
│       ├── IUart.hpp            ← UART 인터페이스
│       ├── ISpi.hpp             ← SPI 인터페이스
│       └── II2c.hpp             ← I2C 인터페이스
│
├── src/                         ← 라이브러리 구현 소스
│   └── hal/                     ← (플랫폼 공통 유틸리티)
│
├── platform/                    ← 플랫폼별 HAL 구현 예시
│   └── stm32/
│       ├── Gpio.hpp
│       ├── Gpio.cpp
│       ├── Uart.hpp
│       └── Uart.cpp
│
├── mock/                        ← 단위 테스트용 Mock 구현
│   ├── MockGpio.hpp
│   └── MockUart.hpp
│
├── drivers/                     ← HAL 위에서 동작하는 재사용 드라이버
│   ├── include/
│   │   ├── Led.hpp              ← LED 드라이버 (GPIO 래퍼)
│   │   └── Console.hpp         ← 콘솔 드라이버 (UART 래퍼)
│   └── src/
│       ├── Led.cpp
│       └── Console.cpp
│
├── tests/                       ← Google Test 단위 테스트
│   ├── CMakeLists.txt
│   ├── test_gpio.cpp
│   ├── test_uart.cpp
│   └── test_led_driver.cpp
│
├── examples/                    ← 사용 예시
│   └── blinky/
│       ├── main.cpp             ← Blinky 예시 (LED 점멸)
│       └── CMakeLists.txt
│
├── CMakeLists.txt               ← 루트 빌드 스크립트
└── README.md
```

---

## HAL 인터페이스 설계

### GPIO
- `setDirection(Direction)` — Input/Output 설정
- `write(Level)` — High/Low 출력
- `read() → Level` — 입력 읽기
- `toggle()` — 출력 반전
- `setPull(PullMode)` — PullUp/PullDown/None

### UART
- `init(Config)` — 보드레이트, 데이터비트, 패리티 설정
- `write(std::span<const uint8_t>)` — 데이터 전송
- `read(std::span<uint8_t>) → size_t` — 데이터 수신
- `isDataAvailable() → bool` — 수신 대기 데이터 확인
- `flush()` — 송신 버퍼 비우기

### SPI
- `init(Config)` — 클럭 극성, 위상, 속도 설정
- `transfer(std::span<const uint8_t>, std::span<uint8_t>)` — 동시 송수신

### I2C
- `write(uint8_t addr, std::span<const uint8_t>)` — 슬레이브에 쓰기
- `read(uint8_t addr, std::span<uint8_t>)` — 슬레이브에서 읽기

---

## 빌드 시스템

- **CMake 3.21+** (C++17 설정, FetchContent로 GoogleTest 자동 다운로드)
- 빌드 타겟:
  - `hal_interfaces` — 헤더 전용 인터페이스 라이브러리
  - `hal_drivers` — LED, Console 드라이버 정적 라이브러리
  - `run_tests` — Google Test 실행 파일
  - `blinky_example` — 예시 실행 파일

---

## 완성 기준 (Definition of Done)

| 항목 | 기준 |
|---|---|
| HAL 인터페이스 | GPIO, UART, SPI, I2C 4종 구현 |
| 플랫폼 구현 예시 | STM32 스텁 구현 (실제 레지스터 없이 구조만) |
| Mock 구현 | MockGpio, MockUart (Google Test 연동) |
| 드라이버 | Led, Console 드라이버 (HAL 의존성 주입) |
| 테스트 | GPIO, UART, LED 드라이버 테스트 3종 |
| 빌드 | `cmake .. && make` 오류 없이 통과 |
| 문서 | README.md + docs/ 4문서 완비 |

---

*Last updated: 2026-03-01 (초안 작성)*
