/* ──────────────────────────────────────────────
 * task_led.c — LED Blink FreeRTOS Task
 *
 * Pattern: Infinite loop with vTaskDelay.
 * This is the fundamental FreeRTOS task pattern.
 * ────────────────────────────────────────────── */

#include "task_led.h"
#include "hal_gpio.h"
#include "logger.h"

/* Static task allocation */
StaticTask_t g_task_led_tcb;
StackType_t  g_task_led_stack[TASK_LED_STACK_SIZE];

/* ── Task function ────────────────────────────── */

void task_led_run(void *pv_params)
{
    /* Cast parameters — caller owns the storage */
    const task_led_params_t *params = (const task_led_params_t *)pv_params;
    uint8_t  pin       = (params != NULL) ? params->pin       : GPIO_LED_STATUS_PIN;
    uint32_t period_ms = (params != NULL) ? params->period_ms : LED_BLINK_PERIOD_MS;

    /* ── Init ────────────────────────────────── */
    hal_err_t err = hal_gpio_init(pin, HAL_GPIO_DIR_OUTPUT);
    if (err != HAL_OK) {
        LOG_ERROR("task_led: gpio init failed (%d)", (int)err);
        vTaskSuspend(NULL); /* Suspend self on critical error */
    }

    LOG_INFO("task_led: started pin=%u period=%ums", (unsigned)pin, (unsigned)period_ms);

    /* ── Main loop ───────────────────────────── */
    for (;;) {
        hal_gpio_write(pin, HAL_GPIO_LEVEL_HIGH);
        vTaskDelay(pdMS_TO_TICKS(period_ms / 2U));

        hal_gpio_write(pin, HAL_GPIO_LEVEL_LOW);
        vTaskDelay(pdMS_TO_TICKS(period_ms / 2U));
    }

    /* Should never reach here */
    hal_gpio_deinit(pin);
}
