#include <cstdint>

struct LisCfgRegA {
    union {
        struct {
            /// @brief These bits select the mode of operation of the device. (00: Continuous mode, 01: Single mode, 10
            /// or 11 IDLE mode) Default value: 11.
            uint8_t MD : 2;

            /// @brief Output data rate configuration (00: 10Hz, 01: 20Hz, 10: 50Hz, 11: 100Hz) Default value: 00
            uint8_t ODR : 2;

            /// @brief Enables low-power mode. Default value: 0 (0: high-resolution mode 1: low-power mode enabled)
            uint8_t LP : 1;

            /// @brief When this bit is set, the configuration registers and user registers are reset. Flash registers
            /// keep their values. Default value: 0
            uint8_t SOFT_RST : 1;

            /// @brief Reboot magnetometer memory content. Default value: 0 (0: normal mode; 1: reboot memory content)
            uint8_t REBOOT : 1;

            /// @brief Enables the magnetometer temperature compensation. Default value: 0 (0: temperature compensation
            /// disabled; 1: temperature compensation enabled)
            uint8_t COMP_TEMP_EN : 1;
        };
        uint8_t reg;
    };
};

struct LisCfgRegB {
    union {
        struct {
            /// @brief Enables low-pass filter (see Table 29). Default value: 0 (0: digital filter disabled; 1: digital
            /// filter enabled)
            uint8_t LPF : 1;

            /// @brief Enables offset cancellation. Default value: 0 (0: offset cancellation disabled; 1: offset
            /// cancellation enabled)
            uint8_t OFF_CANC : 1;

            /// @brief Selects the frequency of the set pulse. Default value: 0 (0: set pulse is released every 63 ODR;
            /// 1: set pulse is released only at power-on after PD condition)
            uint8_t Set_FREQ : 1;

            /// @brief If ‘1’, the interrupt block recognition checks data after the hard-iron correction to discover
            /// the interrupt. Default value: 0
            uint8_t INT_on_DataOFF : 1;

            /// @brief Enables offset cancellation in single measurement mode. The OFF_CANC bit must be set to 1 when
            /// enabling offset cancellation in single measurement mode. Default value: 0 (0: offset cancellation in
            /// single measurement mode disabled; 1: offset cancellation in single measurement mode enabled)
            uint8_t OFF_CANC_ONE_SHOT : 1;
        };
        uint8_t reg;
    };
};

struct LisCfgRegC {
    union {
        struct {
            /// @brief If '1', the data-ready signal (Zyxda bit in STATUS_REG (67h)) is driven on the INT/DRDY pin. The
            /// INT/DRDY pin is configured in push-pull output mode. Default value: 0
            uint8_t DRDY_on_PIN : 1;

            /// @brief If ‘1’, the self-test is enabled.
            uint8_t Self_test : 1;

            /// @brief Set to '1' to enable SDO line on pin 7.
            uint8_t WSPI : 1;

            /// @brief If ‘1’, an inversion of the low and high parts of the data occurs.
            uint8_t BLE : 1;

            /// @brief If enabled, reading of incorrect data is avoided when the user reads asynchronously. In fact if
            /// the read request arrives during an update of the output data, a latch is possible, reading incoherent
            /// high and low parts of the same register. Only one part is updated and the other one remains old.
            uint8_t BDU : 1;

            /// @brief If ‘1’, the I2C interface is inhibited. Only the SPI interface can be used.
            uint8_t I2C_DIS : 1;

            /// @brief If '1', the INTERRUPT signal (INT bit in INT_SOURCE_REG (64h)) is driven on the INT/DRDY pin. The
            /// INT/DRDY pin is configured in push-pull output mode. Default value: 0
            uint8_t INT_on_PIN : 1;
        };
        uint8_t reg;
    };
};