#include "STM32AdvertisingBLE.h"

#include "clock.h"
#include "codal_target_hal.h"

using namespace std;
using namespace codal;

STM32AdvertisingBLE::STM32AdvertisingBLE(uint16_t id, uint8_t channel)
    : CodalComponent(id, 0),
      state(OFF),
      hasData(false),
      isRunning(false),
      channel(channel),
      durationScan(5000),
      durationEmit(5000),
      retainingTime(60000),
      lastStateChange(0),
      localName("Default Name")
{
    scanResults.clear();
    status = DEVICE_COMPONENT_STATUS_SYSTEM_TICK;
}

void STM32AdvertisingBLE::begin()
{
    lastStateChange = 0;
    if (hasData) {
        enableAdvertising();
    }
    else {
        enableScan();
    }

    isRunning = true;
}

void STM32AdvertisingBLE::end()
{
    isRunning = false;
    state     = OFF;
    disableAdvertising();
    disableScan();
}

void STM32AdvertisingBLE::setServiceData(uint16_t uuidService, uint8_t* data, size_t length)
{
    setData(uuidService, data, length, true);
}

void STM32AdvertisingBLE::setServiceData(uint16_t uuidService, const char* data)
{
    setData(uuidService, (uint8_t*)data, strlen(data), true);
}

void STM32AdvertisingBLE::setManufacturerData(uint16_t companyUUID, uint8_t* data, size_t length)
{
    setData(companyUUID, data, length, false);
}

void STM32AdvertisingBLE::setManufacturerData(uint16_t companyUUID, const char* data)
{
    setData(companyUUID, (uint8_t*)data, strlen(data), false);
}

void STM32AdvertisingBLE::clearData()
{
    advData.clear();
    hasData = false;
}

bool STM32AdvertisingBLE::hasResultWithManufacturerData()
{
    return scanResults.size() > 0;
}

size_t STM32AdvertisingBLE::getResultWithManufacturerData(BLEDevice output[], size_t max_length)
{
    size_t length = 0;

    for (auto& device : scanResults) {
        if (length >= max_length) {
            break;
        }

        output[length] = device.second.device;
        length++;
    }

    return length;
}

void STM32AdvertisingBLE::periodicCallback()
{
    if (!isRunning) {
        return;
    }

    uint32_t delta = getCurrentMillis() - lastStateChange;

    switch (state) {
        case EMIT:
            if (delta >= durationEmit && durationScan > 0) {
                reduceScanTime();
                enableScan();
            }
            break;

        case SCAN:
            if (delta >= durationScan && hasData && durationEmit > 0) {
                saveScanResult();
                enableAdvertising();
            }
            else {
                saveScanResult();
            }
            break;

        case OFF:
        default:
            break;
    }
}

void STM32AdvertisingBLE::setData(uint16_t uuid, uint8_t* data, size_t length, bool isService)
{
    size_t dataLength = length > MAX_ADVERTISING_DATA_LENGTH ? MAX_ADVERTISING_DATA_LENGTH : length;

    if (isRunning && state == EMIT) {
        BLE.stopAdvertise();
    }

    if (isService) {
        advData.setManufacturerData(NULL, 0);
        advData.setAdvertisedServiceData(uuid, data, dataLength);
    }
    else {
        advData.setAdvertisedServiceData(uuid, NULL, 0);
        advData.setManufacturerData(uuid, data, dataLength);
    }

    BLE.setAdvertisingData(advData);

    if (isRunning && state == EMIT) {
        BLE.advertise(channel);
    }

    hasData = true;
}

void STM32AdvertisingBLE::enableScan()
{
    disableAdvertising();
    BLE.scan(true);

    state           = SCAN;
    lastStateChange = getCurrentMillis();
}

void STM32AdvertisingBLE::disableScan()
{
    BLE.stopScan();
}

void STM32AdvertisingBLE::enableAdvertising()
{
    disableScan();

    BLE.setLocalName(localName);
    BLE.setConnectable(false);
    BLE.setAdvertisingData(advData);
    BLE.advertise(channel);

    state           = EMIT;
    lastStateChange = getCurrentMillis();
}

void STM32AdvertisingBLE::disableAdvertising()
{
    BLE.stopAdvertise();
}

void STM32AdvertisingBLE::saveScanResult()
{
    for (BLEDevice device = BLE.available(); device; device = BLE.available()) {
        if (scanResults.count(device.address()) > 0) {
            scanResults[device.address()].device = device;
            scanResults[device.address()].time   = getCurrentMillis();
        }
        else {
            scanResults.insert(
                std::pair<string, DeviceScanResult>(device.address(), DeviceScanResult{device, getCurrentMillis()}));
        }
    }

    uint32_t currentTime = getCurrentMillis();

    for (auto it = scanResults.rbegin(); it != scanResults.rend(); ++it) {
        if (currentTime - it->second.time >= retainingTime) {
            scanResults.erase(it->first);
        }
    }
}

void STM32AdvertisingBLE::reduceScanTime()
{
    for (auto& device : scanResults) {
        device.second.time += durationEmit;
    }
}
