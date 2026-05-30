#pragma once
#include "command-declarations.hpp"


namespace CScmdctl {

    class CommandLang final {

    public:
        CommandLang() = delete;

        static CommandWord languageTokenToCommandWord (LanguageTokenId tokenId);
        static LanguageTokenId stringToLanguageTokenId (const std::string& token);

        static bool tokenIsParameter (const LanguageTokenId tokenId) {
            switch (tokenId) {
                case LanguageTokenId::DECODE:
                    return true;
                default:
                    return false;
            }
        }

        static bool tokenIsTempUnits(const LanguageTokenId tokenId) {
            switch (tokenId) {
                case LanguageTokenId::CELSIUS:
                case LanguageTokenId::FAHRENHEIT:
                case LanguageTokenId::KELVIN:
                    return true;

                default:
                    return false;
            }
        }

        static bool tokenIsUnits(const LanguageTokenId tokenId) {
            switch (tokenId) {
                case LanguageTokenId::AMPS:
                case LanguageTokenId::COUNTS:
                case LanguageTokenId::KILOVOLTS:
                case LanguageTokenId::MICROAMPS:
                case LanguageTokenId::MILLIAMPS:
                case LanguageTokenId::MILLIVOLTS:
                case LanguageTokenId::VOLTS:
                    return true;
                default:
                    return false;
            }
        }


        static TokenType classifyToken (const LanguageTokenId tokenId) {
            if (CommandWord::UNKNOWN != languageTokenToCommandWord(tokenId)) {return TokenType::COMMAND_WORD;}
            // Otherwise, we can assume that the command word is in the CommandStructure.

            if (tokenIsTempUnits(tokenId)) {return TokenType::TEMP_UNITS;}
            if (tokenIsUnits(tokenId)) {return TokenType::UNITS;}
            if (tokenIsParameter(tokenId)) {return TokenType::PARAMETER;}

            return TokenType::UNCLASSIFIED;

        }

    };

}
