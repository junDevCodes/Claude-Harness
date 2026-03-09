#pragma once

#include "../../include/hal/IUart.hpp"
#include <cstdint>

namespace drivers {

/**
 * @brief 콘솔(디버그 UART) 드라이버
 *
 * UART 인터페이스 위에서 동작하는 간단한 콘솔 출력 드라이버.
 * printf 스타일 포맷 없이 문자열/정수 출력을 제공한다.
 * 임베디드 환경에서 경량 디버그 출력으로 사용한다.
 *
 * 사용 예:
 * @code
 *   mock::MockUart uart;
 *   drivers::Console console{uart, {115200}};
 *   console.print("boot ok\r\n");
 *   console.printLine("value", 42);
 * @endcode
 */
class Console {
public:
    /**
     * @brief 생성자 — UART 초기화 수행 (RAII)
     * @param uart    IUart 구현체 참조
     * @param config  UART 설정 (보드레이트 등)
     */
    explicit Console(hal::IUart& uart,
                     const hal::IUart::Config& config = {}) noexcept;

    ~Console() = default;

    Console(const Console&) = delete;
    Console& operator=(const Console&) = delete;

    /** @brief 문자열 출력 (개행 없음) */
    void print(const char* str) noexcept;

    /** @brief 문자열 + 개행 출력 */
    void println(const char* str) noexcept;

    /** @brief 부호 있는 정수 출력 */
    void printInt(int32_t value) noexcept;

    /**
     * @brief 레이블과 정수값을 한 줄로 출력
     * @example  console.printLine("temp", 36) → "temp: 36\r\n"
     */
    void printLine(const char* label, int32_t value) noexcept;

    /** @brief 16진수 출력 (0x 접두어 포함) */
    void printHex(uint32_t value) noexcept;

    /** @brief UART 플러시 */
    void flush() noexcept;

private:
    hal::IUart& m_uart;

    static void intToStr(int32_t value, char* buf, size_t bufSize) noexcept;
    static void hexToStr(uint32_t value, char* buf, size_t bufSize) noexcept;
};

} // namespace drivers
