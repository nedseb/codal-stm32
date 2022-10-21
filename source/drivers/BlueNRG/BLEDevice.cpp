#include "BLEDevice.h"

using namespace std;

BLEDevice::BLEDevice(HCI* hci)
    : hci(hci), advertisingState(ModeState::DISABLE), scanningState(ModeState::DISABLE), advData(), advScanData()
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

BLEDeviceError BLEDevice::setAdvertisingData(AdvertisingData data)
{
    advData = data;

    if (advertisingState == ModeState::RUNNING) {
        auto ptrData = advData.toData();
        if (!hci->leSetAdvertisingData(advData.getSignificantSize(), ptrData.data()))
            return BLEDeviceError::LE_SET_ADVERTISING_DATA_ERROR;
    }

    return BLEDeviceError::SUCCESS;
}

BLEDeviceError BLEDevice::setScanResponseData(AdvertisingData data)
{
    advScanData = data;

    if (advertisingState == ModeState::RUNNING) {
        auto ptrData = data.toData();
        if (!hci->leSetScanResponseData(data.getSignificantSize(), ptrData.data()))
            return BLEDeviceError::LE_SET_SCAN_RESPONSE_DATA_ERROR;
    }

    return BLEDeviceError::SUCCESS;
}

BLEDeviceError BLEDevice::swapMode()
{
    if (advertisingState == ModeState::RUNNING && scanningState == ModeState::STAND_BY) {
        if (!hci->leSetAdvertisingEnable(false)) return BLEDeviceError::LE_SET_ADVERTISING_ENABLE_ERROR;
        advertisingState = ModeState::STAND_BY;

        if (startScanning() != BLEDeviceError::SUCCESS) return BLEDeviceError::LE_SET_SCAN_ENABLE_ERROR;
        scanningState = ModeState::RUNNING;
    }
    else if (scanningState == ModeState::RUNNING && advertisingState == ModeState::STAND_BY) {
        if (!hci->leSetScanEnable(false)) return BLEDeviceError::LE_SET_SCAN_ENABLE_ERROR;
        scanningState = ModeState::STAND_BY;

        if (startAdvertising() != BLEDeviceError::SUCCESS) return BLEDeviceError::LE_SET_ADVERTISING_ENABLE_ERROR;
        advertisingState = ModeState::RUNNING;
    }

    return BLEDeviceError::SUCCESS;
}

BLEDeviceError BLEDevice::startAdvertising()
{
    if (advertisingState == ModeState::RUNNING) {
        return BLEDeviceError::ALREADY_ADVERTISING_ERROR;
    }

    if (scanningState == ModeState::RUNNING) {
        advertisingState = ModeState::STAND_BY;
        return BLEDeviceError::SUCCESS;
    }

    auto adv     = advData.toData();
    auto advScan = advScanData.toData();

    hci->leSetAdvertisingEnable(false);

    if (!hci->leSetAdvertisingParameters(100.0f, 100.0f, AdvertisingType::ADV_IND, OwnAddressType::RANDOM))
        return BLEDeviceError::LE_SET_ADVERTISING_PARAMS_ERROR;

    if (hci->leReadAdvertisingPhysicalChannelTxPower() == 127)
        return BLEDeviceError::LE_READ_ADVERTISING_TX_POWER_ERROR;

    if (!hci->leSetAdvertisingData(advData.getSignificantSize(), adv.data()))
        return BLEDeviceError::LE_SET_ADVERTISING_DATA_ERROR;

    if (!hci->leSetScanResponseData(advScanData.getSignificantSize(), advScan.data()))
        return BLEDeviceError::LE_SET_SCAN_RESPONSE_DATA_ERROR;

    if (!hci->leSetAdvertisingEnable(true)) return BLEDeviceError::LE_SET_ADVERTISING_ENABLE_ERROR;

    advertisingState = ModeState::RUNNING;

    return BLEDeviceError::SUCCESS;
}

BLEDeviceError BLEDevice::stopAdvertising()
{
    if (advertisingState == ModeState::DISABLE) {
        return BLEDeviceError::SUCCESS;
    }

    if (!hci->leSetAdvertisingEnable(false)) return BLEDeviceError::LE_SET_ADVERTISING_ENABLE_ERROR;

    advertisingState = ModeState::DISABLE;
    return BLEDeviceError::SUCCESS;
}

BLEDeviceError BLEDevice::startScanning()
{
    if (scanningState == ModeState::RUNNING) {
        return BLEDeviceError::ALREADY_SCANNING_ERROR;
    }

    if (!hci->leSetScanParameters(ScanType::ACTIVE, 10.0f, 10.0f, OwnAddressType::RANDOM,
                                  ScanningFilterPolicy::BASIC_UNFILTERED))
        return BLEDeviceError::LE_SET_SCAN_PARAMS_ERROR;

    if (advertisingState == ModeState::RUNNING) {
        scanningState = ModeState::STAND_BY;
    }
    else {
        if (!hci->leSetScanEnable(true)) return BLEDeviceError::LE_SET_SCAN_ENABLE_ERROR;
        scanningState = ModeState::RUNNING;
    }

    return BLEDeviceError::SUCCESS;
}

BLEDeviceError BLEDevice::stopScanning()
{
    if (scanningState == ModeState::DISABLE) {
        return BLEDeviceError::SUCCESS;
    }

    if (!hci->leSetScanEnable(false)) return BLEDeviceError::LE_SET_SCAN_ENABLE_ERROR;

    scanningState = ModeState::DISABLE;
    return BLEDeviceError::SUCCESS;
}

void BLEDevice::handleAdvReport(deque<BLEAdvertisingReport> reports)
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

size_t BLEDevice::availableScan(string deviceName)
{
    size_t result = 0;

    for (auto d : remoteDevice) {
        if (d.getName() == deviceName) result++;
    }

    return result;
}

vector<BLERemoteDevice> BLEDevice::getScanResult()
{
    auto result = vector<BLERemoteDevice>(remoteDevice.begin(), remoteDevice.end());
    remoteDevice.clear();
    return result;
}

vector<BLERemoteDevice> BLEDevice::getScanResult(string deviceName)
{
    vector<BLERemoteDevice> result;
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
