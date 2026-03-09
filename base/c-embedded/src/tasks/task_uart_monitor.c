/* ──────────────────────────────────────────────
 * task_uart_monitor.c — UART Monitor FreeRTOS Task
 *
 * Pattern: ISR → Queue → Task processing.
 * This demonstrates the correct FreeRTOS pattern for
 * handling interrupt-driven peripheral data in task context.
 * ────────────────────────────────────────────── */

#include "task_uart_monitor.h"
#include "hal_uart.h"
#include "logger.h"
#include <string.h>

/* ── Static allocation ───────────────────────── */
StaticTask_t  g_task_uart_monitor_tcb;
StackType_t   g_task_uart_monitor_stack[TASK_UART_MONITOR_STACK_SIZE];

QueueHandle_t        g_uart_monitor_queue = NULL;
StaticQueue_t        g_uart_monitor_queue_storage;
uart_monitor_msg_t   g_uart_monitor_queue_buf[UART_MONITOR_QUEUE_SIZE];

/* ── Simple command accumulator ──────────────── */
#define CMD_BUF_SIZE (128U)
static uint8_t s_cmd_buf[CMD_BUF_SIZE];
static size_t  s_cmd_len = 0U;

/* ── Command dispatch (customize per project) ── */
static void process_command(const uint8_t *cmd, size_t len)
{
    /* Example: echo command back with prefix */
    LOG_INFO("uart_monitor: cmd[%u]='%.*s'", (unsigned)len, (int)len, cmd);

    /* TODO: Replace with actual command parsing */
    /* e.g., if (strncmp((char*)cmd, "reset", len) == 0) { system_reset(); } */

    hal_uart_print(UART_DEBUG_PORT, "CMD: ");
    hal_uart_transmit(UART_DEBUG_PORT, cmd, len, 100U);
    hal_uart_println(UART_DEBUG_PORT, "");
}

/* ── Init ────────────────────────────────────── */
QueueHandle_t task_uart_monitor_queue_init(void)
{
    g_uart_monitor_queue = xQueueCreateStatic(
        UART_MONITOR_QUEUE_SIZE,
        sizeof(uart_monitor_msg_t),
        (uint8_t *)g_uart_monitor_queue_buf,
        &g_uart_monitor_queue_storage
    );
    return g_uart_monitor_queue;
}

/* ── ISR-safe push ───────────────────────────── */
BaseType_t task_uart_monitor_push_byte_from_isr(uint8_t port, uint8_t byte)
{
    if (g_uart_monitor_queue == NULL) { return pdFAIL; }

    uart_monitor_msg_t msg = {
        .type = UART_MSG_BYTE_RECEIVED,
        .data = byte,
        .port = port,
    };

    BaseType_t higher_prio_woken = pdFALSE;
    BaseType_t result = xQueueSendFromISR(g_uart_monitor_queue, &msg,
                                          &higher_prio_woken);
    /* portYIELD_FROM_ISR(higher_prio_woken); — uncomment for real MCU */
    return result;
}

/* ── Task function ───────────────────────────── */
void task_uart_monitor_run(void *pv_params)
{
    (void)pv_params;

    uart_monitor_msg_t msg;

    LOG_INFO("task_uart_monitor: started");

    for (;;) {
        /* Block indefinitely until a message arrives */
        if (xQueueReceive(g_uart_monitor_queue, &msg, portMAX_DELAY) != pdPASS) {
            continue;
        }

        switch (msg.type) {
        case UART_MSG_BYTE_RECEIVED:
            /* Accumulate into command buffer until CR/LF */
            if (msg.data == '\r' || msg.data == '\n') {
                if (s_cmd_len > 0U) {
                    process_command(s_cmd_buf, s_cmd_len);
                    s_cmd_len = 0U;
                    memset(s_cmd_buf, 0, sizeof(s_cmd_buf));
                }
            } else if (s_cmd_len < (CMD_BUF_SIZE - 1U)) {
                s_cmd_buf[s_cmd_len++] = msg.data;
            } else {
                /* Buffer overflow — discard and reset */
                LOG_WARN("uart_monitor: cmd buffer overflow, discarding");
                s_cmd_len = 0U;
                memset(s_cmd_buf, 0, sizeof(s_cmd_buf));
            }
            break;

        case UART_MSG_TX_REQUEST:
            /* TODO: Handle TX request messages */
            break;

        default:
            LOG_WARN("uart_monitor: unknown msg type %d", (int)msg.type);
            break;
        }
    }
}
