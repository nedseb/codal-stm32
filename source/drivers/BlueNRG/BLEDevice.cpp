#include "BLEDevice.h"

BLEDevice::BLEDevice(HCI* hci)
    : hci(hci), advData(nullptr), advScanData(nullptr), isAdvertising(false), isScanning(false)
{
}

BLEDevice::~BLEDevice() {}

bool BLEDevice::init()
{
    if (!hci->init()) {
        return false;
    }

    auto maskBT = EventMaskBuilder().setDefault().setLEMeta();
    auto maskLE = LeEventMaskBuilder().setDefault();

    if (hci->isDebugEnable()) printf("[init] Set Event Mask Bluetooth\r\n");
    if (!hci->setEventMask(maskBT.getMSBMask(), maskBT.getLSBMask())) {
        if (hci->isDebugEnable()) printf("[init] ACI GAP Init FAILEDr\n");
        return false;
    }

    if (hci->isDebugEnable()) printf("[init] Set Event Mask BLE\r\n");
    if (!hci->leSetEventMask(maskLE.getMSBMask(), maskLE.getLSBMask())) {
        if (hci->isDebugEnable()) printf("[init] ACI GAP Init FAILEDr\n");
        return false;
    }

    return true;
}

void BLEDevice::poll()
{
    hci->poll();

    if (hci->availableAdvReport() > 0) {
        handleAdvReport(hci->getAdvReports());
    }
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

    if (!hci->leSetAdvertisingParameters(1280.0f, 1280.0f, AdvertisingType::ADV_IND, OwnAddressType::RANDOM))
        return BLEDeviceError::LE_SET_ADVERTISING_PARAMS_ERROR;

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

BLEDeviceError BLEDevice::startScanning()
{
    if (isScanning) {
        return BLEDeviceError::ALREADY_SCANNING_ERROR;
    }

    if (!hci->leSetScanParameters(ScanType::ACTIVE, 10.0f, 10.0f, OwnAddressType::RANDOM,
                                  ScanningFilterPolicy::BASIC_UNFILTERED))
        return BLEDeviceError::LE_SET_SCAN_PARAMS_ERROR;

    if (!hci->leSetScanEnable(true)) return BLEDeviceError::LE_SET_SCAN_ENABLE_ERROR;

    isScanning = true;
    return BLEDeviceError::SUCCESS;
}

BLEDeviceError BLEDevice::stopScanning()
{
    if (!hci->leSetScanEnable(false)) return BLEDeviceError::LE_SET_SCAN_ENABLE_ERROR;

    isScanning = false;
    return BLEDeviceError::SUCCESS;
}

void BLEDevice::handleAdvReport(std::deque<BLEAdvertisingReport> reports)
{
    auto devices = BLERemoteDevice::buildFromAdvertisingReports(reports);

    for (auto newDevice : devices) {
        for (auto it = remoteDevice.begin(); it != remoteDevice.end(); ++it) {
            if (it->getAddress() == newDevice.getAddress()) {
                remoteDevice.erase(it);
                break;
            }
        }

        remoteDevice.push_back(newDevice);
    }
}

size_t BLEDevice::availableScan(std::string deviceName)
{
    size_t result = 0;

    for (auto d : remoteDevice) {
        if (d.getName() == deviceName) result++;
    }

    return result;
}

std::vector<BLERemoteDevice> BLEDevice::getScanResult()
{
    auto result = std::vector<BLERemoteDevice>(remoteDevice.begin(), remoteDevice.end());
    remoteDevice.clear();
    return result;
}

std::vector<BLERemoteDevice> BLEDevice::getScanResult(std::string deviceName)
{
    std::vector<BLERemoteDevice> result;
    auto it = remoteDevice.begin();

    while (it != remoteDevice.end()) {
        if (it->getName() == deviceName) {
            result.push_back(*it);
            it = remoteDevice.erase(it);
        }
        else {
            ++it;
        }
    }

    return result;
}
