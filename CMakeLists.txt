cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico2_w CACHE STRING "Board type")

include(pico-sdk/pico_sdk_init.cmake)
include(pico-sdk/external/pico_sdk_import.cmake)
project(Embedded C CXX ASM)
pico_sdk_init()

## Hello World example

add_executable(hello_pico
    1_hello_pico/hello_pico.c
)

target_link_libraries(hello_pico pico_stdlib pico_cyw43_arch_none)

pico_enable_stdio_usb(hello_pico 1)
pico_enable_stdio_uart(hello_pico 0)

pico_add_extra_outputs(hello_pico)

## Arithmetics example

add_executable(arithmetics
    2_arithmetic/arithmetic.c
)

target_link_libraries(arithmetics pico_stdlib)

pico_enable_stdio_usb(arithmetics 1)
pico_enable_stdio_uart(arithmetics 0)

pico_add_extra_outputs(arithmetics)

## Arithmetics with history example
add_executable(arithmetics_history
    3_arithmetic_history/arithmetics.c
)

target_link_libraries(arithmetics_history pico_stdlib)

pico_enable_stdio_usb(arithmetics_history 1)
pico_enable_stdio_uart(arithmetics_history 0)

pico_add_extra_outputs(arithmetics_history)

## Hysteresis button example
add_subdirectory(pico2w-hysteresis-lib)
add_executable(hysteria_button
    4_hysteresis/hysteria_button.c
)

target_link_libraries(hysteria_button pico_stdlib hardware_pwm pico_binary_info input_hysteresis)

pico_enable_stdio_usb(hysteria_button 1)
pico_enable_stdio_uart(hysteria_button 0)

pico_add_extra_outputs(hysteria_button)
