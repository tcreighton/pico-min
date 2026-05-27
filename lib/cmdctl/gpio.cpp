
#include <hardware/gpio.h>
#include "gpio.hpp"
#include "board-config.hpp"

namespace CScmdctl {

    void Gpio::init() {
        // Initialize different pin groups
        initI2CPins();
        initInputPins();
        initOutputPins();
        
        // Set safe initial states
        setInitialOutputStates();

    }

    void Gpio::initI2CPins() {
        const auto& config = BOARD;
        
        // Initialize I2C pins
        gpio_init(config.i2c.c0_sda);
        gpio_init(config.i2c.c0_scl);
        gpio_init(config.i2c.c1_sda);
        gpio_init(config.i2c.c1_scl);

        // Set I2C function and pull-ups
        gpio_set_function(config.i2c.c0_sda, GPIO_FUNC_I2C);
        gpio_set_function(config.i2c.c0_scl, GPIO_FUNC_I2C);
        gpio_pull_up(config.i2c.c0_sda);
        gpio_pull_up(config.i2c.c0_scl);
        
        gpio_set_function(config.i2c.c1_sda, GPIO_FUNC_I2C);
        gpio_set_function(config.i2c.c1_scl, GPIO_FUNC_I2C);
        gpio_pull_up(config.i2c.c1_sda);
        gpio_pull_up(config.i2c.c1_scl);
    }

    void Gpio::initInputPins() {
        const auto& config = BOARD;
        
        /*
        // Only initialize input pins if the board has interlocks
        if (!config.capabilities.hasInterlocks) {
            return;
        }
        */

        // Always initialize. If they aren't used, that's fine.

        // Initialize input pins
//        gpio_init(config.inputs.door_interlock);
//        gpio_init(config.inputs.hv_enabled_interlock);
//        gpio_init(config.inputs.voltage_48v_enabled);
//        gpio_init(config.inputs.pump_interlock);
//        gpio_init(config.inputs.flow_interlock);
        gpio_init(config.inputs.system_shutdown);
        gpio_init(config.inputs.adc0);
        gpio_init(config.inputs.adc1);
        gpio_init(config.inputs.adc2);
        gpio_init(config.inputs.adc3);

        // Set input directions
//        gpio_set_dir(config.inputs.door_interlock, GPIO_IN);
//        gpio_set_dir(config.inputs.hv_enabled_interlock, GPIO_IN);
//        gpio_set_dir(config.inputs.voltage_48v_enabled, GPIO_IN);
//        gpio_set_dir(config.inputs.pump_interlock, GPIO_IN);
//        gpio_set_dir(config.inputs.flow_interlock, GPIO_IN);
        gpio_set_dir(config.inputs.system_shutdown, GPIO_IN);
        gpio_set_dir(config.inputs.adc0, GPIO_IN);
        gpio_set_dir(config.inputs.adc1, GPIO_IN);
        gpio_set_dir(config.inputs.adc2, GPIO_IN);
        gpio_set_dir(config.inputs.adc3, GPIO_IN);

        // Configure pull resistors as needed
        // Note: Pump interlock needs no pulls
//        gpio_set_pulls(config.inputs.pump_interlock, false, false);
        
        // Set default pulls for other inputs (adjust as needed based on hardware)
        // gpio_set_pulls(config.inputs.door_interlock, false, true);  // pull down
        // gpio_set_pulls(config.inputs.hv_enabled_interlock, false, true);  // pull down
        // gpio_set_pulls(config.inputs.voltage_48v_enabled, false, true);  // pull down
        // gpio_set_pulls(config.inputs.flow_interlock, false, true);  // pull down
    }

    void Gpio::initOutputPins() {
        const auto& config = BOARD;
        
        /*
        // Only initialize output pins if the board has HV control
        if (!config.capabilities.hasHVControl) {
            return;
        }
        */
        // Always initialize. If they aren't used, that's fine.

        // Initialize output pins
        gpio_init(config.outputs.filament_shutdown);
        gpio_init(config.outputs.tube_shutdown);

        // Set output directions

        gpio_set_dir(config.outputs.filament_shutdown, GPIO_OUT);
        gpio_set_dir(config.outputs.tube_shutdown, GPIO_OUT);

    }

    void Gpio::setInitialOutputStates() {

        if (!isSystemShutDownEnabled()) {
            startUpFilament();
            startUpTube();
        }
        /*
        if (const auto& config = BOARD;
            (config.capabilities.hasTubeControl &&
             config.capabilities.hasFilamentControl &&
             config.capabilities.hasSystemControl) &&
             !isSystemShutDownEnabled()) {
                startUpFilament();
                startUpTube();
            }
    */
    }

    void Gpio::setFilamentShutdown (const bool shutdown) {
        if (BOARD.capabilities.hasFilamentControl) {
            // ReSharper disable once CppDFAUnreachableCode
            gpio_put(BOARD.outputs.filament_shutdown, shutdown);
        }
    }

    void Gpio::setTubeShutdown(const bool shutdown) {
        if (BOARD.capabilities.hasTubeControl) {
            // ReSharper disable once CppDFAUnreachableCode
            gpio_put(BOARD.outputs.tube_shutdown, shutdown);
        }
    }

    bool Gpio::isSystemShutDownEnabled() {
        if (BOARD.capabilities.hasSystemControl) {
            // ReSharper disable once CppDFAUnreachableCode
            const auto retVal =  gpio_get(BOARD.inputs.system_shutdown);
            return retVal;
        }
        // This is here in case a board does not have system control. Override the optimization.
        // ReSharper disable once CppDFAUnreachableCode
        return false;
    }

    constexpr bool Gpio::boardHasUART0() {
        return BOARD.capabilities.hasUART0;
    }

    constexpr bool Gpio::boardHasUART1() {
        return BOARD.capabilities.hasUART1;
    }

    /*
    constexpr bool Gpio::boardHasInterlocks() {
        return BOARD.capabilities.hasInterlocks;
    }

    constexpr bool Gpio::boardHasHVControl() {
        return BOARD.capabilities.hasTubeControl;
    }
    */

    constexpr bool Gpio::boardHasUART() {
        // ReSharper disable once CppDFAUnreachableCode
        // ReSharper disable once CppRedundantBooleanExpressionArgument
        return boardHasUART0() || boardHasUART1();
    }

    /*
    constexpr BoardRevision Gpio::getBoardRevision() {
        return BOARD.revision;
    }
    */



} // namespace CScore
