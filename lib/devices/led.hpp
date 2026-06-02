#pragma once

#include <hardware/gpio.h>


namespace CSdevices {

    class Led {

    public:
        explicit Led (uint8_t gpioId) : gpioId_ (gpioId) {
            gpio_init(gpioId);
            gpio_set_dir(gpioId, GPIO_OUT);
        };

        void setEnabled (bool enabled) {enabled_ = enabled;}
        [[nodiscard]] bool isEnabled () const {return enabled_;}

        [[nodiscard]] uint16_t getPeriod () const {return period_;}
        void setPeriod (uint16_t period) {period > 0 ? period_ = period: DEFAULT_LED_PERIOD_MS;}
        void setOn () {on_ = true;}
        void setOff () {on_ = false;}
        void setBlink (const bool blink) {blink_ = blink;}

    private:
        static constexpr uint16_t DEFAULT_LED_PERIOD_MS = 500; // default period is 500 milliseconds.

        uint8_t gpioId_;
        bool enabled_{false};
        bool on_{false};
        bool blink_{false};
        uint16_t period_{DEFAULT_LED_PERIOD_MS};
    };

}
