#pragma once

#include "hardware/uart.h"

#include "gpio-declarations.hpp"

namespace CScmdctl {

    /**
     * Board revision enumeration
     */
    enum class BoardRevision : uint8_t {
        REV_0,  // First test board
        REV_A,  // Production board with corrected I2C mapping
        REV_B   // Future revision with additional changes
    };

// Board definition set in CMake.
#define SHOW_INFO_AT_STARTUP    // Shows startup info.

#if defined(BOARD_PICO_MIN_REV_A)
    constexpr auto CURRENT_BOARD = BoardRevision::REV_A;
    constexpr auto CURRENT_BOARD_NAME = "LFPico Min160 Rev A";
#elif defined(BOARD_PICO_MIN_REV_B)
    constexpr auto CURRENT_BOARD = BoardRevision::REV_B;
    constexpr auto CURRENT_BOARD_NAME = "Pico Min Rev B";
#else
    constexpr auto CURRENT_BOARD = BoardRevision::REV_0;
    constexpr auto CURRENT_BOARD_NAME = "Pico Min Rev 0";
#endif

    constexpr auto getBoardRevision() {return CURRENT_BOARD;}
    /**
     * Board capability flags
     */
    struct BoardCapabilities {
        bool hasEEPROM;
        bool hasFram;
        bool hasUART0;
        bool hasUART1;
        bool hasRS232;
        bool hasLed;
        bool hasSystemControl;
    };

    /**
     * I2C pin configuration
     */
    struct I2CConfig {
        uint8_t c0_sda{};
        uint8_t c0_scl{};
        uint8_t c1_sda{};
        uint8_t c1_scl{};
    };

    /**
     * UART instance identifiers
     */
    enum class UartId : uint8_t {
        UART0 = 0,
        UART1,
        RS232,
    };

    /**
     * Single UART configuration
     */
    struct SingleUARTConfig {
        uint8_t tx_pin{GPIO_INVALID};
        uint8_t rx_pin{GPIO_INVALID};
        uint32_t baud_rate{115200};
        bool enabled{false};
        bool commandHandler{false};     // Is this UART used for user commands?
        UartId uartId{UartId::UART0};   // Self-identifying. If you have a config, you can get the id.

        /**
         * Validate that the UART configuration is safe to use
         */
        [[nodiscard]] constexpr bool isValid() const {
            if (!enabled) return true;  // Disabled UARTs are always valid
            auto retCode = true;

            // Validate correct pins. Just check the possible pairs. If enabled, one of them must be true.
            switch (uartId) {
                case UartId::UART0:
                    retCode = GPIO_00 == tx_pin && GPIO_01 == rx_pin ||
                              GPIO_12 == tx_pin && GPIO_13 == rx_pin ||
                              GPIO_16 == tx_pin && GPIO_17 == rx_pin;
                    break;

                case UartId::UART1:
                    retCode = GPIO_04 == tx_pin && GPIO_05 == rx_pin ||
                              GPIO_08 == tx_pin && GPIO_09 == rx_pin;
                    break;

                default:
                    retCode = false;    // This would be really weird. But the initializer is nullptr, so it would fail.
                    break;
            }

            retCode &= (baud_rate > 0);     // Require non-zero baud rate.
            retCode &= (baud_rate <= 1000000); // No more than 1M baud rate.

            return retCode;
        }
    };


    /*******************
     * UARTConfig usage patterns:
     * Iterate over array
     *
     * for (int i = 0; i < 2; i++) {
     *   if (BOARD.uart[i].enabled) {
     *     initUART(static_cast<UARTId>(i), BOARD.uart[i]);
     *   }
     * }
     *
     * Specific UART access
     *
     * if (BOARD.capabilities.hasUART0) {
     *   initUART(UARTId::UART0, BOARD.uart.uartConfig0);
     * }
     *
     * if (BOARD.capabilities.hasUART1) {
     *   initUART(UARTId::UART1, BOARD.uart.uartConfig1);
     * }
    */


    /**
     * UART configuration for all UARTs
     */
    struct UARTConfig {
        SingleUARTConfig uartConfig0;
        SingleUARTConfig uartConfig1;

        // Convenience accessor for array-style access
        constexpr const SingleUARTConfig& operator[](const size_t index) const {
            return (index == 0) ? uartConfig0 : uartConfig1;
        }

        /**
         * Validate all UART configurations
         */
        [[nodiscard]] constexpr bool isValid() const {
            return uartConfig0.isValid() && uartConfig1.isValid();
        }


        /**
         * Get the UART designated as the command handler.
         * It prioritizes the first UART that is enabled and configured as a command handler.
         * Returns nullptr if no UART is configured as the command handler.
         */
        [[nodiscard]] constexpr const SingleUARTConfig* getCommandHandlerUART() const {
            if (uartConfig0.enabled && uartConfig0.commandHandler) {
                return &uartConfig0;
            }
            if (uartConfig1.enabled && uartConfig1.commandHandler) {
                return &uartConfig1;
            }
            return nullptr;
        }

        /**
         * Get the UARTId enum for the command handler
         * Returns UART0 if no command handler is configured (for backward compatibility)
         */
        [[nodiscard]] constexpr UartId getCommandHandlerId() const {
            const auto uartConfig = getCommandHandlerUART();
            return nullptr == uartConfig ? UartId::UART0 : uartConfig->uartId;
        }
    };


    /**
     * Input pins configuration (interlocks, sensors, etc.)
     */
    struct InputPins {
        uint8_t system_shutdown{};
        uint8_t adc0{};             // AINSEL_0
        uint8_t adc1{};             // AINSEL_1
        uint8_t adc2{};             // AINSEL_2
        uint8_t adc3{};             // AINSEL_3
    };

    /**
     * Output pins configuration (control signals)
     */
    struct OutputPins {
        uint8_t led{};
    };

    /**
     * Complete board configuration
     */
    struct BoardConfig {
        BoardRevision revision{CURRENT_BOARD};
        BoardCapabilities capabilities{};
        I2CConfig i2c{};
        UARTConfig uart{};
        InputPins inputs{};
        OutputPins outputs{};
    };

    /**
     * Board-specific configurations
     */
    namespace configs {

        constexpr BoardConfig REV_0_CONFIG = {
            .revision = BoardRevision::REV_0,
            .capabilities = {
                .hasEEPROM          = false,
                .hasFram            = false,
                .hasUART0           = false,
                .hasUART1           = false,
                .hasLed             = true
            },
            .i2c = {
                .c0_sda = GPIO_04,
                .c0_scl = GPIO_05,
                .c1_sda = GPIO_06,
                .c1_scl = GPIO_07
            },
            .uart = {
                .uartConfig0 = {
                    .tx_pin     = GPIO_12,
                    .rx_pin     = GPIO_13,
                    .baud_rate  = 115200,
                    .enabled    = true,
                    .commandHandler = true,
                    .uartId = UartId::UART0
                },
                .uartConfig1 = {
                    .tx_pin     = GPIO_INVALID,  // Not used on Rev 0
                    .rx_pin     = GPIO_INVALID,  // Not used on Rev 0
                    .baud_rate  = 115200,
                    .enabled    = false,
                    .commandHandler = false,
                    .uartId = UartId::UART1
                }
            },
            .inputs = {
                .system_shutdown        = GPIO_20,
                .adc0                   = GPIO_26,
                .adc1                   = GPIO_27,
                .adc2                   = GPIO_28,
                .adc3                   = GPIO_29
            },

            .outputs = {
                .led = GPIO_25,
            }
        };

        constexpr BoardConfig REV_A_CONFIG = {
            .revision = BoardRevision::REV_A,
            .capabilities = {
                .hasEEPROM          = false,
                .hasFram            = false,
                .hasUART0           = false,
                .hasUART1           = false,  // UART1 is rs232
                .hasLed             = true
            },
            .i2c = {
                .c0_sda = GPIO_04,
                .c0_scl = GPIO_05,
                .c1_sda = GPIO_06,
                .c1_scl = GPIO_07
            },
            .uart = {
                .uartConfig0 = {
                    .tx_pin     = GPIO_12,
                    .rx_pin     = GPIO_13,
                    .baud_rate  = 115200,
                    .enabled    = true,
                    .commandHandler = true,
                    .uartId = UartId::UART0
                },
                .uartConfig1 = {    // UART1 or RS232
                    .tx_pin     = GPIO_08,  // TX UART1/RS232
                    .rx_pin     = GPIO_09,  // RX UART1/RS232
                    .baud_rate  = 115200,
                    .enabled    = true,
                    .commandHandler = true,
                    .uartId = UartId::UART1
                }
            },
            .inputs = {
                .system_shutdown        = GPIO_20,
                .adc0                   = GPIO_26,
                .adc1                   = GPIO_27,
                .adc2                   = GPIO_28,
                .adc3                   = GPIO_29
            },
            .outputs = {
                .led = GPIO_25,
            }
        };

        constexpr BoardConfig REV_B_CONFIG = {
            .revision = BoardRevision::REV_B,
            .capabilities = {
                .hasEEPROM          = false,
                .hasFram            = false,
                .hasUART0           = false,
                .hasUART1           = false,
                .hasLed             = true
            },
            .i2c = {
                .c0_sda = GPIO_04,
                .c0_scl = GPIO_05,
                .c1_sda = GPIO_06,
                .c1_scl = GPIO_07
            },
            .uart = {
                .uartConfig0 = {
                    .tx_pin     = GPIO_12,  // Different from Rev A
                    .rx_pin     = GPIO_13,  // Different from Rev A
                    .baud_rate  = 115200,
                    .enabled    = true,
                    .commandHandler = false,
                    .uartId = UartId::UART0
                },
                .uartConfig1 = {
                    .tx_pin     = GPIO_08,  // New UART1
                    .rx_pin     = GPIO_09,  // New UART1
                    .baud_rate  = 9600,                   // Different baud rate for UART1
                    .enabled    = true,
                    .commandHandler = true,
                    .uartId = UartId::UART1
                }
            },
            .inputs = {
                .system_shutdown        = GPIO_20,
                .adc0                   = GPIO_26,
                .adc1                   = GPIO_27,
                .adc2                   = GPIO_28,
                .adc3                   = GPIO_29
            },
            .outputs = {
                .led = GPIO_25,
            }
        };

        // Compile-time validation of all board configurations
        static_assert(REV_0_CONFIG.uart.isValid(), "REV_0 UART configuration is invalid");
        static_assert(REV_A_CONFIG.uart.isValid(), "REV_A UART configuration is invalid");
        static_assert(REV_B_CONFIG.uart.isValid(), "REV_B UART configuration is invalid");
    }

    /**
     * Get the current board configuration
     */
    constexpr const BoardConfig& getCurrentBoardConfig() {
        // While the code for non-current config is always unreachable, we need the compiler to generate code
        // for all the possibilities. Otherwise when current board is changed, there will be nothing to return
        // the proper config!

        // ReSharper disable once CppDFAUnreachableCode
        switch (CURRENT_BOARD) {
            default:
            case BoardRevision::REV_0:
                return configs::REV_0_CONFIG;
            case BoardRevision::REV_A:
                return configs::REV_A_CONFIG;
            case BoardRevision::REV_B:
                return configs::REV_B_CONFIG;
        }
    }

    /**
     * Convenience accessors for current board
     */
    inline constexpr const BoardConfig& BOARD = getCurrentBoardConfig();


    /**
     * Utility functions for GPIO validation
     */
    constexpr bool isValidGPIOPin(const uint8_t pin) {
        return pin <= GPIO_28;
    }

    constexpr bool isInvalidGPIOPin(const uint8_t pin) {
        return pin == GPIO_INVALID;
    }

    /**
     * Runtime utility to get the hardware UART pointer from UARTId
     * This can't be constexpr because uart0/uart1 are runtime pointers
     */
    inline uart_inst_t* getUartHardware(const UartId uartId) {
        return (uartId == UartId::UART0) ? uart0 : uart1;
    }

    // This initializes the low-level devices: gpio, i2c, communication.
    // All of these need to be in place before pretty much anything else.
    // There is a little bit of confusion since this centralization of the init functions happened late in the game.
    // This is called from main.
    bool boardInit ();


} // namespace CSboard

