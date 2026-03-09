enable_testing()

# ── 테스트 실행 파일 ──────────────────────────────────────────────────────────
add_executable(run_tests
    test_gpio.cpp
    test_uart.cpp
    test_led_driver.cpp
)

target_link_libraries(run_tests
    PRIVATE
        hal_interfaces
        hal_drivers
        GTest::gtest_main
)

target_include_directories(run_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/mock
    ${CMAKE_SOURCE_DIR}/drivers/include
)

# ── CTest 등록 ────────────────────────────────────────────────────────────────
include(GoogleTest)
gtest_discover_tests(run_tests)
