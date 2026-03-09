/* ──────────────────────────────────────────────
 * ring_buffer.c — Static Ring Buffer Implementation
 *
 * Assumes single-producer / single-consumer model:
 *   - ISR (or one task) writes via ring_buf_write_byte
 *   - Another task reads via ring_buf_read / ring_buf_read_byte
 * No mutex needed in this model (volatile + index ordering).
 * ────────────────────────────────────────────── */

#include "ring_buffer.h"
#include <string.h>

/* Check if n is a power of 2 */
static int is_power_of_two(size_t n)
{
    return (n > 0U) && ((n & (n - 1U)) == 0U);
}

hal_err_t ring_buf_init(ring_buf_t *rb, uint8_t *storage, size_t size)
{
    if (rb == NULL || storage == NULL) { return HAL_ERR_PARAM; }
    if (!is_power_of_two(size))        { return HAL_ERR_PARAM; }

    rb->buf      = storage;
    rb->capacity = size;
    rb->mask     = size - 1U;
    rb->head     = 0U;
    rb->tail     = 0U;
    memset(storage, 0, size);
    return HAL_OK;
}

size_t ring_buf_available(const ring_buf_t *rb)
{
    return rb->tail - rb->head;  /* unsigned wrap-around safe */
}

size_t ring_buf_free_space(const ring_buf_t *rb)
{
    return rb->capacity - ring_buf_available(rb);
}

int ring_buf_is_empty(const ring_buf_t *rb)
{
    return ring_buf_available(rb) == 0U;
}

int ring_buf_is_full(const ring_buf_t *rb)
{
    return ring_buf_available(rb) >= rb->capacity;
}

void ring_buf_reset(ring_buf_t *rb)
{
    rb->head = 0U;
    rb->tail = 0U;
}

hal_err_t ring_buf_write_byte(ring_buf_t *rb, uint8_t byte)
{
    if (rb == NULL)           { return HAL_ERR_PARAM; }
    if (ring_buf_is_full(rb)) { return HAL_ERR_OVERFLOW; }

    rb->buf[rb->tail & rb->mask] = byte;
    rb->tail++;          /* Increment AFTER write — visibility to consumer */
    return HAL_OK;
}

hal_err_t ring_buf_write(ring_buf_t *rb, const uint8_t *data, size_t len)
{
    if (rb == NULL || data == NULL) { return HAL_ERR_PARAM; }
    if (ring_buf_free_space(rb) < len) { return HAL_ERR_OVERFLOW; }

    for (size_t i = 0U; i < len; i++) {
        rb->buf[rb->tail & rb->mask] = data[i];
        rb->tail++;
    }
    return HAL_OK;
}

hal_err_t ring_buf_read_byte(ring_buf_t *rb, uint8_t *out)
{
    if (rb == NULL || out == NULL)  { return HAL_ERR_PARAM; }
    if (ring_buf_is_empty(rb))      { return HAL_ERR_TIMEOUT; }

    *out = rb->buf[rb->head & rb->mask];
    rb->head++;          /* Increment AFTER read */
    return HAL_OK;
}

hal_err_t ring_buf_read(ring_buf_t *rb, uint8_t *buf, size_t len)
{
    if (rb == NULL || buf == NULL)           { return HAL_ERR_PARAM; }
    if (ring_buf_available(rb) < len)        { return HAL_ERR_TIMEOUT; }

    for (size_t i = 0U; i < len; i++) {
        buf[i] = rb->buf[rb->head & rb->mask];
        rb->head++;
    }
    return HAL_OK;
}
