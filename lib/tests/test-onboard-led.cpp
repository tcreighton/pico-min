
#include "drivers-container.hpp"
#include "test-onboard-led.hpp"

#include "pico/time.h"

void CStest::TestOnboardLed::testSetOnOff(Assertion::verbosity level) const {
    auto& led = CSdrivers::getOnBoardLed();
    led.setEnabled(true);

    displayMessage(getVerbosityLevel(), "Led on/off three times; 1500 ms intervals.");
    for (int i = 0; i < 3; i++) {
        led.setOn();
        sleep_ms(1500);
        led.setOff();
        sleep_ms(1500);
    }

    led.setEnabled(false);
}

void CStest::TestOnboardLed::testBlink(Assertion::verbosity level) const {
    auto& led = CSdrivers::getOnBoardLed();
    led.setEnabled(true);

    displayMessage(getVerbosityLevel(),
        "Led blinks 4 times on/off, once every 500 ms.");

    led.startBlinking(500, 4);  // blink every 500 ms; do it 4 times.
    do {
        led.checkBlinking();    // This gives the led code some cycles.
    }
    while (led.isBlinking());

    displayMessage(getVerbosityLevel(),
        "Led blinks on/off 6 times, once every 1000 ms.");

    led.startBlinking(1000, 6);
    do {
        led.checkBlinking();    // This gives the led code some cycles.
    }
    while (led.isBlinking());

    led.setEnabled(false);
}

