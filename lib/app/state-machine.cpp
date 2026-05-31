
#include "state-machine.hpp"

#include "command.hpp"
#include "communication.hpp"
#include "devices-container.hpp"

using namespace CScore;

namespace CSapp {

    bool StateMachine::visitCurrentState() {
        auto retValue = false;

        switch (getCurrentState()) {
            case State::INITIAL_STATE:
                retValue = initialState();
                if (retValue) {
                    setCurrentState(State::STATE_ONE);
                }
                break;

            case State::STATE_ONE:
                retValue = stateOne();
                if (retValue) {
                    setCurrentState(State::STATE_TWO);
                }
                break;

            case State::STATE_TWO:
                retValue = stateTwo();
                if (retValue) {
                    setCurrentState(State::STATE_THREE);
                }
                break;

            case State::STATE_THREE:
                retValue = stateThree();
                if (retValue) {
                    setCurrentState(State::STATE_FOUR);
                }
                break;

            case State::STATE_FOUR:
                retValue = stateFour();
                if (retValue) {
                    setCurrentState(State::STATE_FIVE);
                }
                break;

            case State::STATE_FIVE:
                retValue = stateFive();
                if (retValue) {
                    setCurrentState(State::STATE_SIX);
                }
                break;

            case State::STATE_SIX:
                retValue = stateSix();
                if (retValue) {
                    setCurrentState(State::STATE_SEVEN);
                }
                break;

            case State::STATE_SEVEN:
                retValue = stateSeven();
                if (retValue) {
                    setCurrentState(State::STATE_EIGHT);
                }
                break;

            case State::STATE_EIGHT:
                retValue = stateEight();
                if (retValue) {
                    setCurrentState(State::STATE_NINE);
                }
                break;

            case State::STATE_NINE:
                retValue = stateNine();
                if (retValue) {
                    setCurrentState(State::STATE_ONE);
                }
                break;

            case State::FINAL_STATE:
                retValue = finalState();
                break;
        }

        return retValue;
    }

    bool StateMachine::initialState() {
        auto retValue = CSdevices::getMCUTempSensor().init();

        return retValue;
    }

    /// @brief StateOne reads all temperature sensors.
    bool StateMachine::stateOne() {
        auto retValue = true;
//        CSdevices::logger_.log(CSdevices::LogLevel::Trace, "State One");

        retValue &= CScmdctl::Command::doCommand();

        auto& mcuTemp = CSdevices::getMCUTempSensor();
        mcuTemp.startConversion();

        return retValue;
    }

    bool StateMachine::stateTwo() {
//        CSdevices::logger_.log(CSdevices::LogLevel::Trace, "State Two");

        auto& mcuTemp = CSdevices::getMCUTempSensor();  // Always need to complete before the next one starts!
        mcuTemp.completeConversion(); // Important is just to call completeConversion.

        const auto retValue = CScmdctl::Command::doCommand();

        return retValue;
    }

    bool StateMachine::stateThree() {
//        CSdevices::logger_.log(CSdevices::LogLevel::Trace, "State Three");

        const auto retValue = CScmdctl::Command::doCommand();

        return retValue;
    }

    /// @brief StateFour initiates adc conversions for anode voltage and current
    /// @return true if everything works in this state.
    bool StateMachine::stateFour() {
//        CSdevices::logger_.log(CSdevices::LogLevel::Trace, "State Four");

        const auto retValue = CScmdctl::Command::doCommand();

        return retValue;

    }

    bool StateMachine::stateFive() {
//        CSdevices::logger_.log(CSdevices::LogLevel::Trace, "State Five");

        const auto retValue = CScmdctl::Command::doCommand();

        return retValue;
    }

    bool StateMachine::stateSix() {
//        CSdevices::logger_.log(CSdevices::LogLevel::Trace, "State Six");

        const auto retValue = CScmdctl::Command::doCommand();

        return retValue;
    }

    bool StateMachine::stateSeven() {
//        CSdevices::logger_.log(CSdevices::LogLevel::Trace, "State Seven");

        const auto retValue = CScmdctl::Command::doCommand();

        return retValue;
    }

    bool StateMachine::stateEight() {
//        CSdevices::logger_.log(CSdevices::LogLevel::Trace, "State Eight");

        const auto retValue = CScmdctl::Command::doCommand();

        return retValue;
    }

    bool StateMachine::stateNine() {

        const auto retValue = CScmdctl::Command::doCommand();

        return retValue;
    }

    /// @brief finalState starts the shut-down process.
    /// @return true if everything works in this state.
    bool StateMachine::finalState() {
        auto retValue = true;

        return retValue;
    }

}
