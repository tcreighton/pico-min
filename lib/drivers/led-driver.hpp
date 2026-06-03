#pragma once

#include "drivers-declarations.hpp"
#include "led.hpp"


namespace CSdrivers {

    class LedDriver : public CSdevices::Led {

    public:

        LedDriver(const LedId id, const uint8_t gpio) : Led(gpio), id_(id) {};

    private:
        LedId id_;
    };
}
