# Plan — base/c-embedded/ 베이스 코드 구성 계획

> **문서 역할:** `base/c-embedded/` 스택의 전체 구성 방향과 아키텍처 결정 사항을 관리하는 기획서.

---

## 목표

C99 기반 임베디드 시스템 개발 시 즉시 사용 가능한 **범용 베이스 코드**를 제공한다.
- MCU 독립적인 HAL(Hardware Abstraction Layer) 인터페이스 설계
- FreeRTOS 태스크 패턴으로 RTOS 기반 펌웨어 개발 기반 제공
- 호스트(PC)에서도 빌드·테스트 가능한 구조 (Mock 드라이버 포함)
- Unity 테스트 프레임워크로 HAL 단위 테스트 템플릿 제공

---

## 기술 스택

| 항목 | 선택 |
|---|---|
| 언어 표준 | C99 |
| 빌드 시스템 | CMake 3.16+ |
| RTOS | FreeRTOS (태스크 패턴 기준) |
| 테스트 프레임워크 | Unity (오픈소스, MIT 라이선스) |
| 대상 MCU | STM32 계열 (reference) + Host(mock) |
| 컴파일러 | arm-none-eabi-gcc (MCU) / gcc (Host) |

---

## 아키텍처 결정

### 1. HAL 설계 패턴: Function Pointer Table (Ops Table)

```c
typedef struct {
    int (*init)(uint8_t pin, hal_gpio_dir_t dir);
    int (*write)(uint8_t pin, hal_gpio_level_t level);
    int (*read)(uint8_t pin, hal_gpio_level_t *out);
    int (*toggle)(uint8_t pin);
    void (*deinit)(uint8_t pin);
} hal_gpio_ops_t;
```

- 런타임에 플랫폼별 구현체를 등록 (`hal_gpio_register()`)
- 동일한 앱 코드가 STM32 / ESP32 / Host 전부에서 동작
- 테스트 시 Mock 구현체로 교체 → 하드웨어 없이 검증 가능

### 2. 이중 빌드 타깃

```
PLATFORM_HOST   → Mock 드라이버 + Unity 테스트 실행
PLATFORM_STM32  → STM32 HAL 드라이버 + 실 하드웨어 구동
```

### 3. FreeRTOS 태스크 패턴

- 태스크 함수 시그니처: `void task_xxx(void *pvParams)`
- 태스크 간 통신: Queue, Semaphore, EventGroup 패턴 예시 포함
- Host 빌드 시: FreeRTOS 타입 Mock 헤더로 컴파일 가능하게 유지

### 4. 메모리 정책

- **동적 메모리 할당 금지** (no malloc/free in application code)
- 정적 배열 + Ring Buffer 패턴 사용
- FreeRTOS 태스크 스택도 정적 할당 (`StaticTask_t`)

---

## 디렉토리 구조

```
base/c-embedded/
├── docs/                    ← 4문서 체계
├── README.md
├── CMakeLists.txt           ← 최상위 빌드 정의
├── Makefile                 ← 편의 래퍼 (host/test 빌드)
├── include/
│   └── app_config.h         ← 프로젝트 전역 설정 (스택 크기, 버퍼 크기 등)
├── src/
│   ├── main.c               ← 진입점 + FreeRTOS 태스크 등록
│   ├── hal/                 ← HAL 인터페이스 + 디스패처
│   │   ├── hal_types.h      ← 공통 타입 정의
│   │   ├── hal_gpio.h / .c  ← GPIO 인터페이스
│   │   ├── hal_uart.h / .c  ← UART 인터페이스
│   │   └── hal_timer.h / .c ← Timer 인터페이스
│   ├── drivers/
│   │   ├── mock/            ← Host 테스트용 Mock 구현체
│   │   └── stm32/           ← STM32 레퍼런스 구현체 (stub)
│   ├── tasks/               ← FreeRTOS 태스크 패턴
│   │   ├── freertos_types.h ← FreeRTOS 타입 정의 (real/mock 공용)
│   │   ├── task_led.*       ← LED 블링크 태스크 (기본 패턴)
│   │   └── task_uart_monitor.* ← UART 모니터링 태스크 (큐 패턴)
│   └── utils/               ← 유틸리티
│       ├── ring_buffer.*    ← 정적 Ring Buffer
│       └── logger.*         ← UART 디버그 로거
└── tests/
    ├── CMakeLists.txt
    ├── unity/               ← Unity 테스트 프레임워크
    ├── test_hal_gpio.c      ← GPIO HAL 단위 테스트
    ├── test_ring_buffer.c   ← Ring Buffer 단위 테스트
    └── run_tests.c          ← 테스트 러너
```

---

## 완성 기준 (Definition of Done)

| 항목 | 기준 |
|---|---|
| `docs/` 4문서 | plan / task / history / checklist 모두 작성 |
| HAL 추상화 레이어 | GPIO / UART / Timer 인터페이스 + Mock 구현체 |
| CMakeLists.txt | Host 빌드(`-DBUILD_FOR_HOST=ON`) 성공 |
| 테스트 | Unity 기반 테스트 2개 이상, `ctest` 통과 |
| README.md | 구조 설명, 빌드 방법, 커스터마이징 포인트 포함 |

---

*Last updated: 2026-03-01 (초안 작성)*
