cmake_minimum_required(VERSION 3.21)
project(CppEmbeddedBase
    VERSION 1.0.0
    DESCRIPTION "C++17 Embedded Base Code — HAL Abstraction + RAII + Google Test"
    LANGUAGES CXX
)

# ── C++ 표준 ─────────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── 컴파일 경고 ───────────────────────────────────────────────────────────────
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# ── Google Test (FetchContent 자동 다운로드) ──────────────────────────────────
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
# Windows: gtest가 공유 런타임(DLL) 대신 정적 런타임을 사용하도록 설정
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ── HAL 인터페이스 라이브러리 (헤더 전용) ────────────────────────────────────
add_library(hal_interfaces INTERFACE)
target_include_directories(hal_interfaces INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(hal_interfaces INTERFACE cxx_std_17)

# ── HAL 드라이버 라이브러리 (정적) ───────────────────────────────────────────
add_library(hal_drivers STATIC
    drivers/src/Led.cpp
    drivers/src/Console.cpp
    platform/stm32/Gpio.cpp
    platform/stm32/Uart.cpp
)
target_include_directories(hal_drivers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/include
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/stm32
)
target_link_libraries(hal_drivers PUBLIC hal_interfaces)

# ── 테스트 빌드 ───────────────────────────────────────────────────────────────
option(BUILD_TESTING "Build unit tests" ON)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

# ── 예시 빌드 ─────────────────────────────────────────────────────────────────
option(BUILD_EXAMPLES "Build example applications" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(examples/blinky)
endif()

# ── 빌드 정보 출력 ────────────────────────────────────────────────────────────
message(STATUS "")
message(STATUS "=== C++ Embedded Base Code ===")
message(STATUS "  C++ standard   : ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build type     : ${CMAKE_BUILD_TYPE}")
message(STATUS "  Build tests    : ${BUILD_TESTING}")
message(STATUS "  Build examples : ${BUILD_EXAMPLES}")
message(STATUS "  Install prefix : ${CMAKE_INSTALL_PREFIX}")
message(STATUS "==============================")
message(STATUS "")
