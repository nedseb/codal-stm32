#pragma once

#if defined(STM32WBxx)
#include <vector>

#include "HCI.h"
#include "opcode.h"

class HCI_SharedMemory : public HCI {
  public:
    HCI_SharedMemory();

    bool init() override final;

    void resetHardware() override final {}

  protected:
    std::vector<uint8_t> sendCommand(OpCodeCommand command, uint8_t nbArgs, const uint8_t* args) override final;

    void readBytes(uint8_t* data, uint8_t size) override final {}
    uint8_t available() override final { return 0; }

  private:
    void start_ble_rf(void);
    void stm32wb_reset(void);
    bool transport_init(void);
    bool stm32wb_start_ble();
};
#endif