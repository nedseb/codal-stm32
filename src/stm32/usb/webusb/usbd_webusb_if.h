/**
 ******************************************************************************
 * @file    usbd_WebUSB_if.h
 * @author  MCD Application Team
 * @brief   Header for usbd_WebUSB_if.c file.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                      www.st.com/SLA0044
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_WebUSB_IF_H
#define __USBD_WebUSB_IF_H

#ifdef USBCON
#ifdef USBD_USE_CDC

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "cdc_queue.h"
#include "usbd_webusb.h"

/* Periodically, the state of the buffer "UserTxBuffer" is checked.
   The period depends on WebUSB_POLLING_INTERVAL */
#define WebUSB_POLLING_INTERVAL 2 /* in ms. The max is 65 and the min is 1 */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

extern USBD_WebUSB_ItfTypeDef USBD_WebUSB_fops;
extern CDC_TransmitQueue_TypeDef WebUSB_TransmitQueue;
extern CDC_ReceiveQueue_TypeDef WebUSB_ReceiveQueue;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void webUSB_continue_transmit(void);
bool webUSB_resume_receive(void);
void webUSB_init(void);
void webUSB_deInit(void);
bool webUSB_connected(void);

void webUSB_OnDataReceived(void (*callback)(void));

#ifdef __cplusplus
}
#endif
#endif /* USBD_USE_CDC */
#endif /* USBCON */
#endif /* __USBD_WebUSB_IF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
