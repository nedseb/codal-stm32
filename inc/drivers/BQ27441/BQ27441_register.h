#pragma once

#include <cstdint>

struct BQ27441_Control_Status {
    union {
        struct {
            /**
             * @brief Reserved
             */
            uint16_t RSVD_2 : 1;

            /**
             * @brief Indicates cell voltages are ok for Qmax updates. True when set.
             */
            uint16_t VOK : 1;

            /**
             * @brief Indicates the fuel gauge Ra table updates are disabled. Updates are disabled when set.
             */
            uint16_t RUP_DIS : 1;

            /**
             * @brief Indicates the algorithm is using constant-power model. True when set. Default is 1.
             */
            uint16_t LDMD : 1;

            /**
             * @brief Indicates the fuel gauge is in SLEEP mode. True when set.
             */
            uint16_t SLEEP : 1;

            /**
             * @brief Reserved
             */
            uint16_t RSVD_1 : 1;

            /**
             * @brief Indicates a request for entry into HIBERNATE from SLEEP mode has been issued. True when set.
             */
            uint16_t HIBERNATE : 1;

            /**
             * @brief Initialization completion bit indicating the initialization is complete. True when set.
             */
            uint16_t INITCOMP : 1;

            /**
             * @brief Indicates that resistance has been updated. True when set. This bit is cleared after a POR or when
             * the Flags() [BAT_DET] bit is set. Also, this bit can only be set after Qmax is updated ([QMAX_UP] bit is
             * set). When this bit is cleared, it enables fast learning of battery impedance.
             */
            uint16_t RES_UP : 1;

            /**
             * @brief Indicates Qmax has updated. True when set. This bit is cleared after a POR or when the Flags()
             * [BAT_DET] bit is set. When this bit is cleared, it enables fast learning of battery Qmax.
             */
            uint16_t QMAX_UP : 1;

            /**
             * @brief Indicates the fuel gauge board calibration routine is active. Active when set.
             */
            uint16_t BCA : 1;

            /**
             * @brief Indicates the fuel gauge Coulomb Counter Auto-Calibration routine is active. The CCA routine will
             * take place approximately 3 minutes and 45 seconds after the initialization as well as periodically as
             * conditions permit. Active when set.
             */
            uint16_t CCA : 1;

            /**
             * @brief Indicates the fuel gauge is in calibration mode. Active when set.
             */
            uint16_t CALMODE : 1;

            /**
             * @brief Indicates the fuel gauge is in the SEALED state. Active when set.
             */
            uint16_t SS : 1;

            /**
             * @brief icates the fuel gauge has performed a Watchdog Reset. Active when set.
             */
            uint16_t WDRESET : 1;

            /**
             * @brief Indicates the fuel gauge has received the SHUTDOWN_ENABLE subcommand and is enabled for SHUTDOWN.
             * Active when set.
             */
            uint16_t SHUTDOWNEN : 1;
        };

        uint16_t control_status;
    };
};

struct BQ27441_Flags {
    union {
        struct {
            /**
             * @brief Discharging detected. True when set.
             */
            uint16_t DSG : 1;

            /**
             * @brief If set, StateOfCharge() ≤ SOCF Set Threshold. The [SOCF] bit will remain set until StateOfCharge()
             * ≥ SOCF Clear Threshold.
             */
            uint16_t SOCF : 1;

            /**
             * @brief If set, StateOfCharge() ≤ SOC1 Set Threshold. The [SOC1] bit will remain set until StateOfCharge()
             * ≥ SOC1 Clear Threshold.
             */
            uint16_t SOC1 : 1;

            /**
             * @brief Battery insertion detected. True when set. When OpConfig [BIE] is set, [BAT_DET] is set by
             * detecting a logic high-to-low transition at the BIN pin. When OpConfig [BIE] is low, [BAT_DET] is set
             * when host issues BAT_INSERT subcommand and is cleared when ho
             */
            uint16_t BAT_DET : 1;

            /**
             * @brief Fuel gauge is in CONFIG UPDATE mode. True when set. Default is 0. Refer to Section 2.4.3 for
             * details.
             */
            uint16_t CFGUPMODE : 1;

            /**
             * @brief Indicates a POR or RESET subcommand has occurred. If set, this bit generally indicates that the
             * RAM configuration registers have been reset to default values and the host should reload the
             * configuration parameters using the CONFIG UPDATE mode. This bit is cleared after the SOFT_RESET
             * subcommand is received.
             */
            uint16_t ITPOR : 1;

            /**
             * @brief Rreserved
             */
            uint16_t RSVD_1 : 1;

            /**
             * @brief Cleared on entry to relax mode and set to 1 when OCV measurement is performed in relax mode.
             */
            uint16_t OCVTAKEN : 1;

            /**
             * @briefFast charging allowed. If SOC changes from 98% to 99% during charging, the [CHG] bit is cleared.
             * The [CHG] bit will become set again when SOC ≤ 95%.
             */
            uint16_t CHG : 1;

            /**
             * @brief Full-charge is detected. If the FC Set% is a positive threshold, [FC] is set when SOC ≥ FC Set %
             * and is cleared when SOC ≤ FC Clear % (default = 98%). By default, FC Set% = –1, therefore [FC] is set
             * when the fuel gauge has detected charge termination.
             */
            uint16_t FC : 1;

            /**
             * @brief Rreserved
             */
            uint16_t RSVD_2 : 4;

            /**
             * @brief Under-Temperature condition is detected. [UT] is set when Temperature() ≤ Under Temp (default =
             * 0°C). [UT] is cleared when Temperature() > Under Temp + Temp Hys.
             */
            uint16_t UT : 1;

            /**
             * @brief Over-Temperature condition is detected. [OT] is set when Temperature() ≥ Over Temp (default =
             * 55°C). [OT] is cleared when Temperature() < Over Temp – Temp Hys.
             */
            uint16_t OT : 1;
        };

        uint16_t flags_status;
    };
};
