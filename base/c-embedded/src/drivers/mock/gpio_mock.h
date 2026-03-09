#ifndef GPIO_MOCK_H
#define GPIO_MOCK_H

#include "hal_gpio.h"

/* Mock driver ops table — pass to hal_gpio_register() */
extern const hal_gpio_ops_t gpio_mock_ops;

/* Test helpers */
void             gpio_mock_set_input_level(uint8_t pin, hal_gpio_level_t level);
hal_gpio_level_t gpio_mock_get_level(uint8_t pin);

#endif /* GPIO_MOCK_H */
