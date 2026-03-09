#ifndef HAL_TYPES_H
#define HAL_TYPES_H

/* ──────────────────────────────────────────────
 * hal_types.h — Common HAL type definitions
 *
 * All HAL modules use these shared types.
 * Platform-specific code must not leak into this header.
 * ────────────────────────────────────────────── */

#include <stdint.h>
#include <stddef.h>

/* ── Error codes ─────────────────────────────── */
typedef enum {
    HAL_OK           =  0,   /* Success */
    HAL_ERR_GENERIC  = -1,   /* Generic error */
    HAL_ERR_PARAM    = -2,   /* Invalid parameter */
    HAL_ERR_BUSY     = -3,   /* Resource busy */
    HAL_ERR_TIMEOUT  = -4,   /* Operation timed out */
    HAL_ERR_NOT_INIT = -5,   /* HAL not initialized */
    HAL_ERR_OVERFLOW = -6,   /* Buffer overflow */
} hal_err_t;

/* ── GPIO types ──────────────────────────────── */
typedef enum {
    HAL_GPIO_DIR_INPUT  = 0,
    HAL_GPIO_DIR_OUTPUT = 1,
} hal_gpio_dir_t;

typedef enum {
    HAL_GPIO_LEVEL_LOW  = 0,
    HAL_GPIO_LEVEL_HIGH = 1,
} hal_gpio_level_t;

typedef enum {
    HAL_GPIO_PULL_NONE = 0,
    HAL_GPIO_PULL_UP   = 1,
    HAL_GPIO_PULL_DOWN = 2,
} hal_gpio_pull_t;

/* ── UART types ──────────────────────────────── */
typedef enum {
    HAL_UART_PARITY_NONE = 0,
    HAL_UART_PARITY_EVEN = 1,
    HAL_UART_PARITY_ODD  = 2,
} hal_uart_parity_t;

typedef enum {
    HAL_UART_STOP_BITS_1 = 0,
    HAL_UART_STOP_BITS_2 = 1,
} hal_uart_stop_bits_t;

typedef struct {
    uint32_t           baud_rate;
    uint8_t            data_bits;    /* 7 or 8 */
    hal_uart_parity_t  parity;
    hal_uart_stop_bits_t stop_bits;
} hal_uart_config_t;

/* ── Timer types ─────────────────────────────── */
typedef enum {
    HAL_TIMER_MODE_ONESHOT  = 0,
    HAL_TIMER_MODE_PERIODIC = 1,
} hal_timer_mode_t;

typedef void (*hal_timer_callback_t)(void *arg);

#endif /* HAL_TYPES_H */
