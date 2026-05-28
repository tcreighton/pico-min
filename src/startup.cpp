
#include <cstdlib>
#include <sstream>

#include "board-config.hpp"
#include "serial-comm.hpp"
#include "startup.hpp"

#include "communication.hpp"
#include "product-info.hpp"

using namespace CScmdctl;
using namespace CScore;

void startup() {

    // Ignore the return value. We are neither displaying it nor executing logic based on it.
    // This call will also initialize the UART0.
    if (!boardInit()) {
        std::exit(1000); // bad day!
    }
    auto connectTime = SerialComm::getUsbConnectTime(); // If disabled, should be 0.
    const auto uartActualBaudRate = SerialComm::getCommandUartBaudRate();

//    sleep_ms(2 * 1000);

#if defined SHOW_INFO_AT_STARTUP

    std::stringstream ss;
    ss << "\r\n\n****************************************************************\n";
    Communication::prepAndSendOutput(ss.str());
    ss = std::stringstream();

    if (SerialComm::isUsbEnabled()) {
        ss << "USB enabled.";
    }
    if (SerialComm::isUartEnabled()) {
        ss << "UART enabled @ "+ std::to_string(uartActualBaudRate) + " baud. ";
    }
    Communication::prepAndSendOutput(ss.str());

    ss = std::stringstream();
    ss << "Pico Min initializing. USB connect time about " << connectTime << " milliseconds.";
    Communication::prepAndSendOutput(ss.str());
    ss = std::stringstream();
    ss << "Pico Min ID: " << getPicoBoardId();
    Communication::prepAndSendOutput(ss.str());
    ss = std::stringstream();
    auto [seconds, minutes, hours, day, month, year]
                        = PackedDateTime::getUnpackedDateTime(PackedDateTime::getPackedBuildDateTime());
    ss << "Build: " << getFirmwareVersion();
    Communication::prepAndSendOutput(ss.str());

    ss = std::stringstream();

    ss << "Current board name: " << CURRENT_BOARD_NAME;
    auto s = ss.str();
    Communication::prepAndSendOutput(s);

    ss = std::stringstream();
    ss << "\r\n\n****************************************************************\n";
    s = ss.str();
    Communication::prepAndSendOutput(s);
#endif


}
