#pragma once

#include <hardware/gpio.h>

#include "pico/time.h"

namespace CSdevices {

    class Led {

    public:
        explicit Led (uint8_t gpioId) : gpioId_ (gpioId){   // This duplicates what's in Gpio::initOutputPins()

            gpio_init(gpioId);
            gpio_set_dir(gpioId, GPIO_OUT);
        }

        void setEnabled (const bool enabled) {
            enabled_ = enabled;
            setOff();   // Any time the enabled state changes, we make sure the led is off.
        }

        [[nodiscard]] bool isEnabled () const {return enabled_;}

        [[nodiscard]] bool isOn () const {return on_;}
        void setOn () {
            on_ = true;
            gpio_put(gpioId_, on_);
        }

        void setOff () {
            on_ = false;
            gpio_put(gpioId_, on_);
        }

        [[nodiscard]] bool isBlinking () const {return blinking_;}

        bool startBlinking(const uint32_t period, const uint8_t requestedBlinkCount) {
            auto retVal = false;
            if (isEnabled() && !isBlinking()) {
                period_ = period;
                blinking_ = true;
                setOn();
                nextBlink_ = make_timeout_time_ms(period_);
                requestedBlinkCount_ = requestedBlinkCount * 2; // This allows a count each for on/off
                blinkCounts_ = 1;   // Needs to start at 1 since we already set it on.
                retVal = true;
            }

            return retVal;
        }

        void checkBlinking() {
            if (isEnabled() && isBlinking() && time_reached(nextBlink_)) {
                blink();    // toggle on/off
                ++blinkCounts_;  // Increment the blinks done in the current session.
                nextBlink_ = make_timeout_time_ms(period_);
            }
            if (isBlinking() && blinkCounts_ > requestedBlinkCount_) {
                stopBlinking();
            }
        }

        void stopBlinking () {
            requestedBlinkCount_ = 0;
            blinkCounts_ = 0;
            blinking_ = false;
            setOff();
        }

        void blink () {
            if (isEnabled() && isBlinking()) {
                if (isOn()) {
                    setOff();
                } else {
                    setOn();
                }
            }
        }

    private:
        static constexpr uint16_t DEFAULT_LED_PERIOD_MS = 500; // default period is 500 milliseconds.
        static constexpr uint8_t DEFAULT_BLINK_COUNT = 255;

        uint8_t gpioId_;
        bool enabled_{false};
        bool on_{false};
        bool blinking_{false};
        uint32_t period_{DEFAULT_LED_PERIOD_MS};
        absolute_time_t nextBlink_{};
        uint8_t requestedBlinkCount_{DEFAULT_BLINK_COUNT};   // This is the max blinks for the current blink session.
        uint8_t blinkCounts_{};                          // This is where we accumulate blinks
    };

}
