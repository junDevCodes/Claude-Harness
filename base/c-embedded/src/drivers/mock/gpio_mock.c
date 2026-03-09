/* ──────────────────────────────────────────────
 * gpio_mock.c — GPIO Mock Driver (Host Testing)
 *
 * Simulates GPIO in memory. Usable on any host PC.
 * Registered via: hal_gpio_register(&gpio_mock_ops)
 * ────────────────────────────────────────────── */

#include "hal_gpio.h"
#include "app_config.h"
#include <string.h>
#include <stdio.h>

/* ── Internal state ──────────────────────────── */
typedef struct {
    hal_gpio_dir_t   dir;
    hal_gpio_level_t level;
    uint8_t          initialized;
} gpio_pin_state_t;

static gpio_pin_state_t s_pins[GPIO_MAX_PINS];

/* ── Implementation ──────────────────────────── */

static hal_err_t mock_gpio_init(uint8_t pin, hal_gpio_dir_t dir)
{
    if (pin >= GPIO_MAX_PINS) { return HAL_ERR_PARAM; }
    s_pins[pin].dir         = dir;
    s_pins[pin].level       = HAL_GPIO_LEVEL_LOW;
    s_pins[pin].initialized = 1U;
    printf("[mock_gpio] init pin=%u dir=%s\n", pin,
           (dir == HAL_GPIO_DIR_OUTPUT) ? "OUT" : "IN");
    return HAL_OK;
}

static hal_err_t mock_gpio_write(uint8_t pin, hal_gpio_level_t level)
{
    if (pin >= GPIO_MAX_PINS)         { return HAL_ERR_PARAM; }
    if (!s_pins[pin].initialized)     { return HAL_ERR_NOT_INIT; }
    if (s_pins[pin].dir != HAL_GPIO_DIR_OUTPUT) {
        fprintf(stderr, "[mock_gpio] write on INPUT pin %u\n", pin);
        return HAL_ERR_PARAM;
    }
    s_pins[pin].level = level;
    printf("[mock_gpio] write pin=%u level=%s\n", pin,
           (level == HAL_GPIO_LEVEL_HIGH) ? "HIGH" : "LOW");
    return HAL_OK;
}

static hal_err_t mock_gpio_read(uint8_t pin, hal_gpio_level_t *out_level)
{
    if (pin >= GPIO_MAX_PINS)     { return HAL_ERR_PARAM; }
    if (!s_pins[pin].initialized) { return HAL_ERR_NOT_INIT; }
    if (out_level == NULL)        { return HAL_ERR_PARAM; }
    *out_level = s_pins[pin].level;
    return HAL_OK;
}

static hal_err_t mock_gpio_toggle(uint8_t pin)
{
    if (pin >= GPIO_MAX_PINS)     { return HAL_ERR_PARAM; }
    if (!s_pins[pin].initialized) { return HAL_ERR_NOT_INIT; }
    s_pins[pin].level = (s_pins[pin].level == HAL_GPIO_LEVEL_HIGH)
                        ? HAL_GPIO_LEVEL_LOW
                        : HAL_GPIO_LEVEL_HIGH;
    printf("[mock_gpio] toggle pin=%u → %s\n", pin,
           (s_pins[pin].level == HAL_GPIO_LEVEL_HIGH) ? "HIGH" : "LOW");
    return HAL_OK;
}

static void mock_gpio_deinit(uint8_t pin)
{
    if (pin >= GPIO_MAX_PINS) { return; }
    memset(&s_pins[pin], 0, sizeof(s_pins[pin]));
    printf("[mock_gpio] deinit pin=%u\n", pin);
}

/* ── Test Helper ─────────────────────────────── */

/**
 * Directly set a mock pin's level (simulates external signal).
 * Useful in unit tests to drive INPUT pins.
 */
void gpio_mock_set_input_level(uint8_t pin, hal_gpio_level_t level)
{
    if (pin < GPIO_MAX_PINS) {
        s_pins[pin].level = level;
    }
}

/**
 * Get raw pin level from mock state (for assertion in tests).
 */
hal_gpio_level_t gpio_mock_get_level(uint8_t pin)
{
    if (pin >= GPIO_MAX_PINS) { return HAL_GPIO_LEVEL_LOW; }
    return s_pins[pin].level;
}

/* ── Ops Table ───────────────────────────────── */

const hal_gpio_ops_t gpio_mock_ops = {
    .init   = mock_gpio_init,
    .write  = mock_gpio_write,
    .read   = mock_gpio_read,
    .toggle = mock_gpio_toggle,
    .deinit = mock_gpio_deinit,
};
