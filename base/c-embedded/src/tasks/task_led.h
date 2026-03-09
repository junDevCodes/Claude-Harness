#ifndef TASK_LED_H
#define TASK_LED_H

/* ──────────────────────────────────────────────
 * task_led.h — LED Blink FreeRTOS Task
 *
 * Pattern: Basic periodic task with vTaskDelay loop.
 * Demonstrates:
 *   - Task function signature
 *   - Static task allocation (no heap)
 *   - HAL GPIO usage from a task
 *   - Configurable blink period via task parameter
 * ────────────────────────────────────────────── */

#include <stdint.h>
#include "freertos_types.h"
#include "app_config.h"

/* Task parameter structure */
typedef struct {
    uint8_t  pin;           /* GPIO pin for LED */
    uint32_t period_ms;     /* Blink period in milliseconds */
} task_led_params_t;

/* Static allocation storage (define in exactly one .c file) */
extern StaticTask_t  g_task_led_tcb;
extern StackType_t   g_task_led_stack[TASK_LED_STACK_SIZE];

/**
 * FreeRTOS task function for LED blink.
 *
 * Usage:
 *   task_led_params_t params = { .pin = GPIO_LED_STATUS_PIN, .period_ms = 500 };
 *   xTaskCreateStatic(task_led_run, "LED", TASK_LED_STACK_SIZE,
 *                     &params, TASK_LED_PRIORITY,
 *                     g_task_led_stack, &g_task_led_tcb);
 */
void task_led_run(void *pv_params);

#endif /* TASK_LED_H */
