#include <gtest/gtest.h>
#include "../mock/MockUart.hpp"
#include <string>

using mock::MockUart;

// ── 초기화 테스트 ─────────────────────────────────────────────────────────────

TEST(MockUartTest, InitialState) {
    MockUart uart;
    EXPECT_TRUE(uart.txBuffer.empty());
    EXPECT_FALSE(uart.isDataAvailable());
    EXPECT_EQ(uart.initCount, 0u);
    EXPECT_EQ(uart.flushCount, 0u);
}

TEST(MockUartTest, Init_RecordsConfig) {
    MockUart uart;
    hal::IUart::Config cfg;
    cfg.baudRate = 9600;
    cfg.dataBits = 8;
    cfg.stopBits = 1;

    uart.init(cfg);

    EXPECT_EQ(uart.initCount, 1u);
    EXPECT_EQ(uart.lastConfig.baudRate, 9600u);
}

TEST(MockUartTest, Init_CalledMultipleTimes) {
    MockUart uart;
    uart.init({115200});
    uart.init({9600});
    EXPECT_EQ(uart.initCount, 2u);
    EXPECT_EQ(uart.lastConfig.baudRate, 9600u);
}

// ── 송신 테스트 ───────────────────────────────────────────────────────────────

TEST(MockUartTest, Write_StoresData) {
    MockUart uart;
    const uint8_t data[] = {0x01, 0x02, 0x03};
    uart.write(data, sizeof(data));

    ASSERT_EQ(uart.txBuffer.size(), 3u);
    EXPECT_EQ(uart.txBuffer[0], 0x01);
    EXPECT_EQ(uart.txBuffer[1], 0x02);
    EXPECT_EQ(uart.txBuffer[2], 0x03);
}

TEST(MockUartTest, WriteString_StoresString) {
    MockUart uart;
    uart.writeString("Hello");
    EXPECT_EQ(uart.txString(), "Hello");
}

TEST(MockUartTest, WriteString_Accumulates) {
    MockUart uart;
    uart.writeString("Hello");
    uart.writeString(", World");
    EXPECT_EQ(uart.txString(), "Hello, World");
}

TEST(MockUartTest, WriteByte_StoresSingleByte) {
    MockUart uart;
    uart.writeByte(0xAB);
    ASSERT_EQ(uart.txBuffer.size(), 1u);
    EXPECT_EQ(uart.txBuffer[0], 0xABu);
}

TEST(MockUartTest, Write_NullPtr_DoesNothing) {
    MockUart uart;
    uart.write(nullptr, 10);
    EXPECT_TRUE(uart.txBuffer.empty());
}

TEST(MockUartTest, Write_ZeroSize_DoesNothing) {
    MockUart uart;
    const uint8_t data[] = {0xFF};
    uart.write(data, 0);
    EXPECT_TRUE(uart.txBuffer.empty());
}

// ── 수신 테스트 ───────────────────────────────────────────────────────────────

TEST(MockUartTest, IsDataAvailable_FalseWhenEmpty) {
    MockUart uart;
    EXPECT_FALSE(uart.isDataAvailable());
}

TEST(MockUartTest, IsDataAvailable_TrueAfterSetRxData) {
    MockUart uart;
    uart.setRxData({0x01, 0x02});
    EXPECT_TRUE(uart.isDataAvailable());
}

TEST(MockUartTest, Read_ReturnsInjectedData) {
    MockUart uart;
    uart.setRxData({0xAA, 0xBB, 0xCC});

    uint8_t buf[3] = {};
    size_t n = uart.read(buf, sizeof(buf));

    EXPECT_EQ(n, 3u);
    EXPECT_EQ(buf[0], 0xAAu);
    EXPECT_EQ(buf[1], 0xBBu);
    EXPECT_EQ(buf[2], 0xCCu);
}

TEST(MockUartTest, Read_PartialRead) {
    MockUart uart;
    uart.setRxData({0x01, 0x02, 0x03, 0x04});

    uint8_t buf[2] = {};
    size_t n = uart.read(buf, 2);

    EXPECT_EQ(n, 2u);
    EXPECT_EQ(buf[0], 0x01u);
    EXPECT_EQ(buf[1], 0x02u);
    EXPECT_TRUE(uart.isDataAvailable());  // 2바이트 남음
}

TEST(MockUartTest, SetRxString_WorksWithRead) {
    MockUart uart;
    uart.setRxString("OK\r\n");

    uint8_t buf[4] = {};
    size_t n = uart.read(buf, sizeof(buf));

    EXPECT_EQ(n, 4u);
    EXPECT_EQ(std::string(reinterpret_cast<char*>(buf), n), "OK\r\n");
}

// ── 플러시 테스트 ─────────────────────────────────────────────────────────────

TEST(MockUartTest, Flush_RecordsCall) {
    MockUart uart;
    uart.flush();
    uart.flush();
    EXPECT_EQ(uart.flushCount, 2u);
}

// ── 리셋 테스트 ───────────────────────────────────────────────────────────────

TEST(MockUartTest, Reset_ClearsAllState) {
    MockUart uart;
    uart.writeString("test");
    uart.setRxData({0x01});
    uart.flush();

    uart.reset();

    EXPECT_TRUE(uart.txBuffer.empty());
    EXPECT_FALSE(uart.isDataAvailable());
    EXPECT_EQ(uart.initCount, 0u);
    EXPECT_EQ(uart.flushCount, 0u);
}
