#ifndef GPIO_STM32_H
#define GPIO_STM32_H

#include "hal_gpio.h"

/* STM32 GPIO driver ops table — pass to hal_gpio_register() */
extern const hal_gpio_ops_t gpio_stm32_ops;

#endif /* GPIO_STM32_H */
