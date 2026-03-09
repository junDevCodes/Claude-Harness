#include <gtest/gtest.h>
#include "../mock/MockGpio.hpp"

using hal::IGpio;
using mock::MockGpio;

// ── 기본 동작 테스트 ─────────────────────────────────────────────────────────

TEST(MockGpioTest, InitialState) {
    MockGpio gpio;
    EXPECT_EQ(gpio.writeCount, 0u);
    EXPECT_EQ(gpio.toggleCount, 0u);
    EXPECT_EQ(gpio.lastLevel, IGpio::Level::Low);
    EXPECT_EQ(gpio.read(), IGpio::Level::Low);
}

TEST(MockGpioTest, WriteHigh) {
    MockGpio gpio;
    gpio.write(IGpio::Level::High);

    EXPECT_EQ(gpio.writeCount, 1u);
    EXPECT_EQ(gpio.lastLevel, IGpio::Level::High);
}

TEST(MockGpioTest, WriteLow) {
    MockGpio gpio;
    gpio.write(IGpio::Level::High);
    gpio.write(IGpio::Level::Low);

    EXPECT_EQ(gpio.writeCount, 2u);
    EXPECT_EQ(gpio.lastLevel, IGpio::Level::Low);
}

TEST(MockGpioTest, MultipleWrites) {
    MockGpio gpio;
    for (int i = 0; i < 5; ++i) {
        gpio.write(IGpio::Level::High);
        gpio.write(IGpio::Level::Low);
    }
    EXPECT_EQ(gpio.writeCount, 10u);
}

// ── 읽기 시뮬레이션 테스트 ───────────────────────────────────────────────────

TEST(MockGpioTest, ReadResponse_Default_Low) {
    MockGpio gpio;
    EXPECT_EQ(gpio.read(), IGpio::Level::Low);
}

TEST(MockGpioTest, SetReadResponse_High) {
    MockGpio gpio;
    gpio.setReadResponse(IGpio::Level::High);
    EXPECT_EQ(gpio.read(), IGpio::Level::High);
}

TEST(MockGpioTest, SetReadResponse_Low) {
    MockGpio gpio;
    gpio.setReadResponse(IGpio::Level::High);
    gpio.setReadResponse(IGpio::Level::Low);
    EXPECT_EQ(gpio.read(), IGpio::Level::Low);
}

// ── 토글 테스트 ───────────────────────────────────────────────────────────────

TEST(MockGpioTest, Toggle_FromLow_BecomesHigh) {
    MockGpio gpio;
    gpio.write(IGpio::Level::Low);
    gpio.toggle();

    EXPECT_EQ(gpio.toggleCount, 1u);
    EXPECT_EQ(gpio.lastLevel, IGpio::Level::High);
}

TEST(MockGpioTest, Toggle_FromHigh_BecomesLow) {
    MockGpio gpio;
    gpio.write(IGpio::Level::High);
    gpio.toggle();

    EXPECT_EQ(gpio.toggleCount, 1u);
    EXPECT_EQ(gpio.lastLevel, IGpio::Level::Low);
}

TEST(MockGpioTest, Toggle_MultipleTimes) {
    MockGpio gpio;
    gpio.write(IGpio::Level::Low);
    for (int i = 0; i < 6; ++i) {
        gpio.toggle();
    }
    EXPECT_EQ(gpio.toggleCount, 6u);
    EXPECT_EQ(gpio.lastLevel, IGpio::Level::Low);  // 짝수 번 토글 → 원래대로
}

// ── 방향 설정 테스트 ──────────────────────────────────────────────────────────

TEST(MockGpioTest, SetDirection_Input) {
    MockGpio gpio;
    gpio.setDirection(IGpio::Direction::Input);
    EXPECT_EQ(gpio.lastDirection, IGpio::Direction::Input);
}

TEST(MockGpioTest, SetDirection_Output) {
    MockGpio gpio;
    gpio.setDirection(IGpio::Direction::Output);
    EXPECT_EQ(gpio.lastDirection, IGpio::Direction::Output);
}

// ── 풀 설정 테스트 ────────────────────────────────────────────────────────────

TEST(MockGpioTest, SetPull_PullUp) {
    MockGpio gpio;
    gpio.setPull(IGpio::PullMode::PullUp);
    EXPECT_EQ(gpio.lastPullMode, IGpio::PullMode::PullUp);
}

TEST(MockGpioTest, SetPull_PullDown) {
    MockGpio gpio;
    gpio.setPull(IGpio::PullMode::PullDown);
    EXPECT_EQ(gpio.lastPullMode, IGpio::PullMode::PullDown);
}

// ── 리셋 테스트 ───────────────────────────────────────────────────────────────

TEST(MockGpioTest, Reset_ClearsAllState) {
    MockGpio gpio;
    gpio.write(IGpio::Level::High);
    gpio.toggle();
    gpio.setDirection(IGpio::Direction::Input);
    gpio.setPull(IGpio::PullMode::PullUp);
    gpio.setReadResponse(IGpio::Level::High);

    gpio.reset();

    EXPECT_EQ(gpio.writeCount, 0u);
    EXPECT_EQ(gpio.toggleCount, 0u);
    EXPECT_EQ(gpio.lastLevel, IGpio::Level::Low);
    EXPECT_EQ(gpio.lastDirection, IGpio::Direction::Output);
    EXPECT_EQ(gpio.lastPullMode, IGpio::PullMode::None);
    EXPECT_EQ(gpio.read(), IGpio::Level::Low);
}
