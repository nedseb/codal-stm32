#include "BLEAdvertisingReport.h"

#include <cstdio>

#include "ble_utils.h"

using namespace std;

BLEAdvertisingReport::BLEAdvertisingReport(vector<uint8_t>& eventData)
    : rssi(0x7B), address(0, 0), flags(0xFF), name("")
{
    /**
     * WARNING :
     * Most of the time, there is only one report per event, but the BLE Specification allows more
     * It seems that the BLUENRG only sends one report per event.
     */

    // eventData[0] == 0x02 (Advertising report event)
    // uint8_t numReport = eventData[1];
    // uint8_t eventType   = eventData[2];
    // uint8_t addressType = eventData[3];
    uint32_t addressLSB =
        ((uint32_t)eventData[7] << 24) | ((uint32_t)eventData[6] << 16) | ((uint32_t)eventData[5] << 8) | eventData[4];
    uint32_t addressMSB = ((uint32_t)eventData[9] << 8) | eventData[8];
    uint8_t dataLength  = eventData[10];
    rssi                = eventData[11 + dataLength];
    address             = BLEAddress(addressMSB, addressLSB);

    uint8_t dataIdx     = 11;
    while (dataIdx < dataLength + 11) {
        uint8_t adLength    = eventData[dataIdx++];
        GAP_DataType adType = static_cast<GAP_DataType>(eventData[dataIdx]);

        switch (adType) {
            case GAP_DataType::FLAGS:
                flags = eventData[dataIdx + 1];
                break;

            case GAP_DataType::COMPLETE_LOCAL_NAME:
                name.clear();
                name.resize(adLength - 1);
                for (uint8_t i = 1; i < adLength; ++i) {
                    name[i - 1] = BLE_Utils::safeChar(eventData[dataIdx + i]);
                }
                break;

            case GAP_DataType::SERVICE_DATA: {
                GAPServiceData data(adLength - 3);

                data.uuid = ((uint16_t)eventData[dataIdx + 2] << 8) | eventData[dataIdx + 1];

                for (int i = 3; i < adLength; ++i) {
                    data.data[i - 3] = eventData[dataIdx + i];
                }
                serviceData.push_back(data);
            } break;

            default:
                break;
        }

        if (adLength > 0) {
            dataIdx += adLength;
        }
    }
}