#ifndef HAL_TIMER_H
#define HAL_TIMER_H

/* ──────────────────────────────────────────────
 * hal_timer.h — Timer Hardware Abstraction Layer
 *
 * Provides:
 *   - Periodic / one-shot software timers via callback
 *   - System tick (ms uptime counter)
 *
 * Customization:
 *   - Implement hal_timer_ops_t for your platform
 *   - hal_timer_get_tick_ms() must return monotonic ms uptime
 * ────────────────────────────────────────────── */

#include <stdint.h>
#include "hal_types.h"

/* ── Ops Table ───────────────────────────────── */
typedef struct {
    /**
     * Initialize the timer subsystem (system tick source, etc.)
     */
    hal_err_t (*init)(void);

    /**
     * Start a timer.
     * @param timer_id   Caller-assigned timer ID (0..N)
     * @param period_ms  Period in milliseconds
     * @param mode       ONESHOT or PERIODIC
     * @param callback   Function called when timer fires
     * @param arg        Argument passed to callback
     */
    hal_err_t (*start)(uint8_t timer_id, uint32_t period_ms,
                       hal_timer_mode_t mode,
                       hal_timer_callback_t callback,
                       void *arg);

    /**
     * Stop a running timer.
     */
    hal_err_t (*stop)(uint8_t timer_id);

    /**
     * Return monotonic uptime in milliseconds.
     */
    uint32_t (*get_tick_ms)(void);

    /**
     * Blocking delay in milliseconds.
     * In FreeRTOS context, prefer vTaskDelay instead.
     */
    void (*delay_ms)(uint32_t ms);

    /**
     * Deinitialize timer subsystem.
     */
    void (*deinit)(void);
} hal_timer_ops_t;

/* ── Registration ────────────────────────────── */
hal_err_t hal_timer_register(const hal_timer_ops_t *ops);

/* ── Public API ──────────────────────────────── */
hal_err_t hal_timer_init(void);
hal_err_t hal_timer_start(uint8_t timer_id, uint32_t period_ms,
                          hal_timer_mode_t mode,
                          hal_timer_callback_t callback,
                          void *arg);
hal_err_t hal_timer_stop(uint8_t timer_id);
uint32_t  hal_timer_get_tick_ms(void);
void      hal_timer_delay_ms(uint32_t ms);
void      hal_timer_deinit(void);

#endif /* HAL_TIMER_H */
