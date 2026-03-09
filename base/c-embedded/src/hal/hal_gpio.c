/* ──────────────────────────────────────────────
 * hal_gpio.c — GPIO HAL Dispatcher
 *
 * Dispatches all GPIO calls to the registered platform driver.
 * Does NOT contain platform-specific code.
 * ────────────────────────────────────────────── */

#include "hal_gpio.h"
#include <stddef.h>

/* Single registered driver (one GPIO driver per system) */
static const hal_gpio_ops_t *s_ops = NULL;

hal_err_t hal_gpio_register(const hal_gpio_ops_t *ops)
{
    if (ops == NULL) {
        return HAL_ERR_PARAM;
    }
    s_ops = ops;
    return HAL_OK;
}

hal_err_t hal_gpio_init(uint8_t pin, hal_gpio_dir_t dir)
{
    if (s_ops == NULL || s_ops->init == NULL) {
        return HAL_ERR_NOT_INIT;
    }
    return s_ops->init(pin, dir);
}

hal_err_t hal_gpio_write(uint8_t pin, hal_gpio_level_t level)
{
    if (s_ops == NULL || s_ops->write == NULL) {
        return HAL_ERR_NOT_INIT;
    }
    return s_ops->write(pin, level);
}

hal_err_t hal_gpio_read(uint8_t pin, hal_gpio_level_t *out_level)
{
    if (s_ops == NULL || s_ops->read == NULL) {
        return HAL_ERR_NOT_INIT;
    }
    if (out_level == NULL) {
        return HAL_ERR_PARAM;
    }
    return s_ops->read(pin, out_level);
}

hal_err_t hal_gpio_toggle(uint8_t pin)
{
    if (s_ops == NULL || s_ops->toggle == NULL) {
        return HAL_ERR_NOT_INIT;
    }
    return s_ops->toggle(pin);
}

void hal_gpio_deinit(uint8_t pin)
{
    if (s_ops != NULL && s_ops->deinit != NULL) {
        s_ops->deinit(pin);
    }
}
