#ifndef UNITY_INTERNALS_H
#define UNITY_INTERNALS_H

/* ──────────────────────────────────────────────
 * unity_internals.h — Unity Test Framework Internals
 * Based on Unity v2.5.2 (MIT License)
 * https://github.com/ThrowTheSwitch/Unity
 *
 * For production use, replace with the official Unity source:
 *   git clone https://github.com/ThrowTheSwitch/Unity.git
 * ────────────────────────────────────────────── */

#include <stdint.h>
#include <stddef.h>
#include <setjmp.h>

/* Result types */
typedef enum {
    UNITY_DISPLAY_STYLE_INT = 0,
    UNITY_DISPLAY_STYLE_UINT,
    UNITY_DISPLAY_STYLE_HEX8,
    UNITY_DISPLAY_STYLE_HEX16,
    UNITY_DISPLAY_STYLE_HEX32,
} UNITY_DISPLAY_STYLE_T;

/* Unity state struct */
typedef struct UNITY_STORAGE_T {
    const char *TestFile;
    const char *CurrentTestName;
    uint32_t    CurrentTestLineNumber;
    uint32_t    NumberOfTests;
    uint32_t    TestFailures;
    uint32_t    TestIgnores;
    uint8_t     CurrentTestFailed;
    uint8_t     CurrentTestIgnored;
    jmp_buf     AbortFrame;
} UNITY_STORAGE_T;

extern UNITY_STORAGE_T Unity;

/* Internal assertion helper */
void UnityFail(const char *msg, uint32_t line);
void UnityIgnore(const char *msg, uint32_t line);
void UnityAssertEqualInt(int32_t expected, int32_t actual,
                         const char *msg, uint32_t line,
                         UNITY_DISPLAY_STYLE_T style);
void UnityAssertEqualUint(uint32_t expected, uint32_t actual,
                          const char *msg, uint32_t line);
void UnityAssertEqualString(const char *expected, const char *actual,
                            const char *msg, uint32_t line);
void UnityAssertNull(const void *ptr, const char *msg, uint32_t line, int expect_null);

#endif /* UNITY_INTERNALS_H */
