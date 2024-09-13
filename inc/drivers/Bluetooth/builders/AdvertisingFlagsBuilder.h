#pragma once

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