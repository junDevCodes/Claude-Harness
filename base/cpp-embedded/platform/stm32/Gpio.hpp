#pragma once

#include "../../include/hal/IGpio.hpp"
#include <cstdint>

namespace platform::stm32 {

/**
 * @brief STM32 GPIO 구현 (스텁)
 *
 * 실제 STM32 HAL(HAL_GPIO_WritePin 등)을 호출하는 구현 예시.
 * 이 파일은 STM32CubeIDE 또는 Makefile 기반 프로젝트에서 사용한다.
 *
 * 커스터마이징 포인트:
 *  1. 생성자에서 GPIOx 포트와 핀 번호를 받는다.
 *  2. `setDirection()` → `GPIO_InitTypeDef` 설정 후 `HAL_GPIO_Init()` 호출
 *  3. `write()` → `HAL_GPIO_WritePin()` 호출
 *  4. `read()`  → `HAL_GPIO_ReadPin()` 호출
 *
 * @note  실제 레지스터 없이 빌드 가능한 구조만 제공 (헤더 인클루드 제거 상태).
 *        실 MCU 적용 시 stm32fxxx_hal.h 인클루드 및 함수 본문을 채운다.
 */
class Gpio final : public hal::IGpio {
public:
    /**
     * @param port  GPIO 포트 번호 (0=A, 1=B, ...)  — 실제 구현 시 GPIO_TypeDef*
     * @param pin   핀 번호 (0–15)
     */
    explicit Gpio(uint8_t port, uint8_t pin) noexcept;
    ~Gpio() override = default;

    void  setDirection(Direction dir)  noexcept override;
    void  write(Level level)           noexcept override;
    Level read()             const     noexcept override;
    void  toggle()                     noexcept override;
    void  setPull(PullMode mode)       noexcept override;

private:
    uint8_t   m_port;
    uint8_t   m_pin;
    Level     m_level{Level::Low};
    Direction m_direction{Direction::Output};
};

} // namespace platform::stm32
