#pragma once

#include <cstdint>
#include <deque>
#include <string>
#include <vector>

#include "BLEAdress.h"
#include "data_format.h"

class BLEAdvertisingReport {
  public:
    int8_t rssi;
    BLEAddress address;
    uint8_t flags;
    std::string name;
    std::vector<GAPServiceData> serviceData;

    BLEAdvertisingReport(std::vector<uint8_t>& eventData);
};