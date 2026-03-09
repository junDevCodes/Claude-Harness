#ifndef LOGGER_H
#define LOGGER_H

/* ──────────────────────────────────────────────
 * logger.h — Debug Logger via UART
 *
 * Macros: LOG_ERROR / LOG_WARN / LOG_INFO / LOG_DEBUG
 * All disabled at compile time if LOGGER_ENABLED == 0.
 *
 * Output format: [LEVEL] message\r\n
 * ────────────────────────────────────────────── */

#include "app_config.h"
#include "hal_uart.h"

#if LOGGER_ENABLED

void logger_init(uint8_t uart_port);
void logger_write(const char *level, const char *fmt, ...);

#define LOG_ERROR(fmt, ...) \
    logger_write("ERROR", fmt, ##__VA_ARGS__)

#define LOG_WARN(fmt, ...) \
    do { if (LOGGER_CURRENT_LEVEL >= LOGGER_LEVEL_WARN) \
             logger_write("WARN ", fmt, ##__VA_ARGS__); } while (0)

#define LOG_INFO(fmt, ...) \
    do { if (LOGGER_CURRENT_LEVEL >= LOGGER_LEVEL_INFO) \
             logger_write("INFO ", fmt, ##__VA_ARGS__); } while (0)

#define LOG_DEBUG(fmt, ...) \
    do { if (LOGGER_CURRENT_LEVEL >= LOGGER_LEVEL_DEBUG) \
             logger_write("DEBUG", fmt, ##__VA_ARGS__); } while (0)

#else /* LOGGER_ENABLED == 0 */

#define logger_init(port)          ((void)0)
#define LOG_ERROR(fmt, ...)        ((void)0)
#define LOG_WARN(fmt, ...)         ((void)0)
#define LOG_INFO(fmt, ...)         ((void)0)
#define LOG_DEBUG(fmt, ...)        ((void)0)

#endif /* LOGGER_ENABLED */

#endif /* LOGGER_H */
