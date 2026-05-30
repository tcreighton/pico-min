#pragma once


#include <string>

#include "command-declarations.hpp"
#include "json-report.hpp"
#include "factory-data.hpp"

namespace CScmdctl {

class Report {

  public:
    Report () = default;
    ~Report () = default;

    /**
     * @brief This overload runs a report that is not tied to a power supply.
     * @param reportType Tells what type of report to run.
     * @return The string production - a JSON report.
     */
    static std::string runReport (ReportType_t reportType);

    static std::string nameFromReportType(const ReportType_t reportType) {
        switch (reportType) {
            default:
            case ReportType_t::INFO:
                return "Information";
        }
    }

    static void getReportHeader (   JsonReport& report,
                                    const std::string& reportName);

    static std::string reportHvUsage ();
    static void reportHvUsage (JsonReport& report);

    static std::string reportSystemTemperature (CScore::Unit_t units = CScore::DEFAULT_TEMP_UNITS,
                                                bool showDetails = false);

    static void reportTempData (JsonReport &report,
                                CScore::Unit_t units = CScore::DEFAULT_TEMP_UNITS,
                                bool showDetails = false);

    static std::string fullSystemReport ();

    static std::string reportInfo (bool decode = false);

  private:
    // global metadata here.

};


}

