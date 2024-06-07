#pragma once

#include <STM32I2C.h>

#include <cstdint>
#include <vector>

#include "BQ27441_register.h"

const uint16_t BQ27441_ADDRESS = 0xAA;

namespace codal {
class BQ27441 {
  public:
    BQ27441(STM32I2C* i2c);

    /**
     * @brief Initialize gauge (reset to zero), then seal. After the batterie is inserted the initialization may take up
     * to 4 seconds. Use the `is_init()` function to check if the gauge is ready.
     *
     */
    void init();

    /**
     * @brief Read INITCOMP of gauge control status.
     *
     * @return TRUE if the gauge is ready, FALSE otherwise.
     */
    bool is_init();

    /**
     * @brief Instructs the fuel gauge to return status information to Control() addresses 0x00 and 0x01. The read-only
     * status word contains status bits that are set or cleared either automatically as conditions warrant or through
     * using specified subcommands.
     *
     * @return BQ27441_Control_Status
     */
    BQ27441_Control_Status read_control_status();

    /**
     * @brief Instructs the fuel gauge to return the device type to addresses 0x00 and 0x01. The value returned is
     * 0x0421. (Note: Value returned is 0x0421 even if the product is bq27441-G1 so the distinguishing identification
     * requires both DEVICE_TYPE and DM_CODE)
     *
     * @return uint16_t The Device Type (0x0421)
     */
    uint16_t device_type();

    /**
     * @brief Instructs the fuel gauge to return the firmware version to addresses 0x00 and 0x01.
     *
     * @return uint16_t The firmware version
     */
    uint16_t firmware_version();

    /**
     * @brief Instructs the fuel gauge to return the 8-bit DM Code as the least significant byte of the 16-bit return
     * value at addresses 0x00 and 0x01. The DM_CODE subcommand provides a simple method to determine the configuration
     * code stored in Data Memory.
     *
     * @return uint16_t
     */
    uint16_t dm_code();

    /**
     * @brief returns the contents of the fuel gauging status register, depicting the current operating status.
     *
     * @return BQ27441_Flags
     */
    BQ27441_Flags read_flags();

    /**
     * @brief Check if battery detected by gauge, FALSE otherwise.
     *
     * @return TRUE if detected, FALSE otherwise
     */
    bool is_battery_detected();

    /**
     * @brief Discharging detected.
     *
     * @return TRUE if discharging, FALSE otherwise
     */
    bool is_battery_discharges();

    /**
     * @brief Get the value of the predicted remaining battery capacity expressed as a percentage with a range of 0 to
     * 100%.
     *
     * @return uint8_t Value between 0 and 100
     */
    uint8_t state_of_charge();

    /**
     * @brief The internal temperature sensor value
     *
     * @return float Temperature value in Celsius
     */
    float get_temperature();

    /**
     * @brief The value of the measured cell-pack voltage in V with a range of 0.0 to 6.0 V.
     *
     * @return float Voltage value in Volt
     */
    float get_voltage();

    /**
     * @brief The value that is the average current flow through the sense resistor
     *
     * @return float Current value in Ampere
     */
    float get_average_current();

    /**
     * @brief The value of the average power during charging and discharging of the battery. It is negative during
     * discharge and positive during charge. A value of 0 indicates that the battery is not being discharged.
     *
     * @return float Power value in Watt
     */
    float get_average_power();

    /**
     * @brief This command instructs the fuel gauge to perform a full device reset and reinitialize RAM data to the
     * default values from ROM and is therefore not typically used in field operation. The gauge sets the Flags()
     * [ITPOR] bit and enters the INITIALIZE mode.
     *
     */
    void reset();

    /**
     * @brief This subcommand instructs the fuel gauge to perform a partial (soft) reset from any mode with an OCV
     * measurement. The Flags() [ITPOR] and [CFGUPMODE] bits are cleared and a resimulation occurs to update both
     * StateOfCharge() and StateOfChargeUnfiltered(). Upon exit from CONFIG UPDATE mode, the fuel gauge will check bit 7
     * (0x80) in the Update Status register. If bit 7 (0x80) in the Update Status register is set, the fuel gauge will
     * be placed into the SEALED state.
     *
     */
    void soft_reset();

  private:
    STM32I2C* i2c;

    void seal();
    void unseal();

    void send_control_command(uint16_t sub_command);
    std::vector<uint8_t> read_data(uint8_t command, uint8_t len_data);
};
}  // namespace codal