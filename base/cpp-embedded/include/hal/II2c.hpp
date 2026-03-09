#pragma once

#include <cstddef>
#include <cstdint>

namespace hal {

/**
 * @brief I2C 하드웨어 추상화 인터페이스
 *
 * 마스터 모드 I2C 통신을 위한 순수 가상 인터페이스.
 * 7비트 슬레이브 주소 기준 (8비트 R/W 비트 포함 전처리는 구현체가 담당).
 */
class II2c {
public:
    /** @brief I2C 통신 결과 */
    enum class Result : uint8_t {
        Ok,
        NackAddress,  ///< 슬레이브 주소 ACK 없음
        NackData,     ///< 데이터 ACK 없음
        Timeout,
        BusError,
    };

    /** @brief I2C 초기화 설정 */
    struct Config {
        uint32_t clockHz{400'000};  ///< 400 kHz = Fast Mode, 100 kHz = Standard
    };

    virtual ~II2c() = default;

    /**
     * @brief I2C 초기화
     * @param config 클럭 속도 설정
     */
    virtual void init(const Config& config) noexcept = 0;

    /**
     * @brief 슬레이브에 데이터 쓰기
     * @param address  7비트 슬레이브 주소
     * @param data     송신 데이터 포인터
     * @param size     송신 바이트 수
     * @return         통신 결과
     */
    [[nodiscard]] virtual Result write(uint8_t        address,
                                       const uint8_t* data,
                                       size_t         size) noexcept = 0;

    /**
     * @brief 슬레이브에서 데이터 읽기
     * @param address  7비트 슬레이브 주소
     * @param buffer   수신 버퍼 포인터
     * @param size     수신할 바이트 수
     * @return         통신 결과
     */
    [[nodiscard]] virtual Result read(uint8_t  address,
                                      uint8_t* buffer,
                                      size_t   size) noexcept = 0;

    /**
     * @brief 레지스터 쓰기 (write-then-write 패턴)
     * @param address   슬레이브 주소
     * @param reg       레지스터 주소
     * @param value     쓸 값
     */
    [[nodiscard]] Result writeRegister(uint8_t address,
                                       uint8_t reg,
                                       uint8_t value) noexcept {
        const uint8_t buf[2] = {reg, value};
        return write(address, buf, 2);
    }

    /**
     * @brief 레지스터 읽기 (write-then-read 패턴)
     * @param address   슬레이브 주소
     * @param reg       레지스터 주소
     * @param value     읽은 값을 저장할 참조
     */
    [[nodiscard]] Result readRegister(uint8_t  address,
                                      uint8_t  reg,
                                      uint8_t& value) noexcept {
        Result r = write(address, &reg, 1);
        if (r != Result::Ok) return r;
        return read(address, &value, 1);
    }

protected:
    II2c() = default;
    II2c(const II2c&) = delete;
    II2c& operator=(const II2c&) = delete;
};

} // namespace hal
