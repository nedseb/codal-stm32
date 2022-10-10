#pragma once

#include <cstdint>

enum class AdvertisingType : uint8_t {
    /**
     * @brief Connectable and scannable undirected advertising (ADV_IND)
     *
     */
    ADV_IND = 0x00,

    /**
     * @brief Connectable high duty cycle directed advertising (ADV_DIRECT_IND, high duty cycle)
     *
     */
    ADV_DIRECT_IND_HIGH = 0x01,

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
     * @brief Connectable low duty cycle directed advertising (ADV_DIRECT_IND, low duty cycle)
     *
     */
    ADV_DIRECT_IND_LOW = 0x04
};

enum class OwnAddressType : uint8_t {
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
     * @brief Controller generates Resolvable Private Address based on the local IRK from the resolving list. If the
     * resolving list contains no matching entry, use the public address.
     *
     */
    CONTROLLER_OR_PUBLIC = 0x02,

    /**
     * @briefController generates Resolvable Private Address based on the local IRK from the resolving list. If the
     * resolving list contains no matching entry, use the random address from LE_Set_Random_Address.
     *
     */
    CONTROLLER_OR_RANDOM = 0x03
};

enum class PeerAddressType : uint8_t {
    /**
     * @brief Public Device Address (default) or Public Identity Address
     *
     */
    PUBLIC = 0x00,

    /**
     * @brief Random Device Address or Random (static) Identity Address
     *
     */
    RANDOM = 0x01
};

enum class AdvertisingFilterPolicy : uint8_t {
    /**
     * @brief Process scan and connection requests from all devices (i.e., the Filter Accept List is not in use)
     *
     */
    ALL_DEVICE = 0x00,

    /**
     * @brief Process connection requests from all devices and scan requests only from devices that are in the Filter
     * Accept List.
     *
     */
    SCAN_ACCESS_LIST = 0x01,

    /**
     * @brief Process scan requests from all devices and connection requests only from devices that are in the Filter
     * Accept List.
     *
     */
    CONN_ACCESS_LIST = 0x02,

    /**
     * @brief Process scan and connection requests only from devices in the Filter Accept List.
     *
     */
    ONLY_ACCESS_LIST = 0x03
};

enum class ScanType : uint8_t {
    /**
     * @brief Passive Scanning. No scanning PDUs shall be sent.
     *
     */
    PASSIVE = 0x00,

    /**
     * @brief Active scanning. Scanning PDUs may be sent.
     *
     */
    ACTIVE = 0x01,
};

enum class ScanningFilterPolicy : uint8_t {
    /**
     * @brief Basic unfiltered scanning filter policy
     *
     */
    BASIC_UNFILTERED = 0x00,

    /**
     * @brief Basic filtered scanning filter policy
     *
     */
    BASIC_FILTERED = 0x01,

    /**
     * @brief Extended unfiltered scanning filter policy
     *
     */
    EXTENDED_UNFILTERED = 0x02,

    /**
     * @brief Extended filtered scanning filter policy
     *
     */
    EXTENDED_FILTERED = 0x03,
};