/* ──────────────────────────────────────────────
 * hal_timer.c — Timer HAL Dispatcher
 * ────────────────────────────────────────────── */

#include "hal_timer.h"
#include <stddef.h>

static const hal_timer_ops_t *s_ops = NULL;

hal_err_t hal_timer_register(const hal_timer_ops_t *ops)
{
    if (ops == NULL) { return HAL_ERR_PARAM; }
    s_ops = ops;
    return HAL_OK;
}

hal_err_t hal_timer_init(void)
{
    if (s_ops == NULL || s_ops->init == NULL) { return HAL_ERR_NOT_INIT; }
    return s_ops->init();
}

hal_err_t hal_timer_start(uint8_t timer_id, uint32_t period_ms,
                          hal_timer_mode_t mode,
                          hal_timer_callback_t callback,
                          void *arg)
{
    if (s_ops == NULL || s_ops->start == NULL) { return HAL_ERR_NOT_INIT; }
    if (callback == NULL) { return HAL_ERR_PARAM; }
    return s_ops->start(timer_id, period_ms, mode, callback, arg);
}

hal_err_t hal_timer_stop(uint8_t timer_id)
{
    if (s_ops == NULL || s_ops->stop == NULL) { return HAL_ERR_NOT_INIT; }
    return s_ops->stop(timer_id);
}

uint32_t hal_timer_get_tick_ms(void)
{
    if (s_ops == NULL || s_ops->get_tick_ms == NULL) { return 0U; }
    return s_ops->get_tick_ms();
}

void hal_timer_delay_ms(uint32_t ms)
{
    if (s_ops != NULL && s_ops->delay_ms != NULL) {
        s_ops->delay_ms(ms);
    }
}

void hal_timer_deinit(void)
{
    if (s_ops != NULL && s_ops->deinit != NULL) {
        s_ops->deinit();
    }
    s_ops = NULL;
}
