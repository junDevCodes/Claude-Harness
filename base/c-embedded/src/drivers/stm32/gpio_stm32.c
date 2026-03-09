/* ──────────────────────────────────────────────
 * gpio_stm32.c — STM32 GPIO Driver (Reference Stub)
 *
 * HOW TO USE:
 *   1. Include your STM32 HAL headers (e.g., stm32f4xx_hal.h)
 *   2. Replace the TODO sections with actual STM32 HAL calls
 *   3. Register at startup: hal_gpio_register(&gpio_stm32_ops)
 *
 * Pin Mapping Convention:
 *   Pin numbers are project-defined. Recommend mapping to
 *   a lookup table: pin_num → (GPIOx, GPIO_Pin).
 * ────────────────────────────────────────────── */

#include "hal_gpio.h"

/* TODO: Include STM32 HAL headers */
/* #include "stm32f4xx_hal.h" */

/* ── Pin Map (customize per board) ───────────── */
/*
 * Example mapping for STM32F4:
 *
 * typedef struct { GPIO_TypeDef *port; uint16_t pin; } stm32_gpio_pin_t;
 *
 * static const stm32_gpio_pin_t s_pin_map[] = {
 *     [0]  = { GPIOA, GPIO_PIN_0  },
 *     [13] = { GPIOC, GPIO_PIN_13 },  // Onboard LED (Nucleo)
 * };
 */

/* ── Implementation ──────────────────────────── */

static hal_err_t stm32_gpio_init(uint8_t pin, hal_gpio_dir_t dir)
{
    (void)pin;
    (void)dir;
    /*
     * TODO:
     * GPIO_InitTypeDef init = {0};
     * init.Pin   = s_pin_map[pin].pin;
     * init.Mode  = (dir == HAL_GPIO_DIR_OUTPUT)
     *              ? GPIO_MODE_OUTPUT_PP : GPIO_MODE_INPUT;
     * init.Pull  = GPIO_NOPULL;
     * init.Speed = GPIO_SPEED_FREQ_LOW;
     * HAL_GPIO_Init(s_pin_map[pin].port, &init);
     */
    return HAL_OK;
}

static hal_err_t stm32_gpio_write(uint8_t pin, hal_gpio_level_t level)
{
    (void)pin;
    (void)level;
    /*
     * TODO:
     * GPIO_PinState state = (level == HAL_GPIO_LEVEL_HIGH)
     *                       ? GPIO_PIN_SET : GPIO_PIN_RESET;
     * HAL_GPIO_WritePin(s_pin_map[pin].port, s_pin_map[pin].pin, state);
     */
    return HAL_OK;
}

static hal_err_t stm32_gpio_read(uint8_t pin, hal_gpio_level_t *out_level)
{
    if (out_level == NULL) { return HAL_ERR_PARAM; }
    (void)pin;
    /*
     * TODO:
     * GPIO_PinState state = HAL_GPIO_ReadPin(s_pin_map[pin].port, s_pin_map[pin].pin);
     * *out_level = (state == GPIO_PIN_SET) ? HAL_GPIO_LEVEL_HIGH : HAL_GPIO_LEVEL_LOW;
     */
    *out_level = HAL_GPIO_LEVEL_LOW; /* placeholder */
    return HAL_OK;
}

static hal_err_t stm32_gpio_toggle(uint8_t pin)
{
    (void)pin;
    /*
     * TODO:
     * HAL_GPIO_TogglePin(s_pin_map[pin].port, s_pin_map[pin].pin);
     */
    return HAL_OK;
}

static void stm32_gpio_deinit(uint8_t pin)
{
    (void)pin;
    /*
     * TODO:
     * HAL_GPIO_DeInit(s_pin_map[pin].port, s_pin_map[pin].pin);
     */
}

/* ── Ops Table ───────────────────────────────── */
const hal_gpio_ops_t gpio_stm32_ops = {
    .init   = stm32_gpio_init,
    .write  = stm32_gpio_write,
    .read   = stm32_gpio_read,
    .toggle = stm32_gpio_toggle,
    .deinit = stm32_gpio_deinit,
};
