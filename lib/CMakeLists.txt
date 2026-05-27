
# Create a library for drivers

add_library(drivers
        data-logger.hpp
        drivers-container.hpp
        hv-usage-logger.cpp
        hv-usage-logger.hpp
        hw/adc-driver.cpp
        hw/adc-driver.hpp
        hw/dac-driver.hpp
        powersupply/filament.cpp
        powersupply/filament.hpp
        powersupply/pid.cpp
        powersupply/pid.hpp
        powersupply/pid-declarations.hpp
        powersupply/powersupply.cpp
        powersupply/powersupply.hpp
#        powersupply/powersupply-container.hpp
        powersupply/ps-declarations.hpp
        powersupply/tube.cpp
        powersupply/tube.hpp
        storage/eeprom0.cpp
        storage/eeprom0.hpp
        storage/eeprom0-page-declarations.hpp
        storage/fram-declarations.hpp
        storage/fram-handler.cpp
        storage/fram-handler.hpp
        storage/fram0.hpp
        storage/fram0.cpp
        storage/fram1.hpp
)

target_include_directories(drivers PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link against Pico SDK libraries this module needs
target_link_libraries(drivers PUBLIC
        cmdctl
        core
        devices
# pico interfaces here
#       hardware_adc
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
