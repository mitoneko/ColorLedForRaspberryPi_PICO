cmake_minimum_required(VERSION 3.12)
include (pico_sdk_import.cmake)
project (colorled C CXX ASM)
set (CMAKE_C_STANDARD 11)
set (CMAKE_CXX_STANDARD 17)
pico_sdk_init()
add_executable(colorled
    src/main.cpp
    )
pico_generate_pio_header(colorled ${CMAKE_CURRENT_LIST_DIR}/src/led.pio)
target_link_libraries(colorled
   pico_stdlib
   hardware_pio 
   hardware_clocks 
   hardware_gpio
   hardware_dma
   )
pico_add_extra_outputs(colorled)
