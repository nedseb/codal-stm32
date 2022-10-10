#pragma once

#include <array>
#include <cstdint>
#include <list>
#include <string>

class AdvertisingData {
  public:
    AdvertisingData();
    ~AdvertisingData();

    void setFlags(uint8_t flags)
    {
        isFlagsSet  = true;
        this->flags = flags;
    }

    void setLocalName(std::string name) { localName = name; }

    void setUserData(std::string data) { userdata = data; }

    /**
     * @brief Generate the Advertising data (significant and non-significant parts).
     *
     * @return std::array<uint8_t, 31>
     */
    std::array<uint8_t, 31> toData();

    /**
     * @brief Get the Significant part size. CAUTION: Call `toData()` before calling this function.
     *
     * @return uint8_t
     */
    uint8_t getSignificantSize() { return significantPartSize; }

  private:
    uint8_t significantPartSize;

    bool isFlagsSet;
    uint8_t flags;

    std::string localName;
    std::string userdata;
};