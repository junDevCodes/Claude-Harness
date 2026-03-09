#ifndef TASK_UART_MONITOR_H
#define TASK_UART_MONITOR_H

/* ──────────────────────────────────────────────
 * task_uart_monitor.h — UART Monitor FreeRTOS Task
 *
 * Pattern: Queue-based inter-task communication.
 * Demonstrates:
 *   - Receiving messages from a Queue (blocking receive)
 *   - ISR → Task handoff via xQueueSendFromISR
 *   - Static Queue allocation
 *   - Processing incoming UART bytes in task context
 * ────────────────────────────────────────────── */

#include <stdint.h>
#include <stddef.h>
#include "freertos_types.h"
#include "app_config.h"

/* ── Message types ───────────────────────────── */
typedef enum {
    UART_MSG_BYTE_RECEIVED = 0,
    UART_MSG_TX_REQUEST    = 1,
} uart_msg_type_t;

typedef struct {
    uart_msg_type_t type;
    uint8_t         data;       /* Single byte for BYTE_RECEIVED */
    uint8_t         port;       /* UART port index */
} uart_monitor_msg_t;

/* Static allocation storage */
extern StaticTask_t  g_task_uart_monitor_tcb;
extern StackType_t   g_task_uart_monitor_stack[TASK_UART_MONITOR_STACK_SIZE];

/* Queue handle (shared with ISR) */
extern QueueHandle_t g_uart_monitor_queue;

/* Static queue storage */
extern StaticQueue_t  g_uart_monitor_queue_storage;
extern uart_monitor_msg_t g_uart_monitor_queue_buf[UART_MONITOR_QUEUE_SIZE];

/**
 * Initialize queue and return handle.
 * Call once before starting the scheduler.
 */
QueueHandle_t task_uart_monitor_queue_init(void);

/**
 * FreeRTOS task function.
 * Blocks on queue, processes each UART byte.
 *
 * Usage:
 *   task_uart_monitor_queue_init();
 *   xTaskCreateStatic(task_uart_monitor_run, "UART_MON",
 *                     TASK_UART_MONITOR_STACK_SIZE, NULL,
 *                     TASK_UART_MONITOR_PRIORITY,
 *                     g_task_uart_monitor_stack,
 *                     &g_task_uart_monitor_tcb);
 */
void task_uart_monitor_run(void *pv_params);

/**
 * ISR-safe: Push a received byte to the queue.
 * Call from UART RX ISR or HAL callback.
 *
 * @param port UART port index
 * @param byte Received byte
 * @return pdTRUE if queued successfully
 */
BaseType_t task_uart_monitor_push_byte_from_isr(uint8_t port, uint8_t byte);

#endif /* TASK_UART_MONITOR_H */
