#pragma once

#include <queue>
#include <vector>

#include "GAP_const.h"
#include "events_code.h"
#include "opcode.h"

class EventPacket {
  public:
    uint8_t eventCode;
    uint8_t length;
    std::vector<uint8_t> params;
    uint32_t time;

    EventPacket() : eventCode(0), length(0), time(0) {}

    bool isFromCommand(OpCodeCommand command)
    {
        return params.size() >= 3 && params[0] == 0x01 && params[1] == ((uint16_t)command & 0x00FF) &&
               params[2] == ((uint16_t)command >> 8);
    }
};

class AsyncDataPacket {
  public:
    uint16_t handle;
    uint8_t flags;
    uint16_t length;
    std::vector<uint8_t> data;
    uint32_t time;

    AsyncDataPacket() : handle(0), flags(0), length(0), time(0){};
};

class ADStructureData {
  public:
    GAP_DataType adType;
    std::vector<uint8_t> adData;

    ADStructureData(size_t dataLength = 0) : adType(GAP_DataType::MANUFACTURER_DATA), adData(dataLength, 0x00) {}
};

class GAPServiceData {
  public:
    uint16_t uuid;
    std::vector<uint8_t> data;

    GAPServiceData(size_t dataLength = 0) : uuid(0x0000), data(dataLength, 0x00) {}
};