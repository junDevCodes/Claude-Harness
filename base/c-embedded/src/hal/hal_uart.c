/* ──────────────────────────────────────────────
 * hal_uart.c — UART HAL Dispatcher
 * ────────────────────────────────────────────── */

#include "hal_uart.h"
#include <string.h>
#include <stddef.h>

static const hal_uart_ops_t *s_ops[HAL_UART_MAX_PORTS] = { NULL };

/* ── Registration ────────────────────────────── */

hal_err_t hal_uart_register(uint8_t port, const hal_uart_ops_t *ops)
{
    if (port >= HAL_UART_MAX_PORTS || ops == NULL) {
        return HAL_ERR_PARAM;
    }
    s_ops[port] = ops;
    return HAL_OK;
}

/* ── Internal helper ─────────────────────────── */

static hal_err_t check_port(uint8_t port)
{
    if (port >= HAL_UART_MAX_PORTS || s_ops[port] == NULL) {
        return HAL_ERR_NOT_INIT;
    }
    return HAL_OK;
}

/* ── Public API ──────────────────────────────── */

hal_err_t hal_uart_init(uint8_t port, const hal_uart_config_t *config)
{
    hal_err_t err = check_port(port);
    if (err != HAL_OK) { return err; }
    if (config == NULL) { return HAL_ERR_PARAM; }
    if (s_ops[port]->init == NULL) { return HAL_ERR_NOT_INIT; }
    return s_ops[port]->init(port, config);
}

hal_err_t hal_uart_transmit(uint8_t port, const uint8_t *data, size_t len, uint32_t timeout_ms)
{
    hal_err_t err = check_port(port);
    if (err != HAL_OK) { return err; }
    if (data == NULL || len == 0U) { return HAL_ERR_PARAM; }
    if (s_ops[port]->transmit == NULL) { return HAL_ERR_NOT_INIT; }
    return s_ops[port]->transmit(port, data, len, timeout_ms);
}

hal_err_t hal_uart_receive(uint8_t port, uint8_t *buf, size_t len, uint32_t timeout_ms)
{
    hal_err_t err = check_port(port);
    if (err != HAL_OK) { return err; }
    if (buf == NULL || len == 0U) { return HAL_ERR_PARAM; }
    if (s_ops[port]->receive == NULL) { return HAL_ERR_NOT_INIT; }
    return s_ops[port]->receive(port, buf, len, timeout_ms);
}

hal_err_t hal_uart_set_rx_callback(uint8_t port, hal_uart_rx_callback_t cb, void *user_data)
{
    hal_err_t err = check_port(port);
    if (err != HAL_OK) { return err; }
    if (s_ops[port]->set_rx_callback == NULL) { return HAL_ERR_NOT_INIT; }
    return s_ops[port]->set_rx_callback(port, cb, user_data);
}

int32_t hal_uart_available(uint8_t port)
{
    if (port >= HAL_UART_MAX_PORTS || s_ops[port] == NULL) {
        return (int32_t)HAL_ERR_NOT_INIT;
    }
    if (s_ops[port]->available == NULL) { return 0; }
    return s_ops[port]->available(port);
}

void hal_uart_deinit(uint8_t port)
{
    if (port >= HAL_UART_MAX_PORTS || s_ops[port] == NULL) { return; }
    if (s_ops[port]->deinit != NULL) {
        s_ops[port]->deinit(port);
    }
    s_ops[port] = NULL;
}

/* ── Convenience helpers ─────────────────────── */

hal_err_t hal_uart_print(uint8_t port, const char *str)
{
    if (str == NULL) { return HAL_ERR_PARAM; }
    size_t len = strlen(str);
    if (len == 0U) { return HAL_OK; }
    return hal_uart_transmit(port, (const uint8_t *)str, len, 1000U);
}

hal_err_t hal_uart_println(uint8_t port, const char *str)
{
    hal_err_t err = hal_uart_print(port, str);
    if (err != HAL_OK) { return err; }
    return hal_uart_transmit(port, (const uint8_t *)"\r\n", 2U, 100U);
}
