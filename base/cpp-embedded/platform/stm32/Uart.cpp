#include "Uart.hpp"
#include <algorithm>

// ── 실제 STM32 환경에서는 아래 헤더를 활성화한다 ──────────────────────────
// #include "stm32f4xx_hal.h"
// static UART_HandleTypeDef s_huarts[8];  // 인스턴스별 핸들
// ────────────────────────────────────────────────────────────────────────────

namespace platform::stm32 {

Uart::Uart(uint8_t instance) noexcept
    : m_instance{instance} {}

void Uart::init(const Config& config) noexcept {
    m_config = config;
    // 실제 구현 예시:
    // UART_HandleTypeDef& huart = s_huarts[m_instance - 1];
    // huart.Instance        = instanceToHandle(m_instance);
    // huart.Init.BaudRate   = config.baudRate;
    // huart.Init.WordLength = (config.dataBits == 9) ? UART_WORDLENGTH_9B
    //                                                : UART_WORDLENGTH_8B;
    // huart.Init.StopBits   = (config.stopBits == 2) ? UART_STOPBITS_2
    //                                                : UART_STOPBITS_1;
    // huart.Init.Parity     = (config.parity == Config::Parity::Even) ? UART_PARITY_EVEN :
    //                         (config.parity == Config::Parity::Odd)  ? UART_PARITY_ODD  :
    //                                                                    UART_PARITY_NONE;
    // huart.Init.Mode       = UART_MODE_TX_RX;
    // HAL_UART_Init(&huart);
}

void Uart::write(const uint8_t* data, size_t size) noexcept {
    if (data == nullptr || size == 0) return;
    // 실제 구현 예시:
    // HAL_UART_Transmit(&s_huarts[m_instance - 1],
    //                   const_cast<uint8_t*>(data),
    //                   static_cast<uint16_t>(size),
    //                   HAL_MAX_DELAY);
    (void)data;
    (void)size;
}

size_t Uart::read(uint8_t* buffer, size_t size) noexcept {
    if (buffer == nullptr || size == 0) return 0;
    // 실제 구현 예시:
    // HAL_StatusTypeDef status =
    //     HAL_UART_Receive(&s_huarts[m_instance - 1], buffer,
    //                      static_cast<uint16_t>(size), 1000 /*ms*/);
    // return (status == HAL_OK) ? size : 0;

    // 스텁: 내부 rxBuffer에서 복사
    const size_t count = std::min(size, m_rxBuffer.size());
    for (size_t i = 0; i < count; ++i) {
        buffer[i] = m_rxBuffer[i];
    }
    m_rxBuffer.erase(m_rxBuffer.begin(),
                     m_rxBuffer.begin() + static_cast<ptrdiff_t>(count));
    return count;
}

bool Uart::isDataAvailable() const noexcept {
    // 실제 구현 예시:
    // return (__HAL_UART_GET_FLAG(&s_huarts[m_instance - 1], UART_FLAG_RXNE) != RESET);
    return !m_rxBuffer.empty();
}

void Uart::flush() noexcept {
    // 실제 구현 예시:
    // while (__HAL_UART_GET_FLAG(&s_huarts[m_instance - 1], UART_FLAG_TC) == RESET) {}
}

} // namespace platform::stm32
