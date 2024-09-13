#pragma once

#include "BLEAdvertisingReport.h"
#include "data_format.h"

class BLERemoteDevice {
  public:
    BLERemoteDevice() : address(0, 0), name("") {}

    BLEAddress getAddress() { return address; }
    std::string getName() { return name; }
    int8_t getRSSI() { return rssi; }
    std::vector<GAPServiceData> getAllServiceDatas() { return serviceDatas; }

    static std::vector<BLERemoteDevice> buildFromAdvertisingReports(std::deque<BLEAdvertisingReport> reports);

  private:
    int8_t rssi;
    BLEAddress address;
    std::string name;
    std::vector<GAPServiceData> serviceDatas;

    static size_t isReportInVector(std::vector<BLERemoteDevice>& devices, BLEAdvertisingReport& report);
};