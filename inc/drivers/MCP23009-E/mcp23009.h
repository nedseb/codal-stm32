#pragma once

#include <cstdint>

namespace codal {

constexpr uint8_t MCP_IODIR_REGISTER   = 0x00;
constexpr uint8_t MCP_IPOL_REGISTER    = 0x01;
constexpr uint8_t MCP_GPINTEN_REGISTER = 0x02;
constexpr uint8_t MCP_DEFVAL_REGISTER  = 0x03;
constexpr uint8_t MCP_INTCON_REGISTER  = 0x04;
constexpr uint8_t MCP_IOCON_REGISTER   = 0x05;
constexpr uint8_t MCP_GPPU_REGISTER    = 0x06;
constexpr uint8_t MCP_INTF_REGISTER    = 0x07;
constexpr uint8_t MCP_INTCAP_REGISTER  = 0x08;
constexpr uint8_t MCP_GPIO_REGISTER    = 0x09;
constexpr uint8_t MCP_OLAT_REGISTER    = 0x0A;

enum class MCP_DIR : uint8_t { OUTPUT = 0, INPUT = 1 };
enum class MCP_POL : uint8_t { SAME = 0, OPPOSITE = 1 };
enum class MCP_INTEN : uint8_t { DISABLE = 0, ENABLE = 1 };
enum class MCP_LOGIC_LEVEL : uint8_t { LOW = 0, HIGH = 1 };
enum class MCP_INTCON_COMP : uint8_t { PREVIOUS_STATE = 0, DEFVAL_STATE = 1 };
enum class MCP_PULLUP : uint8_t { NO_PULLUP = 0, PULLUP = 1 };
enum class MCP_INT_FLAG : uint8_t { NO_INTERRUPT = 0, INTERRUPT = 1 };

class MCP23009Config {
  public:
    MCP23009Config() : reg(0x00) {}
    MCP23009Config(uint8_t reg) : reg(reg & 0b0010'0111) {}

    /**
     * @brief Set the SEQOP (SEQuential OPeration) configuration bit. Sequential operation disabled, address pointer
     * does not increment.
     *
     * @return MCP23009Config
     */
    MCP23009Config setSEQOP()
    {
        reg |= 0x20;
        return *this;
    }

    /**
     * @brief Clear the SEQOP (SEQuential OPeration) configuration bit. Sequential operation enabled, address pointer
     * increments.
     *
     * @return MCP23009Config
     */
    MCP23009Config clearSEQOP()
    {
        reg &= ~(0x20);
        return *this;
    }

    /**
     * @brief Get if the SEQOP (SEQuential OPeration) configuration bit is set.
     *
     * @return bool
     */
    bool hasSEQOP() { return (reg & 0x20) > 0; }

    /**
     * @brief Set the ODR (Open-DRain output) configuration bit. Open-drain output (overrides the INTPOL bit).
     *
     * @return MCP23009Config
     */
    MCP23009Config setODR()
    {
        reg |= 0x04;
        return *this;
    }

    /**
     * @brief Clear the ODR (Open-DRain output) configuration bit. Active driver output (INTPOL bit sets the polarity).
     *
     * @return MCP23009Config
     */
    MCP23009Config clearODR()
    {
        reg &= ~(0x04);
        return *this;
    }

    /**
     * @brief Get if the ODR (Open-DRain output) configuration bit is set.
     *
     * @return bool
     */
    bool hasODR() { return (reg & 0x04) > 0; }

    /**
     * @brief Set the INTPOL (INT POLarity) configuration bit. Set the polarity to Active-High.
     *
     * @return MCP23009Config
     */
    MCP23009Config setINTPOL()
    {
        reg |= 0x02;
        return *this;
    }

    /**
     * @brief Clear the INTPOL (INT POLarity) configuration bit. Set the polarity to Active-Low.
     *
     * @return MCP23009Config
     */
    MCP23009Config clearINTPOL()
    {
        reg &= ~(0x02);
        return *this;
    }

    /**
     * @brief Get if the INTPOL (INT POLarity) configuration bit is set.
     *
     * @return MCP23009Config
     */
    bool hasINTPOL() { return (reg & 0x02) > 0; }

    /**
     * @brief Set the INTCC (INTerrupt Clearing Control) configuration bit. Reading INTCAP register clears the
     * Interrupt.
     *
     * @return MCP23009Config
     */
    MCP23009Config setINTCC()
    {
        reg |= 0x01;
        return *this;
    }

    /**
     * @brief Clear the INTCC (INTerrupt Clearing Control) configuration bit. Reading GPIO register clears the
     * Interrupt.
     *
     * @return MCP23009Config
     */
    MCP23009Config clearINTCC()
    {
        reg &= ~(0x01);
        return *this;
    }

    /**
     * @brief Get if the INTCC (INTerrupt Clearing Control) configuration bit is set.
     *
     * @return bool
     */
    bool hasINTCC() { return (reg & 0x01) > 0; }

    uint8_t getRegisterValue() { return reg; }

  private:
    uint8_t reg;
};

}  // namespace codal