#ifndef UNITY_H
#define UNITY_H

/* ──────────────────────────────────────────────
 * unity.h — Unity Test Framework Public API
 * Based on Unity v2.5.2 (MIT License)
 * https://github.com/ThrowTheSwitch/Unity
 *
 * IMPORTANT: Replace with official Unity for production.
 * This is a compatible stub for demonstration.
 * ────────────────────────────────────────────── */

#include <stdint.h>
#include <stddef.h>
#include "unity_internals.h"

/* ── Test lifecycle ──────────────────────────── */
void UnityBegin(const char *filename);
int  UnityEnd(void);
void setUp(void);     /* User-defined: called before each test */
void tearDown(void);  /* User-defined: called after each test  */

/* ── Test runner macro ───────────────────────── */
#define RUN_TEST(func) \
    UnityDefaultTestRun(func, #func, __LINE__)

void UnityDefaultTestRun(void (*func)(void), const char *name, uint32_t line);

/* ── Assertion macros ────────────────────────── */

#define TEST_FAIL_MESSAGE(msg) \
    UnityFail((msg), __LINE__)

#define TEST_FAIL() \
    UnityFail(NULL, __LINE__)

#define TEST_IGNORE_MESSAGE(msg) \
    UnityIgnore((msg), __LINE__)

#define TEST_IGNORE() \
    UnityIgnore(NULL, __LINE__)

/* --- Integer equality --- */
#define TEST_ASSERT_EQUAL_INT(expected, actual) \
    UnityAssertEqualInt((int32_t)(expected), (int32_t)(actual), \
                        NULL, __LINE__, UNITY_DISPLAY_STYLE_INT)

#define TEST_ASSERT_EQUAL_INT_MESSAGE(expected, actual, msg) \
    UnityAssertEqualInt((int32_t)(expected), (int32_t)(actual), \
                        (msg), __LINE__, UNITY_DISPLAY_STYLE_INT)

#define TEST_ASSERT_EQUAL(expected, actual) \
    TEST_ASSERT_EQUAL_INT(expected, actual)

/* --- Unsigned / HEX --- */
#define TEST_ASSERT_EQUAL_UINT(expected, actual) \
    UnityAssertEqualUint((uint32_t)(expected), (uint32_t)(actual), NULL, __LINE__)

#define TEST_ASSERT_EQUAL_HEX8(expected, actual) \
    UnityAssertEqualInt((int32_t)(expected), (int32_t)(actual), \
                        NULL, __LINE__, UNITY_DISPLAY_STYLE_HEX8)

/* --- Boolean / Generic --- */
#define TEST_ASSERT_TRUE(condition) \
    UnityAssertEqualInt(1, (condition) ? 1 : 0, \
                        "Expected TRUE", __LINE__, UNITY_DISPLAY_STYLE_INT)

#define TEST_ASSERT_FALSE(condition) \
    UnityAssertEqualInt(0, (condition) ? 1 : 0, \
                        "Expected FALSE", __LINE__, UNITY_DISPLAY_STYLE_INT)

/* --- NULL / Not NULL --- */
#define TEST_ASSERT_NULL(ptr) \
    UnityAssertNull((ptr), NULL, __LINE__, 1)

#define TEST_ASSERT_NOT_NULL(ptr) \
    UnityAssertNull((ptr), NULL, __LINE__, 0)

/* --- String --- */
#define TEST_ASSERT_EQUAL_STRING(expected, actual) \
    UnityAssertEqualString((expected), (actual), NULL, __LINE__)

/* --- Memory --- */
#define TEST_ASSERT_EQUAL_MEMORY(expected, actual, len) \
    UnityAssertEqualMemory((expected), (actual), (len), NULL, __LINE__)

void UnityAssertEqualMemory(const void *expected, const void *actual,
                            size_t len, const char *msg, uint32_t line);

#endif /* UNITY_H */
