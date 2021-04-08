
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_WEBUSB_H
#define __USB_WEBUSB_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbd_ioreq.h"
#include "usbd_def.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_WEBUSB
  * @brief This file is the header file for usbd_webusb.c
  * @{
  */


/** @defgroup USBD_WEBUSB_Exported_Defines
  * @{
  */
// #define TEMPLATE_EPIN_ADDR                 0x81U
// #define TEMPLATE_EPIN_SIZE                 0x10U

#define USB_WEBUSB_CONFIG_DESC_SIZ       64U

/**
  * @}
  */


/** @defgroup USBD_WEBUSB_Exported_TypesDefinitions
  * @{
  */

/**
  * @}
  */



/** @defgroup USBD_WEBUSB_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_WEBUSB_Exported_Variables
  * @{
  */

extern USBD_ClassTypeDef USBD_WEBUSB_ClassDriver;
#define USBD_WEBUSB_Class &USBD_WEBUSB_ClassDriver
/**
  * @}
  */

/** @defgroup USBD_WEBUSB_Exported_Functions
  * @{
  */
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif  /* __USB_WEBUSB_H */
/**
  * @}
  */

/**
  * @}
  */
