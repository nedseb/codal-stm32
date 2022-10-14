#pragma once

#include "AdvertisingData.h"
#include "BLERemoteDevice.h"
#include "HCI.h"

enum class BLEDeviceError : uint8_t {
    SUCCESS                            = 0x00,
    READ_BD_ERROR                      = 0x01,
    LE_RAND_ERROR                      = 0x02,
    LE_SET_ADVERTISING_PARAMS_ERROR    = 0x03,
    LE_READ_ADVERTISING_TX_POWER_ERROR = 0x04,
    LE_SET_ADVERTISING_DATA_ERROR      = 0x05,
    LE_SET_SCAN_RESPONSE_DATA_ERROR    = 0x06,
    LE_SET_ADVERTISING_ENABLE_ERROR    = 0x07,
    LE_SET_SCAN_PARAMS_ERROR           = 0x08,
    LE_SET_SCAN_ENABLE_ERROR           = 0x09,

    ALREADY_ADVERTISING_ERROR = 0xF0,
    ALREADY_SCANNING_ERROR    = 0xF1,
};

class BLEDevice {
  public:
    BLEDevice(HCI* hci);
    ~BLEDevice();

    bool init();
    void poll();

    void setAdvertisingData(AdvertisingData* data);
    void setScanResponseData(AdvertisingData* scan);

    BLEDeviceError startAdvertising();
    BLEDeviceError updateAdvertisingData();
    BLEDeviceError updateScanResponseData();
    BLEDeviceError stopAdvertising();

    BLEDeviceError startScanning();
    BLEDeviceError stopScanning();

    size_t availableScan() { return remoteDevice.size(); }
    size_t availableScan(std::string deviceName);

    std::vector<BLERemoteDevice> getScanResult();
    std::vector<BLERemoteDevice> getScanResult(std::string deviceName);

  private:
    HCI* hci;
    AdvertisingData* advData;
    AdvertisingData* advScanData;
    std::list<BLERemoteDevice> remoteDevice;

    bool isAdvertising;
    bool isScanning;

    void handleAdvReport(std::deque<BLEAdvertisingReport> reports);
};
