
#include <unordered_map>

#include "command-lang.hpp"


namespace CScmdctl {

    CommandWord CommandLang::languageTokenToCommandWord(LanguageTokenId tokenId) {
        switch (tokenId) {
            default:
            case LanguageTokenId::UNKNOWN:
                return CommandWord::UNKNOWN;

            case LanguageTokenId::SAY_HELLO:
                return CommandWord::SAY_HELLO;

            case LanguageTokenId::SALUDA:
                return CommandWord::SALUDA;
        }
    }

    LanguageTokenId CommandLang::stringToLanguageTokenId(const std::string& token) {
        static const std::unordered_map<std::string, LanguageTokenId> tokenMap {
            {"decode",      LanguageTokenId::DECODE},
            {"info",        LanguageTokenId::RPT_INFO},
            {"information", LanguageTokenId::RPT_INFO},
            {"cel",         LanguageTokenId::CELSIUS},
            {"celsius",     LanguageTokenId::CELSIUS},
            {"fah",         LanguageTokenId::FAHRENHEIT},
            {"far",         LanguageTokenId::FAHRENHEIT},
            {"fahr",        LanguageTokenId::FAHRENHEIT},
            {"fahrenheit",  LanguageTokenId::FAHRENHEIT},
            {"kel",         LanguageTokenId::KELVIN},
            {"kelvin",      LanguageTokenId::KELVIN},
            {"hello",       LanguageTokenId::SAY_HELLO},
            {"hola",        LanguageTokenId::SALUDA},
            {"counts",      LanguageTokenId::COUNTS},
            {"kv",          LanguageTokenId::KILOVOLTS},
            {"kvolts",      LanguageTokenId::KILOVOLTS},
            {"ma",          LanguageTokenId::MILLIAMPS},
            {"milliamps",   LanguageTokenId::MILLIAMPS},
            {"microamps",   LanguageTokenId::MICROAMPS},
            {"ua",          LanguageTokenId::MICROAMPS},
            {"volts",       LanguageTokenId::VOLTS},
            {"v",           LanguageTokenId::VOLTS},

        };

        const auto it = tokenMap.find(token);
        return (it != tokenMap.end()) ? it->second : LanguageTokenId::UNKNOWN;
    }

}
