#pragma once

#include <cstddef>
#include <cstdint>
#include <map>

#include "CodalComponent.h"
#include "Event.h"
#include "STM32duinoBLE.h"

namespace codal {

enum AdvertisingState { OFF, SCAN, EMIT };

struct DeviceScanResult {
    BLEDevice device;
    uint32_t time;
};

class STM32AdvertisingBLE {
  public:
    /**
     * @brief Construct a new STM32AdvertisingBLE object
     *
     * @param id
     * @param channel the channel use to send advertising data (Note: advertising channel are: 37[default], 38 or 39)
     */
    STM32AdvertisingBLE(uint8_t channel = 37);

    /**
     * @brief Destroy the STM32AdvertisingBLE object
     *
     */
    ~STM32AdvertisingBLE() {}

    /**
     * @brief Set the Service Data to send over advertising. Due to the limit of the advertising frame, the message will
     * be truncated if has a length greater than MAX_ADVERTISING_DATA_LENGTH bytes.
     *
     * Note : Both types of data (Service or Manufacturer) cannot be sent at the same time. The last one to be set will
     * be sent.
     *
     * @param uuidService The service UUID to use (see: https://www.bluetooth.com/specifications/assigned-numbers/)
     * @param data The data to be sent
     * @param length The length of data to send
     */
    void setServiceData(uint16_t uuidService, uint8_t* data, size_t length);

    /**
     * @brief Set the Manufacturer Data to send over advertising. Due to the limit of the advertising frame, the message
     * will be truncated if has a length greater than MAX_ADVERTISING_DATA_LENGTH bytes.
     *
     * Note : Both types of data (Service or Manufacturer) cannot be sent at the same time. The last one to be set will
     * be sent.
     *
     * @param uuidService The company UUID to use (see:
     * https://www.bluetooth.com/specifications/assigned-numbers/company-identifiers/)
     * @param data The data to be sent
     * @param length The length of data to send
     */
    void setManufacturerData(uint16_t companyUUID, uint8_t* data, size_t length);

    /**
     * @brief Clear all data, and stop emitting
     *
     */
    void clearData();

    /**
     * @brief Set the Duration scanning
     *
     * @param ms the duration in milliseconds (default: 5000 ms). 0 to disable scanning and only emitting.
     */
    void setDurationScanning(uint16_t ms) { durationScan = ms; }

    /**
     * @brief Set the Duration emitting
     *
     * @param ms the duration in milliseconds (default: 5000 ms). 0 to disable emitting and only scanning.
     */
    void setDurationEmitting(uint16_t ms) { durationEmit = ms; }

    /**
     * @brief Retaining time of scan result, after this delay the result will be delete
     *
     * @param t The retaining time in milliseconds (default: 60000 ms)
     */
    void setRetainingTimeResult(uint32_t t) { retainingTime = t; }

    /**
     * @brief Start Advertising and Scanning
     *
     */
    void begin();

    /**
     * @brief Stop Advertising and Scanning
     *
     */
    void end();

    /**
     * @brief Set the Local Name
     *
     * @param localName The device name sent in advertising packets
     */
    void setLocalName(const char* localName) { this->localName = localName; }

    /**
     * @brief Know if there is result with manufacturer data
     *
     * @return true is there are data, false otherwise
     */
    bool hasResultWithManufacturerData();

    /**
     * @brief Get all Device with Manufacturer data
     *
     * @param output The array will contains results
     * @param maxLength The maximum result to retrieve (e.g. the size of ouput array)
     * @return uint8_t The number of results
     */
    size_t getResultWithManufacturerData(BLEDevice output[], size_t maxLength);

    /**
     * @brief Know if there is result with advertising data
     *
     * @return true is there are data, false otherwise
     */
    bool hasResultWithAdvertisingData();

    /**
     * @brief Get all Device with advertising data
     *
     * @param output The array will contains results
     * @param maxLength The maximum result to retrieve (e.g. the size of ouput array)
     * @return uint8_t The number of results
     */
    size_t getResultWithAdvertisingData(BLEDevice output[], size_t maxLength);

    /**
     * @brief Know if there is result
     *
     * @return true is there are results, false otherwise
     */
    bool hasResults();

    /**
     * @brief Get all device
     *
     * @param output The array will contains results
     * @param maxLength The maximum result to retrieve (e.g. the size of ouput array)
     * @return size_t The number of result
     */
    size_t getAllResults(BLEDevice output[], size_t maxLength);

    /**
     * @brief Clear all scan result
     *
     */
    void clearResult() { scanResults.clear(); }

    /**
     * @brief Get if the current status is EMITTING
     *
     * @return true is the device currently emitting, false otherwise
     */
    bool isEmitting() { return state == EMIT; }

    /**
     * @brief Get if the current status is SCANNING
     *
     * @return true is the device currently scanning, false otherwise
     */
    bool isScanning() { return state == SCAN; }

    /**
     * @brief Maximum advertising data length
     *
     */
    static constexpr size_t MAX_ADVERTISING_DATA_LENGTH = 27;

  private:
    void state_update(Event);
    void setData(uint16_t uuid, uint8_t* data, size_t length, bool isService);
    void enableScan();
    void disableScan();
    void enableAdvertising();
    void disableAdvertising();
    void saveScanResult();
    void reduceScanTime();

    AdvertisingState state;
    BLEAdvertisingData advData;
    bool hasData;
    bool isRunning;
    const uint8_t channel;
    uint16_t durationScan;
    uint16_t durationEmit;
    uint32_t retainingTime;
    uint32_t lastStateChange;
    const char* localName;
    std::map<std::string, DeviceScanResult> scanResults;

    static bool isTimerSet;
};
}  // namespace codal