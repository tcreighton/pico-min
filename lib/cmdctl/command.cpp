
#include "command.hpp"

namespace CScmdctl {


    //
    // 1) Get a command string from the vector.
    // 2) Create a CommandParser passing a reference to the CommandStruct.
    // 3) Create a CommandHandler passing a reference to the CommandStruct.
    // 4) If the parser succeeds, call the handler.
    //
    bool Command::doCommand() {
        auto retVal = false;
        CommandStructure commandStructure{};

        return retVal;
    }

    std::queue<std::string> Command::commandStrings_;    // definition of the private queue.

}

