#include "BQ27441.h"

#include <CodalFiber.h>

#include <cstdio>

using namespace codal;
using namespace std;

/*
 * ==== STANDARD COMMANDS ====
 */

constexpr uint8_t CMD_CONTROL                       = 0x00;
constexpr uint8_t CMD_TEMPERATURE                   = 0x02;
constexpr uint8_t CMD_VOLTAGE                       = 0x04;
constexpr uint8_t CMD_FLAGS                         = 0x06;
constexpr uint8_t CMD_AVERAGE_CURRENT               = 0x10;
constexpr uint8_t CMD_AVERAGE_POWER                 = 0x18;
constexpr uint8_t CMD_STATE_OF_CHARGE               = 0x1C;
constexpr uint8_t CMD_REMAIN_CAPACITY_FILTERED      = 0x2A;
constexpr uint8_t CMD_FULL_CHARGE_CAPACITY_FILTERED = 0x2E;

/*
 * ==== CONTROL SUB-COMMANDS ====
 */

constexpr uint16_t CONTROL_CONTROL_STATUS  = 0x0000;
constexpr uint16_t CONTROL_DEVICE_TYPE     = 0x0001;
constexpr uint16_t CONTROL_FW_VERSION      = 0x0002;
constexpr uint16_t CONTROL_DM_CODE         = 0x0004;
constexpr uint16_t CONTROL_PREV_MACWRITE   = 0x0007;
constexpr uint16_t CONTROL_CHEM_ID         = 0x0008;
constexpr uint16_t CONTROL_BAT_INSERT      = 0x000C;
constexpr uint16_t CONTROL_BAT_REMOVE      = 0x000D;
constexpr uint16_t CONTROL_SET_HIBERNATE   = 0x0011;
constexpr uint16_t CONTROL_CLEAR_HIBERNATE = 0x0012;
constexpr uint16_t CONTROL_SET_CGFUPDATE   = 0x0013;
constexpr uint16_t CONTROL_SHUTDOWN_ENABLE = 0x001B;
constexpr uint16_t CONTROL_SHUTDOWN        = 0x001C;
constexpr uint16_t CONTROL_SEALED          = 0x0020;
constexpr uint16_t CONTROL_TOGGLE_GPIO     = 0x0023;
constexpr uint16_t CONTROL_RESET           = 0x0041;
constexpr uint16_t CONTROL_SOFT_RESET      = 0x0042;
constexpr uint16_t CONTROL_EXIT_CFGUPDATE  = 0x0043;
constexpr uint16_t CONTROL_EXIT_RESIM      = 0x0044;
constexpr uint16_t CONTROL_UNSEAL          = 0x8000;

void BQ27441::send_control_command(uint16_t sub_command)
{
    i2c->beginTransmission(BQ27441_ADDRESS);
    i2c->write(CMD_CONTROL);
    i2c->write(sub_command & 0x00FF);
    i2c->write((sub_command & 0xFF00) >> 8);
    i2c->endTransmission();
}

vector<uint8_t> BQ27441::read_data(uint8_t command, uint8_t len_data)
{
    i2c->beginTransmission(BQ27441_ADDRESS);
    i2c->write(command);
    i2c->endTransmission();

    return i2c->read(BQ27441_ADDRESS, len_data);
}

void BQ27441::seal()
{
    send_control_command(CONTROL_SEALED);
}

void BQ27441::unseal()
{
    send_control_command(CONTROL_UNSEAL);
    send_control_command(CONTROL_UNSEAL);
}

BQ27441::BQ27441(STM32I2C* i2c) : i2c{i2c} {}

void BQ27441::init()
{
    reset();
}

bool BQ27441::is_init()
{
    BQ27441_Control_Status status = read_control_status();

    return status.INITCOMP == 1;
}

BQ27441_Control_Status BQ27441::read_control_status()
{
    BQ27441_Control_Status status;

    send_control_command(CONTROL_CONTROL_STATUS);
    auto data             = read_data(CMD_CONTROL, 2);

    status.control_status = (uint16_t(data[1]) << 8) | data[0];

    return status;
}

uint16_t BQ27441::device_type()
{
    send_control_command(CONTROL_DEVICE_TYPE);
    auto data = read_data(CMD_CONTROL, 2);

    return (uint16_t(data[1]) << 8) | data[0];
}

uint16_t BQ27441::firmware_version()
{
    send_control_command(CONTROL_FW_VERSION);
    auto data = read_data(CMD_CONTROL, 2);

    return (uint16_t(data[1]) << 8) | data[0];
}

uint16_t BQ27441::dm_code()
{
    send_control_command(CONTROL_DM_CODE);
    auto data = read_data(CMD_CONTROL, 2);

    return (uint16_t(data[1]) << 8) | data[0];
}

BQ27441_Flags BQ27441::read_flags()
{
    BQ27441_Flags flags;
    auto data          = read_data(CMD_FLAGS, 2);

    flags.flags_status = (uint16_t(data[1]) << 8) | data[0];
    return flags;
}

bool BQ27441::is_battery_detected()
{
    BQ27441_Flags flags = read_flags();

    return flags.BAT_DET == 1;
}

bool BQ27441::is_battery_discharges()
{
    BQ27441_Flags flags = read_flags();

    return flags.DSG == 1;
}

uint8_t BQ27441::state_of_charge()
{
    return read_data(CMD_STATE_OF_CHARGE, 1)[0];
}

float BQ27441::get_temperature()
{
    auto data              = read_data(CMD_TEMPERATURE, 2);

    int16_t raw_decikelvin = (int16_t(data[1]) << 8) | int16_t(data[0]);

    return static_cast<float>(raw_decikelvin) / 10.0 - 273.15;
}

float BQ27441::get_voltage()
{
    auto data            = read_data(CMD_VOLTAGE, 2);

    int16_t raw_millivol = (int16_t(data[1]) << 8) | int16_t(data[0]);

    return static_cast<float>(raw_millivol) / 1000.0;
}

float BQ27441::get_average_current()
{
    auto data            = read_data(CMD_AVERAGE_CURRENT, 2);

    int16_t raw_milliamp = (int16_t(data[1]) << 8) | int16_t(data[0]);

    return static_cast<float>(raw_milliamp) / 1000.0;
}

float BQ27441::get_average_power()
{
    auto data             = read_data(CMD_AVERAGE_POWER, 2);

    int16_t raw_milliwatt = (int16_t(data[1]) << 8) | int16_t(data[0]);

    return static_cast<float>(raw_milliwatt) / 1000.0;
}

void BQ27441::reset()
{
    unseal();
    send_control_command(CONTROL_RESET);
    seal();
}

void BQ27441::soft_reset()
{
    unseal();
    send_control_command(CONTROL_SOFT_RESET);
    seal();
}
