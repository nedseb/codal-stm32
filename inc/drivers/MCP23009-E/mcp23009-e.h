#pragma once

#include <functional>

#include "Event.h"
#include "STM32I2C.h"
#include "STM32Pin.h"
#include "mcp23009.h"

namespace codal {

class MCP23009E {
  public:
    MCP23009E(STM32I2C& i2c, uint16_t address, STM32Pin& rst, STM32Pin& inter);

    /**
     * @brief Hardware reset the MCP
     *
     */
    void reset();

    /**
     * @brief Setup a GPIO
     *
     * @param gpx The GP number (0 to 7)
     * @param dir The GP direction (Input or Output)
     * @param pullup Enable the PULLUP
     * @param polarity The pin polarity
     */
    void setup(uint8_t gpx, MCP_DIR dir, MCP_PULLUP pullup = MCP_PULLUP::NO_PULLUP, MCP_POL polarity = MCP_POL::SAME);

    /**
     * @brief Set the logical level (HIGH or LOW) of a GP
     *
     * @param gpx the GP number (0 to 7)
     * @param level The logical level (HIGH or LOW)
     */
    void setLevel(uint8_t gpx, MCP_LOGIC_LEVEL level);

    /**
     * @brief Get the logical level (HIGH or LOW) of a GP
     *
     * @param gpx the GP number (0 to 7)
     * @return MCP_LOGIC_LEVEL
     */
    MCP_LOGIC_LEVEL getLevel(uint8_t gpx);

    /**
     * @brief Enable and register callback for Change event on GP
     *
     * @param gpx the GP number (0 to 7)
     * @param callback called on cahnge event
     */
    void interruptOnChange(uint8_t gpx, std::function<void(MCP_LOGIC_LEVEL)> callback);

    /**
     * @brief Enable and register callback for Falling event on GP
     *
     * @param gpx the GP number (0 to 7)
     * @param callback called on falling event
     */
    void interruptOnFalling(uint8_t gpx, std::function<void(void)> callback);

    /**
     * @brief Enable and register callback for Raising event on GP
     *
     * @param gpx the GP number (0 to 7)
     * @param callback called on raising event
     */
    void interruptOnRaising(uint8_t gpx, std::function<void(void)> callback);

    /**
     * @brief Disable and unregisters callbacks (Change, Falling, Raising) for a GP
     *
     * @param gpx the GP number (0 to 7)
     */
    void disableInterrupt(uint8_t gpx);

    /**
     * @brief Inner CODAL callback. Should not be called.
     */
    void interruptEvent(codal::Event);

    /**
     * @brief Set the IODIR (Input/Output DIRection) register
     *
     * @param raw the register value
     */
    void setIODIR(uint8_t raw) { sendRegister(MCP_IODIR_REGISTER, raw); }

    /**
     * @brief Get the IODIR (Input/Output DIRection) register
     *
     * @return uint8_t the register value
     */
    uint8_t getIODIR() { return readRegister(MCP_IODIR_REGISTER); }

    /**
     * @brief Set the IPOL (Input POLarity) register
     *
     * @param raw the register value
     */
    void setIPOL(uint8_t raw) { sendRegister(MCP_IPOL_REGISTER, raw); }

    /**
     * @brief Get the IPOL (Input POLarity) register
     *
     * @return uint8_t the register value
     */
    uint8_t getIPOL() { return readRegister(MCP_IPOL_REGISTER); }

    /**
     * @brief Set the GPINTEN (GPio INTerrupt-on-change ENable) register
     *
     * @param raw the register value
     */
    void setGPINTEN(uint8_t raw) { sendRegister(MCP_GPINTEN_REGISTER, raw); }

    /**
     * @brief Get the GPINTEN (GPio INTerrupt-on-change ENable) register
     *
     * @return uint8_t the register value
     */
    uint8_t getGPINTEN() { return readRegister(MCP_GPINTEN_REGISTER); }

    /**
     * @brief Set the DEFVAL (DEFault VALue) register
     *
     * @param raw the register value
     */
    void setDEFVAL(uint8_t raw) { sendRegister(MCP_DEFVAL_REGISTER, raw); }

    /**
     * @brief Get the DEFVAL (DEFault VALue) register
     *
     * @return uint8_t the register value
     */
    uint8_t getDEFVAL() { return readRegister(MCP_DEFVAL_REGISTER); }

    /**
     * @brief Set the INTCON (INTerrupt on change CONtrol) register
     *
     * @param raw the register value
     */
    void setINTCON(uint8_t raw) { sendRegister(MCP_INTCON_REGISTER, raw); }

    /**
     * @brief Get the INTCON (INTerrupt on change CONtrol) register
     *
     * @return uint8_t the register value
     */
    uint8_t getINTCON() { return readRegister(MCP_INTCON_REGISTER); }

    /**
     * @brief Set the IOCON (I/O expander CONfiguration) register
     *
     * @param raw the register value
     */
    void setIOCON(MCP23009Config config) { sendRegister(MCP_IOCON_REGISTER, config.getRegisterValue()); }

    /**
     * @brief Get the IOCON (I/O expander CONfiguration) register
     *
     * @return uint8_t the register value
     */
    MCP23009Config getIOCON() { return MCP23009Config(readRegister(MCP_IOCON_REGISTER)); }

    /**
     * @brief Set the GPPU (GPio Pull-Up resistor) register
     *
     * @param raw the register value
     */
    void setGPPU(uint8_t raw) { sendRegister(MCP_GPPU_REGISTER, raw); }

    /**
     * @brief Get the GPPU (GPio Pull-Up resistor) register
     *
     * @return uint8_t the register value
     */
    uint8_t getGPPU() { return readRegister(MCP_GPPU_REGISTER); }

    /**
     * @brief Get the INTF (INTerrupt Flag) register
     *
     * @return uint8_t the register value
     */
    uint8_t getINTF() { return readRegister(MCP_INTF_REGISTER); }

    /**
     * @brief Get the INTCAP (INTerrupt CAPtured value) register
     *
     * @return uint8_t the register value
     */
    uint8_t getINTCAP() { return readRegister(MCP_INTCAP_REGISTER); }

    /**
     * @brief Set the GPIO (General Purpose I/O port) register
     *
     * @param raw the register value
     */
    void setGPIO(uint8_t raw) { sendRegister(MCP_GPIO_REGISTER, raw); }
    /**
     * @brief Get the GPIO (General Purpose I/O port) register
     *
     * @return uint8_t the register value
     */
    uint8_t getGPIO() { return readRegister(MCP_GPIO_REGISTER); }

    /**
     * @brief Set the OLAT (Ouput LATch) register
     *
     * @param raw the register value
     */
    void setOLAT(uint8_t raw) { sendRegister(MCP_OLAT_REGISTER, raw); }

    /**
     * @brief Get the OLAT (Ouput LATch) register
     *
     * @return uint8_t the register value
     */
    uint8_t getOLAT() { return readRegister(MCP_OLAT_REGISTER); }

  private:
    STM32I2C& i2c;
    uint16_t address;
    STM32Pin& resetPin;
    STM32Pin& interruptPin;

    std::function<void(MCP_LOGIC_LEVEL)> eventsChange[8];
    std::function<void(void)> eventsFall[8];
    std::function<void(void)> eventsRise[8];

    void sendRegister(uint8_t reg, uint8_t value);
    uint8_t readRegister(uint8_t reg);
    void sendEnableInterrupt(uint8_t gpx);
    void sendDisableInterrupt(uint8_t gpx);
};
}  // namespace codal
