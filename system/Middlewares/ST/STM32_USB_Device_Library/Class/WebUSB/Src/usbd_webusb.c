
/* Includes ------------------------------------------------------------------*/
#include "usbd_webusb.h"
#include "usbd_ctlreq.h"


/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_WEBUSB
  * @brief usbd core module
  * @{
  */

/** @defgroup USBD_WEBUSB_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_WEBUSB_Private_Defines
  * @{
  */

/**
  * @}
  */


/** @defgroup USBD_WEBUSB_Private_Macros
  * @{
  */

/**
  * @}
  */




/** @defgroup USBD_WEBUSB_Private_FunctionPrototypes
  * @{
  */


static uint8_t USBD_WEBUSB_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t USBD_WEBUSB_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t USBD_WEBUSB_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static uint8_t USBD_WEBUSB_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_WEBUSB_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_WEBUSB_EP0_RxReady(USBD_HandleTypeDef *pdev);
static uint8_t USBD_WEBUSB_EP0_TxReady(USBD_HandleTypeDef *pdev);
static uint8_t USBD_WEBUSB_SOF(USBD_HandleTypeDef *pdev);
static uint8_t USBD_WEBUSB_IsoINIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_WEBUSB_IsoOutIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t *USBD_WEBUSB_GetCfgDesc(uint16_t *length);
static uint8_t *USBD_WEBUSB_GetDeviceQualifierDesc(uint16_t *length);
/**
  * @}
  */

/** @defgroup USBD_WEBUSB_Private_Variables
  * @{
  */

USBD_ClassTypeDef USBD_WEBUSB_ClassDriver =
{
  USBD_WEBUSB_Init,
  USBD_WEBUSB_DeInit,
  USBD_WEBUSB_Setup,
  USBD_WEBUSB_EP0_TxReady,
  USBD_WEBUSB_EP0_RxReady,
  USBD_WEBUSB_DataIn,
  USBD_WEBUSB_DataOut,
  USBD_WEBUSB_SOF,
  USBD_WEBUSB_IsoINIncomplete,
  USBD_WEBUSB_IsoOutIncomplete,
  USBD_WEBUSB_GetCfgDesc,
  USBD_WEBUSB_GetCfgDesc,
  USBD_WEBUSB_GetCfgDesc,
  USBD_WEBUSB_GetDeviceQualifierDesc,
};

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
/* USBD_WEBUSB device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_WEBUSB_CfgDesc[USB_WEBUSB_CONFIG_DESC_SIZ] __ALIGN_END =
{
  /* Configuration Descriptor */
  0x09, /* bLength: Configuation Descriptor size */
  USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
  USB_WEBUSB_CONFIG_DESC_SIZ, /* wTotalLength: Bytes returned */
  0x00,
  0x01,         /*bNumInterfaces: 1 interface*/
  0x01,         /*bConfigurationValue: Configuration value*/
  0x00,         /*iConfiguration: Index of string descriptor describing the configuration*/
  0xC0,         /*bmAttributes: bus powered and Supports Remote Wakeup */
  0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/

  /* Interface Descriptor */
  0x09,                     // bLength
  USB_DESC_TYPE_INTERFACE,  // bDescriptorType : Interface
  0x00,                     // bInterfaceNumber
  0x00,                     // bAlternateSetting
  0x00,                     // bNumEndpoints
  0xFF,                     // bInterfaceClass
  0x03,                     // bInterfaceSubClass
  0x00,                     // bInterfaceProtocol
  0x00,                     // bInterface
};

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_WEBUSB_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};

/**
  * @}
  */

/** @defgroup USBD_WEBUSB_Private_Functions
  * @{
  */

/**
  * @brief  USBD_WEBUSB_Init
  *         Initialize the TEMPLATE interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t USBD_WEBUSB_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_WEBUSB_Init
  *         DeInitialize the TEMPLATE layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t USBD_WEBUSB_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_WEBUSB_Setup
  *         Handle the TEMPLATE specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t USBD_WEBUSB_Setup(USBD_HandleTypeDef *pdev,
                                   USBD_SetupReqTypedef *req)
{
  USBD_StatusTypeDef ret = USBD_OK;

  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
  case USB_REQ_TYPE_CLASS :
    switch (req->bRequest)
    {
    default:
      USBD_CtlError(pdev, req);
      ret = USBD_FAIL;
      break;
    }
    break;

  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest)
    {
    default:
      USBD_CtlError(pdev, req);
      ret = USBD_FAIL;
      break;
    }
    break;

  default:
    USBD_CtlError(pdev, req);
    ret = USBD_FAIL;
    break;
  }

  return (uint8_t)ret;
}


/**
  * @brief  USBD_WEBUSB_GetCfgDesc
  *         return configuration descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t *USBD_WEBUSB_GetCfgDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_WEBUSB_CfgDesc);
  return USBD_WEBUSB_CfgDesc;
}

/**
* @brief  DeviceQualifierDescriptor
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
uint8_t *USBD_WEBUSB_DeviceQualifierDescriptor(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_WEBUSB_DeviceQualifierDesc);
  return USBD_WEBUSB_DeviceQualifierDesc;
}


/**
  * @brief  USBD_WEBUSB_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_WEBUSB_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_WEBUSB_EP0_RxReady
  *         handle EP0 Rx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t USBD_WEBUSB_EP0_RxReady(USBD_HandleTypeDef *pdev)
{

  return (uint8_t)USBD_OK;
}
/**
  * @brief  USBD_WEBUSB_EP0_TxReady
  *         handle EP0 TRx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t USBD_WEBUSB_EP0_TxReady(USBD_HandleTypeDef *pdev)
{

  return (uint8_t)USBD_OK;
}
/**
  * @brief  USBD_WEBUSB_SOF
  *         handle SOF event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t USBD_WEBUSB_SOF(USBD_HandleTypeDef *pdev)
{

  return (uint8_t)USBD_OK;
}
/**
  * @brief  USBD_WEBUSB_IsoINIncomplete
  *         handle data ISO IN Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_WEBUSB_IsoINIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum)
{

  return (uint8_t)USBD_OK;
}
/**
  * @brief  USBD_WEBUSB_IsoOutIncomplete
  *         handle data ISO OUT Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_WEBUSB_IsoOutIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum)
{

  return (uint8_t)USBD_OK;
}
/**
  * @brief  USBD_WEBUSB_DataOut
  *         handle data OUT Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_WEBUSB_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum)
{

  return (uint8_t)USBD_OK;
}

/**
* @brief  DeviceQualifierDescriptor
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
uint8_t *USBD_WEBUSB_GetDeviceQualifierDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_WEBUSB_DeviceQualifierDesc);

  return USBD_WEBUSB_DeviceQualifierDesc;
}

/**
  * @}
  */


/**
  * @}
  */


/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
