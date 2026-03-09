/**
 * @file  main.cpp
 * @brief Blinky 예시 — LED 드라이버 + MockGpio 사용 데모
 *
 * 실제 하드웨어 없이 호스트(PC)에서 빌드/실행 가능.
 * 실제 MCU에서는 MockGpio 대신 platform::stm32::Gpio를 주입한다.
 *
 * 커스터마이징 포인트:
 *  - MockGpio → platform::stm32::Gpio 교체
 *  - delay() 구현 → HAL_Delay() 또는 vTaskDelay() 교체
 */

#include <iostream>
#include "../../mock/MockGpio.hpp"
#include "../../mock/MockUart.hpp"
#include "../../drivers/include/Led.hpp"
#include "../../drivers/include/Console.hpp"

// ── 플랫폼 추상화 딜레이 (호스트 빌드용) ─────────────────────────────────────
#include <thread>
#include <chrono>

static void delay_ms(uint32_t ms) {
    // 실제 MCU에서는 아래를 교체:
    // HAL_Delay(ms);          // STM32 HAL
    // vTaskDelay(pdMS_TO_TICKS(ms));  // FreeRTOS
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// ── 진입점 ────────────────────────────────────────────────────────────────────

int main() {
    // ── 하드웨어 객체 생성 ──────────────────────────────────────────────────
    mock::MockGpio ledGpio;   // 실제: platform::stm32::Gpio gpio{0 /*GPIOA*/, 5};
    mock::MockUart debugUart; // 실제: platform::stm32::Uart uart{2 /*USART2*/};

    // ── 드라이버 초기화 (RAII) ──────────────────────────────────────────────
    drivers::Led     led{ledGpio, drivers::Led::ActiveLevel::High};
    drivers::Console console{debugUart, {115200}};

    console.println("[BLINKY] System boot OK");
    console.printLine("BuildType", 1);

    // ── 메인 루프: 5회 점멸 ─────────────────────────────────────────────────
    constexpr int kBlinkCount = 5;

    for (int i = 0; i < kBlinkCount; ++i) {
        led.on();
        console.printLine("[BLINKY] LED on,  cycle", i + 1);
        delay_ms(500);

        led.off();
        console.printLine("[BLINKY] LED off, cycle", i + 1);
        delay_ms(500);
    }

    console.println("[BLINKY] Done.");

    // ── 호스트 빌드용: Mock 결과 출력 ───────────────────────────────────────
    std::cout << "\n=== MockGpio write count: " << ledGpio.writeCount << " ===\n";
    std::cout << "=== UART TX buffer: " << debugUart.txString() << "===\n";

    return 0;
}
