/* ──────────────────────────────────────────────
 * run_tests.c — Test Runner Entry Point
 *
 * Runs all test suites and exits with failure count.
 * ────────────────────────────────────────────── */

#include <stdio.h>

/* Declarations from each test file */
int test_hal_gpio_run_all(void);
int test_ring_buffer_run_all(void);

int main(void)
{
    int failures = 0;

    printf("========================================\n");
    printf(" c-embedded base — Unit Test Suite\n");
    printf("========================================\n\n");

    printf("--- GPIO HAL Tests ---\n");
    failures += test_hal_gpio_run_all();

    printf("\n--- Ring Buffer Tests ---\n");
    failures += test_ring_buffer_run_all();

    printf("\n========================================\n");
    if (failures == 0) {
        printf(" ALL TESTS PASSED\n");
    } else {
        printf(" %d TEST(S) FAILED\n", failures);
    }
    printf("========================================\n");

    return failures;
}
