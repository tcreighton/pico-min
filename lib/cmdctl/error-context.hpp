#pragma once

#include <sstream>
#include <string>
#include "errors.hpp"

namespace CScmdctl {

    enum class ErrorSeverity : uint8_t {
        INFO = 0,
        WARNING,
        ERROR,
        CRITICAL,
    };


    class ErrorContext {

    public:

        ErrorContext () = default;

        void set (  const ResponseCode code,
                    const ErrorSeverity severity,
                    const std::string_view psId,
                    const std::string_view className,
                    const std::string_view function,
                    const int line,
                    const std::string& message) {
            clear();
            className_      = className;
            psId_           = psId;
            function_       = function;
            line_           = line;
            message_        = message;
            responseCode_   = code;
            severity_       = severity;
        }


        void set (  const ResponseCode code,
                    const ErrorSeverity severity,
                    const std::string_view className,
                    const std::string_view function,
                    const int line,
                    const std::string& message) {
            clear();
            className_      = className;
            function_       = function;
            line_           = line;
            message_        = message;
            responseCode_   = code;
            severity_       = severity;
        }

        void set (  const ResponseCode code,
            const ErrorSeverity severity,
            const std::string_view className,
            const std::string_view function,
            const std::string& message) {
            clear();
            className_      = className;
            function_       = function;
            message_        = message;
            responseCode_   = code;
            severity_       = severity;
        }

        void set (  const ResponseCode code,
                    const ErrorSeverity severity,
                    const std::string& message) {
                        clear();
                        severity_       = severity;
                        message_        = message;
                        responseCode_   = code;
        }

        void clear () {
            psId_.clear();
            className_.clear();
            function_.clear();
            file_.clear();
            line_ = 0;
            message_.clear();
            responseCode_ = ResponseCode::NOT_AN_ERROR;
            severity_ = ErrorSeverity::INFO;
        }

        [[nodiscard]] bool isVerbose () const {return verbose_;}
        void setVerbose (const bool verbose) {verbose_ = verbose;}
        [[nodiscard]] bool hasError () const {
            return ResponseCode::NOT_AN_ERROR != responseCode_ && ResponseCode::APP_MESSAGE_OK != responseCode_;
        }

        [[nodiscard]] bool isCritical () const {
            return ErrorSeverity::CRITICAL == severity_;
        }

        [[nodiscard]] std::string_view getPsId () const {return psId_;}
        [[nodiscard]] std::string_view getClassName () const {return className_;}
        [[nodiscard]] std::string_view getFunction () const {return function_;}
        [[nodiscard]] std::string_view getFile () const {return file_;}
        [[nodiscard]] int getLine () const {return line_;}
        [[nodiscard]] std::string_view getMessage () const {return message_;}
        [[nodiscard]] ResponseCode getResponseCode () const {return responseCode_;}
        [[nodiscard]] ErrorSeverity getSeverity () const {return severity_;}

        // For detailed logging (verbose mode)
        [[nodiscard]] std::string getDetailedMessage() const {
            if (!hasError()) return "";

            std::stringstream ss;
            ss << severityToString(severity_) << " error in "
               << psId_ << "; " << className_ << "::" << function_
               << " (" << line_ << "): "
               << message_
               << " [Code: " << responseToString(responseCode_) << "]";
            return ss.str();
        }

        // For client display (terse mode)
        [[nodiscard]] std::string getClientMessage() const {
            if (!hasError()) return "OK";

            std::stringstream ss;
            ss << responseToInt(responseCode_) << ": " << responseToString(responseCode_);
            if (!message_.empty()) {
                ss << ": " << message_;
            }
            return ss.str();
        }


        [[nodiscard]] std::string getErrorMessage () const {
            if (!hasError()) return "";

            std::stringstream ss;
            ss << "Error in " << psId_ << ":" << className_ << ":" << function_ << ":" << file_ << "@" << line_ << ": " <<
                        message_ << "; code:" << responseToString(responseCode_);

            return ss.str();
        }

        static std::string severityToString(ErrorSeverity sev) {
            switch (sev) {
                case ErrorSeverity::INFO: return "INFO";
                case ErrorSeverity::WARNING: return "WARNING";
                case ErrorSeverity::ERROR: return "ERROR";
                case ErrorSeverity::CRITICAL: return "CRITICAL";
                default: return "UNKNOWN";
            }
        }


    private:

        std::string psId_;
        std::string className_;
        std::string function_;
        std::string file_;
        int line_{0};
        std::string message_;
        ResponseCode responseCode_{ResponseCode::NOT_AN_ERROR};
        ErrorSeverity severity_{ErrorSeverity::INFO};
        bool verbose_{false};

    };

}

