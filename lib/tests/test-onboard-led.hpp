#pragma once
#include "test.hpp"

namespace CStest {

    class TestOnboardLed final : public Test {

    public:
        TestOnboardLed(const Assertion::verbosity level, const std::string_view className) : Test(level, className) {
            displayMessage(getVerbosityLevel(), "In " + getClassName() + "::TestOnboardLed()");
            // Call addTestFunction for each test function. Then executeAll will run each one in order.
            addTestFunction([this](const Assertion::verbosity v) { testSetOnOff(v); });
            addTestFunction([this](const Assertion::verbosity v) { testBlink(v); });
        }

        ~TestOnboardLed () override = default;

        void setUp() override {Test::setUp();}
        void tearDown () override {Test::tearDown();}

        void testSetOnOff (Assertion::verbosity level = Assertion::VERBOSE) const;
        void testBlink (Assertion::verbosity level = Assertion::VERBOSE) const;
    };
}
