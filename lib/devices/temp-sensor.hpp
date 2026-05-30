#pragma once

#include <utility>

#include "factory-data.hpp"

namespace CSdevices {

    class TempSensor {  // An abstract base class for temperature sensors
    public:
        explicit TempSensor (const CScore::TempSensor_t tempSensorId) : tempSensorId_(tempSensorId){

        }

        virtual ~TempSensor () = default;


        /**
         * @brief This function turns the counts taken from the adc and processes the value to produce what the
         * sensor is trying to measure. This interface doesn't know how to do that conversion
         * @param counts Counts were read in readValue. It's the result of an ADC conversion.
         * @return The sensor's value (counts) converted to sensor-centric units, eg. temperature, pressure...
         *
         */
        virtual double processCounts (uint16_t counts) = 0;

        /**
         * @brief These call processCounts so can't be const. Allows processing only on demand.
         * @return The cached temperature in units C.
         */
        virtual double getTemperatureInC () {
            processCounts(getCounts());
            return temperature_;
        };
        virtual double getCachedTemperatureInF () {
            processCounts(getCounts());
            return temperature_ * 9/5 + 32;
        };
        virtual double getCachedTemperatureInK () {
            processCounts(getCounts());
            return temperature_ + REF_K;
        };

        /**
         * @brief This returns the cached temperature. That gets updated whenever readValue is called.
         * @return The cached temperature.
         */
        virtual double getTemperature () {
            return getTemperature(CScore::DEFAULT_TEMP_UNITS);
        }

        virtual double getTemperature (const CScore::Unit_t units) {
            switch (units) {
                case CScore::Unit_t::CELSIUS:
                    return getTemperatureInC();
                case CScore::Unit_t::FAHRENHEIT:
                    return getCachedTemperatureInF();
                case CScore::Unit_t::KELVIN:
                    return getCachedTemperatureInK();
                default:
                    std::unreachable(); // This kills!
            }
        }

        [[nodiscard]] virtual uint16_t getCounts() const {
            return counts_;
        }

        virtual void setCounts (const uint16_t counts) {
            counts_ = counts;
        }

        virtual bool completeConversion () = 0;

        [[nodiscard]] CScore::TempSensor_t getSensorId () const {return tempSensorId_;}
        [[nodiscard]] virtual absolute_time_t getConversionStartTime () const {return conversionStartTime_;}
        virtual void setConversionStartTime (const absolute_time_t startTime) {conversionStartTime_ = startTime;}

    protected:

        void setTemperature (const double temperature, const auto units) {
            switch (units) {
                case CScore::Unit_t::CELSIUS:
                    temperature_ = temperature;     // default is CELSIUS.
                    return;
                case CScore::Unit_t::FAHRENHEIT:
                    temperature_ = (temperature - 32) * 5/9;
                    return;
                case CScore::Unit_t::KELVIN:
                    temperature_ = temperature - REF_K;
                    return;
                default:
                    std::unreachable(); // This kills!
            }
        }

        [[nodiscard]] virtual double getRawCachedTemperature() const {
            return temperature_;
        }

        uint16_t    counts_ = 0;
        double      temperature_ = 0.0;    // This value is always in Celsius units.

        CScore::TempSensor_t    tempSensorId_{};
        absolute_time_t conversionStartTime_{}; // Used for logging stats.

        constexpr static auto REF_K = 273.15;                         // 32F == 0C == 273.15K
    };
}

