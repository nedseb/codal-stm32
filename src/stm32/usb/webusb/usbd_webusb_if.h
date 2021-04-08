/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_WEBUSB_IF_H__
#define __USBD_WEBUSB_IF_H__

#ifdef USBCON

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include "usbd_webusb.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void webUSB_init(void);
void webUSB_deInit(void);

#ifdef __cplusplus
}
#endif
#endif /* USBCON */
#endif /* __USBD_WEBUSB_IF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/