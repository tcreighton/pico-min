
#include "command.hpp"

#include "command-handler.hpp"
#include "command-parser.hpp"

namespace CScmdctl {


    //
    // 1) Get a command string from the vector.
    // 2) Create a CommandParser passing a reference to the CommandStruct.
    // 3) Create a CommandHandler passing a reference to the CommandStruct.
    // 4) If the parser succeeds, call the handler.
    //
    bool Command::doCommand() {
        auto retVal = true; // If there is no command string ready, we return true.

        if (!commandStrings_.empty()) {
            CommandStructure commandStruct{};
            const std::string commandString = commandStrings_.front();
            commandStrings_.pop();
            if (!commandString.empty()) {
                commandStruct.commandString = commandString;
                const CommandParser commandParser(commandStruct);
                retVal = commandParser.parseCommand();
            }

            if (retVal) { // If it parsed ok, let's run it!
                const CommandHandler commandHandler(commandStruct);
                retVal = commandHandler.handleCommand();
            }
        }

        return retVal;
    }

    std::queue<std::string> Command::commandStrings_;    // definition of the private queue.

}

