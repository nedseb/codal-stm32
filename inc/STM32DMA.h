#pragma once

#if defined(STM32WBxx)
#include <cstdint>

#include "stm32_def.h"

enum class DMA_Request : uint32_t {
    MEM2MEM = DMA_REQUEST_MEM2MEM, /*!< memory to memory transfer  */

    GENERATOR0 = DMA_REQUEST_GENERATOR0, /*!< DMAMUX request generator 0 */
    GENERATOR1 = DMA_REQUEST_GENERATOR1, /*!< DMAMUX request generator 1 */
    GENERATOR2 = DMA_REQUEST_GENERATOR2, /*!< DMAMUX request generator 2 */
    GENERATOR3 = DMA_REQUEST_GENERATOR3, /*!< DMAMUX request generator 3 */

    ADC_1 = DMA_REQUEST_ADC1, /*!< DMAMUX ADC1 request        */

    SPI_1_RX = DMA_REQUEST_SPI1_RX, /*!< DMAMUX SPI1 RX request     */
    SPI_1_TX = DMA_REQUEST_SPI1_TX, /*!< DMAMUX SPI1 TX request     */
#if defined(SPI2)
    SPI_2_RX = DMA_REQUEST_SPI2_RX, /*!< DMAMUX SPI2 RX request     */
    SPI_2_TX = DMA_REQUEST_SPI2_TX, /*!< DMAMUX SPI2 TX request     */
#endif                              /* SPI2 */

    I2C_1_RX = DMA_REQUEST_I2C1_RX, /*!< DMAMUX I2C1 RX request     */
    I2C_1_TX = DMA_REQUEST_I2C1_TX, /*!< DMAMUX I2C1 TX request     */
#if defined(I2C3)
    I2C_3_RX = DMA_REQUEST_I2C3_RX, /*!< DMAMUX I2C3 RX request     */
    I2C_3_TX = DMA_REQUEST_I2C3_TX, /*!< DMAMUX I2C3 TX request     */
#endif                              /* I2C3 */

    USART_1_RX = DMA_REQUEST_USART1_RX, /*!< DMAMUX USART1 RX request   */
    USART_1_TX = DMA_REQUEST_USART1_TX, /*!< DMAMUX USART1 TX request   */

#if defined(LPUART1)
    LPUART_1_RX = DMA_REQUEST_LPUART1_RX, /*!< DMAMUX LP_UART1_RX request */
    LPUART_1_TX = DMA_REQUEST_LPUART1_TX, /*!< DMAMUX LP_UART1_RX request */
#endif                                    /* LPUART1 */

#if defined(SAI1)
    SAI_1_A = DMA_REQUEST_SAI1_A, /*!< DMAMUX SAI1 A request      */
    SAI_1_B = DMA_REQUEST_SAI1_B, /*!< DMAMUX SAI1 B request      */
#endif                            /* SAI1 */

#if defined(QUADSPI)
    QUADSPI_1 = DMA_REQUEST_QUADSPI, /*!< DMAMUX QUADSPI request     */
#endif                               /* QUADSPI */

    TIM_1_CH1  = DMA_REQUEST_TIM1_CH1,  /*!< DMAMUX TIM1 CH1 request    */
    TIM_1_CH2  = DMA_REQUEST_TIM1_CH2,  /*!< DMAMUX TIM1 CH2 request    */
    TIM_1_CH3  = DMA_REQUEST_TIM1_CH3,  /*!< DMAMUX TIM1 CH3 request    */
    TIM_1_CH4  = DMA_REQUEST_TIM1_CH4,  /*!< DMAMUX TIM1 CH4 request    */
    TIM_1_UP   = DMA_REQUEST_TIM1_UP,   /*!< DMAMUX TIM1 UP  request    */
    TIM_1_TRIG = DMA_REQUEST_TIM1_TRIG, /*!< DMAMUX TIM1 TRIG request   */
    TIM_1_COM  = DMA_REQUEST_TIM1_COM,  /*!< DMAMUX TIM1 COM request    */

    TIM_2_CH1 = DMA_REQUEST_TIM2_CH1, /*!< DMAMUX TIM2 CH1 request    */
    TIM_2_CH2 = DMA_REQUEST_TIM2_CH2, /*!< DMAMUX TIM2 CH2 request    */
    TIM_2_CH3 = DMA_REQUEST_TIM2_CH3, /*!< DMAMUX TIM2 CH3 request    */
    TIM_2_CH4 = DMA_REQUEST_TIM2_CH4, /*!< DMAMUX TIM2 CH4 request    */
    TIM_2_UP  = DMA_REQUEST_TIM2_UP,  /*!< DMAMUX TIM2 UP  request    */

    TIM_16_CH1 = DMA_REQUEST_TIM16_CH1, /*!< DMAMUX TIM16 CH1 request   */
    TIM_16_UP  = DMA_REQUEST_TIM16_UP,  /*!< DMAMUX TIM16 UP  request   */

    TIM_17_CH1 = DMA_REQUEST_TIM17_CH1, /*!< DMAMUX TIM17 CH1 request   */
    TIM_17_UP  = DMA_REQUEST_TIM17_UP,  /*!< DMAMUX TIM17 UP  request   */

#if defined(AES1)
    AES_1_IN  = DMA_REQUEST_AES1_IN,  /*!< DMAMUX AES1 IN request     */
    AES_1_OUT = DMA_REQUEST_AES1_OUT, /*!< DMAMUX AES1 OUT request    */
#endif                                /* AES1 */

    AES_2_IN  = DMA_REQUEST_AES2_IN,  /*!< DMAMUX AES2 IN request     */
    AES_2_OUT = DMA_REQUEST_AES2_OUT, /*!< DMAMUX AES2 OUT request    */
};

enum class DMA_Direction : uint32_t {
    PERIPH_TO_MEMORY = DMA_PERIPH_TO_MEMORY, /*!< Peripheral to memory direction */
    MEMORY_TO_PERIPH = DMA_MEMORY_TO_PERIPH, /*!< Memory to peripheral direction */
    MEMORY_TO_MEMORY = DMA_MEMORY_TO_MEMORY, /*!< Memory to memory direction     */
};

enum class DMA_PeripheralIncrementMode : uint32_t {
    ENABLE  = DMA_PINC_ENABLE,  /*!< Peripheral increment mode Enable  */
    DISABLE = DMA_PINC_DISABLE, /*!< Peripheral increment mode Disable */
};

enum class DMA_MemoryIncrementMode : uint32_t {
    ENABLE  = DMA_MINC_ENABLE,  /*!< Memory increment mode Enable  */
    DISABLE = DMA_MINC_DISABLE, /*!< Memory increment mode Disable */
};

enum class DMA_PeripheralDataSize : uint32_t {
    BYTE     = DMA_PDATAALIGN_BYTE,     /*!< Peripheral data alignment : Byte     */
    HALFWORD = DMA_PDATAALIGN_HALFWORD, /*!< Peripheral data alignment : HalfWord */
    WORD     = DMA_PDATAALIGN_WORD,     /*!< Peripheral data alignment : Word     */
};

enum class DMA_MemoryDataSize : uint32_t {
    BYTE     = DMA_MDATAALIGN_BYTE,     /*!< Memory data alignment : Byte     */
    HALFWORD = DMA_MDATAALIGN_HALFWORD, /*!< Memory data alignment : HalfWord */
    WORD     = DMA_MDATAALIGN_WORD,     /*!< Memory data alignment : Word     */
};

enum class DMA_Mode : uint32_t {
    NORMAL   = DMA_NORMAL,   /*!< Normal mode   */
    CIRCULAR = DMA_CIRCULAR, /*!< Circular mode */
};

enum class DMA_Priority : uint32_t {
    LOW       = DMA_PRIORITY_LOW,       /*!< Priority level : Low       */
    MEDIUM    = DMA_PRIORITY_MEDIUM,    /*!< Priority level : Medium    */
    HIGH      = DMA_PRIORITY_HIGH,      /*!< Priority level : High      */
    VERY_HIGH = DMA_PRIORITY_VERY_HIGH, /*!< Priority level : Very_High */
};

namespace codal {
class STM32DMA {
  public:
    STM32DMA();
    ~STM32DMA();

    /**
     * @brief Initialize DMA
     *
     * @param instance
     * @param request Specifies the request selected for the specified channel.
     * @param direction Specifies if the data will be transferred from memory to peripheral, from memory to memory or
     * from peripheral to memory.
     * @param peripheralIncrement Specifies whether the Peripheral address register should be incremented or not.
     * @param memoryIncrement Specifies whether the memory address register should be incremented or not.
     * @param peripheralDataAlingment Specifies the Peripheral data width.
     * @param memoryDataAlingment Specifies the Memory data width.
     * @param mode Specifies the operation mode of the DMAy Channelx. @note The circular buffer mode cannot be used if
     * the memory-to-memory data transfer is configured on the selected Channel
     * @param priority Specifies the software priority for the DMAy Channelx.
     * @return `true` if the initialization is success, `false` otherwise.
     */
    bool init(DMA_Channel_TypeDef* instance, DMA_Request request, DMA_Direction direction,
              DMA_PeripheralIncrementMode peripheralIncrement = DMA_PeripheralIncrementMode::DISABLE,
              DMA_MemoryIncrementMode memoryIncrement         = DMA_MemoryIncrementMode::ENABLE,
              DMA_PeripheralDataSize peripheralDataAlingment  = DMA_PeripheralDataSize::WORD,
              DMA_MemoryDataSize memoryDataAlingment = DMA_MemoryDataSize::WORD, DMA_Mode mode = DMA_Mode::CIRCULAR,
              DMA_Priority priority = DMA_Priority::LOW);

    void deinit();

    void enableIRQ(uint32_t priority, uint32_t subPriority);
    void disableIRQ();

    bool isInitialized() { return initialized; }
    DMA_HandleTypeDef* getHandle() { return hdma; };

  private:
    DMA_HandleTypeDef* hdma;
    IRQn_Type irqn;
    bool initialized;

    IRQn_Type getIRQn(DMA_Channel_TypeDef* channel);
};
}  // namespace codal
#else
#warning "STM32DMA is not supported by your target !"
#endif