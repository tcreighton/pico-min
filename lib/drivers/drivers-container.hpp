#pragma once

#include <utility>

#include "drivers-declarations.hpp"
#include "gpio-declarations.hpp"
#include "led-driver.hpp"

namespace CSdrivers {

    inline LedDriver& getOnBoardLed () {
        static LedDriver onBoardLed_(LedId::ONBOARD, CScmdctl::LED_PIN);
        return onBoardLed_;
    }

    // Other LED driver instantiations would go here.


    inline LedDriver& getLed (const LedId ledId) {
        switch (ledId) {
            case LedId::ONBOARD:
                return getOnBoardLed();
            // Other case statements for other LEDs go here.
        }
        assert(false && "Invalid Pid Type"); // Remember that in Debug this has a value. Production: ((void)0);
        std::unreachable();

    }
}
