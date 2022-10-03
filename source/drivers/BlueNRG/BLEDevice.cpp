#include "BLEDevice.h"

BLEDevice::BLEDevice(HCI* hci) : hci(hci), advData(nullptr), advScanData(nullptr), isAdvertising(false) {}

BLEDevice::~BLEDevice() {}

void BLEDevice::init()
{
    hci->init();
}

void BLEDevice::setAdvertisingData(AdvertisingData* data)
{
    advData = data;
}

void BLEDevice::setScanResponseData(AdvertisingData* scan)
{
    advScanData = scan;
}

BLEDeviceError BLEDevice::startAdvertising()
{
    if (isAdvertising) {
        return BLEDeviceError::ALREADY_ADVERTISING_ERROR;
    }

    if (!hci->leSetAdvertisingParameters()) return BLEDeviceError::LE_SET_ADVERTISING_PARAMS_ERROR;

    if (hci->leReadAdvertisingPhysicalChannelTxPower() == 127)
        return BLEDeviceError::LE_READ_ADVERTISING_TX_POWER_ERROR;

    if (updateAdvertisingData() != BLEDeviceError::SUCCESS) return BLEDeviceError::LE_SET_ADVERTISING_DATA_ERROR;

    if (updateScanResponseData() != BLEDeviceError::SUCCESS) return BLEDeviceError::LE_SET_SCAN_RESPONSE_DATA_ERROR;

    if (!hci->leSetAdvertisingEnable(true)) return BLEDeviceError::LE_SET_ADVERTISING_ENABLE_ERROR;

    isAdvertising = true;
    return BLEDeviceError::SUCCESS;
}

BLEDeviceError BLEDevice::updateAdvertisingData()
{
    uint8_t* advDataPtr = advData->toData().data();
    if (!hci->leSetAdvertisingData(advData->getSignificantSize(), advDataPtr))
        return BLEDeviceError::LE_SET_ADVERTISING_DATA_ERROR;

    return BLEDeviceError::SUCCESS;
}

BLEDeviceError BLEDevice::updateScanResponseData()
{
    uint8_t* advScanDataPtr = advScanData->toData().data();
    if (!hci->leSetScanResponseData(advScanData->getSignificantSize(), advScanDataPtr))
        return BLEDeviceError::LE_SET_SCAN_RESPONSE_DATA_ERROR;

    return BLEDeviceError::SUCCESS;
}

BLEDeviceError BLEDevice::stopAdvertising()
{
    if (!hci->leSetAdvertisingEnable(false)) return BLEDeviceError::LE_SET_ADVERTISING_ENABLE_ERROR;

    isAdvertising = false;
    return BLEDeviceError::SUCCESS;
}
