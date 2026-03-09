#pragma once

#include <cstddef>
#include <cstdint>

namespace hal {

/**
 * @brief SPI 하드웨어 추상화 인터페이스
 *
 * Full-duplex SPI 통신을 위한 순수 가상 인터페이스.
 * CS(Chip Select)는 별도 GPIO로 제어하는 것을 권장한다.
 */
class ISpi {
public:
    /** @brief SPI 클럭 극성 및 위상 (CPOL/CPHA) */
    enum class Mode : uint8_t {
        Mode0 = 0,  ///< CPOL=0, CPHA=0
        Mode1 = 1,  ///< CPOL=0, CPHA=1
        Mode2 = 2,  ///< CPOL=1, CPHA=0
        Mode3 = 3,  ///< CPOL=1, CPHA=1
    };

    /** @brief 비트 전송 순서 */
    enum class BitOrder : uint8_t {
        MSBFirst,
        LSBFirst,
    };

    /** @brief SPI 초기화 설정 */
    struct Config {
        uint32_t  clockHz{1'000'000};          ///< SPI 클럭 주파수 (Hz)
        Mode      mode{Mode::Mode0};
        BitOrder  bitOrder{BitOrder::MSBFirst};
    };

    virtual ~ISpi() = default;

    /**
     * @brief SPI 초기화
     * @param config 클럭, 모드, 비트 순서 설정
     */
    virtual void init(const Config& config) noexcept = 0;

    /**
     * @brief Full-duplex 전송/수신 (동시)
     * @param txData  송신 데이터 포인터 (nullptr이면 더미 전송)
     * @param rxData  수신 버퍼 포인터  (nullptr이면 수신 데이터 버림)
     * @param size    교환할 바이트 수
     */
    virtual void transfer(const uint8_t* txData,
                          uint8_t*       rxData,
                          size_t         size) noexcept = 0;

    /** @brief 송신 전용 (수신 데이터 버림) */
    void transmit(const uint8_t* data, size_t size) noexcept {
        transfer(data, nullptr, size);
    }

    /** @brief 수신 전용 (더미 0xFF 송신) */
    void receive(uint8_t* buffer, size_t size) noexcept {
        transfer(nullptr, buffer, size);
    }

protected:
    ISpi() = default;
    ISpi(const ISpi&) = delete;
    ISpi& operator=(const ISpi&) = delete;
};

} // namespace hal
