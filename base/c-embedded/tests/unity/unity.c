/* ──────────────────────────────────────────────
 * unity.c — Unity Test Framework Implementation
 * Based on Unity v2.5.2 (MIT License)
 * https://github.com/ThrowTheSwitch/Unity
 *
 * IMPORTANT: Replace with official Unity for production.
 * ────────────────────────────────────────────── */

#include "unity.h"
#include <stdio.h>
#include <string.h>
#include <setjmp.h>

/* TEST_PROTECT: setjmp wrapper — must be defined before use */
#ifndef TEST_PROTECT
#define TEST_PROTECT() (setjmp(Unity.AbortFrame) == 0)
#endif

/* ── Global state ────────────────────────────── */
UNITY_STORAGE_T Unity;

/* ── Lifecycle ───────────────────────────────── */
void UnityBegin(const char *filename)
{
    Unity.TestFile              = filename;
    Unity.CurrentTestName       = NULL;
    Unity.CurrentTestLineNumber = 0;
    Unity.NumberOfTests         = 0;
    Unity.TestFailures          = 0;
    Unity.TestIgnores           = 0;
    Unity.CurrentTestFailed     = 0;
    Unity.CurrentTestIgnored    = 0;
}

int UnityEnd(void)
{
    printf("\n-----------------------\n");
    printf("%u Tests %u Failures %u Ignored\n",
           Unity.NumberOfTests, Unity.TestFailures, Unity.TestIgnores);
    if (Unity.TestFailures == 0) {
        printf("OK\n");
    } else {
        printf("FAIL\n");
    }
    return (int)Unity.TestFailures;
}

/* ── Test runner ─────────────────────────────── */
void UnityDefaultTestRun(void (*func)(void), const char *name, uint32_t line)
{
    Unity.CurrentTestName       = name;
    Unity.CurrentTestLineNumber = line;
    Unity.CurrentTestFailed     = 0;
    Unity.CurrentTestIgnored    = 0;
    Unity.NumberOfTests++;

    if (TEST_PROTECT()) {
        setUp();
        func();
    }
    if (Unity.CurrentTestFailed == 0 && Unity.CurrentTestIgnored == 0) {
        printf("%s:%s:PASS\n", Unity.TestFile, name);
    }
    tearDown();
}

/* ── Fail / Ignore ───────────────────────────── */
void UnityFail(const char *msg, uint32_t line)
{
    Unity.CurrentTestFailed = 1;
    Unity.TestFailures++;
    printf("%s:%u:%s:FAIL", Unity.TestFile, line, Unity.CurrentTestName);
    if (msg != NULL) {
        printf(":%s", msg);
    }
    printf("\n");
    longjmp(Unity.AbortFrame, 1);
}

void UnityIgnore(const char *msg, uint32_t line)
{
    Unity.CurrentTestIgnored = 1;
    Unity.TestIgnores++;
    printf("%s:%u:%s:IGNORE", Unity.TestFile, line, Unity.CurrentTestName);
    if (msg != NULL) {
        printf(":%s", msg);
    }
    printf("\n");
    longjmp(Unity.AbortFrame, 1);
}

/* ── Assertions ──────────────────────────────── */
void UnityAssertEqualInt(int32_t expected, int32_t actual,
                         const char *msg, uint32_t line,
                         UNITY_DISPLAY_STYLE_T style)
{
    if (expected == actual) { return; }

    Unity.CurrentTestFailed = 1;
    Unity.TestFailures++;

    printf("%s:%u:%s:FAIL", Unity.TestFile, line, Unity.CurrentTestName);
    if (style == UNITY_DISPLAY_STYLE_HEX8) {
        printf(":Expected 0x%02X Was 0x%02X", (uint8_t)expected, (uint8_t)actual);
    } else if (style == UNITY_DISPLAY_STYLE_HEX16) {
        printf(":Expected 0x%04X Was 0x%04X", (uint16_t)expected, (uint16_t)actual);
    } else {
        printf(":Expected %d Was %d", (int)expected, (int)actual);
    }
    if (msg != NULL) { printf(" (%s)", msg); }
    printf("\n");
    longjmp(Unity.AbortFrame, 1);
}

void UnityAssertEqualUint(uint32_t expected, uint32_t actual,
                          const char *msg, uint32_t line)
{
    if (expected == actual) { return; }

    Unity.CurrentTestFailed = 1;
    Unity.TestFailures++;
    printf("%s:%u:%s:FAIL:Expected %u Was %u", Unity.TestFile, line,
           Unity.CurrentTestName, expected, actual);
    if (msg != NULL) { printf(" (%s)", msg); }
    printf("\n");
    longjmp(Unity.AbortFrame, 1);
}

void UnityAssertEqualString(const char *expected, const char *actual,
                            const char *msg, uint32_t line)
{
    if (expected == actual) { return; }
    if (expected != NULL && actual != NULL && strcmp(expected, actual) == 0) { return; }

    Unity.CurrentTestFailed = 1;
    Unity.TestFailures++;
    printf("%s:%u:%s:FAIL:Expected \"%s\" Was \"%s\"",
           Unity.TestFile, line, Unity.CurrentTestName,
           expected ? expected : "(null)", actual ? actual : "(null)");
    if (msg != NULL) { printf(" (%s)", msg); }
    printf("\n");
    longjmp(Unity.AbortFrame, 1);
}

void UnityAssertNull(const void *ptr, const char *msg, uint32_t line, int expect_null)
{
    int is_null = (ptr == NULL);
    if (is_null == expect_null) { return; }

    Unity.CurrentTestFailed = 1;
    Unity.TestFailures++;
    printf("%s:%u:%s:FAIL:%s",
           Unity.TestFile, line, Unity.CurrentTestName,
           expect_null ? "Expected NULL" : "Expected NOT NULL");
    if (msg != NULL) { printf(" (%s)", msg); }
    printf("\n");
    longjmp(Unity.AbortFrame, 1);
}

void UnityAssertEqualMemory(const void *expected, const void *actual,
                            size_t len, const char *msg, uint32_t line)
{
    if (memcmp(expected, actual, len) == 0) { return; }

    Unity.CurrentTestFailed = 1;
    Unity.TestFailures++;
    printf("%s:%u:%s:FAIL:Memory mismatch (%zu bytes)",
           Unity.TestFile, line, Unity.CurrentTestName, len);
    if (msg != NULL) { printf(" (%s)", msg); }
    printf("\n");
    longjmp(Unity.AbortFrame, 1);
}
