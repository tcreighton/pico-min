#pragma once

#include <queue>

#include "communication.hpp"
#include "pystring.h"


namespace CScmdctl {

    class Command final {
    public:

        Command() = default;

        static bool doCommand ();

        static void recordCommandString (const std::string& commandString) {
            // This allows multiple commands on a line.
            std::vector<std::string> tokens;
            pystring::split(commandString, tokens, sSEMI);
            for (const auto& token : tokens) {
                commandStrings_.push(pystring::strip(pystring::lower(token)));
            }
        }

    private:

        // This holds the command strings.
        // Produced in CScommands::Command::recordCommandString(inputBuffer);
        // Called from CScommunications::handleInputBuffer ()
        static std::queue<std::string> commandStrings_; // Note it is static!

    };

}
