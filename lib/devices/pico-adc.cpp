
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/regs/adc.h" // For direct register access if needed
#include "pico-adc.hpp"


namespace CSdevices {

    void PicoAdc::startConversion() {
        // This while loop is defensive programming. If somehow this gets called while a conversion is in process,
        // this will force a wait for it to complete. It shouldn't happen, but in the future, interrupt handling
        // or multi-core programming could change it.

        waitUntilAdcIsReady();

        adc_select_input(static_cast<std::underlying_type_t<PicoAin>>(getAin()));   // Select ADC channel
        hw_set_bits(&adc_hw->cs, ADC_CS_START_ONCE_BITS);   // Set for 1 conversion.
                                                            // Auto-clears the bit when done. (2us).
                                                            // Non-blocking!
    }

    /**
     * @brief This always returns true.
     * It used to return counts. But now just true to match signature of TempSensor::completeConversion().
     * @return true.
     */
    bool PicoAdc::completeConversion() {
        waitUntilAdcIsReady();

        setCounts(static_cast<uint16_t>(adc_hw->result & 0xfff)); // retrieve 12-bit result.

        return true;
    }

}

