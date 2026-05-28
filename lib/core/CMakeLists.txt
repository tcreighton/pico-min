# Create a library for drivers

add_library(core
        logger.hpp
        logger.cpp
        packed-datetime.cpp
        packed-datetime.hpp
        product-info.hpp
        pystring.cpp
        pystring.h
        random.hpp
        utilities.hpp
        utilities.cpp
)


target_include_directories(core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(core PUBLIC
        #cmdctl
        # pico interfaces here
        hardware_adc
        hardware_gpio
        hardware_i2c
        #       hardware_pwm
        #       hardware_spi
        #       hardware_timer
        #       hardware_uart
        #       pico_multicore
        pico_stdlib
        pico_stdio_usb
        #       pico_sync
        pico_unique_id
)
