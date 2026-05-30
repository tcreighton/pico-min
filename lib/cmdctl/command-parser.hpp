#pragma once

#include "command-declarations.hpp"


namespace CScmdctl {

    class CommandParser final {

    public:

        explicit CommandParser(CommandStructure& commandStructure) : commandStruct_(commandStructure) {};
        ~CommandParser() = default;

        bool parseCommand () const;

    private:

        bool parseCommand (const std::vector<std::string>& tokens) const;
        bool parseNumber (TokenValue& tokenValue, size_t& pos) const;
        bool processCommandTokens() const;

        // Utilties
        TokenValue getCommandTokenValue() const {return commandStruct_.tokenValues[0];}

        CommandStructure& commandStruct_;
    };
};