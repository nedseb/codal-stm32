#include "STM32InterruptHandlers.h"
#include "stm32_def.h"

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */
/******************************************************************************/
void init_Handlers(void) {}

#define UNWEAK(x)   \
    void x(void) {  \
        while (1) { \
        }           \
    }

UNWEAK(HardFault_Handler)
UNWEAK(MemManage_Handler)
UNWEAK(BusFault_Handler)
UNWEAK(UsageFault_Handler)

UNWEAK(WWDG_IRQHandler)
UNWEAK(PVD_PVM_IRQHandler)
UNWEAK(TAMP_STAMP_IRQHandler)
UNWEAK(RTC_WKUP_IRQHandler)
UNWEAK(FLASH_IRQHandler)
UNWEAK(RCC_IRQHandler)
//UNWEAK(EXTI0_IRQHandler)
//UNWEAK(EXTI1_IRQHandler)
//UNWEAK(EXTI2_IRQHandler)
//UNWEAK(EXTI3_IRQHandler)
//UNWEAK(EXTI4_IRQHandler)
// UNWEAK(DMA1_Channel1_IRQHandler)
// UNWEAK(DMA1_Channel2_IRQHandler)
// UNWEAK(DMA1_Channel3_IRQHandler)
//UNWEAK(DMA1_Channel4_IRQHandler)
// UNWEAK(DMA1_Channel5_IRQHandler)
// UNWEAK(DMA1_Channel6_IRQHandler)
// UNWEAK(DMA1_Channel7_IRQHandler)
UNWEAK(ADC1_2_IRQHandler)
UNWEAK(CAN1_TX_IRQHandler)
UNWEAK(CAN1_RX0_IRQHandler)
UNWEAK(CAN1_RX1_IRQHandler)
UNWEAK(CAN1_SCE_IRQHandler)
//UNWEAK(EXTI9_5_IRQHandler)
//UNWEAK(I2C1_EV_IRQHandler)
//UNWEAK(I2C1_ER_IRQHandler)
//UNWEAK(I2C2_EV_IRQHandler)
//UNWEAK(I2C2_ER_IRQHandler)
UNWEAK(SPI1_IRQHandler)
UNWEAK(SPI2_IRQHandler)
//UNWEAK(USART1_IRQHandler)
//UNWEAK(USART2_IRQHandler)
//UNWEAK(USART3_IRQHandler)
//UNWEAK(EXTI15_10_IRQHandler)
UNWEAK(RTC_Alarm_IRQHandler)
UNWEAK(DFSDM1_FLT3_IRQHandler)
UNWEAK(TIM8_BRK_IRQHandler)
UNWEAK(TIM8_TRG_COM_IRQHandler)
UNWEAK(ADC3_IRQHandler)
UNWEAK(FMC_IRQHandler)
UNWEAK(SDMMC1_IRQHandler)
UNWEAK(SPI3_IRQHandler)
//UNWEAK(UART4_IRQHandler)
//UNWEAK(UART5_IRQHandler)
// UNWEAK(DMA2_Channel1_IRQHandler)
// UNWEAK(DMA2_Channel2_IRQHandler)
// UNWEAK(DMA2_Channel3_IRQHandler)
// UNWEAK(DMA2_Channel4_IRQHandler)
// UNWEAK(DMA2_Channel5_IRQHandler)
UNWEAK(DFSDM1_FLT0_IRQHandler)
UNWEAK(DFSDM1_FLT1_IRQHandler)
UNWEAK(DFSDM1_FLT2_IRQHandler)
UNWEAK(COMP_IRQHandler)
UNWEAK(LPTIM1_IRQHandler)
UNWEAK(LPTIM2_IRQHandler)
//UNWEAK(OTG_FS_IRQHandler)
UNWEAK(DMA2_Channel6_IRQHandler)
UNWEAK(DMA2_Channel7_IRQHandler)
//UNWEAK(LPUART1_IRQHandler)
UNWEAK(QUADSPI_IRQHandler)
//UNWEAK(I2C3_EV_IRQHandler)
//UNWEAK(I2C3_ER_IRQHandler)
UNWEAK(SAI1_IRQHandler)
UNWEAK(SAI2_IRQHandler)
UNWEAK(SWPMI1_IRQHandler)
UNWEAK(TSC_IRQHandler)
UNWEAK(RNG_IRQHandler)
UNWEAK(FPU_IRQHandler)

// UNWEAK()

/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void) {}

/**
 * @brief This function handles System service call via SWI instruction.
 */
void SVC_Handler(void) {}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void) {}

/**
 * @brief This function handles Pendable request for system service.
 */
void PendSV_Handler(void) {}

void noOsSystickHandler() {}

void osSystickHandler() __attribute__((weak, alias("noOsSystickHandler")));

/**
  * @brief  Function called when the tick interruption falls
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  osSystickHandler();
}
