#pragma once

#include "../../include/hal/IGpio.hpp"

namespace drivers {

/**
 * @brief LED 드라이버 — GPIO 추상화 위에서 동작하는 재사용 드라이버
 *
 * RAII 원칙을 적용한다:
 *  - 생성자: GPIO를 Output으로 초기화하고 LED를 Off 상태로 만든다.
 *  - 소멸자: LED를 안전하게 Off 상태로 만든다.
 *
 * Active-high/Active-low 모두 지원한다.
 *
 * 사용 예:
 * @code
 *   mock::MockGpio gpio;
 *   drivers::Led led{gpio, drivers::Led::ActiveLevel::High};
 *   led.on();
 *   led.toggle();
 *   led.off();
 * @endcode
 */
class Led {
public:
    /** @brief LED 활성화 레벨 (Active-high / Active-low 선택) */
    enum class ActiveLevel : uint8_t {
        High,  ///< LED가 High일 때 점등 (Active-high)
        Low,   ///< LED가 Low일 때 점등  (Active-low, pull-up 회로)
    };

    /**
     * @brief 생성자 — GPIO를 Output으로 초기화하고 LED를 소등한다.
     * @param gpio        IGpio 구현체 참조 (의존성 주입)
     * @param activeLevel 활성화 레벨 (기본: Active-high)
     */
    explicit Led(hal::IGpio& gpio,
                 ActiveLevel activeLevel = ActiveLevel::High) noexcept;

    /** @brief 소멸자 — 안전하게 LED 소등 */
    ~Led() noexcept;

    // 복사/이동 금지 (GPIO 참조를 보유하므로)
    Led(const Led&) = delete;
    Led& operator=(const Led&) = delete;
    Led(Led&&) = delete;
    Led& operator=(Led&&) = delete;

    /** @brief LED 점등 */
    void on() noexcept;

    /** @brief LED 소등 */
    void off() noexcept;

    /** @brief LED 토글 */
    void toggle() noexcept;

    /** @brief 현재 점등 상태 반환 */
    [[nodiscard]] bool isOn() const noexcept;

private:
    hal::IGpio& m_gpio;
    ActiveLevel m_activeLevel;
    bool        m_isOn{false};

    hal::IGpio::Level activeGpioLevel() const noexcept;
    hal::IGpio::Level inactiveGpioLevel() const noexcept;
};

} // namespace drivers
