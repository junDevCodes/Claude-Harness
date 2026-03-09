/* ──────────────────────────────────────────────
 * test_ring_buffer.c — Ring Buffer Unit Tests
 *
 * Tests:
 *   - Init, write, read
 *   - Boundary conditions (full, empty, overflow)
 *   - Single-byte read/write, wrap-around
 * ────────────────────────────────────────────── */

#include "unity.h"
#include "ring_buffer.h"
#include <string.h>

/* ── Test fixtures ───────────────────────────── */
#define TEST_BUF_SIZE  (16U)

static ring_buf_t s_rb;
static uint8_t    s_storage[TEST_BUF_SIZE];

void setUp(void)
{
    memset(s_storage, 0, sizeof(s_storage));
    ring_buf_init(&s_rb, s_storage, TEST_BUF_SIZE);
}

void tearDown(void)
{
    /* nothing */
}

/* ── Tests ───────────────────────────────────── */

void test_ring_buf_init_ok(void)
{
    ring_buf_t rb;
    uint8_t    buf[8];
    hal_err_t  err = ring_buf_init(&rb, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(HAL_OK, (int)err);
    TEST_ASSERT_TRUE(ring_buf_is_empty(&rb));
}

void test_ring_buf_init_non_power_of_two_returns_error(void)
{
    ring_buf_t rb;
    uint8_t    buf[10];
    hal_err_t  err = ring_buf_init(&rb, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(HAL_ERR_PARAM, (int)err);
}

void test_ring_buf_empty_initially(void)
{
    TEST_ASSERT_TRUE(ring_buf_is_empty(&s_rb));
    TEST_ASSERT_EQUAL_UINT(0U, ring_buf_available(&s_rb));
}

void test_ring_buf_write_byte_increases_available(void)
{
    ring_buf_write_byte(&s_rb, 0xABU);
    TEST_ASSERT_EQUAL_UINT(1U, ring_buf_available(&s_rb));
    TEST_ASSERT_FALSE(ring_buf_is_empty(&s_rb));
}

void test_ring_buf_read_byte_decreases_available(void)
{
    ring_buf_write_byte(&s_rb, 0x42U);
    uint8_t out = 0U;
    ring_buf_read_byte(&s_rb, &out);
    TEST_ASSERT_EQUAL_UINT(0U, ring_buf_available(&s_rb));
    TEST_ASSERT_EQUAL_UINT(0x42U, out);
}

void test_ring_buf_read_byte_correct_value(void)
{
    ring_buf_write_byte(&s_rb, 0xDEU);
    uint8_t out = 0U;
    ring_buf_read_byte(&s_rb, &out);
    TEST_ASSERT_EQUAL_UINT(0xDEU, out);
}

void test_ring_buf_fifo_order(void)
{
    ring_buf_write_byte(&s_rb, 1U);
    ring_buf_write_byte(&s_rb, 2U);
    ring_buf_write_byte(&s_rb, 3U);

    uint8_t a, b, c;
    ring_buf_read_byte(&s_rb, &a);
    ring_buf_read_byte(&s_rb, &b);
    ring_buf_read_byte(&s_rb, &c);

    TEST_ASSERT_EQUAL_UINT(1U, a);
    TEST_ASSERT_EQUAL_UINT(2U, b);
    TEST_ASSERT_EQUAL_UINT(3U, c);
}

void test_ring_buf_full_when_capacity_reached(void)
{
    for (uint8_t i = 0U; i < TEST_BUF_SIZE; i++) {
        hal_err_t err = ring_buf_write_byte(&s_rb, i);
        TEST_ASSERT_EQUAL_INT(HAL_OK, (int)err);
    }
    TEST_ASSERT_TRUE(ring_buf_is_full(&s_rb));
}

void test_ring_buf_overflow_returns_error(void)
{
    for (uint8_t i = 0U; i < TEST_BUF_SIZE; i++) {
        ring_buf_write_byte(&s_rb, i);
    }
    hal_err_t err = ring_buf_write_byte(&s_rb, 0xFFU);
    TEST_ASSERT_EQUAL_INT(HAL_ERR_OVERFLOW, (int)err);
}

void test_ring_buf_read_empty_returns_error(void)
{
    uint8_t out = 0U;
    hal_err_t err = ring_buf_read_byte(&s_rb, &out);
    TEST_ASSERT_EQUAL_INT(HAL_ERR_TIMEOUT, (int)err);
}

void test_ring_buf_write_read_block(void)
{
    const uint8_t data[] = { 0x11U, 0x22U, 0x33U, 0x44U };
    uint8_t       out[4] = { 0U };

    hal_err_t err = ring_buf_write(&s_rb, data, sizeof(data));
    TEST_ASSERT_EQUAL_INT(HAL_OK, (int)err);
    TEST_ASSERT_EQUAL_UINT(4U, ring_buf_available(&s_rb));

    err = ring_buf_read(&s_rb, out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(HAL_OK, (int)err);
    TEST_ASSERT_EQUAL_MEMORY(data, out, sizeof(data));
}

void test_ring_buf_wrap_around(void)
{
    for (uint8_t i = 0U; i < TEST_BUF_SIZE / 2U; i++) {
        ring_buf_write_byte(&s_rb, i);
    }
    for (uint8_t i = 0U; i < TEST_BUF_SIZE / 2U; i++) {
        uint8_t out;
        ring_buf_read_byte(&s_rb, &out);
        TEST_ASSERT_EQUAL_UINT(i, out);
    }
    for (uint8_t i = 0U; i < TEST_BUF_SIZE / 2U; i++) {
        hal_err_t err = ring_buf_write_byte(&s_rb, (uint8_t)(i + 10U));
        TEST_ASSERT_EQUAL_INT(HAL_OK, (int)err);
    }
    for (uint8_t i = 0U; i < TEST_BUF_SIZE / 2U; i++) {
        uint8_t out;
        ring_buf_read_byte(&s_rb, &out);
        TEST_ASSERT_EQUAL_UINT((uint8_t)(i + 10U), out);
    }
    TEST_ASSERT_TRUE(ring_buf_is_empty(&s_rb));
}

void test_ring_buf_reset_clears_state(void)
{
    ring_buf_write_byte(&s_rb, 0x55U);
    ring_buf_write_byte(&s_rb, 0xAAU);
    ring_buf_reset(&s_rb);
    TEST_ASSERT_TRUE(ring_buf_is_empty(&s_rb));
    TEST_ASSERT_EQUAL_UINT(0U, ring_buf_available(&s_rb));
}

void test_ring_buf_free_space_decreases_on_write(void)
{
    size_t initial = ring_buf_free_space(&s_rb);
    TEST_ASSERT_EQUAL_UINT(TEST_BUF_SIZE, initial);

    ring_buf_write_byte(&s_rb, 0x01U);
    TEST_ASSERT_EQUAL_UINT(TEST_BUF_SIZE - 1U, ring_buf_free_space(&s_rb));
}

/* ── Main (separate executable) ─────────────── */
int main(void)
{
    UnityBegin("test_ring_buffer.c");

    RUN_TEST(test_ring_buf_init_ok);
    RUN_TEST(test_ring_buf_init_non_power_of_two_returns_error);
    RUN_TEST(test_ring_buf_empty_initially);
    RUN_TEST(test_ring_buf_write_byte_increases_available);
    RUN_TEST(test_ring_buf_read_byte_decreases_available);
    RUN_TEST(test_ring_buf_read_byte_correct_value);
    RUN_TEST(test_ring_buf_fifo_order);
    RUN_TEST(test_ring_buf_full_when_capacity_reached);
    RUN_TEST(test_ring_buf_overflow_returns_error);
    RUN_TEST(test_ring_buf_read_empty_returns_error);
    RUN_TEST(test_ring_buf_write_read_block);
    RUN_TEST(test_ring_buf_wrap_around);
    RUN_TEST(test_ring_buf_reset_clears_state);
    RUN_TEST(test_ring_buf_free_space_decreases_on_write);

    return UnityEnd();
}
