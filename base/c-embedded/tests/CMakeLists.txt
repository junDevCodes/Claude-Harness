cmake_minimum_required(VERSION 3.16)

# ─────────────────────────────────────────────
# Unity Test Framework
# ─────────────────────────────────────────────
add_library(unity STATIC unity/unity.c)
target_include_directories(unity PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/unity)

# Common include dirs for all tests
set(TEST_INCLUDES
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/hal
    ${CMAKE_SOURCE_DIR}/src/utils
    ${CMAKE_SOURCE_DIR}/src/drivers/mock
    ${CMAKE_CURRENT_SOURCE_DIR}/unity
)

# Common libs for all tests
set(TEST_LIBS unity hal drivers utils)

# ─────────────────────────────────────────────
# Helper macro: add one test executable
# ─────────────────────────────────────────────
macro(add_unity_test TARGET SRC)
    add_executable(${TARGET} ${SRC})
    target_include_directories(${TARGET} PRIVATE ${TEST_INCLUDES})
    target_link_libraries(${TARGET} PRIVATE ${TEST_LIBS})
    # Suppress pedantic for test files (Unity uses GNU extensions)
    target_compile_options(${TARGET} PRIVATE -Wno-pedantic)
    add_test(NAME ${TARGET} COMMAND ${TARGET})
    set_tests_properties(${TARGET} PROPERTIES
        PASS_REGULAR_EXPRESSION "OK"
        FAIL_REGULAR_EXPRESSION "FAIL"
    )
endmacro()

# ─────────────────────────────────────────────
# Test executables (one per test file)
# ─────────────────────────────────────────────
add_unity_test(test_hal_gpio     test_hal_gpio.c)
add_unity_test(test_ring_buffer  test_ring_buffer.c)
