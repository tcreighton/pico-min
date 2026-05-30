
#include "command-handler.hpp"

#include "communication.hpp"

namespace CScmdctl {

    bool CommandHandler::handleCommand() const {
        auto retVal = false;

        switch (commandStruct_.tokenValues[0].commandWord) {
            case CommandWord::SALUDA:
                handleSaluda();
                retVal = true;
                break;

            case CommandWord::SAY_HELLO:
                handleSayHello();
                retVal = true;
                break;

            default:
            case CommandWord::UNKNOWN:
                break;
        }

        return retVal;
    }

    void CommandHandler::handleSaluda() {
        Communication::prepAndSendOutput("¿Que onda, güey?");
    }

    void CommandHandler::handleSayHello() {
        Communication::prepAndSendOutput("What up, dude?");
    }
}
