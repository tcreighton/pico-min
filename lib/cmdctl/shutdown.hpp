#pragma once

#include <sstream>

#include "drivers-container.hpp"
#include "reports.hpp"

// pico sdk
#include "hardware/watchdog.h"

namespace CScmdctl {


    enum class RunMode : uint8_t {
        NORMAL = 0,
        SHUTDOWN,
        PAUSE,
        REBOOT
    };

    inline auto runMode_ = RunMode::NORMAL;

    inline void performEmergencyShutdown() {

        // TODO: Disable circuitry!

        // Now send the reports.

        Communication::serialOutputLF160("**Critical error encountered. System shutting down.**\n");
        const std::string s = Report::fullSystemReport();
        Communication::serialOutputLF160(s);

        std::stringstream ss;
        ss << "Shutting down...";
        Communication::serialOutputLF160(ss.str());
        runMode_ = RunMode::SHUTDOWN;
    }

    inline void doRestart () {
        runMode_ = RunMode::REBOOT;
        CSdrivers::getFilament().setCounts(0);
        CSdrivers::getTube().setCounts(0);
        watchdog_reboot(0, 0, 0);
    }

    inline void awaitResetCommand() {
        Communication::serialOutputLF160("\nPress ENTER to restart.");
        // ReSharper disable once CppDFALoopConditionNotUpdated
        while (runMode_ == RunMode::SHUTDOWN) {

            sleep_ms(1000);
            Communication::handleInputBuffer();   // This will eventually call doRestart.

        }
    }
}

