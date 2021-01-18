#ifndef CODAL_STM32_H
#define CODAL_STM32_H

#define USE_HAL_DRIVER 1

#if defined(STM32F4)
#include "stm32f4xx.h"
#elif defined(STM32L4)
#include "stm32l4xx.h"
#else
#error "TODO: add the rest"
#endif //defined(STM32F4)

#define M24SR64_Y_RF_DISABLE_Pin GPIO_PIN_2
#define M24SR64_Y_RF_DISABLE_GPIO_Port GPIOE
#define USB_OTG_FS_OVRCR_EXTI3_Pin GPIO_PIN_3
#define USB_OTG_FS_OVRCR_EXTI3_GPIO_Port GPIOE
#define M24SR64_Y_GPO_Pin GPIO_PIN_4
#define M24SR64_Y_GPO_GPIO_Port GPIOE
#define SPSGRF_915_GPIO3_EXTI5_Pin GPIO_PIN_5
#define SPSGRF_915_GPIO3_EXTI5_GPIO_Port GPIOE
#define SPSGRF_915_GPIO3_EXTI5_EXTI_IRQn EXTI9_5_IRQn
#define SPBTLE_RF_IRQ_EXTI6_Pin GPIO_PIN_6
#define SPBTLE_RF_IRQ_EXTI6_GPIO_Port GPIOE
#define SPBTLE_RF_IRQ_EXTI6_EXTI_IRQn EXTI9_5_IRQn
#define BUTTON_EXTI13_Pin GPIO_PIN_13
#define BUTTON_EXTI13_GPIO_Port GPIOC
#define BUTTON_EXTI13_EXTI_IRQn EXTI15_10_IRQn
#define ARD_A5_Pin GPIO_PIN_0
#define ARD_A5_GPIO_Port GPIOC
#define ARD_A4_Pin GPIO_PIN_1
#define ARD_A4_GPIO_Port GPIOC
#define ARD_A3_Pin GPIO_PIN_2
#define ARD_A3_GPIO_Port GPIOC
#define ARD_A2_Pin GPIO_PIN_3
#define ARD_A2_GPIO_Port GPIOC
#define ARD_D1_Pin GPIO_PIN_0
#define ARD_D1_GPIO_Port GPIOA
#define ARD_D0_Pin GPIO_PIN_1
#define ARD_D0_GPIO_Port GPIOA
#define ARD_D10_Pin GPIO_PIN_2
#define ARD_D10_GPIO_Port GPIOA
#define ARD_D4_Pin GPIO_PIN_3
#define ARD_D4_GPIO_Port GPIOA
#define ARD_D7_Pin GPIO_PIN_4
#define ARD_D7_GPIO_Port GPIOA
#define ARD_D13_Pin GPIO_PIN_5
#define ARD_D13_GPIO_Port GPIOA
#define ARD_D12_Pin GPIO_PIN_6
#define ARD_D12_GPIO_Port GPIOA
#define ARD_D11_Pin GPIO_PIN_7
#define ARD_D11_GPIO_Port GPIOA
#define ARD_A1_Pin GPIO_PIN_4
#define ARD_A1_GPIO_Port GPIOC
#define ARD_A0_Pin GPIO_PIN_5
#define ARD_A0_GPIO_Port GPIOC
#define ARD_D3_Pin GPIO_PIN_0
#define ARD_D3_GPIO_Port GPIOB
#define ARD_D6_Pin GPIO_PIN_1
#define ARD_D6_GPIO_Port GPIOB
#define ARD_D8_Pin GPIO_PIN_2
#define ARD_D8_GPIO_Port GPIOB
#define DFSDM1_DATIN2_Pin GPIO_PIN_7
#define DFSDM1_DATIN2_GPIO_Port GPIOE
#define ISM43362_RST_Pin GPIO_PIN_8
#define ISM43362_RST_GPIO_Port GPIOE
#define DFSDM1_CKOUT_Pin GPIO_PIN_9
#define DFSDM1_CKOUT_GPIO_Port GPIOE
#define QUADSPI_CLK_Pin GPIO_PIN_10
#define QUADSPI_CLK_GPIO_Port GPIOE
#define QUADSPI_NCS_Pin GPIO_PIN_11
#define QUADSPI_NCS_GPIO_Port GPIOE
#define OQUADSPI_BK1_IO0_Pin GPIO_PIN_12
#define OQUADSPI_BK1_IO0_GPIO_Port GPIOE
#define QUADSPI_BK1_IO1_Pin GPIO_PIN_13
#define QUADSPI_BK1_IO1_GPIO_Port GPIOE
#define QUAD_SPI_BK1_IO2_Pin GPIO_PIN_14
#define QUAD_SPI_BK1_IO2_GPIO_Port GPIOE
#define QUAD_SPI_BK1_IO3_Pin GPIO_PIN_15
#define QUAD_SPI_BK1_IO3_GPIO_Port GPIOE
#define INTERNAL_I2C2_SCL_Pin GPIO_PIN_10
#define INTERNAL_I2C2_SCL_GPIO_Port GPIOB
#define INTERNAL_I2C2_SDA_Pin GPIO_PIN_11
#define INTERNAL_I2C2_SDA_GPIO_Port GPIOB
#define ISM43362_BOOT0_Pin GPIO_PIN_12
#define ISM43362_BOOT0_GPIO_Port GPIOB
#define ISM43362_WAKEUP_Pin GPIO_PIN_13
#define ISM43362_WAKEUP_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_14
#define LED2_GPIO_Port GPIOB
#define SPSGRF_915_SDN_Pin GPIO_PIN_15
#define SPSGRF_915_SDN_GPIO_Port GPIOB
#define INTERNAL_UART3_TX_Pin GPIO_PIN_8
#define INTERNAL_UART3_TX_GPIO_Port GPIOD
#define INTERNAL_UART3_RX_Pin GPIO_PIN_9
#define INTERNAL_UART3_RX_GPIO_Port GPIOD
#define LPS22HB_INT_DRDY_EXTI0_Pin GPIO_PIN_10
#define LPS22HB_INT_DRDY_EXTI0_GPIO_Port GPIOD
#define LPS22HB_INT_DRDY_EXTI0_EXTI_IRQn EXTI15_10_IRQn
#define LSM6DSL_INT1_EXTI11_Pin GPIO_PIN_11
#define LSM6DSL_INT1_EXTI11_GPIO_Port GPIOD
#define LSM6DSL_INT1_EXTI11_EXTI_IRQn EXTI15_10_IRQn
#define USB_OTG_FS_PWR_EN_Pin GPIO_PIN_12
#define USB_OTG_FS_PWR_EN_GPIO_Port GPIOD
#define SPBTLE_RF_SPI3_CSN_Pin GPIO_PIN_13
#define SPBTLE_RF_SPI3_CSN_GPIO_Port GPIOD
#define ARD_D2_Pin GPIO_PIN_14
#define ARD_D2_GPIO_Port GPIOD
#define ARD_D2_EXTI_IRQn EXTI15_10_IRQn
#define HTS221_DRDY_EXTI15_Pin GPIO_PIN_15
#define HTS221_DRDY_EXTI15_GPIO_Port GPIOD
#define HTS221_DRDY_EXTI15_EXTI_IRQn EXTI15_10_IRQn
#define VL53L0X_XSHUT_Pin GPIO_PIN_6
#define VL53L0X_XSHUT_GPIO_Port GPIOC
#define VL53L0X_GPIO1_EXTI7_Pin GPIO_PIN_7
#define VL53L0X_GPIO1_EXTI7_GPIO_Port GPIOC
#define VL53L0X_GPIO1_EXTI7_EXTI_IRQn EXTI9_5_IRQn
#define LSM3MDL_DRDY_EXTI8_Pin GPIO_PIN_8
#define LSM3MDL_DRDY_EXTI8_GPIO_Port GPIOC
#define LSM3MDL_DRDY_EXTI8_EXTI_IRQn EXTI9_5_IRQn
#define LED3_WIFI__LED4_BLE_Pin GPIO_PIN_9
#define LED3_WIFI__LED4_BLE_GPIO_Port GPIOC
#define SPBTLE_RF_RST_Pin GPIO_PIN_8
#define SPBTLE_RF_RST_GPIO_Port GPIOA
#define USB_OTG_FS_VBUS_Pin GPIO_PIN_9
#define USB_OTG_FS_VBUS_GPIO_Port GPIOA
#define USB_OTG_FS_ID_Pin GPIO_PIN_10
#define USB_OTG_FS_ID_GPIO_Port GPIOA
#define USB_OTG_FS_DM_Pin GPIO_PIN_11
#define USB_OTG_FS_DM_GPIO_Port GPIOA
#define USB_OTG_FS_DP_Pin GPIO_PIN_12
#define USB_OTG_FS_DP_GPIO_Port GPIOA
#define SYS_JTMS_SWDIO_Pin GPIO_PIN_13
#define SYS_JTMS_SWDIO_GPIO_Port GPIOA
#define SYS_JTCK_SWCLK_Pin GPIO_PIN_14
#define SYS_JTCK_SWCLK_GPIO_Port GPIOA
#define ARD_D9_Pin GPIO_PIN_15
#define ARD_D9_GPIO_Port GPIOA
#define INTERNAL_SPI3_SCK_Pin GPIO_PIN_10
#define INTERNAL_SPI3_SCK_GPIO_Port GPIOC
#define INTERNAL_SPI3_MISO_Pin GPIO_PIN_11
#define INTERNAL_SPI3_MISO_GPIO_Port GPIOC
#define INTERNAL_SPI3_MOSI_Pin GPIO_PIN_12
#define INTERNAL_SPI3_MOSI_GPIO_Port GPIOC
#define PMOD_RESET_Pin GPIO_PIN_0
#define PMOD_RESET_GPIO_Port GPIOD
#define PMOD_SPI2_SCK_Pin GPIO_PIN_1
#define PMOD_SPI2_SCK_GPIO_Port GPIOD
#define PMOD_IRQ_EXTI12_Pin GPIO_PIN_2
#define PMOD_IRQ_EXTI12_GPIO_Port GPIOD
#define PMOD_UART2_CTS_Pin GPIO_PIN_3
#define PMOD_UART2_CTS_GPIO_Port GPIOD
#define PMOD_UART2_RTS_Pin GPIO_PIN_4
#define PMOD_UART2_RTS_GPIO_Port GPIOD
#define PMOD_UART2_TX_Pin GPIO_PIN_5
#define PMOD_UART2_TX_GPIO_Port GPIOD
#define PMOD_UART2_RX_Pin GPIO_PIN_6
#define PMOD_UART2_RX_GPIO_Port GPIOD
#define STSAFE_A100_RESET_Pin GPIO_PIN_7
#define STSAFE_A100_RESET_GPIO_Port GPIOD
#define SYS_JTD0_SWO_Pin GPIO_PIN_3
#define SYS_JTD0_SWO_GPIO_Port GPIOB
#define ARD_D5_Pin GPIO_PIN_4
#define ARD_D5_GPIO_Port GPIOB
#define SPSGRF_915_SPI3_CSN_Pin GPIO_PIN_5
#define SPSGRF_915_SPI3_CSN_GPIO_Port GPIOB
#define ST_LINK_UART1_TX_Pin GPIO_PIN_6
#define ST_LINK_UART1_TX_GPIO_Port GPIOB
#define ST_LINK_UART1_RX_Pin GPIO_PIN_7
#define ST_LINK_UART1_RX_GPIO_Port GPIOB
#define ARD_D15_Pin GPIO_PIN_8
#define ARD_D15_GPIO_Port GPIOB
#define ARD_D14_Pin GPIO_PIN_9
#define ARD_D14_GPIO_Port GPIOB
#define ISM43362_SPI3_CSN_Pin GPIO_PIN_0
#define ISM43362_SPI3_CSN_GPIO_Port GPIOE
#define ISM43362_DRDY_EXTI1_Pin GPIO_PIN_1
#define ISM43362_DRDY_EXTI1_GPIO_Port GPIOE


/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32L475E_IOT01
  * @{
  */
      
/** @addtogroup STM32L475E_IOT01_LOW_LEVEL
  * @{
  */ 

/** @defgroup STM32L475E_IOT01_LOW_LEVEL_Exported_Types LOW LEVEL Exported Types
  * @{
  */
typedef enum 
{
  DISCO_LED2 = 0,
  DISCO_LED_GREEN = DISCO_LED2,
}Led_TypeDef;


typedef enum 
{  
  BUTTON_USER = 0
}Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
}ButtonMode_TypeDef;

typedef enum 
{
  COM1 = 0,
  COM2 = 0,
}COM_TypeDef;
/**
  * @}
  */ 

/** @defgroup STM32L475E_IOT01_LOW_LEVEL_Exported_Constants LOW LEVEL Exported Constants
  * @{
  */ 

/** 
  * @brief  Define for STM32L475E_IOT01 board
  */ 
#if !defined (USE_STM32L475E_IOT01)
 #define USE_STM32L475E_IOT01
#endif

#define LEDn                             ((uint8_t)1)

#define LED2_PIN                         GPIO_PIN_14
#define LED2_GPIO_PORT                   GPIOB
#define LED2_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()



#define LEDx_GPIO_CLK_ENABLE(__INDEX__)  do{if((__INDEX__) == 0) LED2_GPIO_CLK_ENABLE();}while(0)

#define LEDx_GPIO_CLK_DISABLE(__INDEX__)  do{if((__INDEX__) == 0) LED2_GPIO_CLK_DISABLE();}while(0)

/* Only one User/Wakeup button */
#define BUTTONn                             ((uint8_t)1)

/**
  * @brief Wakeup push-button
  */
#define USER_BUTTON_PIN                   GPIO_PIN_13
#define USER_BUTTON_GPIO_PORT             GPIOC
#define USER_BUTTON_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()
#define USER_BUTTON_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOC_CLK_DISABLE()
#define USER_BUTTON_EXTI_IRQn             EXTI15_10_IRQn

/**
  * @brief  NFC Gpio PINs
  */
#define NFC_GPIO_GPO_PIN                         GPIO_PIN_4
#define NFC_GPIO_GPO_PIN_PORT                    GPIOE
#define NFC_GPIO_RFDISABLE_PIN                   GPIO_PIN_2
#define NFC_GPIO_RFDISABLE_PIN_PORT              GPIOE  
#define NFC_GPIO_CLK_ENABLE()                    __HAL_RCC_GPIOE_CLK_ENABLE();
#define NFC_GPIO_CLK_DISABLE()                   __HAL_RCC_GPIOE_CLK_DISABLE();


#define COMn                              ((uint8_t)1)

/**
 * @brief Definition for COM port1, connected to USART1
 */ 
#define DISCOVERY_COM1                          USART1
#define DISCOVERY_COM1_CLK_ENABLE()             __HAL_RCC_USART1_CLK_ENABLE()
#define DISCOVERY_COM1_CLK_DISABLE()            __HAL_RCC_USART1_CLK_DISABLE()

#define DISCOVERY_COM1_TX_PIN                   GPIO_PIN_6
#define DISCOVERY_COM1_TX_GPIO_PORT             GPIOB
#define DISCOVERY_COM1_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()   
#define DISCOVERY_COM1_TX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOB_CLK_DISABLE()  
#define DISCOVERY_COM1_TX_AF                    GPIO_AF7_USART1

#define DISCOVERY_COM1_RX_PIN                   GPIO_PIN_7
#define DISCOVERY_COM1_RX_GPIO_PORT             GPIOB
#define DISCOVERY_COM1_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()   
#define DISCOVERY_COM1_RX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOB_CLK_DISABLE()  
#define DISCOVERY_COM1_RX_AF                    GPIO_AF7_USART1

#define DISCOVERY_COM1_IRQn                     USART1_IRQn
                                              

#define DISCOVERY_COMx_CLK_ENABLE(__INDEX__)            do { if((__INDEX__) == COM1) {DISCOVERY_COM1_CLK_ENABLE();}} while(0)
#define DISCOVERY_COMx_CLK_DISABLE(__INDEX__)           do { if((__INDEX__) == COM1) {DISCOVERY_COM1_CLK_DISABLE();}} while(0)

#define DISCOVERY_COMx_TX_GPIO_CLK_ENABLE(__INDEX__)    do { if((__INDEX__) == COM1) {DISCOVERY_COM1_TX_GPIO_CLK_ENABLE();}} while(0)
#define DISCOVERY_COMx_TX_GPIO_CLK_DISABLE(__INDEX__)   do { if((__INDEX__) == COM1) {DISCOVERY_COM1_TX_GPIO_CLK_DISABLE();}} while(0)

#define DISCOVERY_COMx_RX_GPIO_CLK_ENABLE(__INDEX__)    do { if((__INDEX__) == COM1) {DISCOVERY_COM1_RX_GPIO_CLK_ENABLE();}} while(0)
#define DISCOVERY_COMx_RX_GPIO_CLK_DISABLE(__INDEX__)   do { if((__INDEX__) == COM1) {DISCOVERY_COM1_RX_GPIO_CLK_DISABLE();}} while(0)


/* User can use this section to tailor I2C2 instance and associated resources */
/* Definition for I2Cx resources */
#define DISCOVERY_I2C2                             I2C2
#define DISCOVERY_I2C2_CLK_ENABLE()                __HAL_RCC_I2C2_CLK_ENABLE()
#define DISCOVERY_I2C2_CLK_DISABLE()               __HAL_RCC_I2C2_CLK_DISABLE()   
#define DISCOVERY_DMA1_CLK_ENABLE()                __HAL_RCC_DMA1_CLK_ENABLE()
#define DISCOVERY_I2C2_SCL_SDA_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()
#define DISCOVERY_I2C2_SCL_SDA_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOB_CLK_DISABLE()
   
#define DISCOVERY_I2C2_FORCE_RESET()               __HAL_RCC_I2C2_FORCE_RESET()
#define DISCOVERY_I2C2_RELEASE_RESET()             __HAL_RCC_I2C2_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define DISCOVERY_I2C2_SCL_PIN                     GPIO_PIN_10
#define DISCOVERY_I2C2_SDA_PIN                     GPIO_PIN_11                                               
#define DISCOVERY_I2C2_SCL_SDA_GPIO_PORT           GPIOB
#define DISCOVERY_I2C2_SCL_SDA_AF                  GPIO_AF4_I2C2

/* I2C interrupt requests */
#define DISCOVERY_I2C2_EV_IRQn                     I2C2_EV_IRQn
#define DISCOVERY_I2C2_ER_IRQn                     I2C2_ER_IRQn


/* User can use this section to tailor I2C1 instance used and associated resources */
/* Definition for I2Cx resources */
#define DISCOVERY_I2C1                             I2C1
#define DISCOVERY_I2C1_CLK_ENABLE()                __HAL_RCC_I2C1_CLK_ENABLE()
#define DISCOVERY_I2C1_CLK_DISABLE()               __HAL_RCC_I2C1_CLK_DISABLE()   
#define DISCOVERY_DMA2_CLK_ENABLE()                __HAL_RCC_DMA2_CLK_ENABLE()
#define DISCOVERY_I2C1_SCL_SDA_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()
#define DISCOVERY_I2C1_SCL_SDA_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOB_CLK_DISABLE()
   
#define DISCOVERY_I2C1_FORCE_RESET()               __HAL_RCC_I2C1_FORCE_RESET()
#define DISCOVERY_I2C1_RELEASE_RESET()             __HAL_RCC_I2C1_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define DISCOVERY_I2C1_SCL_PIN                     GPIO_PIN_8
#define DISCOVERY_I2C1_SDA_PIN                     GPIO_PIN_9                                               
#define DISCOVERY_I2C1_SCL_SDA_GPIO_PORT           GPIOB
#define DISCOVERY_I2C1_SCL_SDA_AF                  GPIO_AF4_I2C1

/* I2C interrupt requests */
#define DISCOVERY_I2C1_EV_IRQn                     I2C1_EV_IRQn
#define DISCOVERY_I2C1_ER_IRQn                     I2C1_ER_IRQn



/* I2C clock speed configuration (in Hz)
  WARNING:
   Make sure that this define is not already declared in other files.
   It can be used in parallel by other modules. */
#ifndef DISCOVERY_I2C_SPEED
 #define DISCOVERY_I2C_SPEED                             100000
#endif /* DISCOVERY_I2C_SPEED */

#ifndef DISCOVERY_I2C_TIMING  
#define DISCOVERY_I2C_TIMING                     ((uint32_t)0x00702681)
#endif /* DISCOVERY_I2Cx_TIMING */ 


/* I2C Sensors address */
/* LPS22HB (Pressure) I2C Address */
#define LPS22HB_I2C_ADDRESS  (uint8_t)0xBA
/* HTS221 (Humidity) I2C Address */
#define HTS221_I2C_ADDRESS   (uint8_t)0xBE

#ifdef USE_LPS22HB_TEMP
/* LPS22HB Sensor hardware I2C address */ 
#define TSENSOR_I2C_ADDRESS     LPS22HB_I2C_ADDRESS
#else /* USE_HTS221_TEMP */
/* HTS221 Sensor hardware I2C address */ 
#define TSENSOR_I2C_ADDRESS     HTS221_I2C_ADDRESS
#endif

#define VL53L0X_I2C_ADDRESS (uint8_t)0x52

/* NFC I2C address and specific config parameters */
#define M24SR_I2C_ADDR             (uint8_t)  0xAC /*!< M24SR address */
#define NFC_I2C_STATUS_SUCCESS     (uint16_t) 0x0000
#define NFC_I2C_ERROR_TIMEOUT      (uint16_t) 0x0011
#define NFC_I2C_TIMEOUT_STD        (uint32_t) 8 /* I2C Time out (ms), used to call Transmit/Receive HAL functions */
#define NFC_I2C_TIMEOUT_MAX        (uint32_t) 200 /* I2C Time out (ms), this is the maximum time needed by M24SR to complete any command */
#define NFC_I2C_TRIALS             (uint32_t) 1 /* In case M24SR will reply ACK failed allow to perform retry before returning error (HAL option not used) */


/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
#define USE_FULL_ASSERT    1U

/**
 * Libc porting layers
 */
#if defined (  __GNUC__  ) /* GCC CS3 */
#define WEAK __attribute__ ((weak))
#endif

#ifdef __cplusplus
extern "C" {
#endif
void _Error_Handler(const char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

#define codal_error(msg) _Error_Handler((msg), 0)

#ifdef __cplusplus
}
#endif



#endif //CODAL_STM32_H
