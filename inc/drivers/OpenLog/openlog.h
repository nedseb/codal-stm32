#pragma once

#include "STM32Pin.h"
#include "STM32Serial.h"

namespace codal {

/**
 * @brief the class representing the OpenLog micro-sd card reader
 */
class OpenLog {
  public:
    /**
     * @brief Constructor of the OpenLog class
     *
     * @param tx the pin used to transmit data via serial to the OpenLog
     * @param rx the pin used to receive data via serial to the OpenLog
     * @param reset the pin used to reset the OpenLog (via OpenLog's GRN pin)
     *
     * @note As serial is used to communicate with the OpenLog, serial-compatible pins must be provided
     */
    OpenLog(STM32Pin& tx, STM32Pin& rx, STM32Pin& reset);
    ~OpenLog();

    /**
     * @brief Writes data to the micro-sd card, in a file which name's defined in the .cpp.
     *
     * @param data the text to write to the micro-sd card
     * @param dataSize the size of the text to write
     *
     * @return Wether the write operation was successful or not
     *
     * @note As the OpenLog is implemented in order to allow offline sensor data logging,
     * being able to set a file name has not been implemented (as not needed at the moment)
     */
    bool write(char* data, unsigned dataSize);

  private:
    STM32Serial* serial;
    STM32Pin& resetPin;
};
}  // namespace codal