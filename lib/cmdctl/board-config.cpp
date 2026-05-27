

#include <cstdlib>

#include "board-config.hpp"
#include "gpio.hpp"
#include "serial-comm.hpp"
#include "hardware/adc.h"
#include "pico/stdio.h"

namespace CScmdctl {
    bool boardInit() {
        auto retCode = true;

        if (!stdio_init_all()) {    // get set to handle stdio
            exit(-1);
        }

        Gpio::init();

        if (!SerialComm::initUsb()) { // if no usb, try uart; only return one or the other
            retCode |= SerialComm::initUart();
        }

        adc_init();
        adc_set_temp_sensor_enabled(true);  // If using the temperature sensor

        return retCode;
    }

}
