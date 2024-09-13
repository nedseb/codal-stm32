#pragma once

#include <cstdint>
#include <string>
#include <vector>

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
    static void printHex(uint32_t msb, uint32_t lsb) { printf("0x%0.8X%0.8X", msb, lsb); }

    static std::string hexToString(uint8_t val, char separator = 0) { return hexToString(val, (uint8_t)1, separator); }
    static std::string hexToString(uint16_t val, char separator = 0) { return hexToString(val, (uint8_t)2, separator); }
    static std::string hexToString(uint32_t val, char separator = 0) { return hexToString(val, (uint8_t)4, separator); }
    static std::string hexToString(uint32_t val, uint8_t nbBytes, char separator)
    {
        std::string result;

        for (int8_t i = nbBytes - 1; i >= 0; --i) {
            char buffer[2] = {0};
            uint8_t tmp    = (val >> (i * 8)) & 0x000000FF;
            sprintf(buffer, "%0.2X", tmp);
            result.append(buffer);

            if (separator != 0) result.push_back(separator);
        }

        if (separator != 0)
            return result.substr(0, result.size() - 1);
        else
            return result;
    }

    static std::string dataToSafeString(std::vector<uint8_t>& data, char defaultChar = '?')
    {
        std::string str(data.size(), 0);
        for (uint8_t i = 0; i < data.size(); ++i) {
            str[i] = safeChar(data[i], defaultChar);
        }

        return str;
    }

    static char safeChar(uint8_t c, char def = '?') { return (c >= 32 && c <= 126) ? (char)c : def; }
};