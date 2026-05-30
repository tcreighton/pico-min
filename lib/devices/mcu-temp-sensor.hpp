#pragma once

#include "pico-adc.hpp"
#include "temp-sensor.hpp"
#include "pico/time.h"

namespace CSdevices {

    class MCUTempSensor final : public PicoAdc, public TempSensor {
    public:
        // To use this with the QFN-80, pass in PICO_AINSEL_8.
        explicit MCUTempSensor() :
                                    PicoAdc(PicoAin::PICO_AINSEL_4),
                                    TempSensor(CScore::TempSensor_t::MCU_SENSOR) {
        }

        ~MCUTempSensor() override = default;
        MCUTempSensor(const MCUTempSensor&) = delete;
        MCUTempSensor& operator=(const MCUTempSensor&) = delete;

        void startConversion () override {
            PicoAdc::startConversion();
            setConversionStartTime(get_absolute_time());

        };
        bool completeConversion () override {
            //Stats::logTemperatureConversionTime(getSensorId(), getConversionStartTime(), get_absolute_time());
            return PicoAdc::completeConversion();
        };
        [[nodiscard]] uint16_t getCounts() const override;
        void setCounts (uint16_t counts) override;

        /**
         *
         * @param counts This is the value returned from the ADC.
         * @return The temperature read by the onboard temperature sensor.
         */
        double processCounts (uint16_t counts) override;

    };

}

