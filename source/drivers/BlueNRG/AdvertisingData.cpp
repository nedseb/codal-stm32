#include "AdvertisingData.h"

#include "GAP_const.h"
#include "ble_utils.h"

constexpr uint16_t SERVICEDTA_UUID = 0x181C;

AdvertisingData::AdvertisingData() : significantPartSize(0), isFlagsSet(false), flags(0), localName("") {}

AdvertisingData::~AdvertisingData() {}

std::array<uint8_t, 31> AdvertisingData::toData()
{
    std::array<uint8_t, 31> result = {0x00};
    uint8_t index                  = 0;

    // Flags
    if (isFlagsSet) {
        result[index++] = 2;
        result[index++] = 0x01;
        result[index++] = flags;
    }

    // local name (short)
    if (!localName.empty()) {
        result[index++] = localName.size() + 1;
        result[index++] = (uint8_t)GAP_DataType::SHORT_LOCAL_NAME;

        for (auto it = localName.begin(); it != localName.end() && index < 31; it++) {
            result[index++] = (uint8_t)*it;
        }
    }

    // Send user data
    if (!userdata.empty() && index < 28) {
        result[index++] = userdata.size() + 1 + 2;
        result[index++] = (uint8_t)GAP_DataType::SERVICE_DATA;
        result[index++] = BLE_Utils::getLsb(SERVICEDTA_UUID);
        result[index++] = BLE_Utils::getMsb(SERVICEDTA_UUID);

        for (auto it = userdata.begin(); it != userdata.end() && index < 31; it++) {
            result[index++] = (uint8_t)*it;
        }
    }

    significantPartSize = index;
    return result;
}