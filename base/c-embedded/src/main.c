/* ──────────────────────────────────────────────
 * main.c — Application Entry Point
 *
 * Startup sequence:
 *   1. Register platform HAL drivers
 *   2. Initialize HAL peripherals
 *   3. Create FreeRTOS tasks (static allocation)
 *   4. Start FreeRTOS scheduler
 *
 * HOW TO CUSTOMIZE:
 *   - Replace gpio_mock_ops / uart_mock_ops with your platform driver
 *   - Add/remove tasks as needed
 *   - Adjust stack sizes in include/app_config.h
 * ────────────────────────────────────────────── */

#include <stdint.h>
#include <stdio.h>

#include "app_config.h"
#include "hal_gpio.h"
#include "hal_uart.h"
#include "hal_timer.h"
#include "logger.h"
#include "task_led.h"
#include "task_uart_monitor.h"
#include "freertos_types.h"

/* ── Platform driver selection ───────────────── */
#ifdef PLATFORM_HOST
  #include "gpio_mock.h"
  #include "uart_mock.h"
  #define GPIO_OPS   gpio_mock_ops
  #define UART_OPS   uart_mock_ops
#elif defined(PLATFORM_STM32)
  #include "gpio_stm32.h"
  #include "uart_stm32.h"
  #define GPIO_OPS   gpio_stm32_ops
  #define UART_OPS   uart_stm32_ops
#else
  #error "No platform defined. Set PLATFORM_HOST or PLATFORM_STM32."
#endif

/* ── Static task parameters ──────────────────── */
static task_led_params_t s_led_params = {
    .pin       = GPIO_LED_STATUS_PIN,
    .period_ms = LED_BLINK_PERIOD_MS,
};

/* ── HAL initialization ──────────────────────── */
static void hal_init(void)
{
    hal_err_t err;

    /* Register platform drivers */
    err = hal_gpio_register(&GPIO_OPS);
    if (err != HAL_OK) {
        printf("[main] FATAL: gpio register failed\n");
        return;
    }

    err = hal_uart_register(UART_DEBUG_PORT, &UART_OPS);
    if (err != HAL_OK) {
        printf("[main] FATAL: uart register failed\n");
        return;
    }

    /* Initialize UART for debug output */
    hal_uart_config_t uart_cfg = {
        .baud_rate = UART_DEBUG_BAUD,
        .data_bits = 8U,
        .parity    = HAL_UART_PARITY_NONE,
        .stop_bits = HAL_UART_STOP_BITS_1,
    };
    hal_uart_init(UART_DEBUG_PORT, &uart_cfg);

    /* Initialize logger */
    logger_init(UART_DEBUG_PORT);
}

/* ── Task creation ───────────────────────────── */
static void tasks_create(void)
{
    /* Init UART monitor queue */
    QueueHandle_t q = task_uart_monitor_queue_init();
    if (q == NULL) {
        LOG_ERROR("main: uart monitor queue init failed");
        return;
    }

    /* Create LED blink task */
    TaskHandle_t led_handle = xTaskCreateStatic(
        task_led_run,
        "LED",
        TASK_LED_STACK_SIZE,
        &s_led_params,
        TASK_LED_PRIORITY,
        g_task_led_stack,
        &g_task_led_tcb
    );
    (void)led_handle;

    /* Create UART monitor task */
    TaskHandle_t uart_mon_handle = xTaskCreateStatic(
        task_uart_monitor_run,
        "UART_MON",
        TASK_UART_MONITOR_STACK_SIZE,
        NULL,
        TASK_UART_MONITOR_PRIORITY,
        g_task_uart_monitor_stack,
        &g_task_uart_monitor_tcb
    );
    (void)uart_mon_handle;

    LOG_INFO("main: tasks created");
}

/* ── Entry point ─────────────────────────────── */
int main(void)
{
    /* 1. Platform-specific system init (clock, etc.) */
    /* SystemInit();  ← Uncomment for STM32 */

    /* 2. HAL drivers */
    hal_init();

    LOG_INFO("main: c-embedded base starting");

    /* 3. Create tasks */
    tasks_create();

    /* 4. Start scheduler */
    /* vTaskStartScheduler();  ← Uncomment for real FreeRTOS */

    /* Host: simple smoke test loop */
#ifdef PLATFORM_HOST
    printf("[main] Host build — scheduler not started (FreeRTOS mock)\n");
    printf("[main] Smoke test: GPIO init & write\n");

    hal_gpio_init(GPIO_LED_STATUS_PIN, HAL_GPIO_DIR_OUTPUT);
    hal_gpio_write(GPIO_LED_STATUS_PIN, HAL_GPIO_LEVEL_HIGH);
    hal_gpio_write(GPIO_LED_STATUS_PIN, HAL_GPIO_LEVEL_LOW);
    hal_gpio_toggle(GPIO_LED_STATUS_PIN);

    printf("[main] Smoke test passed\n");
#endif

    /* Should not reach here on real MCU */
    for (;;) { }

    return 0;
}
