#pragma once

#ifdef STM32L4xx
    #include <cstdint>

    #include "stm32l4xx_hal.h"
    #include "stm32l4xx_hal_dfsdm.h"

constexpr uint32_t MP34DT01_AUDIO_BUFFER = 1'024;

namespace codal {

class MP34DT01 {
  public:
    MP34DT01();

    /**
     * @brief Initailize and start the acquisition
     *
     * @return true if initialization succeed, false otherwise
     */
    bool init();

    /**
     * @brief Stop the acquisition and release ressources
     *
     */
    void deinit();

    /**
     * @brief Init the DFSDM Channel. WARNING: Should not be called directly.
     *
     * @param hdfsdm_channel
     */
    void mspChannelInit(DFSDM_Channel_HandleTypeDef* hdfsdm_channel);

    /**
     * @brief Deinit the DFSDL Channel. WARNING: Should not be called directly.
     *
     * @param hdfsdm_channel
     */
    void mspChannelDeinit(DFSDM_Channel_HandleTypeDef* hdfsdm_channel);

    /**
     * @brief Init the DFSDM Filter. WARNING: Should not be called directly.
     *
     * @param hdfsdm_filter
     */
    void mspFilterInit(DFSDM_Filter_HandleTypeDef* hdfsdm_filter);

    /**
     * @brief Deinit the DFSDM Filter. WARNING: Should not be called directly
     *
     * @param hdfsdm_filter
     */
    void mspFilterDeinit(DFSDM_Filter_HandleTypeDef* hdfsdm_filter);

    /**
     * @brief Read half filter converter buffer. WARNING: Should not be called directly
     *
     * @param hdfsdm_filter
     */
    void mspFilterConversionHalfComplete(DFSDM_Filter_HandleTypeDef* hdfsdm_filter);

    /**
     * @brief Read the second half filter converter buffer. WARNING: Should not be called directly
     *
     * @param hdfsdm_filter
     */
    void mspFilterConversionComplete(DFSDM_Filter_HandleTypeDef* hdfsdm_filter);

    /**
     * @brief Get the decibel value
     *
     * @return float
     */
    int32_t getDecibel();

  private:
    uint32_t measuredSquaredRMS;
    int32_t audioBuffer[MP34DT01_AUDIO_BUFFER] = {0};
    DFSDM_Channel_HandleTypeDef* hdfsdm_channel;
    DFSDM_Filter_HandleTypeDef* hdfsdm_filter;
    DMA_HandleTypeDef* hdma_dfsdm;
};

}  // namespace codal
#endif
