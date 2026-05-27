
#include "assertion.hpp"
#include "cmdctl-container.hpp"
#include "communication.hpp"
#include "logger.hpp"
#include "response-handler.hpp"
#include "shutdown.hpp"
#include "startup.hpp"
#include "statistics.hpp"
#include "test.hpp"
#include "worker.hpp"
#include "pico/time.h"

using namespace CSdevices;
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
    auto startTime = get_absolute_time();
    auto endTime = get_absolute_time();

    while (RunMode::NORMAL == runMode_) {
        constexpr uint16_t artificialLoopDelayMs = 0;

        Communication::handleInputBuffer();

        if (!worker.doWork()) {
            // Check for critical errors requiring shutdown
            if (ResponseHandler::hasCriticalError()) {
                performEmergencyShutdown();             // This will shut down the hv, etc. and set SHUTDOWN_MODE = true
            }
        }
        // Process any errors that occurred
        ResponseHandler::processAllErrors(ResponseHandler::isVerboseMode());    // Display any and all messages.

        sleep_ms(artificialLoopDelayMs);

        std::stringstream ss;

        ss << "Main Loop: " << CScore::uptime_ms() << " ms";
        logger_.log(LogLevel::Trace, ss.str());

        endTime = get_absolute_time();
        getStatistics().logDuration(DurationStatId::MAIN_LOOP, startTime, endTime);
        startTime = endTime;
    }
    ///
    /// Main Loop!
    ///

    if (RunMode::SHUTDOWN == runMode_) {
        awaitResetCommand();
    }
    logger_.log(LogLevel::Info, "Exiting Lumafield 160Kv. Goodbye!\n\n");

    return 0;



}

