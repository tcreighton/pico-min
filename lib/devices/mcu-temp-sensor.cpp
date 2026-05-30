
#include "pico-adc.hpp"
#include "mcu-temp-sensor.hpp"

namespace CSdevices {


    double MCUTempSensor::processCounts(const uint16_t counts) { // calculates in celsius
        static constexpr double REFERENCE_TEMP          = 27.0;     // The temp used by RP to characterize this
        static constexpr double REFERENCE_TEMP_VOLTS    = 0.706;    // The voltage at reference temperature 27°
        static constexpr double DEGREE_VOLTS_SLOPE      = 0.001721; // The rate of change of voltage per degree
        const double adcOutputVoltage = static_cast<double>(counts) * VOLTAGE_CONVERSION_RATIO;
        const double temp =
                    REFERENCE_TEMP - (adcOutputVoltage - REFERENCE_TEMP_VOLTS) / DEGREE_VOLTS_SLOPE;
        setTemperature(temp, CScore::DEFAULT_TEMP_UNITS);
        return temp;
    }

    uint16_t MCUTempSensor::getCounts() const {
        return TempSensor::getCounts();
    }

    void MCUTempSensor::setCounts(uint16_t counts) {
        TempSensor::setCounts(counts);
    }
}
