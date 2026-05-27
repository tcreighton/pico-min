# Create a library for drivers

add_library(cmdctl
        board-config.cpp
        board-config.cpp
        gpio.cpp
        gpio.hpp
        serial-comm.hpp
        serial-comm.cpp
        shutdown.hpp
)


target_include_directories(cmdctl PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cmdctl PUBLIC
        core
        devices
        drivers
        # pico interfaces here
        hardware_adc
        hardware_gpio
        hardware_i2c
        #       hardware_pwm
        #       hardware_spi
        hardware_timer
        hardware_uart
        #       pico_multicore
        pico_stdlib
        pico_stdio_usb
        #       pico_sync
        #       pico_unique_id
)
