#pragma once

#include <cstdint>

enum class AdvertisingReportEventType : uint8_t {
    /**
     * @brief Connectable and scannable undirected advertising (ADV_IND)
     *
     */
    ADV_IND = 0x00,

    /**
     * @brief Connectable directed advertising (ADV_DIRECT_IND)
     *
     */
    ADV_DIRECT_IND = 0x01,

    /**
     * @brief Scannable undirected advertising (ADV_SCAN_IND)
     *
     */
    ADV_SCAN_IND = 0x02,

    /**
     * @brief Non connectable undirected advertising (ADV_NONCONN_IND)
     *
     */
    ADV_NONCONN_IND = 0x03,

    /**
     * @brief Scan Response (SCAN_RSP)
     *
     */
    SCAN_RSP = 0x04
};

enum class AdvertisingReportPeerAddressType : uint8_t {
    /**
     * @brief Public Device Address
     *
     */
    PUBLIC = 0x00,

    /**
     * @brief Random Device Address
     *
     */
    RANDOM = 0x01,

    /**
     * @brief Public Identity Address (Corresponds to Resolved Private Address)
     *
     */
    PUBLIC_IDENTITY_ADDRESS = 0x02,

    /**
     * @brief Random (static) Identity Address (Corresponds to Resolved Private Address)
     *
     */
    RANDOM_IDENITY_ADDRESS = 0x03
};