
#pragma once

#include <string>
#include <vector>
#include <functional>
#include "assertion.hpp"


namespace CStest {

    class Test {

    public:

        Test(   const Assertion::verbosity  level,
                const std::string_view      className) : level_(level), className_(className) {}
        virtual ~Test() = default;

        using TestFunction_t = std::function<void(Assertion::verbosity)>;

        // setUp and tearDown can be overridden. These will be called by runTests.
        // A typical setup would set the level_ to a value desired for the tests.
        virtual void setUp () {
            displayMessage(getVerbosityLevel(), "In " + getClassName() + "::setUp()\n");
        }

        virtual void tearDown () {
            displayMessage(getVerbosityLevel(), "In " + getClassName() + "::tearDown()\n");
        }

        void addTestFunction (const TestFunction_t &function) {
            functionList_.push_back(function);
//            std::cout << "addTestFunction; count: " << functionList_.size() << std::endl;
        }

        [[nodiscard]] std::string getClassName () const {
            return className_;
        }

        [[nodiscard]] Assertion::verbosity getVerbosityLevel () const {
            return level_;
        }

        void executeAll() const;

        void doTests () {
            setUp();
            executeAll();
            tearDown();
        }

        void displayMessage (const std::string &message) const;
        static void displayMessage (Assertion::verbosity level, const std::string &message);

    private:

        void setClassName (const std::string &className) {
            className_ = className;
        }
        void setVerbosityLevel(const Assertion::verbosity level) {
            level_ = level;
        }

        std::vector<TestFunction_t> functionList_{};
        Assertion::verbosity level_ = Assertion::ON_FAILURE;
        std::string className_;

    };

    void runTests (Assertion::verbosity level = Assertion::ON_FAILURE);


}

