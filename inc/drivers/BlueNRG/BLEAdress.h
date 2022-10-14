#pragma once

#include <cstdint>
#include <string>

#include "ble_utils.h"

class BLEAddress {
  public:
    BLEAddress(uint32_t msb, uint32_t lsb) : msb(msb & 0x0000FFFF), lsb(lsb) {}

    uint32_t getLSBAddress() { return lsb; }
    uint32_t getMSBAddress() { return msb; }
    std::string toString()
    {
        return BLE_Utils::hexToString(msb, ':').substr(6) + ":" + BLE_Utils::hexToString(lsb, ':');
    }

    inline bool operator==(const BLEAddress& other) { return other.msb == msb && other.lsb == lsb; }

  private:
    uint32_t msb;
    uint32_t lsb;
};