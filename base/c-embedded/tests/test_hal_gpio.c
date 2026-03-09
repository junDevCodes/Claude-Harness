/* ──────────────────────────────────────────────
 * test_hal_gpio.c — GPIO HAL Unit Tests
 *
 * Tests:
 *   - HAL dispatcher before/after driver registration
 *   - GPIO init, write, read, toggle via mock driver
 *   - Error handling (NULL ops, invalid pin, etc.)
 * ────────────────────────────────────────────── */

#include "unity.h"
#include "hal_gpio.h"
#include "gpio_mock.h"
#include "app_config.h"

/* ── setUp / tearDown ────────────────────────── */
void setUp(void)
{
    hal_gpio_register(&gpio_mock_ops);
}

void tearDown(void)
{
    hal_gpio_deinit(0U);
    hal_gpio_deinit(GPIO_LED_STATUS_PIN);
}

/* ── Tests ───────────────────────────────────── */

void test_gpio_register_null_returns_error(void)
{
    hal_err_t err = hal_gpio_register(NULL);
    TEST_ASSERT_EQUAL_INT(HAL_ERR_PARAM, (int)err);
}

void test_gpio_register_valid_ops_returns_ok(void)
{
    hal_err_t err = hal_gpio_register(&gpio_mock_ops);
    TEST_ASSERT_EQUAL_INT(HAL_OK, (int)err);
}

void test_gpio_init_output_returns_ok(void)
{
    hal_err_t err = hal_gpio_init(GPIO_LED_STATUS_PIN, HAL_GPIO_DIR_OUTPUT);
    TEST_ASSERT_EQUAL_INT(HAL_OK, (int)err);
}

void test_gpio_write_high_returns_ok(void)
{
    hal_gpio_init(GPIO_LED_STATUS_PIN, HAL_GPIO_DIR_OUTPUT);
    hal_err_t err = hal_gpio_write(GPIO_LED_STATUS_PIN, HAL_GPIO_LEVEL_HIGH);
    TEST_ASSERT_EQUAL_INT(HAL_OK, (int)err);
}

void test_gpio_write_high_sets_level(void)
{
    hal_gpio_init(GPIO_LED_STATUS_PIN, HAL_GPIO_DIR_OUTPUT);
    hal_gpio_write(GPIO_LED_STATUS_PIN, HAL_GPIO_LEVEL_HIGH);

    hal_gpio_level_t level;
    hal_gpio_read(GPIO_LED_STATUS_PIN, &level);
    TEST_ASSERT_EQUAL_INT(HAL_GPIO_LEVEL_HIGH, (int)level);
}

void test_gpio_write_low_sets_level(void)
{
    hal_gpio_init(GPIO_LED_STATUS_PIN, HAL_GPIO_DIR_OUTPUT);
    hal_gpio_write(GPIO_LED_STATUS_PIN, HAL_GPIO_LEVEL_HIGH);
    hal_gpio_write(GPIO_LED_STATUS_PIN, HAL_GPIO_LEVEL_LOW);

    hal_gpio_level_t level;
    hal_gpio_read(GPIO_LED_STATUS_PIN, &level);
    TEST_ASSERT_EQUAL_INT(HAL_GPIO_LEVEL_LOW, (int)level);
}

void test_gpio_toggle_changes_level(void)
{
    hal_gpio_init(GPIO_LED_STATUS_PIN, HAL_GPIO_DIR_OUTPUT);
    hal_gpio_write(GPIO_LED_STATUS_PIN, HAL_GPIO_LEVEL_LOW);
    hal_gpio_toggle(GPIO_LED_STATUS_PIN);

    hal_gpio_level_t level;
    hal_gpio_read(GPIO_LED_STATUS_PIN, &level);
    TEST_ASSERT_EQUAL_INT(HAL_GPIO_LEVEL_HIGH, (int)level);
}

void test_gpio_toggle_twice_restores_level(void)
{
    hal_gpio_init(GPIO_LED_STATUS_PIN, HAL_GPIO_DIR_OUTPUT);
    hal_gpio_write(GPIO_LED_STATUS_PIN, HAL_GPIO_LEVEL_HIGH);
    hal_gpio_toggle(GPIO_LED_STATUS_PIN);
    hal_gpio_toggle(GPIO_LED_STATUS_PIN);

    hal_gpio_level_t level;
    hal_gpio_read(GPIO_LED_STATUS_PIN, &level);
    TEST_ASSERT_EQUAL_INT(HAL_GPIO_LEVEL_HIGH, (int)level);
}

void test_gpio_read_null_buffer_returns_error(void)
{
    hal_gpio_init(GPIO_LED_STATUS_PIN, HAL_GPIO_DIR_OUTPUT);
    hal_err_t err = hal_gpio_read(GPIO_LED_STATUS_PIN, NULL);
    TEST_ASSERT_EQUAL_INT(HAL_ERR_PARAM, (int)err);
}

void test_gpio_mock_set_input_level(void)
{
    hal_gpio_init(0U, HAL_GPIO_DIR_INPUT);
    gpio_mock_set_input_level(0U, HAL_GPIO_LEVEL_HIGH);

    hal_gpio_level_t level;
    hal_err_t err = hal_gpio_read(0U, &level);
    TEST_ASSERT_EQUAL_INT(HAL_OK, (int)err);
    TEST_ASSERT_EQUAL_INT(HAL_GPIO_LEVEL_HIGH, (int)level);
}

void test_gpio_write_to_input_pin_returns_error(void)
{
    hal_gpio_init(0U, HAL_GPIO_DIR_INPUT);
    hal_err_t err = hal_gpio_write(0U, HAL_GPIO_LEVEL_HIGH);
    TEST_ASSERT_EQUAL_INT(HAL_ERR_PARAM, (int)err);
}

/* ── Main (separate executable) ─────────────── */
int main(void)
{
    UnityBegin("test_hal_gpio.c");

    RUN_TEST(test_gpio_register_null_returns_error);
    RUN_TEST(test_gpio_register_valid_ops_returns_ok);
    RUN_TEST(test_gpio_init_output_returns_ok);
    RUN_TEST(test_gpio_write_high_returns_ok);
    RUN_TEST(test_gpio_write_high_sets_level);
    RUN_TEST(test_gpio_write_low_sets_level);
    RUN_TEST(test_gpio_toggle_changes_level);
    RUN_TEST(test_gpio_toggle_twice_restores_level);
    RUN_TEST(test_gpio_read_null_buffer_returns_error);
    RUN_TEST(test_gpio_mock_set_input_level);
    RUN_TEST(test_gpio_write_to_input_pin_returns_error);

    return UnityEnd();
}
