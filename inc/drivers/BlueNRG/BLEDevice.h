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

    ALREADY_ADVERTISING_ERROR  = 0xF0,
    ALREADY_SCANNING_ERROR     = 0xF1,
    ADVERTISING_DISABLED_ERROR = 0xF2,
    SCANNING_DISABLED_ERROR    = 0xF3,
};

enum class ModeState : uint8_t { DISABLE = 0x00, STAND_BY = 0x01, RUNNING = 0x02 };

class BLEDevice {
  public:
    BLEDevice(HCI* hci);
    ~BLEDevice();

    /**
     * @brief Initialise the device
     *
     * @return true if success, false otherwise
     */
    bool init();

    /**
     * @brief Poll the BLE Device
     *
     */
    void poll();

    /**
     * @brief Set the Advertsing Data object
     *
     * @param data
     */
    BLEDeviceError setAdvertisingData(AdvertisingData& data);

    /**
     * @brief Set the Scan Response Data object
     *
     * @param scan
     */
    BLEDeviceError setScanResponseData(AdvertisingData& data);

    /**
     * @brief Swap between Advertising and Scanning mode (if needed)
     *
     * @return BLEDeviceError
     */
    BLEDeviceError swapMode();

    /**
     * @brief Set the advertising mode, and enable it (if the scan is running, the advertising will start after the
     * `swapMode()` call)
     *
     * @return BLEDeviceError
     */
    BLEDeviceError startAdvertising(AdvertisingData& advData);
    BLEDeviceError startAdvertising(AdvertisingData& advData, AdvertisingData& scanData);

    // BLEDeviceError updateAdvertisingData();
    // BLEDeviceError updateScanResponseData();

    /**
     * @brief Stop the advertsing mode, and disable it for `swapMode()`
     *
     * @return BLEDeviceError
     */
    BLEDeviceError stopAdvertising();

    /**
     * @brief Set the Scan mode, and enable it (if the advertisment is running, the scan will start after the
     * `swapMode()` call)
     *
     * @return BLEDeviceError
     */
    BLEDeviceError startScanning();

    /**
     * @brief Stop the scan mode, and disable it for `swapMode()`
     *
     * @return BLEDeviceError
     */
    BLEDeviceError stopScanning();

    size_t availableScan() { return remoteDevice.size(); }
    size_t availableScan(std::string deviceName);

    std::vector<BLERemoteDevice> getScanResult();
    std::vector<BLERemoteDevice> getScanResult(std::string deviceName);

  private:
    HCI* hci;
    std::list<BLERemoteDevice> remoteDevice;

    ModeState advertisingState;
    ModeState scanningState;

    void handleAdvReport(std::deque<BLEAdvertisingReport> reports);
};
