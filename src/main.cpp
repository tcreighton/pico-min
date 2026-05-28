
#include "assertion.hpp"
#include "communication.hpp"
#include "logger.hpp"
#include "shutdown.hpp"
#include "startup.hpp"
#include "pico/time.h"

using namespace CScmdctl;
using namespace CScore;

int main()
{
    startup();
    logger_.setLogLevel(LogLevel::Info);   // For prod set this to Fatal.

//    CStest::runTests(CStest::Assertion::verbosity::VERBOSE);


    // TODO: Setup and handle watchdog


    ///
    /// Main Loop!
    ///

    while (RunMode::NORMAL == runMode_) {
        constexpr uint16_t artificialLoopDelayMs = 0;

        Communication::handleInputBuffer();

        sleep_ms(artificialLoopDelayMs);

        std::stringstream ss;

        ss << "Main Loop: " << CScore::uptime_ms() << " ms";
        logger_.log(LogLevel::Trace, ss.str());

    }
    ///
    /// Main Loop!
    ///

    if (RunMode::SHUTDOWN == runMode_) {
        awaitResetCommand();
    }
    logger_.log(LogLevel::Info, "Exiting Pico Min. Goodbye!\n\n");

    return 0;



}

