#pragma once

#include "../../include/hal/IUart.hpp"
#include <cstdint>
#include <vector>

namespace platform::stm32 {

/**
 * @brief STM32 UART 구현 (스텁)
 *
 * 실제 STM32 HAL(HAL_UART_Transmit 등)을 호출하는 구현 예시.
 *
 * 커스터마이징 포인트:
 *  1. 생성자에서 UART 인스턴스 번호(1=USART1, 2=USART2...)를 받는다.
 *  2. `init()` → `UART_HandleTypeDef` 설정 후 `HAL_UART_Init()` 호출
 *  3. `write()` → `HAL_UART_Transmit()` 호출
 *  4. `read()`  → `HAL_UART_Receive()` 호출
 *
 * @note 스텁 구현은 내부 벡터 버퍼로 동작 (실제 하드웨어 없이 빌드 가능).
 */
class Uart final : public hal::IUart {
public:
    /**
     * @param instance UART 인스턴스 번호 (1=USART1, 2=USART2, ...)
     */
    explicit Uart(uint8_t instance) noexcept;
    ~Uart() override = default;

    void   init(const Config& config)                         noexcept override;
    void   write(const uint8_t* data, size_t size)            noexcept override;
    size_t read(uint8_t* buffer, size_t size)                 noexcept override;
    bool   isDataAvailable()                        const     noexcept override;
    void   flush()                                            noexcept override;

private:
    uint8_t              m_instance;
    Config               m_config;
    std::vector<uint8_t> m_rxBuffer;  ///< 스텁 전용: 수신 시뮬레이션용
};

} // namespace platform::stm32
