#ifndef HAL_UART_H
#define HAL_UART_H

/* ──────────────────────────────────────────────
 * hal_uart.h — UART Hardware Abstraction Layer
 *
 * Supports:
 *   - Blocking transmit / receive
 *   - Non-blocking receive with callback
 *
 * Customization:
 *   - Implement hal_uart_ops_t for your MCU
 *   - Call hal_uart_register(port, &ops) at startup
 * ────────────────────────────────────────────── */

#include <stdint.h>
#include <stddef.h>
#include "hal_types.h"

/* ── RX Callback ─────────────────────────────── */
typedef void (*hal_uart_rx_callback_t)(uint8_t byte, void *user_data);

/* ── Ops Table ───────────────────────────────── */
typedef struct {
    /**
     * Initialize UART port with given configuration.
     */
    hal_err_t (*init)(uint8_t port, const hal_uart_config_t *config);

    /**
     * Transmit data (blocking).
     * @param port    UART port index
     * @param data    Data buffer
     * @param len     Number of bytes to send
     * @param timeout Timeout in milliseconds (0 = wait forever)
     * @return HAL_OK or error code
     */
    hal_err_t (*transmit)(uint8_t port, const uint8_t *data, size_t len, uint32_t timeout_ms);

    /**
     * Receive data (blocking).
     * @param port       UART port index
     * @param buf        Receive buffer
     * @param len        Number of bytes to read
     * @param timeout_ms Timeout in milliseconds
     * @return HAL_OK or error code
     */
    hal_err_t (*receive)(uint8_t port, uint8_t *buf, size_t len, uint32_t timeout_ms);

    /**
     * Register RX interrupt callback (non-blocking receive).
     * Called from ISR context — keep it short.
     */
    hal_err_t (*set_rx_callback)(uint8_t port, hal_uart_rx_callback_t cb, void *user_data);

    /**
     * Return number of bytes available in RX buffer.
     */
    int32_t (*available)(uint8_t port);

    /**
     * Deinitialize UART port.
     */
    void (*deinit)(uint8_t port);
} hal_uart_ops_t;

/* ── Registration ────────────────────────────── */

/**
 * Register platform UART driver for a specific port.
 * Supports up to HAL_UART_MAX_PORTS ports.
 */
#define HAL_UART_MAX_PORTS (4U)

hal_err_t hal_uart_register(uint8_t port, const hal_uart_ops_t *ops);

/* ── Public API ──────────────────────────────── */

hal_err_t hal_uart_init(uint8_t port, const hal_uart_config_t *config);
hal_err_t hal_uart_transmit(uint8_t port, const uint8_t *data, size_t len, uint32_t timeout_ms);
hal_err_t hal_uart_receive(uint8_t port, uint8_t *buf, size_t len, uint32_t timeout_ms);
hal_err_t hal_uart_set_rx_callback(uint8_t port, hal_uart_rx_callback_t cb, void *user_data);
int32_t   hal_uart_available(uint8_t port);
void      hal_uart_deinit(uint8_t port);

/* ── Convenience helpers ─────────────────────── */

/**
 * Transmit a null-terminated string.
 */
hal_err_t hal_uart_print(uint8_t port, const char *str);

/**
 * Transmit a null-terminated string followed by "\r\n".
 */
hal_err_t hal_uart_println(uint8_t port, const char *str);

#endif /* HAL_UART_H */
