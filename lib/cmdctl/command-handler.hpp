#pragma once

#include "command-declarations.hpp"

namespace CScmdctl {

    class CommandHandler final {

    public:

        explicit CommandHandler (CommandStructure& commandStruct) : commandStruct_(commandStruct) {}
        ~CommandHandler() = default;

        bool handleCommand () const;

    private:
        static void handleSaluda ();
        static void handleSayHello();

        CommandStructure& commandStruct_;
    };


};