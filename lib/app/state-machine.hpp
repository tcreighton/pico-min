#pragma once

#include "command-handler.hpp"

namespace CSapp {

    enum class State: uint8_t {
        INITIAL_STATE = 0,
        STATE_ONE,
        STATE_TWO,
        STATE_THREE,
        STATE_FOUR,
        STATE_FIVE,
        STATE_SIX,
        STATE_SEVEN,
        STATE_EIGHT,
        STATE_NINE,
        /*
        */
        FINAL_STATE
    };

    class StateMachine {
    public:
        StateMachine() = default;
        ~StateMachine() = default;

        [[nodiscard]] State getCurrentState() const {return currentState_;}
        bool visitCurrentState();

        static bool initialState ();
        bool stateOne ();
        bool stateTwo ();
        bool stateThree ();
        bool stateFour ();
        bool stateFive ();
        bool stateSix ();
        bool stateSeven ();
        bool stateEight ();
        bool stateNine ();
        bool finalState ();

    private:
        void setCurrentState(const State state) {currentState_ = state;}

        State currentState_{State::INITIAL_STATE};

    };

}

