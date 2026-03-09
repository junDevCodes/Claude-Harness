#ifndef UART_MOCK_H
#define UART_MOCK_H

#include "hal_uart.h"

/* Mock driver ops table — pass to hal_uart_register(port, &uart_mock_ops) */
extern const hal_uart_ops_t uart_mock_ops;

/* Test helpers */
hal_err_t      uart_mock_feed_rx(uint8_t port, const uint8_t *data, size_t len);
const uint8_t *uart_mock_get_tx_data(uint8_t port, size_t *out_len);
void           uart_mock_clear_tx(uint8_t port);

#endif /* UART_MOCK_H */
