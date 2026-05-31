#pragma once

#include <string>

#include "communication.hpp"
#include "error-context.hpp"

namespace CScmdctl {


    class ResponseHandler final {
    public:

        // Record an error context
        static void recordError(const ErrorContext& context) {
            errorStack_.push(context);

            // Set critical flag if needed.
            criticalErrorOccurred_ = context.isCritical();
        }

        // Quick record for simple cases
        static void recordError(const ResponseCode code,
                               const ErrorSeverity severity,
                               const std::string& message = "") {
            ErrorContext ctx;
            ctx.set(code, severity, message);
            recordError(ctx);
        }

        // Check if emergency shutdown needed
        [[nodiscard]] static bool hasCriticalError() {
            return criticalErrorOccurred_;
        }

        // Process all errors - called from main loop
        static void processAllErrors(const bool verboseMode = false) {
            while (!errorStack_.empty()) {
                ErrorContext ctx = errorStack_.top();
                errorStack_.pop();

                if (verboseMode || ctx.isVerbose()) {
                    Communication::prepAndSendOutput(ctx.getDetailedMessage());
                } else {
                    Communication::prepAndSendOutput(ctx.getClientMessage());
                }
            }
        }
        // Clear critical flag after handling shutdown
        static void clearCriticalFlag() {
            criticalErrorOccurred_ = false;
        }

        [[nodiscard]] static bool hasErrors() {
            return !errorStack_.empty();
        }

        static bool setVerboseMode(const bool verboseMode) {
            verboseMode_ = verboseMode;
            return verboseMode_;
        }

        static bool isVerboseMode() { return verboseMode_; }

    private:

        inline static std::stack<ErrorContext> errorStack_;
        inline static bool criticalErrorOccurred_{false};
        inline static bool verboseMode_{false};
    };

    // Convenience macro for detailed error recording
#define RECORD_ERROR_THREE(code, severity, message) \
    do { \
        CScmdctl::ErrorContext ctx; \
        ctx.set((code), (severity), (message)); \
        CScmdctl::ResponseHandler::recordError(ctx); \
    } while(0)

// five parameters
#define RECORD_ERROR_FIVE(code, severity, className, function, message) \
    do { \
        CScmdctl::ErrorContext ctx; \
        ctx.set((code), (severity), (className), (function), (message)); \
        CScmdctl::ResponseHandler::recordError(ctx); \
    } while(0)

// six parameters
#define RECORD_ERROR_SIX(code, severity, className, function, line, message) \
    do { \
        CScmdctl::ErrorContext ctx; \
        ctx.set((code), (severity), (className), (function), (line), (message)); \
        CScmdctl::ResponseHandler::recordError(ctx); \
    } while(0)

#define RECORD_ERROR_SEVEN(code, severity, psId, className, function, line, message) \
    do {auto mode = CScmdctl::ResponseHandler::isVerboseMode(); \
        if (!mode) CScmdctl::ResponseHandler::setVerboseMode(true); \
        CScmdctl::ErrorContext ctx; \
        ctx.setVerbose(true); \
        ctx.set((code), (severity), (psId), (className), (function), (line), (message)); \
        CScmdctl::ResponseHandler::recordError(ctx); \
        CScmdctl::ResponseHandler::setVerboseMode(mode); \
    } while(0)

}

