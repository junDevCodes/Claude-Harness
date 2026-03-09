/* ──────────────────────────────────────────────
 * uart_mock.c — UART Mock Driver (Host Testing)
 *
 * Simulates UART TX/RX using in-memory buffers.
 * - TX: writes to a capture buffer (inspectable by tests)
 * - RX: reads from a feed buffer (settable by tests)
 *
 * Registered via: hal_uart_register(port, &uart_mock_ops)
 * ────────────────────────────────────────────── */

#include "hal_uart.h"
#include "app_config.h"
#include <string.h>
#include <stdio.h>

#define MOCK_TX_BUF_SIZE  (512U)
#define MOCK_RX_BUF_SIZE  (512U)
#define MOCK_MAX_PORTS    HAL_UART_MAX_PORTS

/* ── Internal state ──────────────────────────── */
typedef struct {
    uint8_t  tx_buf[MOCK_TX_BUF_SIZE];
    size_t   tx_len;

    uint8_t  rx_buf[MOCK_RX_BUF_SIZE];
    size_t   rx_head;
    size_t   rx_tail;

    hal_uart_rx_callback_t rx_cb;
    void                  *rx_cb_data;

    uint8_t  initialized;
} uart_mock_state_t;

static uart_mock_state_t s_state[MOCK_MAX_PORTS];

/* ── Implementation ──────────────────────────── */

static hal_err_t mock_uart_init(uint8_t port, const hal_uart_config_t *config)
{
    if (port >= MOCK_MAX_PORTS || config == NULL) { return HAL_ERR_PARAM; }
    memset(&s_state[port], 0, sizeof(s_state[port]));
    s_state[port].initialized = 1U;
    printf("[mock_uart] port=%u init baud=%u\n", port, config->baud_rate);
    return HAL_OK;
}

static hal_err_t mock_uart_transmit(uint8_t port, const uint8_t *data,
                                    size_t len, uint32_t timeout_ms)
{
    (void)timeout_ms;
    if (port >= MOCK_MAX_PORTS)       { return HAL_ERR_PARAM; }
    if (!s_state[port].initialized)   { return HAL_ERR_NOT_INIT; }
    if (data == NULL || len == 0U)    { return HAL_ERR_PARAM; }

    size_t space = MOCK_TX_BUF_SIZE - s_state[port].tx_len;
    size_t to_copy = (len < space) ? len : space;
    memcpy(s_state[port].tx_buf + s_state[port].tx_len, data, to_copy);
    s_state[port].tx_len += to_copy;

    /* Echo to stdout for human-readable test output */
    fwrite(data, 1, to_copy, stdout);

    return (to_copy == len) ? HAL_OK : HAL_ERR_OVERFLOW;
}

static hal_err_t mock_uart_receive(uint8_t port, uint8_t *buf,
                                   size_t len, uint32_t timeout_ms)
{
    (void)timeout_ms;
    if (port >= MOCK_MAX_PORTS)     { return HAL_ERR_PARAM; }
    if (!s_state[port].initialized) { return HAL_ERR_NOT_INIT; }
    if (buf == NULL || len == 0U)   { return HAL_ERR_PARAM; }

    size_t available = s_state[port].rx_tail - s_state[port].rx_head;
    if (available == 0U) { return HAL_ERR_TIMEOUT; }

    size_t to_read = (len < available) ? len : available;
    memcpy(buf, s_state[port].rx_buf + s_state[port].rx_head, to_read);
    s_state[port].rx_head += to_read;
    return HAL_OK;
}

static hal_err_t mock_uart_set_rx_callback(uint8_t port,
                                            hal_uart_rx_callback_t cb,
                                            void *user_data)
{
    if (port >= MOCK_MAX_PORTS) { return HAL_ERR_PARAM; }
    s_state[port].rx_cb      = cb;
    s_state[port].rx_cb_data = user_data;
    return HAL_OK;
}

static int32_t mock_uart_available(uint8_t port)
{
    if (port >= MOCK_MAX_PORTS || !s_state[port].initialized) { return 0; }
    return (int32_t)(s_state[port].rx_tail - s_state[port].rx_head);
}

static void mock_uart_deinit(uint8_t port)
{
    if (port >= MOCK_MAX_PORTS) { return; }
    memset(&s_state[port], 0, sizeof(s_state[port]));
    printf("[mock_uart] port=%u deinit\n", port);
}

/* ── Test Helpers ────────────────────────────── */

/**
 * Feed bytes into mock RX buffer (simulates incoming data from device).
 */
hal_err_t uart_mock_feed_rx(uint8_t port, const uint8_t *data, size_t len)
{
    if (port >= MOCK_MAX_PORTS || data == NULL) { return HAL_ERR_PARAM; }
    size_t space = MOCK_RX_BUF_SIZE - s_state[port].rx_tail;
    if (len > space) { return HAL_ERR_OVERFLOW; }
    memcpy(s_state[port].rx_buf + s_state[port].rx_tail, data, len);
    s_state[port].rx_tail += len;

    /* Trigger RX callback if registered */
    if (s_state[port].rx_cb != NULL) {
        for (size_t i = 0; i < len; i++) {
            s_state[port].rx_cb(data[i], s_state[port].rx_cb_data);
        }
    }
    return HAL_OK;
}

/**
 * Get contents of TX capture buffer (what was transmitted).
 */
const uint8_t *uart_mock_get_tx_data(uint8_t port, size_t *out_len)
{
    if (port >= MOCK_MAX_PORTS) {
        if (out_len) { *out_len = 0; }
        return NULL;
    }
    if (out_len) { *out_len = s_state[port].tx_len; }
    return s_state[port].tx_buf;
}

/**
 * Clear TX capture buffer.
 */
void uart_mock_clear_tx(uint8_t port)
{
    if (port < MOCK_MAX_PORTS) {
        s_state[port].tx_len = 0;
        memset(s_state[port].tx_buf, 0, MOCK_TX_BUF_SIZE);
    }
}

/* ── Ops Table ───────────────────────────────── */

const hal_uart_ops_t uart_mock_ops = {
    .init           = mock_uart_init,
    .transmit       = mock_uart_transmit,
    .receive        = mock_uart_receive,
    .set_rx_callback = mock_uart_set_rx_callback,
    .available      = mock_uart_available,
    .deinit         = mock_uart_deinit,
};
