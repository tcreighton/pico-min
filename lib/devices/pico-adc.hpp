#pragma once

#include <cstdint>
#include "hardware/adc.h"


namespace CSdevices {

    enum class PicoAin: uint8_t {
        PICO_AINSEL_0   = 0x00,     // 2040/2350A QFN-60 GPIO26 or 2350B QFN-80 GPIO40
        PICO_AINSEL_1   = 0x01,     // 2040/2350A QFN-60 GPIO27 or 2350B QFN-80
        PICO_AINSEL_2   = 0x02,     // 2040/2350A QFN-60 GPIO28 or 2350B QFN-80
        PICO_AINSEL_3   = 0x03,     // 2040/2350A QFN-60 GPIO29 or 2350B QFN-80
        PICO_AINSEL_4   = 0x04,     // 2040/2350A QFN-60 N/A    or 2350B QFN-80 (temperature sensor)
        PICO_AINSEL_5   = 0x05,     // 2350B QFN-80 GPIO44
        PICO_AINSEL_6   = 0x06,     // 2350B QFN-80 GPIO45
        PICO_AINSEL_7   = 0x07,     // 2350B QFN-80 GPIO46
        PICO_AINSEL_8   = 0x08,     // 2350B QFN-80 GPIO47
        PICO_AINSEL_9   = 0x09      // 2350B QFN-80 N/A    (temperature sensor)
    };

    class PicoAdc  {

    public:
        explicit PicoAdc (const PicoAin ainSelect) : ain_(ainSelect) {


        }

        ~PicoAdc () = default;

        /**
         * @brief Begins a conversion process using the AIN in ain_.
         * This does not block. It selects the device and then starts the conversion. Then immediately returns.
         * Note that all the ADCs (0-8) are essentially, if not actually, channels. Only one can be doing a conversion
         * at one time. So you can't start many and then expect to go back and collect the data. To do that you
         * need to code for the continuous conversion and use a FIFO or handle an interrupt to collect results.
         * These devices finish conversions in about 2 microseconds.
         */
        virtual void startConversion ();

        /**
         * @brief This will block until the one-shot conversion completes.
         * Note that the actual result will be 12 bits.
         * This needs to be 'processed' to something that represents the measurement being made by the sensor.
         * @return Returns the result of an ADC conversion. Result comes from the adc register.
         */
        virtual bool completeConversion ();
        [[nodiscard]] virtual uint16_t getCounts() const = 0;
        virtual void setCounts (uint16_t counts) = 0;

        /**
         * @brief Tests if the current conversion is complete. This does not block.
         * @return Returns true if there is data in the adc register. Ie when conversion is complete.
         */
        static bool isAdcReady() {return (adc_hw->cs & ADC_CS_READY_BITS) != 0;}

        static void waitUntilAdcIsReady() {
            while (!isAdcReady()) {
                tight_loop_contents();
            }
        }

        static PicoAin getCurrentlySelectedAin () {
            const auto ain = static_cast<PicoAin>(adc_hw->cs & ADC_CS_AINSEL_BITS);
            return ain;
        }

    protected:
        [[nodiscard]] PicoAin getAin() const {return ain_;};


        // Constants used in conversion from voltage read from the ADC pin to a temperature.
        // Also, some constants are used in the conversions for the external thermistors.
        // See section 12.4.6 of the 2350 datasheet.
        // Vbe at 27° C is typically 0.706v.
        // The slope is -1.721mV/°C

        static constexpr double VREF                        = 3.0f;         // This is different from the spec's 3.3v
        static constexpr double BIT_RANGE                   = 0x1000;       // 12 bit adcs
        static constexpr double VOLTAGE_CONVERSION_RATIO    = VREF / BIT_RANGE; // The LSB size. How many volts per bit.

    private:

        PicoAin ain_;
        uint16_t counts_ = 0;

    };


}

