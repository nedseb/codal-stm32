#pragma once

#include <array>
#include <cstdint>
#include <list>
#include <string>

class AdvertisingFlagsBuilder {
  public:
    AdvertisingFlagsBuilder() : flags(0x00) {}

    AdvertisingFlagsBuilder& addLeLimitedDiscoverableMode()
    {
        flags |= (1 << 0);
        return *this;
    }

    AdvertisingFlagsBuilder& addLeGeneralDiscoverableMode()
    {
        flags |= (1 << 1);
        return *this;
    }

    AdvertisingFlagsBuilder& addBrEdrNotSupported()
    {
        flags |= (1 << 2);
        return *this;
    }

    AdvertisingFlagsBuilder& addSimultaneousLeAndBrEdrController()
    {
        flags |= (1 << 3);
        return *this;
    }

    AdvertisingFlagsBuilder& addSimultaneousLeAndBrEdrHost()
    {
        flags |= (1 << 4);
        return *this;
    }

    uint8_t build() { return flags; }

  private:
    uint8_t flags;
};

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