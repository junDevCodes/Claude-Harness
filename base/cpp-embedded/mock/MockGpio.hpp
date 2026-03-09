#pragma once

#include "../include/hal/IGpio.hpp"
#include <cstdint>

namespace mock {

/**
 * @brief GPIO Mock 구현 — 단위 테스트용
 *
 * 하드웨어 없이 GPIO 동작을 시뮬레이션한다.
 * 상태를 내부에 기록하여 테스트 코드에서 검증할 수 있다.
 *
 * 사용 예:
 * @code
 *   mock::MockGpio gpio;
 *   gpio.write(hal::IGpio::Level::High);
 *   EXPECT_EQ(gpio.writeCount, 1);
 *   EXPECT_EQ(gpio.lastLevel, hal::IGpio::Level::High);
 * @endcode
 */
class MockGpio final : public hal::IGpio {
public:
    MockGpio() = default;
    ~MockGpio() override = default;

    // ── 호출 기록 (테스트에서 직접 접근) ────────────────────────────────

    uint32_t  writeCount{0};
    uint32_t  toggleCount{0};
    Level     lastLevel{Level::Low};
    Direction lastDirection{Direction::Output};
    PullMode  lastPullMode{PullMode::None};

    // ── 수신 시뮬레이션 ─────────────────────────────────────────────────

    /**
     * @brief read() 호출 시 반환할 레벨을 미리 설정한다.
     * @param level 시뮬레이션할 입력 레벨
     */
    void setReadResponse(Level level) noexcept {
        m_readResponse = level;
    }

    // ── IGpio 구현 ───────────────────────────────────────────────────────

    void setDirection(Direction dir) noexcept override {
        lastDirection = dir;
    }

    void write(Level level) noexcept override {
        ++writeCount;
        lastLevel = level;
    }

    [[nodiscard]] Level read() const noexcept override {
        return m_readResponse;
    }

    void toggle() noexcept override {
        ++toggleCount;
        lastLevel = (lastLevel == Level::High) ? Level::Low : Level::High;
    }

    void setPull(PullMode mode) noexcept override {
        lastPullMode = mode;
    }

    // ── 리셋 ────────────────────────────────────────────────────────────

    /** @brief 모든 기록 초기화 */
    void reset() noexcept {
        writeCount    = 0;
        toggleCount   = 0;
        lastLevel     = Level::Low;
        lastDirection = Direction::Output;
        lastPullMode  = PullMode::None;
        m_readResponse = Level::Low;
    }

private:
    Level m_readResponse{Level::Low};
};

} // namespace mock
