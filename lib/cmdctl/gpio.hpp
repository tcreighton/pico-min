#pragma once

#include <hardware/gpio.h>

namespace CScmdctl {

    /**
     * GPIO management class that handles board-specific pin configurations
     */
    class Gpio final {

    private:
        Gpio() = default;
        ~Gpio() = default;

    public:
        /**
         * Initialize GPIO pins based on the current board configuration
         * Note: UART pins are initialized separately in Communication::initUart()
         */
        static void init();

        /**
         * Initialize only I2C pins - can be called independently
         */
        static void initI2CPins();

        /**
         * Initialize only input pins - can be called independently
         */
        static void initInputPins();

        /**
         * Initialize only output pins - can be called independently
         */
        static void initOutputPins();

        // Input pin reading functions
        // This changes based on the compile-time definition of BOARD.
        // As such, it's not always going to return false.
//        static bool isDoorClosed();

        // This changes based on compile-time definition of BOARD.
        // As such, it's not always going to return false.
//        static bool isHVEnabled();

        // This changes based on compile-time definition of BOARD.
        // As such, it's not always going to return false.
//        static bool is48VoltEnabled();

        // This changes based on compile-time definition of BOARD.
        // As such, it's not always going to return false.
//        static bool isIonPumpEnabled();

        // This changes based on compile-time definition of BOARD.
        // As such, it's not always going to return false.
//        static bool isFlowEnabled();

        // Filament control functions
        static void shutDownFilament() { setFilamentShutdown(true); }
        static void startUpFilament() { setFilamentShutdown(false); }
        static void setFilamentShutdown (bool shutdown);

        // Tube control functions
        static void shutDownTube() { setTubeShutdown(true); }
        static void startUpTube() { setTubeShutdown(false); }
        static void setTubeShutdown(bool shutdown);

        // System shutdown control functions
        static bool isSystemShutDownEnabled ();

        static constexpr bool boardHasUART0();
        static constexpr bool boardHasUART1();
        /*
        static constexpr bool boardHasInterlocks();
        static constexpr bool boardHasHVControl();
        */

        // Utility functions for board information
        static constexpr bool boardHasUART();


    private:
        /**
         * Set initial safe states for all output pins
         */
        static void setInitialOutputStates();
    };

} // namespace CScore

