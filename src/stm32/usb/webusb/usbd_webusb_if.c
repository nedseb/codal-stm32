#ifdef USBCON 

#include "usbd_webusb_if.h"
#include "usbd_desc.h"
#include "usbd_ctlreq.h"


USBD_HandleTypeDef hUSB_Device_WebUSB;
static bool webUSB_initialized = false;

USBD_DescriptorsTypeDef USBD_Desc_webusb;


#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
__ALIGN_BEGIN uint8_t USBD_StrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;

/* USB Device Billboard BOS descriptor Template */
#if (USBD_CLASS_BOS_ENABLED == 1)

#define USB_WEBUSB_SIZ_BOS_DESC 0x1D

__ALIGN_BEGIN  uint8_t USBD_WebUSB_BOSDesc[USB_WEBUSB_SIZ_BOS_DESC] __ALIGN_END = {
    0x05,                                /* bLength */
    USB_DESC_TYPE_BOS,                   /* Device Descriptor Type */
    USB_WEBUSB_SIZ_BOS_DESC, 0x00,       /* Total length of BOS descriptor and all of its sub descs */
    0x01,                                /* The number of separate device capability descriptors in the BOS */

    /* ----------- Device Capability Descriptor: PLATFORM ---------- */
    0x18,       //.bLength
    0x10,       //.bDescriptorType
    0x05,       //.bDevCapabilityType
    0x00,       //.bReserved
    0x38, 0xB6, 0x08, 0x34, 0xA9, 0x09, 0xA0, 0x47, 0x8B, 0xFD, 0xA0, 0x76, 0x88, 0x15, 0xB6, 0x65,     //.PlatformCapabilityUUID
    0x01, 0x00, //.bcdVersion
    0x02,
    0x00,
};
#endif


#if (USBD_CLASS_USER_STRING_DESC == 1)
/**
  * @brief  Returns the Class User string descriptor.
  * @param  speed: Current device speed
  * @param  idx: index of string descriptor
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_WebUSB_Class_UserStrDescriptor(USBD_SpeedTypeDef speed, uint8_t idx, uint16_t *length)
{
    UNUSED(speed);
    
    switch(idx){
        case 10:
            USBD_GetString((uint8_t *)"WebUSB Interface", USBD_StrDesc, length);
            break;

        default:
            USBD_GetString((uint8_t *)"UNKNOWN STRING", USBD_StrDesc, length);
            break;
    }

    return USBD_StrDesc;
}
#endif

#if ((USBD_LPM_ENABLED == 1) || (USBD_CLASS_BOS_ENABLED == 1))
/**
  * @brief  USBD_USR_BOSDescriptor
  *         return the BOS descriptor
  * @param  speed : current device speed
  * @param  length : pointer to data length variable
  * @retval pointer to descriptor buffer
  */
uint8_t *USBD_WebUSB_USR_BOSDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  UNUSED(speed);
  *length = sizeof(USBD_WebUSB_BOSDesc);
  return (uint8_t *)USBD_WebUSB_BOSDesc;
}
#endif




void webUSB_init(void){

    USBD_Desc_webusb.GetDeviceDescriptor = USBD_Desc.GetDeviceDescriptor;
    USBD_Desc_webusb.GetLangIDStrDescriptor = USBD_Desc.GetLangIDStrDescriptor;
    USBD_Desc_webusb.GetManufacturerStrDescriptor = USBD_Desc.GetManufacturerStrDescriptor;
    USBD_Desc_webusb.GetProductStrDescriptor = USBD_Desc.GetProductStrDescriptor;
    USBD_Desc_webusb.GetSerialStrDescriptor = USBD_Desc.GetSerialStrDescriptor;
    USBD_Desc_webusb.GetConfigurationStrDescriptor = USBD_Desc.GetConfigurationStrDescriptor;
    USBD_Desc_webusb.GetInterfaceStrDescriptor = USBD_Desc.GetInterfaceStrDescriptor;
    
#if (USBD_CLASS_USER_STRING_DESC == 1)
    USBD_Desc_webusb.GetUserStrDescriptor = USBD_WebUSB_Class_UserStrDescriptor;
#endif

#if ((USBD_LPM_ENABLED == 1) || (USBD_CLASS_BOS_ENABLED == 1))
    USBD_Desc_webusb.GetBOSDescriptor = USBD_WebUSB_USR_BOSDescriptor;
#endif

    if( webUSB_initialized ) return;

    if( USBD_Init(&hUSB_Device_WebUSB, &USBD_Desc_webusb, 0) == USBD_OK ){

        if( USBD_RegisterClass(&hUSB_Device_WebUSB, USBD_WEBUSB_Class) == USBD_OK ){
            
            USBD_Start(&hUSB_Device_WebUSB);
            webUSB_initialized = true;

        }

    }

}

void webUSB_deInit(void){

    if( !webUSB_initialized ) return;

    USBD_Stop(&hUSB_Device_WebUSB);
    USBD_DeInit(&hUSB_Device_WebUSB);
    webUSB_initialized = false;

}

#endif //USBCON