/* ──────────────────────────────────────────────
 * uart_stm32.c — STM32 UART Driver (Reference Stub)
 *
 * HOW TO USE:
 *   1. Include your STM32 HAL headers
 *   2. Replace TODOs with actual STM32 UART HAL calls
 *   3. Register at startup: hal_uart_register(0, &uart_stm32_ops)
 * ────────────────────────────────────────────── */

#include "hal_uart.h"

/* TODO: Include STM32 HAL headers */
/* #include "stm32f4xx_hal.h" */

/* ── UART Handle Map ─────────────────────────── */
/*
 * Example:
 * static UART_HandleTypeDef *s_huart[HAL_UART_MAX_PORTS] = {
 *     [0] = &huart2,  // UART2 = USB-Serial on Nucleo
 * };
 */

/* ── Implementation ──────────────────────────── */

static hal_err_t stm32_uart_init(uint8_t port, const hal_uart_config_t *config)
{
    (void)port;
    (void)config;
    /*
     * TODO:
     * UART_HandleTypeDef *h = s_huart[port];
     * h->Instance          = USARTx;
     * h->Init.BaudRate     = config->baud_rate;
     * h->Init.WordLength   = UART_WORDLENGTH_8B;
     * h->Init.StopBits     = UART_STOPBITS_1;
     * h->Init.Parity       = UART_PARITY_NONE;
     * h->Init.Mode         = UART_MODE_TX_RX;
     * h->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
     * HAL_UART_Init(h);
     */
    return HAL_OK;
}

static hal_err_t stm32_uart_transmit(uint8_t port, const uint8_t *data,
                                     size_t len, uint32_t timeout_ms)
{
    (void)port;
    (void)data;
    (void)len;
    (void)timeout_ms;
    /*
     * TODO:
     * HAL_StatusTypeDef st =
     *     HAL_UART_Transmit(s_huart[port], (uint8_t *)data, len, timeout_ms);
     * return (st == HAL_OK) ? HAL_OK : HAL_ERR_GENERIC;
     */
    return HAL_OK;
}

static hal_err_t stm32_uart_receive(uint8_t port, uint8_t *buf,
                                    size_t len, uint32_t timeout_ms)
{
    (void)port;
    (void)buf;
    (void)len;
    (void)timeout_ms;
    /*
     * TODO:
     * HAL_StatusTypeDef st =
     *     HAL_UART_Receive(s_huart[port], buf, len, timeout_ms);
     * return (st == HAL_OK) ? HAL_OK : HAL_ERR_TIMEOUT;
     */
    return HAL_OK;
}

static hal_err_t stm32_uart_set_rx_callback(uint8_t port,
                                             hal_uart_rx_callback_t cb,
                                             void *user_data)
{
    (void)port;
    (void)cb;
    (void)user_data;
    /*
     * TODO: Enable UART RX interrupt, store callback,
     * call it from HAL_UART_RxCpltCallback or USART IRQHandler.
     */
    return HAL_OK;
}

static int32_t stm32_uart_available(uint8_t port)
{
    (void)port;
    /* TODO: Return number of bytes in RX FIFO/buffer */
    return 0;
}

static void stm32_uart_deinit(uint8_t port)
{
    (void)port;
    /*
     * TODO:
     * HAL_UART_DeInit(s_huart[port]);
     */
}

/* ── Ops Table ───────────────────────────────── */
const hal_uart_ops_t uart_stm32_ops = {
    .init            = stm32_uart_init,
    .transmit        = stm32_uart_transmit,
    .receive         = stm32_uart_receive,
    .set_rx_callback = stm32_uart_set_rx_callback,
    .available       = stm32_uart_available,
    .deinit          = stm32_uart_deinit,
};
