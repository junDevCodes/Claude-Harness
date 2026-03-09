/* ──────────────────────────────────────────────
 * logger.c — Debug Logger Implementation
 * ────────────────────────────────────────────── */

#include "logger.h"

#if LOGGER_ENABLED

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define LOG_BUF_SIZE  (256U)

static uint8_t s_uart_port = UART_DEBUG_PORT;
static char    s_log_buf[LOG_BUF_SIZE];

void logger_init(uint8_t uart_port)
{
    s_uart_port = uart_port;
}

void logger_write(const char *level, const char *fmt, ...)
{
    va_list args;
    int     n;

    /* Format: "[LEVEL] message\r\n" */
    n = snprintf(s_log_buf, sizeof(s_log_buf), "[%s] ", level);
    if (n < 0 || (size_t)n >= sizeof(s_log_buf)) { return; }

    va_start(args, fmt);
    int m = vsnprintf(s_log_buf + n, sizeof(s_log_buf) - (size_t)n, fmt, args);
    va_end(args);

    if (m < 0) { return; }

    size_t total = (size_t)n + (size_t)m;
    if (total + 2U < sizeof(s_log_buf)) {
        s_log_buf[total]     = '\r';
        s_log_buf[total + 1] = '\n';
        total += 2U;
    }

    hal_uart_transmit(s_uart_port, (const uint8_t *)s_log_buf, total, 100U);
}

#endif /* LOGGER_ENABLED */
