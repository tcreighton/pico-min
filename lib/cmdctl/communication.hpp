#pragma once

#include <string>
#include <bits/stdc++.h>

#include "serial-comm.hpp"

namespace CScmdctl {

//    static const auto CONSOLE_COMMAND_PROMPT = std::string("Focus500: ");

    static const auto sCR = std::string("\r");        // 0x0D; carriage return: ctl-M
    static const auto sLF = std::string("\n");        // 0x0A; line feed: ctl-J
    static const auto sFF = std::string("\f");        // 0x0C; form feed: ctl-L
    static const auto sBS = std::string("\b");        // 0x08; backspace: ctl-H
    static const auto sHT = std::string("\t");        // 0x09; horizontal tab: ctl-I
    static const auto sVT = std::string("\v");        // 0x0B; vertical tab: ctl-K
    static const auto sSP = std::string(" ");         // 0x20; space: space
    static const auto sDEL = std::string("\x7F");     // 0x7F; delete: ctl-?
    static const auto sESC = std::string("\x1B");     // 0x1B; escape: ctl-[
    static const auto sSTX = std::string("\x02");     // 0x02; start of text: ctl-B
    static const auto sETX = std::string("\x03");     // 0x03; end of text: ctl-C
    static const auto sEOT = std::string("\x04");     // 0x04; end of transmission: ctl-D
    static const auto sSEMI = std::string(";");       // 0x3B; semicolon: semicolon

    static const auto MESSAGE_TERMINATOR = sCR+sLF;  // Originally it was just LF

    static constexpr auto CR = '\r';
    static constexpr auto LF = '\n';
    static constexpr auto FF = '\f';
    static constexpr auto BS = '\b';
    static constexpr auto HT = '\t';
    static constexpr auto VT = '\v';
    static constexpr auto SP = ' ';         // 0x20
    static constexpr auto DEL = '\x7F';
    static constexpr auto ESC = '\x1B';
    static constexpr auto STX = '\x02';
    static constexpr auto ETX = '\x03';
    static constexpr auto EOT = '\x04';
    static constexpr auto NULL_CHAR = '\0';
    static constexpr auto PLUS = '+';       // 0x2b
    static constexpr auto MINUS = '-';      // 0x2d
    static constexpr auto PERIOD = '.';     // 0x2e
    static constexpr auto COMMA = ',';      // 0x2c
    static constexpr auto COLON = ':';      // 0x3a
    static constexpr auto SEMICOLON = ';';  // 0x3b
    static constexpr auto SLASH = '/';      // 0x2f
    static constexpr auto EQUALS = '=';     // 0x3d
    static constexpr auto UNDERSCORE = '_'; // 0x5f
    static constexpr auto QUESTION = '?';   // 0x3f
    static constexpr auto AT = '@';         // 0x40
    
    // Comment out the following to disable debug mode within the comm stuff.
    // #define COMM_DEBUG_MODE
    
    /*
    enum class DisplayMode : uint8_t {
        STANDARD_DISPLAY,   // Show information independent of request. That is, send info.
        GAMMA_DISPLAY,      // Only show information as response to request.
        WRAPPED_GAMMA_DISPLAY, // GAMMA_DISPLAY wrapped in STX/ETX.
    };
    */

    class Communication {

        Communication() = default;  // Private constructor - everything is static!
        ~Communication() = default;


    public:

        // No copy constructor - no constructor.
        Communication(const Communication& other) = delete;
        Communication& operator=(const Communication& other) = delete;

        static bool hasCommandInterface() {
            return SerialComm::getActiveCommInterface() != CommInterface::NONE;
        }

        static void handleInputBuffer();

        // Output functions - automatically route to the active interface
        // This assumes the output has been prepped with STX/ETX/CRLF.
        static void serialOutput(const std::string& output);

        static void prepAndSendOutput(const std::string& output) {
            std::stringstream ss;
            prepStringForOutput(ss, output);
            serialOutput(ss.str());
        }

        /**
         *
         * @param ss This stream gets the adornments plus the message.
         * @param message This is just a string to output. We need to encapsulate in STX/ETX and terminate with CRLF.
         * @return A pointer to a new string with STX/ETX and CRLF.
         */
        static void prepStringForOutput(std::stringstream& ss, const std::string& message);

        static void bpOff() {bpState_ = false;}
        static void bpOn() {bpState_ = true;}
        static void breakPoint (const std::string &msg);
        static bool processDebugInputCharacter(char ch, std::string &inputBuffer);

    private:

        static bool processInputCharacter(char ch, std::string &inputBuffer);

        // State tracking

        static bool bpState_;   // break point state; This supports a 'user mode' debug. Not used with a real debugger.

    };

}
