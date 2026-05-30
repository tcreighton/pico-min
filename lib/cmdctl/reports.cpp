
#include "command-declarations.hpp"
#include "devices-container.hpp"
#include "json-report.hpp"
#include "product-info.hpp"
#include "reports.hpp"

using namespace CScore;

namespace CScmdctl {
    std::string Report::runReport(ReportType_t reportType) {
        std::string report;

//        report = reportInfo();

        return report;
    }

    void Report::getReportHeader(JsonReport &report, const std::string& reportName) {
        report.openObject(std::string("Metadata"));
        report.addElement(std::string("product"), std::string_view("pico-minimal") /*getProductName()*/ );
        report.addElement(std::string("reportName"), reportName);
        report.closeObject();
    }

    std::string Report::reportSystemTemperature(const Unit_t units, const bool showDetails) {
        JsonReport report;
        report.openDocument();
        getReportHeader(report, std::string("System Temperature"));
        reportTempData(report, units, showDetails);
        report.closeDocument();
        return report.getReportDoc();
    }

    void Report::reportTempData (JsonReport &report,
                                 const Unit_t units,
                                 const bool showDetails) {

        report.openObject(std::string("SystemTemperature"));
        report.addElement(std::string("temperatureUnits"), unitToString(units));

        report.addElement(std::string("mcuSensor"), CSdevices::getMCUTempSensor().getTemperature(units));
        if (showDetails) {
            report.addElement(std::string("mcuSensorCounts"), CSdevices::getMCUTempSensor().getCounts());
        }
        report.closeObject();

    }

    std::string Report::fullSystemReport () {
        JsonReport report{};

        report.openDocument();
        getReportHeader(report, std::string("System Report"));
        const std::string reportName = nameFromReportType(ReportType_t::INFO);
        report.openObject(reportName);
        reportTempData(report, DEFAULT_TEMP_UNITS, true);
        report.closeObject();
        report.closeDocument();

        return report.getReportDoc();

    }

    std::string Report::reportInfo(bool decode) {
        JsonReport report{};
        report.openDocument();
        getReportHeader(report, std::string("Product Information"));

        /*
        struct ProductInfo {
            std::string         companyName             = COMPANY_NAME;
            std::string         productName             = PRODUCT_NAME;
            uint8_t             majorVersion            = FIRMWARE_MAJOR_VERSION;
            uint8_t             minorVersion            = FIRMWARE_MINOR_VERSION;
            PackedDateTime_t    buildNumber             = PackedDateTime::getPackedBuildDateTime();// encoded build date
            std::string         circuitBoardRevision    = CScmdctl::CURRENT_BOARD_NAME;
            std::string         encodedPicoBoardId      = getPicoBoardId();// AKA product serial number
        };
        using ProductInfo_t = ProductInfo;  // Not really necessary.
        */

        auto [  companyName,
                productName,
                majorVersion,
                minorVersion,
                buildNumber,
                circuitBoardRevision,
                encodedPicoBoardId] = getProductInfo();

        std::stringstream ss;
        ss << static_cast<uint16_t>(majorVersion) << "."; // cast necessary so it's not treated as char
        ss << static_cast<uint16_t>(minorVersion) << ".";

        report.openObject(std::string("Details"));
        report.addElement(std::string("companyName"),       companyName);
        report.addElement(std::string("poweredBy"),       std::string("Creighton Scientific"));
//        report.addElement(std::string("productName"),       productName);
        report.addElement(std::string("circuitBoardRevision"), circuitBoardRevision);
        if (decode) {
            report.addElement(std::string("firmwareVersion"), getFirmwareVersionDecoded());
        } else {
            report.addElement(std::string("firmwareVersion"), getFirmwareVersion());
        }
        report.addElement(std::string("serialNumber"),      encodedPicoBoardId);

        report.closeObject();
        report.closeDocument();

        return report.getReportDoc();

    }

}
