#pragma once

#include <cstdint>
#include <string>
#include <vector>


namespace CScmdctl {

    enum class CommandWord : std::uint8_t {
        SALUDA,
        SAY_HELLO,
        UNKNOWN,
    };

    enum class LanguageTokenId {
        AMPS,
        COUNTS,
        KILOVOLTS,
        MILLIVOLTS,
        MICROAMPS,
        MILLIAMPS,
        VOLTS,
        DECODE,
        CELSIUS,
        KELVIN,
        FAHRENHEIT,
        NUMBER,
        RPT_INFO,
        SALUDA,
        SAY_HELLO,
        UNKNOWN,
    };

    enum class TokenType : uint8_t {
        COMMAND_WORD,
        NUMBER,
        PARAMETER,      // Generic designation.
        TEMP_UNITS,     // temperature units: celsius, kelvin, fahrenheit
        UNITS,
        UNCLASSIFIED,
    };

    struct TokenValue {
        LanguageTokenId tokenId{LanguageTokenId::UNKNOWN};
        TokenType       tokenType{TokenType::UNCLASSIFIED};      // unknown is not necessarily an error. Determined in context.
        std::string     tokenString;
        CommandWord     commandWord{CommandWord::UNKNOWN};      // If it is a command token this gets filled in.
        float           value{0};                               // If it needs to be an integer, we just lround it.
    };

    enum class CommandStatus : uint8_t {
        CMD_OK   = 0,       // Parse worked. All pieces needed are there.
        UNKNOWN_COMMAND,    // Did not recognize the CommandWord
        INVALID_COMMAND,    // Command word is out of context - should not happen. Likely indicates programming error.
        INVALID_PARAMETER,  // Some other syntax error - don't know if this is needed.
        UNKNOWN_PARAMETER,  // This just lets the command handler decide what to do with it. See the test command.
        UNKNOWN_ERROR    // Any other problem.
    };

    struct CommandStructure {
        std::string                  commandString{};                        // Command string from the queue.
        CommandWord                  commandWord{CommandWord::UNKNOWN};
        std::vector<TokenValue>      tokenValues{};                          // Holds all the token values.
        CommandStatus                status{CommandStatus::UNKNOWN_ERROR};   // The status of the command.
    };
}
