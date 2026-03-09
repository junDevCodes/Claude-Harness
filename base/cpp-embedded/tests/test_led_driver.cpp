#include <gtest/gtest.h>
#include "../mock/MockGpio.hpp"
#include "../drivers/include/Led.hpp"

using mock::MockGpio;
using drivers::Led;
using hal::IGpio;

// ── 초기화 테스트 ─────────────────────────────────────────────────────────────

TEST(LedDriverTest, Constructor_SetsOutput_AndOff) {
    MockGpio gpio;
    Led led{gpio};

    // setDirection(Output) 호출 확인
    EXPECT_EQ(gpio.lastDirection, IGpio::Direction::Output);
    // 초기 상태: 소등 (Active-high이면 Low)
    EXPECT_EQ(gpio.lastLevel, IGpio::Level::Low);
    EXPECT_FALSE(led.isOn());
}

TEST(LedDriverTest, Constructor_ActiveLow_InitializesHigh) {
    MockGpio gpio;
    Led led{gpio, Led::ActiveLevel::Low};

    // Active-low: 소등 상태는 GPIO High
    EXPECT_EQ(gpio.lastLevel, IGpio::Level::High);
    EXPECT_FALSE(led.isOn());
}

// ── On / Off 테스트 ───────────────────────────────────────────────────────────

TEST(LedDriverTest, On_WritesHighForActiveHigh) {
    MockGpio gpio;
    Led led{gpio, Led::ActiveLevel::High};

    led.on();

    EXPECT_EQ(gpio.lastLevel, IGpio::Level::High);
    EXPECT_TRUE(led.isOn());
}

TEST(LedDriverTest, Off_WritesLowForActiveHigh) {
    MockGpio gpio;
    Led led{gpio, Led::ActiveLevel::High};

    led.on();
    led.off();

    EXPECT_EQ(gpio.lastLevel, IGpio::Level::Low);
    EXPECT_FALSE(led.isOn());
}

TEST(LedDriverTest, On_WritesLowForActiveLow) {
    MockGpio gpio;
    Led led{gpio, Led::ActiveLevel::Low};

    led.on();

    EXPECT_EQ(gpio.lastLevel, IGpio::Level::Low);
    EXPECT_TRUE(led.isOn());
}

TEST(LedDriverTest, Off_WritesHighForActiveLow) {
    MockGpio gpio;
    Led led{gpio, Led::ActiveLevel::Low};

    led.on();
    led.off();

    EXPECT_EQ(gpio.lastLevel, IGpio::Level::High);
    EXPECT_FALSE(led.isOn());
}

// ── 토글 테스트 ───────────────────────────────────────────────────────────────

TEST(LedDriverTest, Toggle_FromOff_TurnsOn) {
    MockGpio gpio;
    Led led{gpio};

    led.toggle();

    EXPECT_TRUE(led.isOn());
    EXPECT_EQ(gpio.lastLevel, IGpio::Level::High);
}

TEST(LedDriverTest, Toggle_FromOn_TurnsOff) {
    MockGpio gpio;
    Led led{gpio};

    led.on();
    led.toggle();

    EXPECT_FALSE(led.isOn());
    EXPECT_EQ(gpio.lastLevel, IGpio::Level::Low);
}

TEST(LedDriverTest, Toggle_ThreeTimes) {
    MockGpio gpio;
    Led led{gpio};

    led.toggle();  // Off → On
    led.toggle();  // On  → Off
    led.toggle();  // Off → On

    EXPECT_TRUE(led.isOn());
}

// ── 소멸자 테스트 ─────────────────────────────────────────────────────────────

TEST(LedDriverTest, Destructor_TurnsOffLed) {
    MockGpio gpio;
    {
        Led led{gpio, Led::ActiveLevel::High};
        led.on();
        EXPECT_EQ(gpio.lastLevel, IGpio::Level::High);
    }  // Led 소멸

    EXPECT_EQ(gpio.lastLevel, IGpio::Level::Low);  // 소멸자에서 소등
}

TEST(LedDriverTest, Destructor_ActiveLow_WritesHigh) {
    MockGpio gpio;
    {
        Led led{gpio, Led::ActiveLevel::Low};
        led.on();
        EXPECT_EQ(gpio.lastLevel, IGpio::Level::Low);
    }  // Led 소멸

    EXPECT_EQ(gpio.lastLevel, IGpio::Level::High);  // 소멸자에서 소등 (Active-low)
}

// ── isOn 상태 일관성 테스트 ───────────────────────────────────────────────────

TEST(LedDriverTest, IsOn_ConsistentWithGpioState) {
    MockGpio gpio;
    Led led{gpio};

    EXPECT_FALSE(led.isOn());
    led.on();
    EXPECT_TRUE(led.isOn());
    led.on();  // 이미 On 상태에서 on() 재호출
    EXPECT_TRUE(led.isOn());
    led.off();
    EXPECT_FALSE(led.isOn());
    led.off();  // 이미 Off 상태에서 off() 재호출
    EXPECT_FALSE(led.isOn());
}
