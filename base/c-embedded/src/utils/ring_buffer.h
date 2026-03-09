#ifndef RING_BUFFER_H
#define RING_BUFFER_H

/* ──────────────────────────────────────────────
 * ring_buffer.h — Static Ring Buffer (FIFO)
 *
 * Features:
 *   - No dynamic memory allocation (static arrays)
 *   - Power-of-2 size for efficient masking
 *   - Thread-safe for single-producer / single-consumer
 *     (ISR writes, task reads — common embedded pattern)
 *   - Byte granularity
 *
 * Usage:
 *   static uint8_t   buf_storage[256];
 *   static ring_buf_t rb;
 *   ring_buf_init(&rb, buf_storage, sizeof(buf_storage));
 *   ring_buf_write(&rb, data, len);
 *   ring_buf_read(&rb, out, len);
 * ────────────────────────────────────────────── */

#include <stdint.h>
#include <stddef.h>
#include "hal_types.h"

typedef struct {
    uint8_t  *buf;       /* Pointer to backing storage */
    size_t    capacity;  /* Must be power of 2 */
    size_t    mask;      /* capacity - 1 */
    volatile size_t head;   /* Read index  (consumer) */
    volatile size_t tail;   /* Write index (producer) */
} ring_buf_t;

/**
 * Initialize ring buffer with caller-provided storage.
 * @param rb       Ring buffer instance
 * @param storage  Backing array (must remain valid for lifetime of rb)
 * @param size     Size of storage in bytes (MUST be power of 2)
 * @return HAL_OK or HAL_ERR_PARAM if size is not power of 2
 */
hal_err_t ring_buf_init(ring_buf_t *rb, uint8_t *storage, size_t size);

/**
 * Write bytes to the ring buffer.
 * Partial writes are NOT performed — returns HAL_ERR_OVERFLOW if full.
 *
 * @param rb   Ring buffer
 * @param data Source data
 * @param len  Number of bytes
 * @return HAL_OK or HAL_ERR_OVERFLOW if insufficient space
 */
hal_err_t ring_buf_write(ring_buf_t *rb, const uint8_t *data, size_t len);

/**
 * Write a single byte. ISR-safe (single producer).
 */
hal_err_t ring_buf_write_byte(ring_buf_t *rb, uint8_t byte);

/**
 * Read bytes from the ring buffer.
 * @param rb   Ring buffer
 * @param buf  Destination buffer
 * @param len  Number of bytes to read
 * @return HAL_OK or HAL_ERR_TIMEOUT if insufficient data
 */
hal_err_t ring_buf_read(ring_buf_t *rb, uint8_t *buf, size_t len);

/**
 * Read a single byte.
 */
hal_err_t ring_buf_read_byte(ring_buf_t *rb, uint8_t *out);

/**
 * Number of bytes available to read.
 */
size_t ring_buf_available(const ring_buf_t *rb);

/**
 * Free space available for writing.
 */
size_t ring_buf_free_space(const ring_buf_t *rb);

/**
 * Return 1 if buffer is empty, 0 otherwise.
 */
int ring_buf_is_empty(const ring_buf_t *rb);

/**
 * Return 1 if buffer is full, 0 otherwise.
 */
int ring_buf_is_full(const ring_buf_t *rb);

/**
 * Reset buffer (discard all data).
 */
void ring_buf_reset(ring_buf_t *rb);

#endif /* RING_BUFFER_H */
