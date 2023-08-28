#pragma once

#if defined(STM32WBxx)

#include <functional>
#include <vector>

#include "STM32DMA.h"
#include "STM32Pin.h"
#include "stm32_def.h"

namespace codal {

class STM32SAI {
  public:
    /**
     * @brief Construct a new STM32SAI
     *
     * @param data The data pin
     * @param clock The clock pin
     * @param alternate Peripheral to be connected to the selected pins. This parameter can be a value of @ref
     * GPIOEx_Alternate_function_selection
     */
    STM32SAI(STM32Pin* data, STM32Pin* clock, uint8_t alternate, uint16_t bufferSize = 1024);
    ~STM32SAI();

    /**
     * @brief Initialise the SAI peripheral
     *
     * @return true if init is succes, false otherwise
     */
    bool init();

    /**
     * @brief Deinit the SAI peripheral
     *
     */
    void deinit();

    /**
     * @brief Start listening from SAI. (Start, the clock, the DMA and interrupts)
     *
     */
    void startListening();

    /**
     * @brief Stop listening from SAI. (Stop the clock, the DMA and interrupts)
     *
     */
    void stopListening();

    /**
     * @brief Resume the DMA listening (re-enable DMA)
     *
     */
    void resumeDMA();

    /**
     * @brief Pause the DMA listening (disable DMA without stopping SAI)
     *
     */
    void pauseDMA();

    /**
     * @brief Set a callback when data is ready to be read (when the buffer is full). During the callback, the DMA is
     * paused. It will resume immediately after the callback returns.
     * @param callback
     */
    void onReceiveData(std::function<void(const int32_t*)> callback);

    /**
     * @brief Set a callback when an error occured. During the callback, the DMA is
     * paused. It will resume immediately after the callback returns.
     * @param callback
     */
    void onError(std::function<void(const uint32_t)> callback);

    /**
     * @brief Internal callback. Should not be called.
     *
     */
    void mspInit();

    /**
     * @brief Internal callback. Should not be called.
     *
     */
    void mspDeinit();

    /**
     * @brief Internal callback. Should not be called.
     *
     */
    void rxCpltCallback();

    /**
     * @brief Internal callback. Should not be called.
     *
     */
    void errorCallback();

  private:
    STM32Pin* data;
    STM32Pin* clock;

    SAI_HandleTypeDef* hsai;
    STM32DMA* dma;
    uint8_t alternate;
    uint16_t bufferSize;
    int32_t* buffer;

    std::function<void(const int32_t*)> cbOnComplete;
    std::function<void(const uint32_t)> cbOnError;

    void initGPIO();
    void enableClock();
    void disableClock();
};
}  // namespace codal

#else
#warning "STM32SAI is not supported by your target !"
#endif