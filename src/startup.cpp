
#include <cstdlib>
#include <sstream>

#include "board-config.hpp"
#include "serial-comm.hpp"
#include "startup.hpp"

#include "communication.hpp"
#include "drivers-container.hpp"
#include "product-info.hpp"
#include "powersupply/powersupply-container.hpp"

using namespace CScmdctl;
using namespace CSdevices;

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
    Communication::serialOutputLine(ss.str());
    ss = std::stringstream();

    if (SerialComm::isUsbEnabled()) {
        ss << "USB enabled.";
    }
    if (SerialComm::isUartEnabled()) {
        ss << "UART enabled @ "+ std::to_string(uartActualBaudRate) + " baud. ";
    }
    Communication::serialOutputLine(ss.str());

    ss = std::stringstream();
    ss << "Lumafield 160Kv initializing. USB connect time about " << connectTime << " milliseconds.";
    Communication::serialOutputLine(ss.str());
    ss = std::stringstream();
    ss << "Lumafield 160Kv Power Supply ID: " << getPicoBoardId();
    Communication::serialOutputLine(ss.str());
    ss = std::stringstream();
    auto [seconds, minutes, hours, day, month, year]
                        = PackedDateTime::getUnpackedDateTime(PackedDateTime::getPackedBuildDateTime());
    ss << "Build: " << getFirmwareVersion();
    Communication::serialOutputLine(ss.str());

    ss = std::stringstream();

    ss << "Current board name: " << CURRENT_BOARD_NAME;
    auto s = ss.str();
    Communication::serialOutputLine(s);

    ss = std::stringstream();
    ss << "\r\n\n****************************************************************\n";
    s = ss.str();
    Communication::serialOutputLine(s);
#endif

    auto& ps1 = CSdrivers::getPowerSupply(PowerSupplyId_t::FILAMENT);
    auto& ps2 = CSdrivers::getPowerSupply(PowerSupplyId_t::TUBE);

    ps1.deviceInit();
    ps2.deviceInit();

}
