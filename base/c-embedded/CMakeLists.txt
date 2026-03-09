cmake_minimum_required(VERSION 3.16)
project(c_embedded_base C)

# ─────────────────────────────────────────────
# C Standard
# ─────────────────────────────────────────────
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# ─────────────────────────────────────────────
# Build Type
# ─────────────────────────────────────────────
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# ─────────────────────────────────────────────
# Options
# ─────────────────────────────────────────────
option(BUILD_FOR_HOST  "Build for host PC (mock drivers + Unity tests)" ON)
option(BUILD_TESTS     "Build unit tests"                                ON)

# ─────────────────────────────────────────────
# Compiler Flags
# ─────────────────────────────────────────────
add_compile_options(
    -Wall
    -Wextra
    -Wno-unused-parameter
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-g -O0)
else()
  add_compile_options(-O2)
endif()

# ─────────────────────────────────────────────
# Platform Selection
# ─────────────────────────────────────────────
if(BUILD_FOR_HOST)
  message(STATUS "[c-embedded] Platform: HOST (mock drivers)")
  add_compile_definitions(PLATFORM_HOST FREERTOS_MOCK)
  set(DRIVER_SRC
      src/drivers/mock/gpio_mock.c
      src/drivers/mock/uart_mock.c
  )
else()
  message(STATUS "[c-embedded] Platform: STM32 (cross-compile)")
  add_compile_definitions(PLATFORM_STM32)
  set(DRIVER_SRC
      src/drivers/stm32/gpio_stm32.c
      src/drivers/stm32/uart_stm32.c
  )
  # Cross-compile toolchain should be set via -DCMAKE_TOOLCHAIN_FILE=...
endif()

# ─────────────────────────────────────────────
# Include Directories (global)
# ─────────────────────────────────────────────
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/hal
    ${CMAKE_SOURCE_DIR}/src/tasks
    ${CMAKE_SOURCE_DIR}/src/utils
)

# ─────────────────────────────────────────────
# Library: HAL
# ─────────────────────────────────────────────
add_library(hal STATIC
    src/hal/hal_gpio.c
    src/hal/hal_uart.c
    src/hal/hal_timer.c
)
target_include_directories(hal PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/hal
)

# ─────────────────────────────────────────────
# Library: Drivers (platform-specific)
# ─────────────────────────────────────────────
add_library(drivers STATIC ${DRIVER_SRC})
target_include_directories(drivers PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/hal
)

# ─────────────────────────────────────────────
# Library: Utils
# ─────────────────────────────────────────────
add_library(utils STATIC
    src/utils/ring_buffer.c
    src/utils/logger.c
)
target_include_directories(utils PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/hal
    ${CMAKE_SOURCE_DIR}/src/utils
)
target_link_libraries(utils PUBLIC hal)

# ─────────────────────────────────────────────
# Library: Tasks
# ─────────────────────────────────────────────
add_library(tasks STATIC
    src/tasks/task_led.c
    src/tasks/task_uart_monitor.c
)
target_include_directories(tasks PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/hal
    ${CMAKE_SOURCE_DIR}/src/tasks
    ${CMAKE_SOURCE_DIR}/src/utils
)
target_link_libraries(tasks PUBLIC hal utils)

# ─────────────────────────────────────────────
# Main Executable (Host only — for integration smoke test)
# ─────────────────────────────────────────────
if(BUILD_FOR_HOST)
  add_executable(app src/main.c)
  target_include_directories(app PRIVATE
      ${CMAKE_SOURCE_DIR}/src/drivers/mock
  )
  target_link_libraries(app PRIVATE hal drivers utils tasks)
endif()

# ─────────────────────────────────────────────
# Tests
# ─────────────────────────────────────────────
if(BUILD_TESTS AND BUILD_FOR_HOST)
  enable_testing()
  add_subdirectory(tests)
endif()

message(STATUS "[c-embedded] Build type : ${CMAKE_BUILD_TYPE}")
message(STATUS "[c-embedded] Build tests: ${BUILD_TESTS}")
