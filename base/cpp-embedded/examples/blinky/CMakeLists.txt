add_executable(blinky_example
    main.cpp
)

target_link_libraries(blinky_example
    PRIVATE
        hal_interfaces
        hal_drivers
)

target_include_directories(blinky_example PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/mock
    ${CMAKE_SOURCE_DIR}/drivers/include
)
