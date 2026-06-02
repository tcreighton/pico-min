
# Create a library for devices

add_library(devices
        csi2c.hpp
        devices-container.hpp
        led.cpp
        led.hpp
        mcu-temp-sensor.cpp
        mcu-temp-sensor.hpp
        pico-adc.hpp
        pico-adc.cpp
        temp-sensor.hpp
)

target_include_directories(devices PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link against Pico SDK libraries this module needs
target_link_libraries(devices PUBLIC
        core
        tests
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
#       pico_sync
)
