#pragma once

#include <numbers>
#include <cstdint>
#include <string_view>

/**
* The purpose of this file is to consolidate constant declarations. It's not going to handle everything,
* but for sure the data that represents configuration and control data. Why do it this way, instead of each
* power supply class doing its own? This makes it so the base class  PowerSupply as well as reports, etc.
* can have access to config data as defined by 'factory', or initial values. Otherwise, we have to write
* pure virtual method declarations in PowerSupply, that are implemented in the subclass, that can provide access
* to the defined constants. It works but is tedious. Since that was already done by the time this was created,
* there may be some of both approaches for a bit.
*/
namespace CScore {

    enum class TempSensor_t : std::uint8_t {
        MCU_SENSOR = 0,     // MCU internal temperature sensor
    };


    enum class Unit_t : std::uint8_t {
        AMPS = 0,
        COUNTS,
        KILOVOLTS,
        MILLIVOLTS,
        MICROVOLTS,
        MILLIAMPS,
        MICROAMPS,
        VOLTS,
        CELSIUS,
        FAHRENHEIT,
        KELVIN,
        UNKNOWN
    };

    constexpr auto DEFAULT_TEMP_UNITS = Unit_t::CELSIUS;

    inline std::string_view unitToString(const Unit_t unit) {
        switch (unit) {
            case Unit_t::AMPS: return "Amps";
            case Unit_t::COUNTS: return "Counts";
            case Unit_t::KILOVOLTS: return "kV";
            case Unit_t::MILLIVOLTS: return "mV";
//            case Unit_t::MICROVOLTS: return "μv";
            case Unit_t::MICROVOLTS: return "uV";   // mu (2 bytes) is not necessarily easy to handle for all clients.
            case Unit_t::MILLIAMPS: return "mA";
//            case Unit_t::MICROAMPS: return "μa";
            case Unit_t::MICROAMPS: return "uA";   // mu (2 bytes) is not necessarily easy to handle for all clients.
            case Unit_t::VOLTS: return "Volts";
            case Unit_t::CELSIUS: return "Celsius";
            case Unit_t::FAHRENHEIT: return "Fahrenheit";
            case Unit_t::KELVIN: return "Kelvin";
            default:
            case Unit_t::UNKNOWN: return "Unknown";
        }
    }

    constexpr double PI = std::numbers::pi;   // Every engineering program needs PI.

}

