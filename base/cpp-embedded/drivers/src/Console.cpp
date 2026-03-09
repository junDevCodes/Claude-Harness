#include "../include/Console.hpp"

namespace drivers {

Console::Console(hal::IUart& uart, const hal::IUart::Config& config) noexcept
    : m_uart{uart} {
    m_uart.init(config);
}

void Console::print(const char* str) noexcept {
    if (str == nullptr) return;
    m_uart.writeString(str);
}

void Console::println(const char* str) noexcept {
    print(str);
    m_uart.writeString("\r\n");
}

void Console::printInt(int32_t value) noexcept {
    char buf[16];
    intToStr(value, buf, sizeof(buf));
    m_uart.writeString(buf);
}

void Console::printLine(const char* label, int32_t value) noexcept {
    print(label);
    m_uart.writeString(": ");
    printInt(value);
    m_uart.writeString("\r\n");
}

void Console::printHex(uint32_t value) noexcept {
    char buf[12];
    hexToStr(value, buf, sizeof(buf));
    m_uart.writeString(buf);
}

void Console::flush() noexcept {
    m_uart.flush();
}

// ── private helpers ────────────────────────────────────────────────────────

void Console::intToStr(int32_t value, char* buf, size_t bufSize) noexcept {
    if (bufSize == 0) return;

    char tmp[16];
    size_t idx = 0;
    bool negative = (value < 0);

    if (value == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }

    uint32_t uval = negative ? static_cast<uint32_t>(-value)
                             : static_cast<uint32_t>(value);

    while (uval > 0 && idx < sizeof(tmp) - 1) {
        tmp[idx++] = static_cast<char>('0' + uval % 10);
        uval /= 10;
    }
    if (negative && idx < sizeof(tmp) - 1) {
        tmp[idx++] = '-';
    }

    // reverse into buf
    size_t outIdx = 0;
    while (idx > 0 && outIdx < bufSize - 1) {
        buf[outIdx++] = tmp[--idx];
    }
    buf[outIdx] = '\0';
}

void Console::hexToStr(uint32_t value, char* buf, size_t bufSize) noexcept {
    if (bufSize < 3) return;

    static constexpr char kHexChars[] = "0123456789ABCDEF";
    char tmp[10];
    size_t idx = 0;

    if (value == 0) {
        tmp[idx++] = '0';
    } else {
        uint32_t v = value;
        while (v > 0 && idx < sizeof(tmp)) {
            tmp[idx++] = kHexChars[v & 0xFu];
            v >>= 4;
        }
    }

    size_t outIdx = 0;
    buf[outIdx++] = '0';
    if (outIdx < bufSize - 1) buf[outIdx++] = 'x';

    // reverse hex digits
    while (idx > 0 && outIdx < bufSize - 1) {
        buf[outIdx++] = tmp[--idx];
    }
    buf[outIdx] = '\0';
}

} // namespace drivers
