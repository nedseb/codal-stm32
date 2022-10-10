#pragma once

#include <cstdint>

struct LeReadBufferSizeResult {
    bool success;
    uint16_t leAclDataPacketLength;
    uint8_t totalNumLEAclDataPacket;
};