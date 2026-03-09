#ifndef FREERTOS_TYPES_H
#define FREERTOS_TYPES_H

/* ──────────────────────────────────────────────
 * freertos_types.h — FreeRTOS Type Definitions
 *
 * Two modes:
 *   FREERTOS_MOCK  → Minimal stub types for host compilation/testing
 *   (default)      → Include real FreeRTOS headers for MCU build
 *
 * In real MCU project:
 *   - Remove FREERTOS_MOCK define
 *   - Add FreeRTOS source to build (port for your MCU)
 *   - Include FreeRTOSConfig.h in your project
 * ────────────────────────────────────────────── */

#ifdef FREERTOS_MOCK
/* ── Mock FreeRTOS types for host build ─────── */

#include <stdint.h>
#include <stddef.h>

typedef uint32_t       TickType_t;
typedef uint32_t       StackType_t;
typedef long           BaseType_t;
typedef unsigned long  UBaseType_t;
typedef void          *TaskHandle_t;
typedef void          *QueueHandle_t;
typedef void          *SemaphoreHandle_t;
typedef void          *TimerHandle_t;

typedef struct {
    uint8_t  dummy[128];   /* placeholder for StaticTask_t */
} StaticTask_t;

typedef struct {
    uint8_t  dummy[64];    /* placeholder for StaticQueue_t */
} StaticQueue_t;

typedef struct {
    uint8_t  dummy[32];    /* placeholder for StaticSemaphore_t */
} StaticSemaphore_t;

#define pdTRUE          ((BaseType_t)1)
#define pdFALSE         ((BaseType_t)0)
#define pdPASS          pdTRUE
#define pdFAIL          pdFALSE
#define errQUEUE_FULL   ((BaseType_t)0)

#define portMAX_DELAY   ((TickType_t)0xFFFFFFFFUL)
#define portTICK_PERIOD_MS  (1U)
#define pdMS_TO_TICKS(ms)   ((TickType_t)(ms))

/* Task / Queue / Semaphore stubs */
static inline void vTaskDelay(TickType_t ticks) { (void)ticks; }
static inline TickType_t xTaskGetTickCount(void) { return 0U; }
static inline void vTaskSuspend(TaskHandle_t h) { (void)h; }
static inline void vTaskResume(TaskHandle_t h) { (void)h; }

static inline TaskHandle_t xTaskCreateStatic(
    void (*func)(void *), const char *name, uint32_t stack_depth,
    void *params, UBaseType_t priority,
    StackType_t *stack, StaticTask_t *tcb)
    { (void)func; (void)name; (void)stack_depth; (void)params;
      (void)priority; (void)stack; (void)tcb; return (TaskHandle_t)tcb; }

static inline QueueHandle_t xQueueCreateStatic(
    UBaseType_t len, UBaseType_t item_size,
    uint8_t *buf, StaticQueue_t *storage)
    { (void)len; (void)item_size; (void)buf; return (QueueHandle_t)storage; }

static inline BaseType_t xQueueSend(QueueHandle_t q, const void *item, TickType_t ticks)
    { (void)q; (void)item; (void)ticks; return pdPASS; }
static inline BaseType_t xQueueReceive(QueueHandle_t q, void *buf, TickType_t ticks)
    { (void)q; (void)buf; (void)ticks; return pdFAIL; }
static inline BaseType_t xQueueSendFromISR(QueueHandle_t q, const void *item, BaseType_t *woken)
    { (void)q; (void)item; (void)woken; return pdPASS; }

static inline BaseType_t xSemaphoreTake(SemaphoreHandle_t s, TickType_t ticks)
    { (void)s; (void)ticks; return pdPASS; }
static inline BaseType_t xSemaphoreGive(SemaphoreHandle_t s)
    { (void)s; return pdPASS; }

#else
/* ── Real FreeRTOS headers ───────────────────── */
/*
 * Add FreeRTOS source to your build system:
 *   - FreeRTOS/Source/tasks.c
 *   - FreeRTOS/Source/queue.c
 *   - FreeRTOS/Source/list.c
 *   - FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c  (or your MCU port)
 *   - FreeRTOS/Source/portable/MemMang/heap_4.c
 *
 * Then include:
 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

#endif /* FREERTOS_MOCK */

#endif /* FREERTOS_TYPES_H */
