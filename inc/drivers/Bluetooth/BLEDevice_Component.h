#pragma once

#include "BLEDevice.h"
#include "CodalComponent.h"
#include "Event.h"

namespace codal {
class BLEDevice_Component : public BLEDevice, CodalComponent {
  public:
    /**
     * @brief Create a news BLEDevice as a Codal Component
     *
     * @param id  The CODAL IS
     * @param hci The HCI object to communicate with the BLUENRG
     * @param swapPeriod Period for switching between Advertising and Scanning (if needed)
     */
    BLEDevice_Component(uint16_t id, HCI* hci, uint16_t swapPeriod = 5'000);
    virtual ~BLEDevice_Component() {}

    virtual int init() override final;

    void onUpdateRequest(Event);
    void onSwapRequest(Event);

  private:
    bool isTimerEventConfigured;
    uint16_t swapPeriod;
};
}  // namespace codal