
#pragma once

#include <string>
#include "pico/unique_id.h"

#include "../cmdctl/board-config.hpp"
#include "packed-datetime.hpp"
#include "utilities.hpp"

namespace CScore {

    inline std::string getPicoBoardId () {
        char boardId[2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1];

        pico_get_unique_board_id_string(&boardId[0], sizeof(boardId));

        return boardId;

    }

    #define COMPANY_NAME  "Lumafield" // This needs to be a #define, not constexpr
    #define PRODUCT_NAME  "This is the greatest product!"  // TODO: Change this to something more meaningful.

    constexpr uint8_t FIRMWARE_MAJOR_VERSION = 0x00;
    constexpr uint8_t FIRMWARE_MINOR_VERSION = 0x05;

    inline std::string getCompanyName () {return COMPANY_NAME;}
    inline std::string getProductName () {return PRODUCT_NAME;}

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

    inline std::string getFirmwareVersion () {
        std::stringstream version;

        const auto packed = PackedDateTime::getPackedBuildDateTime();
        version << static_cast<int>(FIRMWARE_MAJOR_VERSION) << "."
                << static_cast<int>(FIRMWARE_MINOR_VERSION) << "."
                << CScore::int_to_hex(packed);
        return version.str();
    }

    inline std::string getFirmwareVersionDecoded () {
        auto [seconds, minutes, hours, day, month, year]
                            = PackedDateTime::getUnpackedDateTime(PackedDateTime::getPackedBuildDateTime());
        auto ss = std::stringstream();
        ss  << static_cast<int>(FIRMWARE_MAJOR_VERSION) << "."
            << static_cast<int>(FIRMWARE_MINOR_VERSION) << "."
            << static_cast<int>(year) << "-"
            << static_cast<int>(month) << "-"
            << static_cast<int>(day) << "-"
            << static_cast<int>(hours) << ":"
            << static_cast<int>(minutes) << ":"
            << static_cast<int>(seconds);
        auto s = ss.str();

        return s;
    }

    inline ProductInfo_t getProductInfo() {
        ProductInfo_t info;
        // If changes from the declared initializers are needed, this is a good place for them.
        return info;
    }


}

