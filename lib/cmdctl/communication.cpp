
#include <iostream>
#include <class/cdc/cdc_device.h>
#include <device/usbd.h>

#include "commands.hpp"
#include "communication.hpp"
#include "serial-comm.hpp"
#include "shutdown.hpp"
#include "pystring.h"


namespace CScmdctl {

    // Static member definitions
//    DisplayMode Communication::displayMode_ = DisplayMode::GAMMA_DISPLAY;
    bool Communication::bpState_ = false;

    /**
     * Extract potential commands from the buffer.
     */
    void Communication::handleInputBuffer() {
        static std::string inputBuffer;
        std::string commandString;

        switch (SerialComm::getActiveCommInterface()) {

            case CommInterface::USB:
                tud_task();
                while (tud_cdc_available()) {
                    // Because the while loop quits when no characters are in the tud input buffer,
                    // we can use blocking read_char
                    if (const char ch = tud_cdc_read_char(); processInputCharacter(ch, inputBuffer)) {
                        // If we return true, we have a termination character.
                        // We need to process the command string.
                        // If we are in SHUTDOWN mode, it means we don't record a command.
                        // In SHUTDOWN mode, when processInputCharacter returns true, we just call doRestart
                        if (runMode_ == RunMode::SHUTDOWN) {
                            doRestart();
                        }
                        commandString = inputBuffer;
                        recordCommands(commandString);
                        inputBuffer.clear();
                        return; // Process one command at a time.
                    }
                }
                break;

            case CommInterface::UART: {
                uart_inst_t* uart_hw = SerialComm::getCommandUartHardware();  // Could be nullptr
                // ReSharper disable once CppDFAConstantConditions
                while (nullptr != uart_hw && uart_is_readable(uart_hw)) { // compiler options could make this nullptr
                    if (const char ch = uart_getc(uart_hw); processInputCharacter(ch, inputBuffer)) {
                        // If we return true, we have a termination character.
                        // We need to process the command string.
                        commandString = inputBuffer;
                        recordCommands(commandString);
                        inputBuffer.clear();
                        return; // Process one command at a time.
                    }
                }
            }
                break;

            case CommInterface::NONE: // no active interface - skip it.
                break;
        }
    }

    // If a termination character was found, we return true, else false.
    bool Communication::processInputCharacter(const char ch, std::string& inputBuffer) {
        bool retVal = false;

        switch (ch) {
            case LF:
            case CR:
                retVal = true;
                break;
            case BS:  // backspace
                if (!inputBuffer.empty()) {
                    inputBuffer.pop_back();
                }
                break;
            default:
                if (ch == SP || ch == PLUS || ch == MINUS || ch == PERIOD || ch == SEMICOLON ||
                    ch == EQUALS || ch == UNDERSCORE ||
                    (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
                    (ch >= '0' && ch <= '9')) {
                        inputBuffer += ch;
                    }
                break;
        }
        return retVal;
    }

    void Communication::serialOutput(const std::string &output) {
        // Send to USB if connected - ie if UART is not enabled.

        switch (SerialComm::getActiveCommInterface()) {
            case CommInterface::USB:
                std::cout << output << std::flush;
                break;

            case CommInterface::UART:
                // ReSharper disable once CppDFAConstantConditions
                if (uart_inst_t* uart_hw = SerialComm::getCommandUartHardware(); nullptr != uart_hw) {
                    uart_puts(uart_hw, output.c_str());
                }
                break;

            case CommInterface::NONE:
                // No output interface available - could log error or buffer
                break;
        }
    }

    void Communication::serialOutputLine(const std::string& output) {
        std::stringstream ss;
        ss << output << sCR << sLF;
        serialOutput(ss.str());
    }


    void Communication::serialOutputLF160(const std::string &msg) {
        std::stringstream ss;
        // First make sure that all LF turn into CRLF. Shouldn't hurt Linux, but a must for MS and for ftdi
        for (auto& ch : msg) {
            if ('\n' == ch) {
                ss << '\r';
            }
            ss << ch;   // This could give CRCRLF. Should not hurt.
        }
        std::string const msg2 = ss.str();
        ss = std::stringstream();

        ss << sSTX << msg2 << sETX << MESSAGE_TERMINATOR;
        std::string const s = ss.str();
        serialOutput(s);
    }

    void Communication::breakPoint(const std::string& msg) {
        if (!bpState_) {
            return;
        }
        std::stringstream ss;
        ss << "BP: " << msg << "\nEnter 'D' to disable BP; 'R' to run to next BP. " << std::flush;
        serialOutput(ss.str());
        // Now wait for user to press enter
        std::string inputBuffer;
        char ch;

        do {
            ch = tud_cdc_read_char();
        } while (!processDebugInputCharacter(ch, inputBuffer));

        std::string::iterator it = std::find_if(inputBuffer.begin(), inputBuffer.end(), [](unsigned char c)
            { return !std::isspace(c); });

        if (it != inputBuffer.end()) {
            ch = toupper(*it);
            if (ch == 'D') {
                bpOff();
            }
        }

    }

    bool Communication::processDebugInputCharacter(const char ch, std::string &inputBuffer) {
        auto retVal = false;

        switch (ch) {
            case LF:
            case CR:
                retVal = true;
                break;
            case BS:  // backspace
                if (!inputBuffer.empty()) {
                    inputBuffer.pop_back();
                }
                break;
            default:
                if (ch == SP || ch == PLUS || ch == MINUS || ch == PERIOD || ch == SEMICOLON ||
                    ch == EQUALS || ch == UNDERSCORE ||
                    (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
                    (ch >= '0' && ch <= '9')) {
                    inputBuffer += ch;
                    }
                break;
        }
        return retVal;
    }


    void Communication::recordCommands(const std::string &commandString) {
        std::vector<std::string> tokens;
        pystring::split(commandString, tokens, sSEMI);
        for (const auto& token : tokens) {
            Command::recordCommandString(token);
        }
    }
}
