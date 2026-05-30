# Create a library for drivers

add_library(cmdctl
        board-config.cpp
        board-config.hpp
        command.cpp
        command.hpp
        command-declarations.hpp
        command-handler.cpp
        command-handler.hpp
        command-lang.cpp
        command-lang.hpp
        command-parser.cpp
        command-parser.hpp
        communication.cpp
        communication.hpp
        error-context.hpp
        errors.cpp
        errors.hpp
        gpio.cpp
        gpio.hpp
        serial-comm.cpp
        serial-comm.hpp
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
