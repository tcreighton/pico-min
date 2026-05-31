
#include "gpio.hpp"
#include "response-handler.hpp"
#include "worker.hpp"


namespace CSapp {

    bool Worker::doWork() {


        if (CScmdctl::Gpio::isSystemShutDownEnabled()) {
            RECORD_ERROR_FIVE(CScmdctl::ResponseCode::APP_SHUTDOWN_IN_PROGRESS, CScmdctl::ErrorSeverity::CRITICAL,
                             "Worker", __func__, "System shutdown in progress.");
            return false;
        }

        return stateMachine_.visitCurrentState();

    }
}
