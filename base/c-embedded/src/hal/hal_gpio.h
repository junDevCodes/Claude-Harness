#ifndef HAL_GPIO_H
#define HAL_GPIO_H

/* ──────────────────────────────────────────────
 * hal_gpio.h — GPIO Hardware Abstraction Layer
 *
 * Usage:
 *   1. Implement hal_gpio_ops_t for your platform
 *   2. Call hal_gpio_register(&my_ops) at startup
 *   3. Use hal_gpio_init/write/read/toggle anywhere
 *
 * Customization:
 *   - Add pins to your platform driver in src/drivers/<platform>/
 *   - Pull-up/down config goes in hal_gpio_config_t extensions
 * ────────────────────────────────────────────── */

#include <stdint.h>
#include "hal_types.h"

/* ── Ops Table (platform driver interface) ───── */
typedef struct {
    /**
     * Initialize a GPIO pin.
     * @param pin  Platform-specific pin number
     * @param dir  Input or Output
     * @return HAL_OK on success
     */
    hal_err_t (*init)(uint8_t pin, hal_gpio_dir_t dir);

    /**
     * Write level to an output pin.
     * @param pin   Pin number
     * @param level HIGH or LOW
     * @return HAL_OK on success
     */
    hal_err_t (*write)(uint8_t pin, hal_gpio_level_t level);

    /**
     * Read level from a pin.
     * @param pin       Pin number
     * @param out_level Pointer to store result
     * @return HAL_OK on success
     */
    hal_err_t (*read)(uint8_t pin, hal_gpio_level_t *out_level);

    /**
     * Toggle an output pin.
     * @param pin Pin number
     * @return HAL_OK on success
     */
    hal_err_t (*toggle)(uint8_t pin);

    /**
     * Release/deinitialize a pin.
     * @param pin Pin number
     */
    void (*deinit)(uint8_t pin);
} hal_gpio_ops_t;

/* ── Registration ────────────────────────────── */

/**
 * Register the platform GPIO driver.
 * Must be called once before any hal_gpio_* functions.
 *
 * @param ops  Pointer to platform ops table (must remain valid)
 * @return HAL_OK on success, HAL_ERR_PARAM if ops is NULL
 */
hal_err_t hal_gpio_register(const hal_gpio_ops_t *ops);

/* ── Public API ──────────────────────────────── */

hal_err_t hal_gpio_init(uint8_t pin, hal_gpio_dir_t dir);
hal_err_t hal_gpio_write(uint8_t pin, hal_gpio_level_t level);
hal_err_t hal_gpio_read(uint8_t pin, hal_gpio_level_t *out_level);
hal_err_t hal_gpio_toggle(uint8_t pin);
void      hal_gpio_deinit(uint8_t pin);

#endif /* HAL_GPIO_H */
