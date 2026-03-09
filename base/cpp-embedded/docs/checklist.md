# Checklist — base/cpp-embedded 완료 기준

> **문서 역할:** Definition of Done 항목을 점검한다. 작업 완료 선언 전 반드시 모든 항목을 확인한다.

---

## 필수 파일 존재 여부

- [x] `CMakeLists.txt` (루트)
- [x] `README.md`
- [x] `include/hal/IGpio.hpp`
- [x] `include/hal/IUart.hpp`
- [x] `include/hal/ISpi.hpp`
- [x] `include/hal/II2c.hpp`
- [x] `platform/stm32/Gpio.hpp`
- [x] `platform/stm32/Gpio.cpp`
- [x] `platform/stm32/Uart.hpp`
- [x] `platform/stm32/Uart.cpp`
- [x] `mock/MockGpio.hpp`
- [x] `mock/MockUart.hpp`
- [x] `drivers/include/Led.hpp`
- [x] `drivers/src/Led.cpp`
- [x] `drivers/include/Console.hpp`
- [x] `drivers/src/Console.cpp`
- [x] `tests/CMakeLists.txt`
- [x] `tests/test_gpio.cpp`
- [x] `tests/test_uart.cpp`
- [x] `tests/test_led_driver.cpp`
- [x] `examples/blinky/main.cpp`
- [x] `examples/blinky/CMakeLists.txt`
- [x] `docs/plan.md`
- [x] `docs/task.md`
- [x] `docs/history.md`
- [x] `docs/checklist.md`

---

## HAL 인터페이스 품질

- [x] `IGpio` — Direction, Level, PullMode enum class 정의 완비
- [x] `IGpio` — 순수 가상 소멸자 포함
- [x] `IUart` — Config struct (baudRate, dataBits, stopBits, parity) 포함
- [x] `ISpi` — Config struct (clockHz, mode, bitOrder) 포함
- [x] `II2c` — 8비트 주소 기반 write/read 인터페이스 포함
- [x] 모든 인터페이스 파일에 include guard (`#pragma once`) 적용

---

## 구현 품질 (C++17)

- [x] `enum class` 사용 (타입 안전)
- [x] RAII — Led 드라이버 생성자/소멸자에서 리소스 관리
- [x] `[[nodiscard]]` — 반환값 무시 방지 어노테이션 적용
- [x] `noexcept` — 임베디드 환경 예외 비활성화 패턴 적용
- [x] `override` 키워드 — 모든 가상 함수 오버라이드에 명시

---

## Mock 구현 품질

- [x] `MockGpio` — `writeCount`, `lastLevel` 상태 추적
- [x] `MockGpio` — `setReadResponse()` 로 read 시뮬레이션 가능
- [x] `MockUart` — 내부 `txBuffer` 벡터에 전송 데이터 기록
- [x] `MockUart` — `setRxData()` 로 수신 데이터 주입 가능

---

## 빌드 검증

- [x] g++ -std=c++17 구문 검증 오류 없음 (플랫폼 소스 + 드라이버 + 테스트)
- [x] Google Test 링크 빌드 성공 (gtest-1.14.0)
- [x] `./run_tests` 실행 결과: **43/43 PASSED**
- [x] 컴파일 경고 없음 (-Wall -Wextra 기준)

> CMake 설치 환경 기준 정식 빌드: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build`

---

## 테스트 검증

- [x] `test_gpio.cpp` — write/read/toggle/setPull/reset 등 15개 테스트 케이스
- [x] `test_uart.cpp` — 초기화/송신/수신/플러시/리셋 등 16개 테스트 케이스
- [x] `test_led_driver.cpp` — on/off/toggle/destructor/isOn 등 12개 테스트 케이스

---

## 문서 품질

- [x] `README.md` — 디렉토리 구조 설명 포함
- [x] `README.md` — 빌드 명령어 포함
- [x] `README.md` — 커스터마이징 포인트(실제 플랫폼 적용 방법) 포함
- [x] `README.md` — 테스트 실행 방법 포함

---

## 최종 판정

**✅ Definition of Done 충족 — 완성**

- HAL 인터페이스 4종 (GPIO, UART, SPI, I2C)
- STM32 플랫폼 스텁 2종
- Mock 구현 2종
- 재사용 드라이버 2종 (Led, Console)
- Google Test 43개 전체 PASSED
- CMakeLists.txt (FetchContent 자동 GoogleTest)
- README.md + docs/ 4문서 완비

---

## Phase 8 체크리스트 — embedded-cpp-guidelines 스킬 작성 (Phase 2-B)

### 병렬 세션 사전 확인

- [x] 담당 파일 범위: `.claude/skills/embedded-cpp-guidelines/SKILL.md` 만 작성
- [x] `skill-rules.json` 수정 금지 확인
- [x] `.claude/skills/README.md` 수정 금지 확인
- [x] 루트 `docs/` 4문서 수정 금지 확인

### SKILL.md 필수 항목

- [x] `SKILL.md` 파일 생성: `.claude/skills/embedded-cpp-guidelines/SKILL.md`
- [x] YAML frontmatter 포함 (`name`, `description`, `triggers`)
- [x] 트리거 키워드 5개 이상 명시
- [x] 핵심 패턴 4개 이상 + 코드 예시
  - [x] 패턴 1: 순수 가상 HAL 인터페이스 (IGpio 기반)
  - [x] 패턴 2: RAII 리소스 관리 (Led 드라이버)
  - [x] 패턴 3: 경량 직접 작성 Mock 전략 (MockGpio)
  - [x] 패턴 4: Google Test + CMake FetchContent 구조
- [x] 안티패턴 섹션 포함
- [x] 500줄 이하 확인

### 완성 기준 최종 판정

- [x] SKILL.md 500줄 이하 ✅
- [x] 병렬 세션 격리 규칙 준수 (`skill-rules.json` 미수정) ✅
- [x] base/cpp-embedded/ 실제 코드 기반 패턴 작성 ✅
- [x] Phase 2-D에서 처리할 이슈 없음 ✅

---

*Last updated: 2026-03-05 (세션 2 — 스킬 작성 완료)*
