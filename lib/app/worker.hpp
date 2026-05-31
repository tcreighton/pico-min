#pragma once


#include "command-handler.hpp"
#include "state-machine.hpp"

namespace CSapp {

    class Worker final {
    public:
        Worker () = default;
        ~Worker() = default;

        bool doWork();


    private:
        StateMachine stateMachine_{};
    };
};

