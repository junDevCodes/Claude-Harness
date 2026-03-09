#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/* ──────────────────────────────────────────────
 * app_config.h — Project-wide configuration
 *
 * Customize these values per project.
 * ────────────────────────────────────────────── */

/* ── UART ─────────────────────────────────────── */
#define UART_DEBUG_PORT       (0U)          /* UART port index for debug output */
#define UART_DEBUG_BAUD       (115200U)     /* Baud rate */
#define UART_RX_BUFFER_SIZE   (256U)        /* UART RX ring buffer size (bytes) */
#define UART_TX_BUFFER_SIZE   (256U)        /* UART TX ring buffer size (bytes) */

/* ── GPIO ─────────────────────────────────────── */
#define GPIO_LED_STATUS_PIN   (13U)         /* Status LED pin number */
#define GPIO_MAX_PINS         (64U)         /* Maximum number of GPIO pins supported */

/* ── FreeRTOS Task Stack Sizes (in words) ─────── */
#define TASK_LED_STACK_SIZE         (256U)
#define TASK_UART_MONITOR_STACK_SIZE (512U)

/* ── FreeRTOS Task Priorities ─────────────────── */
#define TASK_LED_PRIORITY           (1U)
#define TASK_UART_MONITOR_PRIORITY  (2U)

/* ── FreeRTOS Queue Sizes ─────────────────────── */
#define UART_MONITOR_QUEUE_SIZE     (16U)   /* Number of messages in UART monitor queue */

/* ── Ring Buffer ──────────────────────────────── */
#define RING_BUFFER_DEFAULT_SIZE    (128U)  /* Default ring buffer capacity (bytes) */

/* ── Logger ───────────────────────────────────── */
#define LOGGER_ENABLED              (1)     /* 0 = disable all log output */
#define LOGGER_LEVEL_ERROR          (0)
#define LOGGER_LEVEL_WARN           (1)
#define LOGGER_LEVEL_INFO           (2)
#define LOGGER_LEVEL_DEBUG          (3)
#define LOGGER_CURRENT_LEVEL        LOGGER_LEVEL_DEBUG

/* ── Timing ───────────────────────────────────── */
#define LED_BLINK_PERIOD_MS         (500U)  /* LED blink interval in milliseconds */

#endif /* APP_CONFIG_H */
