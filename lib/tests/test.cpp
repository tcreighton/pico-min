
// #include the hpp file for tests.
#include "test.hpp"

#include "test-onboard-led.hpp"


namespace CStest {

    void runTests(const Assertion::verbosity level) {
        TestOnboardLed testLoadStore(level, "TestOnboardLed");
        // This executes the list of methods in TestOnboardLed. Registered via addTestFunction in the constructor.
        testLoadStore.doTests();
    }


    void Test::executeAll() const {
        for (const auto& func : functionList_) {
            func(getVerbosityLevel());
        }
    }

    void Test::displayMessage(const Assertion::verbosity level, const std::string &message) {

        if (Assertion::VERBOSE == level) {
            std::cout << message << std::endl;
        }
    }

    void Test::displayMessage(const std::string &message) const {
        displayMessage(getVerbosityLevel(), message);
    }
}
