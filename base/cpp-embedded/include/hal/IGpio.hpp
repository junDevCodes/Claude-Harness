#pragma once

#include <cstdint>

namespace hal {

/**
 * @brief GPIO 하드웨어 추상화 인터페이스
 *
 * 플랫폼 독립적인 GPIO 제어를 위한 순수 가상 인터페이스.
 * 구체 구현은 platform/{target}/ 디렉토리에 작성한다.
 * 단위 테스트 시 mock/ 디렉토리의 MockGpio를 사용한다.
 */
class IGpio {
public:
    /** @brief 핀 방향 */
    enum class Direction : uint8_t {
        Input,
        Output,
    };

    /** @brief 핀 레벨 */
    enum class Level : uint8_t {
        Low  = 0,
        High = 1,
    };

    /** @brief 풀업/풀다운 설정 */
    enum class PullMode : uint8_t {
        None,
        PullUp,
        PullDown,
    };

    virtual ~IGpio() = default;

    /**
     * @brief 핀 방향 설정
     * @param dir Input 또는 Output
     */
    virtual void setDirection(Direction dir) noexcept = 0;

    /**
     * @brief 출력 레벨 설정 (Output 모드일 때만 유효)
     * @param level High 또는 Low
     */
    virtual void write(Level level) noexcept = 0;

    /**
     * @brief 현재 핀 레벨 읽기
     * @return High 또는 Low
     */
    [[nodiscard]] virtual Level read() const noexcept = 0;

    /**
     * @brief 출력 레벨 토글 (Output 모드일 때만 유효)
     */
    virtual void toggle() noexcept = 0;

    /**
     * @brief 풀업/풀다운 설정
     * @param mode None, PullUp, PullDown
     */
    virtual void setPull(PullMode mode) noexcept = 0;

protected:
    IGpio() = default;
    IGpio(const IGpio&) = delete;
    IGpio& operator=(const IGpio&) = delete;
};

} // namespace hal
