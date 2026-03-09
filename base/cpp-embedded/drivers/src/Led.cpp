#include "../include/Led.hpp"

namespace drivers {

Led::Led(hal::IGpio& gpio, ActiveLevel activeLevel) noexcept
    : m_gpio{gpio}, m_activeLevel{activeLevel} {
    m_gpio.setDirection(hal::IGpio::Direction::Output);
    m_gpio.write(inactiveGpioLevel());
    m_isOn = false;
}

Led::~Led() noexcept {
    m_gpio.write(inactiveGpioLevel());
    m_isOn = false;
}

void Led::on() noexcept {
    m_gpio.write(activeGpioLevel());
    m_isOn = true;
}

void Led::off() noexcept {
    m_gpio.write(inactiveGpioLevel());
    m_isOn = false;
}

void Led::toggle() noexcept {
    if (m_isOn) {
        off();
    } else {
        on();
    }
}

bool Led::isOn() const noexcept {
    return m_isOn;
}

hal::IGpio::Level Led::activeGpioLevel() const noexcept {
    return (m_activeLevel == ActiveLevel::High)
               ? hal::IGpio::Level::High
               : hal::IGpio::Level::Low;
}

hal::IGpio::Level Led::inactiveGpioLevel() const noexcept {
    return (m_activeLevel == ActiveLevel::High)
               ? hal::IGpio::Level::Low
               : hal::IGpio::Level::High;
}

} // namespace drivers
