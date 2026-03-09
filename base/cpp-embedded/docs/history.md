# History — base/cpp-embedded 작업 맥락

> **문서 역할:** 세션별 작업 내용과 주요 결정 사항을 기록한다. 다음 세션 시작 시 이 문서부터 읽는다.

---

## 세션 1 — 2026-03-01 (초안 및 구현 시작)

### 작업 배경

`docs/plan.md` Phase 1-B에 따라 7개 신규 베이스 코드 스택 중 `base/cpp-embedded/`를 생성한다.
이 세션에서 전체 구현을 완료하는 것을 목표로 한다.

### 주요 설계 결정

#### 1. HAL 인터페이스 설계 철학

- 순수 가상 인터페이스(Abstract Class)를 기반으로 한다.
- 소멸자는 반드시 `virtual`로 선언하여 다형 삭제 안전성을 확보한다.
- 모든 enum은 `enum class`로 타입 안전성을 보장한다.
- `std::span`(C++20)이 아닌 `const uint8_t*` + `size_t` 조합으로 C++17 호환성을 유지한다.
  - 단, C++20 환경에서는 `std::span` 교체 용이하도록 `using` 별칭 주석 추가.

#### 2. 테스트 전략

- Google Test + Google Mock 대신 **직접 작성한 경량 Mock**을 사용한다.
  - 이유: Google Mock은 매크로 중심이라 가독성이 낮고, 임베디드 문서 목적상 직접 작성이 교육적 가치가 높다.
  - Mock은 내부 상태(호출 횟수, 마지막 값)를 추적하는 단순한 구조체로 구현.
- Google Test는 FetchContent로 자동 다운로드 — 별도 설치 불필요.

#### 3. 플랫폼 구현 예시 범위

- STM32 HAL 구현은 실제 레지스터 조작 없이 **스텁(stub) 구조**만 제공.
- 이유: 특정 MCU SDK 없이도 빌드 및 이식 가능한 범용 템플릿이 목적.
- 실제 MCU 적용 시 스텁 함수 내부만 교체하면 됨을 주석으로 명시.

#### 4. RAII 적용 범위

- `Led` 드라이버: 생성 시 GPIO 초기화, 소멸 시 GPIO를 Low로 안전하게 내린다.
- `Console` 드라이버: UART init을 생성자에서 수행.

#### 5. CMake 구조

- `CMakeLists.txt`는 `INTERFACE` 라이브러리 타겟으로 헤더 전용 HAL 인터페이스를 노출.
- `hal_drivers` 정적 라이브러리가 인터페이스에 링크.
- 테스트 빌드는 `BUILD_TESTING` 옵션으로 분리 (기본 ON).

---

### 구현 결과

- 전체 파일 26개 생성 완료
- g++ -std=c++17 구문 검증: 모든 소스 오류 없음
- Google Test 실행 결과: **43/43 PASSED** (0 failed)
  - `LedDriverTest`: 12개
  - `MockUartTest`: 16개
  - `MockGpioTest`: 15개

### 발생한 이슈 및 해결

1. **`IGpio::Level` 네임스페이스 누락** — `platform/stm32/Gpio.cpp`의 반환 타입을 `hal::IGpio::Level`로 수정
2. **`II2c.hpp` 파일 오염** — Write 도구 오류로 태그가 삽입되어 파일 재작성으로 해결

### 다음 세션 인계 사항

- `base/cpp-embedded/` 베이스 코드 완성 (Definition of Done 충족)
- 루트 `docs/`는 수정하지 않는다 (Phase 1-C 통합 검증 시 일괄 업데이트).
- CMake 설치 환경에서 `cmake -S . -B build && cmake --build build` 검증 권장.

---

## 세션 2 — 2026-03-05 (Phase 2-B: embedded-cpp-guidelines 스킬 작성)

### 작업 배경

루트 `docs/plan.md` Phase 2-B에 따라 `embedded-cpp-guidelines` 스킬을 작성한다.
이 세션에서 `base/cpp-embedded/` 베이스 코드를 참조하여 Claude Code 스킬 파일 1개를 생성한다.

**병렬 세션 격리 원칙 적용:**
- 생성 파일: `.claude/skills/embedded-cpp-guidelines/SKILL.md` (단일 파일)
- 수정 금지: `skill-rules.json`, `skills/README.md`, 루트 `docs/` 4문서

### 스킬 설계 결정

#### 1. 핵심 패턴 선정 기준

`base/cpp-embedded/`의 실제 구현 코드에서 검증된 패턴 4개를 추출:
- **HAL 인터페이스**: `IGpio.hpp` 기반 순수 가상 클래스 — 플랫폼 이식성의 핵심
- **RAII**: `Led.hpp/Led.cpp` 생성자/소멸자 리소스 관리 — C++ 임베디드 핵심 원칙
- **Mock 전략**: `MockGpio.hpp`, `MockUart.hpp` — Google Mock 대신 경량 직접 작성
- **빌드 구조**: FetchContent GoogleTest, INTERFACE/STATIC 라이브러리 분리

#### 2. Google Mock 미사용 이유 (의도적)

- Google Mock 매크로(`MOCK_METHOD`, `EXPECT_CALL`)는 가독성이 낮음
- 임베디드 학습 목적상 Mock 동작을 직접 이해하는 것이 교육적 가치가 높음
- 경량 Mock은 호출 횟수/마지막 값만 추적하면 대부분의 단위 테스트에 충분

#### 3. SKILL.md 구성 전략

500줄 이하 제약을 준수하기 위해:
- 각 패턴: 설명 2~3줄 + 핵심 코드 예시
- 불필요한 반복 최소화, 실전 사용 패턴 중심
- 안티패턴 섹션으로 흔한 실수 명시

### 작업 결과

- `base/cpp-embedded/docs/task.md` — Phase 8 섹션 추가 ✅
- `base/cpp-embedded/docs/history.md` — 세션 2 맥락 추가 ✅
- `base/cpp-embedded/docs/checklist.md` — 스킬 체크리스트 추가 ✅
- `.claude/skills/embedded-cpp-guidelines/SKILL.md` 작성 완료 ✅

### 다음 세션 인계 사항

- `embedded-cpp-guidelines` 스킬 SKILL.md 생성 완료
- Phase 2-D 일괄 세션에서 `skill-rules.json`에 트리거 규칙 추가 필요
- 루트 `docs/` 4문서 업데이트도 Phase 2-D 일괄 세션에서 처리

---

*Last updated: 2026-03-05 (세션 2 완료 — embedded-cpp-guidelines 스킬 작성)*
