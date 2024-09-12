/**
 ******************************************************************************
 * @file    usbd_webusb.h
 * @author  MCD Application Team
 * @brief   header file for the usbd_webusb.c file.
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
#ifndef __USB_WEBUSB_H
#define __USB_WEBUSB_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbd_ep_conf.h"
#include "usbd_ioreq.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
 * @{
 */

/** @defgroup usbd_cdc
 * @brief This file is the Header file for usbd_cdc.c
 * @{
 */

/** @defgroup usbd_cdc_Exported_Defines
 * @{
 */
#ifndef WebUSB_HS_BINTERVAL
#define WebUSB_HS_BINTERVAL 0x10U
#endif /* WebUSB_HS_BINTERVAL */

#ifndef WebUSB_FS_BINTERVAL
#define WebUSB_FS_BINTERVAL 0x10U
#endif /* WebUSB_FS_BINTERVAL */

/* CDC Endpoints parameters */

#define USB_WebUSB_CONFIG_DESC_SIZ     76U
#define WebUSB_DATA_HS_IN_PACKET_SIZE  CDC_DATA_HS_MAX_PACKET_SIZE
#define WebUSB_DATA_HS_OUT_PACKET_SIZE CDC_DATA_HS_MAX_PACKET_SIZE

#define WebUSB_DATA_FS_IN_PACKET_SIZE  CDC_DATA_FS_MAX_PACKET_SIZE
#define WebUSB_DATA_FS_OUT_PACKET_SIZE CDC_DATA_FS_MAX_PACKET_SIZE

#define WebUSB_REQ_MAX_DATA_SIZE 0x7U
/*---------------------------------------------------------------------*/
/*  CDC definitions                                                    */
/*---------------------------------------------------------------------*/
#define WebUSB_SEND_ENCAPSULATED_COMMAND 0x00U
#define WebUSB_GET_ENCAPSULATED_RESPONSE 0x01U
#define WebUSB_SET_COMM_FEATURE          0x02U
#define WebUSB_GET_COMM_FEATURE          0x03U
#define WebUSB_CLEAR_COMM_FEATURE        0x04U
#define WebUSB_SET_LINE_CODING           0x20U
#define WebUSB_GET_LINE_CODING           0x21U
#define WebUSB_SET_CONTROL_LINE_STATE    0x22U
#define WebUSB_SEND_BREAK                0x23U

// Control Line State bits
#define CLS_DTR (1 << 0)
#define CLS_RTS (1 << 1)

/**
 * @}
 */

/** @defgroup USBD_CORE_Exported_TypesDefinitions
 * @{
 */

/**
 * @}
 */
typedef struct {
    uint32_t bitrate;
    uint8_t format;
    uint8_t paritytype;
    uint8_t datatype;
} USBD_WebUSB_LineCodingTypeDef;

typedef struct _USBD_WebUSB_Itf {
    int8_t (*Init)(void);
    int8_t (*DeInit)(void);
    int8_t (*Control)(uint8_t cmd, uint8_t* pbuf, uint16_t length);
    int8_t (*Receive)(uint8_t* Buf, uint32_t* Len);
    int8_t (*TransmitCplt)(uint8_t* Buf, uint32_t* Len, uint8_t epnum);
} USBD_WebUSB_ItfTypeDef;

typedef struct {
    uint32_t data[CDC_DATA_HS_MAX_PACKET_SIZE / 4U]; /* Force 32-bit alignment */
    uint8_t CmdOpCode;
    uint8_t CmdLength;
    uint8_t* RxBuffer;
    uint8_t* TxBuffer;
    uint32_t RxLength;
    uint32_t TxLength;

    __IO uint32_t TxState;
    __IO uint32_t RxState;
} USBD_WebUSB_HandleTypeDef;

/** @defgroup USBD_CORE_Exported_Macros
 * @{
 */

/**
 * @}
 */

/** @defgroup USBD_CORE_Exported_Variables
 * @{
 */

extern USBD_ClassTypeDef USBD_WebUSB;
#define USBD_WebUSB_CLASS &USBD_WebUSB
/**
 * @}
 */

/** @defgroup USB_CORE_Exported_Functions
 * @{
 */
uint8_t USBD_WebUSB_RegisterInterface(USBD_HandleTypeDef* pdev, USBD_WebUSB_ItfTypeDef* fops);

#ifdef USE_USBD_COMPOSITE
uint8_t USBD_WebUSB_SetTxBuffer(USBD_HandleTypeDef* pdev, uint8_t* pbuff, uint32_t length, uint8_t ClassId);
uint8_t USBD_WebUSB_TransmitPacket(USBD_HandleTypeDef* pdev, uint8_t ClassId);
uint8_t USBD_WebUSB_ClearBuffer(USBD_HandleTypeDef* pdev, uint8_t ClassId);
#else
uint8_t USBD_WebUSB_SetTxBuffer(USBD_HandleTypeDef* pdev, uint8_t* pbuff, uint32_t length);
uint8_t USBD_WebUSB_TransmitPacket(USBD_HandleTypeDef* pdev);
uint8_t USBD_WebUSB_ClearBuffer(USBD_HandleTypeDef* pdev);
#endif /* USE_USBD_COMPOSITE */
uint8_t USBD_WebUSB_SetRxBuffer(USBD_HandleTypeDef* pdev, uint8_t* pbuff);
uint8_t USBD_WebUSB_ReceivePacket(USBD_HandleTypeDef* pdev);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __USB_WEBUSB_H */
/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
