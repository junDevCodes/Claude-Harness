#pragma once

#include "../include/hal/IUart.hpp"
#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

namespace mock {

/**
 * @brief UART Mock 구현 — 단위 테스트용
 *
 * 하드웨어 없이 UART 송수신을 시뮬레이션한다.
 * - 송신 데이터는 `txBuffer`에 누적된다.
 * - 수신 데이터는 `setRxData()`로 주입한다.
 *
 * 사용 예:
 * @code
 *   mock::MockUart uart;
 *   uart.init({115200});
 *   uart.writeString("Hello");
 *   EXPECT_EQ(uart.txString(), "Hello");
 *
 *   uart.setRxData({'A', 'B', 'C'});
 *   uint8_t buf[3];
 *   size_t n = uart.read(buf, sizeof(buf));
 *   EXPECT_EQ(n, 3);
 * @endcode
 */
class MockUart final : public hal::IUart {
public:
    MockUart() = default;
    ~MockUart() override = default;

    // ── 송신 기록 ────────────────────────────────────────────────────────

    std::vector<uint8_t> txBuffer;  ///< 누적된 송신 데이터

    /** @brief 송신 데이터를 문자열로 변환 */
    [[nodiscard]] std::string txString() const {
        return {txBuffer.begin(), txBuffer.end()};
    }

    // ── 수신 시뮬레이션 ──────────────────────────────────────────────────

    /**
     * @brief 다음 read() 호출 시 반환될 데이터를 주입한다.
     * @param data 수신 시뮬레이션 데이터
     */
    void setRxData(const std::vector<uint8_t>& data) {
        m_rxBuffer.insert(m_rxBuffer.end(), data.begin(), data.end());
    }

    /** @brief 문자열로 수신 데이터 주입 */
    void setRxString(const std::string& str) {
        m_rxBuffer.insert(m_rxBuffer.end(), str.begin(), str.end());
    }

    // ── 호출 기록 ────────────────────────────────────────────────────────

    uint32_t initCount{0};
    uint32_t flushCount{0};
    Config   lastConfig;

    // ── IUart 구현 ────────────────────────────────────────────────────────

    void init(const Config& config) noexcept override {
        ++initCount;
        lastConfig = config;
    }

    void write(const uint8_t* data, size_t size) noexcept override {
        if (data == nullptr || size == 0) return;
        txBuffer.insert(txBuffer.end(), data, data + size);
    }

    [[nodiscard]] size_t read(uint8_t* buffer, size_t size) noexcept override {
        if (buffer == nullptr || size == 0) return 0;
        const size_t count = std::min(size, m_rxBuffer.size());
        for (size_t i = 0; i < count; ++i) {
            buffer[i] = m_rxBuffer[i];
        }
        m_rxBuffer.erase(m_rxBuffer.begin(),
                         m_rxBuffer.begin() + static_cast<ptrdiff_t>(count));
        return count;
    }

    [[nodiscard]] bool isDataAvailable() const noexcept override {
        return !m_rxBuffer.empty();
    }

    void flush() noexcept override {
        ++flushCount;
    }

    // ── 리셋 ─────────────────────────────────────────────────────────────

    void reset() {
        txBuffer.clear();
        m_rxBuffer.clear();
        initCount  = 0;
        flushCount = 0;
    }

private:
    std::vector<uint8_t> m_rxBuffer;
};

} // namespace mock
