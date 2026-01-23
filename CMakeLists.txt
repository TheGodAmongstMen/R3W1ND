cmake_minimum_required(VERSION 3.13)

# Pull in the Pico SDK
include(pico_sdk_import.cmake)

project(r3w1nd C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

add_executable(r3w1nd
    main.c
    screen.c
)

target_include_directories(r3w1nd PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(r3w1nd
    pico_stdlib
    hardware_spi
)

# Enable USB serial if you want printf debugging
pico_enable_stdio_usb(r3w1nd 1)
pico_enable_stdio_uart(r3w1nd 0)

# This macro comes from the Pico SDK
pico_add_extra_outputs(r3w1nd)
