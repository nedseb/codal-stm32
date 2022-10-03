#pragma once

#include <cstdint>
#include <string>

class BLE_Utils {
  public:
    static uint8_t getMsb(uint16_t value) { return (value & 0xFF00) >> 8; }

    static uint8_t getLsb(uint16_t value) { return (value & 0x00FF); }

    static uint8_t getByte(uint32_t value, uint8_t byte)
    {
        if (byte > 3) {
            return 0;
        }

        return ((value & (0xFF << (byte * 8))) >> (byte * 8));
    }

    static void printHex(uint8_t* values, uint8_t size, std::string prefix = "", std::string newline = "\r\n")
    {
        printf("%s", prefix.c_str());
        for (uint8_t i = 0; i < size; i++) {
            printf("0x%0.2X ", values[i]);

            if ((i + 1) % 8 == 0) {
                printf("%s%s", newline.c_str(), prefix.c_str());
            }
        }

        printf("%s", newline.c_str());
    }

    static void printHex(uint8_t val) { printf("0x%0.2X", val); }

    static void printHex(uint16_t val) { printf("0x%0.4X", val); }

    static void printHex(uint32_t val) { printf("0x%0.8X", val); }
};