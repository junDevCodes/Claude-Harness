#include "Gpio.hpp"

// ── 실제 STM32 환경에서는 아래 헤더를 활성화한다 ──────────────────────────
// #include "stm32f4xx_hal.h"   // STM32F4 예시
// #include "stm32h7xx_hal.h"   // STM32H7 예시
// ────────────────────────────────────────────────────────────────────────────

namespace platform::stm32 {

Gpio::Gpio(uint8_t port, uint8_t pin) noexcept
    : m_port{port}, m_pin{pin} {
    // 실제 구현 예시:
    // __HAL_RCC_GPIOA_CLK_ENABLE();  // 포트에 따라 선택
    // GPIO_InitTypeDef init{};
    // init.Pin   = (1u << pin);
    // init.Mode  = GPIO_MODE_OUTPUT_PP;
    // init.Pull  = GPIO_NOPULL;
    // init.Speed = GPIO_SPEED_FREQ_LOW;
    // HAL_GPIO_Init(portToHandle(port), &init);
}

void Gpio::setDirection(Direction dir) noexcept {
    m_direction = dir;
    // 실제 구현 예시:
    // GPIO_InitTypeDef init{};
    // init.Pin  = (1u << m_pin);
    // init.Mode = (dir == Direction::Output) ? GPIO_MODE_OUTPUT_PP
    //                                        : GPIO_MODE_INPUT;
    // HAL_GPIO_Init(portToHandle(m_port), &init);
}

void Gpio::write(Level level) noexcept {
    m_level = level;
    // 실제 구현 예시:
    // HAL_GPIO_WritePin(portToHandle(m_port),
    //                   static_cast<uint16_t>(1u << m_pin),
    //                   (level == Level::High) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

hal::IGpio::Level Gpio::read() const noexcept {
    // 실제 구현 예시:
    // GPIO_PinState state = HAL_GPIO_ReadPin(portToHandle(m_port),
    //                                        static_cast<uint16_t>(1u << m_pin));
    // return (state == GPIO_PIN_SET) ? Level::High : Level::Low;
    return m_level;  // 스텁: 마지막 쓴 값을 반환
}

void Gpio::toggle() noexcept {
    m_level = (m_level == Level::High) ? Level::Low : Level::High;
    // 실제 구현 예시:
    // HAL_GPIO_TogglePin(portToHandle(m_port),
    //                    static_cast<uint16_t>(1u << m_pin));
}

void Gpio::setPull(PullMode mode) noexcept {
    // 실제 구현 예시:
    // GPIO_InitTypeDef init{};
    // init.Pin  = (1u << m_pin);
    // init.Pull = (mode == PullMode::PullUp)   ? GPIO_PULLUP   :
    //             (mode == PullMode::PullDown)  ? GPIO_PULLDOWN : GPIO_NOPULL;
    // HAL_GPIO_Init(portToHandle(m_port), &init);
    (void)mode;  // 스텁
}

} // namespace platform::stm32
