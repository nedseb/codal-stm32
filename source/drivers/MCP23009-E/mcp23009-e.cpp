#include "mcp23009-e.h"

#include <codal_target_hal.h>

#include "MessageBus.h"

using namespace codal;
using namespace std;

constexpr void setBitRegister(uint8_t* reg, uint8_t bit, uint8_t value)
{
    if (value == 0) {
        *reg &= ~(1 << bit);
    }
    else {
        *reg |= 1 << bit;
    }
}

constexpr bool isBitSet(uint8_t reg, uint8_t bit)
{
    return (reg & (1 << bit)) > 0;
}

constexpr uint8_t getBitRegister(uint8_t reg, uint8_t bit)
{
    return isBitSet(reg, bit) ? 1 : 0;
}

MCP23009E::MCP23009E(STM32I2C& i2c, uint16_t address, STM32Pin& rst, STM32Pin& inter)
    : i2c(i2c),
      address(address),
      resetPin(rst),
      interruptPin(inter),
      eventsChange{nullptr},
      eventsFall{nullptr},
      eventsRise{nullptr}
{
    reset();

    interruptPin.eventOn(DEVICE_PIN_EVENT_ON_EDGE);
    MessageBus::defaultEventBus->listen(interruptPin.id, DEVICE_PIN_EVT_FALL, this, &MCP23009E::interruptEvent);
}

void MCP23009E::reset()
{
    resetPin.setDigitalValue(0);
    target_wait(5);
    resetPin.setDigitalValue(1);
    target_wait(10);
}

void MCP23009E::setup(uint8_t gpx, MCP_DIR dir, MCP_PULLUP pullup, MCP_POL polarity)
{
    if (gpx > 7) return;

    uint8_t iodir = getIODIR();
    uint8_t ppu   = getGPPU();
    uint8_t ipol  = getIPOL();

    setBitRegister(&iodir, gpx, (uint8_t)dir);
    setBitRegister(&ppu, gpx, (uint8_t)pullup);
    setBitRegister(&ipol, gpx, (uint8_t)polarity);

    setIODIR(iodir);
    setGPPU(ppu);
    setIPOL(ipol);
}

void MCP23009E::setLevel(uint8_t gpx, MCP_LOGIC_LEVEL level)
{
    if (gpx > 7) return;

    uint8_t dir  = getIODIR();
    uint8_t gpio = getGPIO();

    if ((MCP_DIR)getBitRegister(dir, gpx) == MCP_DIR::INPUT) return;

    setBitRegister(&gpio, gpx, (uint8_t)level);
    setGPIO(gpio);
}

MCP_LOGIC_LEVEL MCP23009E::getLevel(uint8_t gpx)
{
    if (gpx > 7) return;

    uint8_t gpio = getGPIO();

    return (MCP_LOGIC_LEVEL)getBitRegister(gpio, gpx);
}

void MCP23009E::interruptOnChange(uint8_t gpx, std::function<void(MCP_LOGIC_LEVEL)> callback)
{
    if (gpx > 7) return;

    sendEnableInterrupt(gpx);

    eventsChange[gpx] = callback;
}

void MCP23009E::interruptOnFalling(uint8_t gpx, std::function<void(void)> callback)
{
    if (gpx > 7) return;

    sendEnableInterrupt(gpx);

    eventsFall[gpx] = callback;
}

void MCP23009E::interruptOnRaising(uint8_t gpx, std::function<void(void)> callback)
{
    if (gpx > 7) return;

    sendEnableInterrupt(gpx);

    eventsRise[gpx] = callback;
}

void MCP23009E::disableInterrupt(uint8_t gpx)
{
    if (gpx > 7) return;

    sendDisableInterrupt(gpx);

    eventsChange[gpx] = nullptr;
    eventsFall[gpx]   = nullptr;
    eventsRise[gpx]   = nullptr;
}

void MCP23009E::interruptEvent(codal::Event)
{
    MCP23009Config iocon = getIOCON();
    uint8_t intf         = getINTF();
    uint8_t state        = 0x00;

    if (iocon.hasINTCC()) {
        state = getINTCAP();
    }
    else {
        state = getGPIO();
    }

    for (uint8_t i = 0; i < 8; i++) {
        if (isBitSet(intf, i)) {
            MCP_LOGIC_LEVEL lvl = (MCP_LOGIC_LEVEL)getBitRegister(state, i);

            if (lvl == MCP_LOGIC_LEVEL::HIGH) {
                if (eventsRise[i] != nullptr) eventsRise[i]();
            }
            else {
                if (eventsFall[i] != nullptr) eventsFall[i]();
            }

            if (eventsChange[i] != nullptr) eventsChange[i](lvl);
        }
    }
}

void MCP23009E::sendRegister(uint8_t reg, uint8_t value)
{
    i2c.beginTransmission(address);
    i2c.writeRegister(reg, value);
    i2c.endTransmission();
}
uint8_t MCP23009E::readRegister(uint8_t reg)
{
    return i2c.readRegister(address, reg, 1)[0];
}

void MCP23009E::sendEnableInterrupt(uint8_t gpx)
{
    uint8_t gpint  = getGPINTEN();
    uint8_t intcon = getINTCON();

    setBitRegister(&gpint, gpx, (uint8_t)MCP_INTEN::ENABLE);
    setBitRegister(&intcon, gpx, (uint8_t)MCP_INTCON_COMP::PREVIOUS_STATE);

    setGPINTEN(gpint);
    setINTCON(intcon);
}

void MCP23009E::sendDisableInterrupt(uint8_t gpx)
{
    uint8_t gpint  = getGPINTEN();
    uint8_t intcon = getINTCON();
    uint8_t defval = getDEFVAL();

    setBitRegister(&gpint, gpx, (uint8_t)MCP_INTEN::DISABLE);
    setBitRegister(&intcon, gpx, (uint8_t)MCP_INTCON_COMP::PREVIOUS_STATE);
    setBitRegister(&defval, gpx, (uint8_t)MCP_LOGIC_LEVEL::LOW);

    setGPINTEN(gpint);
    setINTCON(intcon);
    setDEFVAL(defval);
}
