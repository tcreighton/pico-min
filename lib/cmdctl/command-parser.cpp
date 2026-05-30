
#include <stdexcept>

#include "command-lang.hpp"
#include "command-parser.hpp"
#include "pystring.h"

namespace CScmdctl {


    bool CommandParser::parseCommand() const {
        auto retVal = false;

        std::vector<std::string> tokens;

        pystring::split(commandStruct_.commandString, tokens);

        retVal = parseCommand(tokens);
        return retVal;
    }

    bool CommandParser::parseCommand(const std::vector<std::string> &tokens) const {
        auto retVal = false;

        for (auto tokenString : tokens) {
            TokenValue tokenValue{  LanguageTokenId::UNKNOWN,
                                    TokenType::UNCLASSIFIED,
                                    "",
                                    CommandWord::UNKNOWN, // Could be invalid command!
                                    0.0f};

            tokenValue.tokenString = tokenString;
            tokenValue.tokenId = CommandLang::stringToLanguageTokenId(tokenValue.tokenString);
            tokenValue.commandWord = CommandLang::languageTokenToCommandWord(tokenValue.tokenId);
            if (CommandWord::UNKNOWN != tokenValue.commandWord) { // It's a command word. Set the classification.
                tokenValue.tokenType = TokenType::COMMAND_WORD;
            } else {    // Otherwise, it's not a command word. Set the classification.
                tokenValue.tokenType = CommandLang::classifyToken(tokenValue.tokenId);
            }

            if (TokenType::COMMAND_WORD == tokenValue.tokenType) {
                if (tokenValue.commandWord == CommandWord::UNKNOWN) {
                    commandStruct_.status = CommandStatus::UNKNOWN_COMMAND;
                    return retVal;
                }
                commandStruct_.commandWord = tokenValue.commandWord;
                commandStruct_.status = CommandStatus::CMD_OK;
                commandStruct_.tokenValues.push_back(tokenValue); // Record the token.
                continue;   // Check next token.
            }

            // At this point, it's not a command token.
            // The language token id is either a valid one or UNKNOWN. If UNKNOWN, it could be a number.
            // If it's a number, it could be followed by units, as in set tube 150 kV

            if (size_t pos;
                TokenType::UNCLASSIFIED == tokenValue.tokenType && parseNumber(tokenValue, pos)) {
                tokenValue.tokenType = TokenType::NUMBER;
                commandStruct_.tokenValues.push_back(tokenValue); // Record the token.
                if (pos > 0) {
                    // This means there are characters after the number, which could be units.
                    std::string unitString = tokenValue.tokenString.substr(pos);

                    // The following can be confusing.
                    // 1) Create a TokenValue struct:
                    // 1a) convert unitString to a TokenId. This could be UNKNOWN or NUMBER...
                    // 1b) place the converted value into unitsTokenValue.tokenId.
                    // 1c) place TokenType::UNITS into unitsTokenValue.tokenType.
                    // 1d) place unitString into unitsTokenValue.tokenString.
                    // 1e) commandWord and value are defaulted.
                    // 2) if the value in unitsTokenValue.tokenId is UNITS, push the unitsTokenValue onto tokenValues.
                    if (const TokenValue unitsTokenValue{
                            CommandLang::stringToLanguageTokenId(unitString), TokenType::UNITS, unitString};
                        CommandLang::tokenIsUnits(unitsTokenValue.tokenId)) {
                        commandStruct_.tokenValues.push_back(unitsTokenValue); // Record the token.
                        }
                } else {
                    commandStruct_.tokenValues.push_back(tokenValue); // Record the token.
                }
            }
            commandStruct_.status = CommandStatus::CMD_OK;

            // Now we have processed all the tokens in the command string.
            // Each token has been classified and recorded in tokenValues_.
            // Next we need to examine each tokenValue to determine if it is valid for the given command.

            retVal = processCommandTokens();
            return retVal;

        }


        return retVal;
    }

    bool CommandParser::parseNumber(TokenValue &tokenValue, size_t &pos) const {
        bool retVal = false;

        try {
            pos = 0;
            tokenValue.value = std::stof(tokenValue.tokenString, &pos);  // try to convert it to a float
            tokenValue.tokenId = LanguageTokenId::NUMBER;
            tokenValue.tokenType = TokenType::NUMBER;
            retVal = pos;
        } catch ([[maybe_unused]] const std::invalid_argument& ia) {
            commandStruct_.status = CommandStatus::UNKNOWN_PARAMETER;
        } catch ([[maybe_unused]] const std::out_of_range& oor) {
            commandStruct_.status = CommandStatus::INVALID_PARAMETER;
        }

        return retVal;
    }

    bool CommandParser::processCommandTokens() const {
        auto retVal = true;

        switch (getCommandTokenValue().commandWord) {

            case CommandWord::SALUDA:       // Respond with a greeting.
            case CommandWord::SAY_HELLO:    // Respond with a greeting.
                break;

            default:
            case CommandWord::UNKNOWN:      // An error. Bad command token.
                retVal = false;
                break;

        }

        return retVal;
    }
}
