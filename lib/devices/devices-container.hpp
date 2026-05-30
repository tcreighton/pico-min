#pragma once

#include "mcu-temp-sensor.hpp"

namespace CSdevices {

    inline MCUTempSensor& getMCUTempSensor () {
        static MCUTempSensor onboardTempSensor_;
        return onboardTempSensor_;
    }

}

