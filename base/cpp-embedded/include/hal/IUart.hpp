#pragma once

#include <cstddef>
#include <cstdint>

namespace hal {

/**
 * @brief UART 하드웨어 추상화 인터페이스
 *
 * 플랫폼 독립적인 UART 통신을 위한 순수 가상 인터페이스.
 * 비동기 수신은 콜백 패턴으로 확장 가능 (이 템플릿은 폴링 방식 기본 제공).
 */
class IUart {
public:
    /** @brief UART 초기화 설정 */
    struct Config {
        uint32_t baudRate{115200};
        uint8_t  dataBits{8};   ///< 데이터 비트 수 (7 또는 8)
        uint8_t  stopBits{1};   ///< 정지 비트 수 (1 또는 2)

        enum class Parity : uint8_t {
            None,
            Even,
            Odd,
        } parity{Parity::None};
    };

    virtual ~IUart() = default;

    /**
     * @brief UART 초기화
     * @param config 보드레이트, 데이터비트 등 설정값
     */
    virtual void init(const Config& config) noexcept = 0;

    /**
     * @brief 데이터 송신 (블로킹)
     * @param data  전송할 데이터 포인터
     * @param size  전송할 바이트 수
     */
    virtual void write(const uint8_t* data, size_t size) noexcept = 0;

    /**
     * @brief 데이터 수신 (블로킹, 타임아웃 없음)
     * @param buffer  수신 버퍼 포인터
     * @param size    수신할 최대 바이트 수
     * @return        실제 수신된 바이트 수
     */
    [[nodiscard]] virtual size_t read(uint8_t* buffer, size_t size) noexcept = 0;

    /**
     * @brief 수신 대기 데이터 존재 여부 확인
     * @return true if data is available
     */
    [[nodiscard]] virtual bool isDataAvailable() const noexcept = 0;

    /**
     * @brief 송신 버퍼 플러시 (모든 데이터 전송 완료 대기)
     */
    virtual void flush() noexcept = 0;

    // ── 편의 오버로드 ─────────────────────────────────────────

    /** @brief 문자열 리터럴 전송 편의 함수 */
    void writeString(const char* str) noexcept {
        if (str == nullptr) return;
        size_t len = 0;
        while (str[len] != '\0') ++len;
        write(reinterpret_cast<const uint8_t*>(str), len);
    }

    /** @brief 단일 바이트 전송 */
    void writeByte(uint8_t byte) noexcept {
        write(&byte, 1);
    }

protected:
    IUart() = default;
    IUart(const IUart&) = delete;
    IUart& operator=(const IUart&) = delete;
};

} // namespace hal
