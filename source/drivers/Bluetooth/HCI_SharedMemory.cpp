#include "HCI_SharedMemory.h"

#if defined(STM32WBxx)
#include "app_conf.h"
#include "clock.h"
#include "hw.h"
#include "otp.h"
#include "shci.h"
#include "shci_tl.h"
#include "stm32_def.h"
#include "tl.h"

using namespace std;

#define CFG_TLBLE_EVT_QUEUE_LENGTH        5
#define CFG_TLBLE_MOST_EVENT_PAYLOAD_SIZE 255 /**< Set to 255 with the memory manager and the mailbox */
#define TL_BLE_EVENT_FRAME_SIZE           (TL_EVT_HDR_SIZE + CFG_TLBLE_MOST_EVENT_PAYLOAD_SIZE)
#define POOL_SIZE                         (CFG_TLBLE_EVT_QUEUE_LENGTH * 4 * DIVC((sizeof(TL_PacketHeader_t) + TL_BLE_EVENT_FRAME_SIZE), 4))

PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static TL_CmdPacket_t BleCmdBuffer;
PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static uint8_t EvtPool[POOL_SIZE];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t SystemCmdBuffer;
PLACE_IN_SECTION("MB_MEM2")
ALIGN(4) static uint8_t SystemSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255];
PLACE_IN_SECTION("MB_MEM2")
ALIGN(4) static uint8_t BleSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t HciAclDataBuffer[sizeof(TL_PacketHeader_t) + 5 + 251];

/* global var used as semaphore to control incoming events */
volatile bool sys_event; /* true : M0 core is already up and running */

constexpr uint32_t BLE_IPCC_TIMEOUT          = 1000;
constexpr uint32_t SEND_COMMAND_TIMEOUT      = 3000;
constexpr uint16_t RECEIVED_BLE_EVENT_LENGTH = 16;
static TL_Evt_t received_ble_evt[RECEIVED_BLE_EVENT_LENGTH];
static uint16_t received_ble_evt_pos = 0;

/*  returns true if sys_event was received, false otherwise */
static bool sysevt_wait(void)
{
    /* sys_event remains false until event is received */
    for (unsigned long start = getCurrentMillis(); (getCurrentMillis() - start) < BLE_IPCC_TIMEOUT;) {
        /* Wait for 10sec max - if not return an error */
        if (sys_event) {
            break;
        }
    }

    if (!sys_event) {
        /*  no event received, timeout occurs */
        return false;
    }
    /*  release immediately, now that M0 runs */
    return true;
}

/*  returns true if sysevt was already received, which means M0 core is
 *  already up and running */
static bool sysevt_check(void)
{
    /*  Check if system is UP and running already */
    for (unsigned long start = getCurrentMillis(); (getCurrentMillis() - start) < 10;) {
        /* Wait for 10ms max - if not return an error */
        if (sys_event) {
            break;
        }
    }
    if (sys_event) {
        /*  release immediately as M0 already runs */
        return true;
    }
    return false;
}

/*  WEAK callbacks from the BLE TL driver - will be called under Interrupt */
static void sysevt_received(void* pdata)
{
    UNUSED(pdata);
    /* For now only READY event is received, so we know this is it */
    __disable_irq();
    sys_event = true;
    __enable_irq();
    /* But later on ... we'll have to parse the answer */
}

static void syscmd_status_not(SHCI_TL_CmdStatus_t status)
{
    (void)status;
}

/* to received BLE packet from the SharedMem */
void evt_received(TL_EvtPacket_t* hcievt)
{
    if (hcievt->evtserial.type == TL_BLEEVT_PKT_TYPE) {
        if (received_ble_evt_pos < RECEIVED_BLE_EVENT_LENGTH) {
            received_ble_evt[received_ble_evt_pos].evtcode = hcievt->evtserial.evt.evtcode;
            received_ble_evt[received_ble_evt_pos].plen    = hcievt->evtserial.evt.plen;
            memcpy(received_ble_evt[received_ble_evt_pos].payload, hcievt->evtserial.evt.payload,
                   hcievt->evtserial.evt.plen);

            received_ble_evt_pos++;
        }
    }

    /*  In case Event belongs to the Evt Pool we need to inform  */
    if (((uint8_t*)hcievt >= EvtPool) && ((uint8_t*)hcievt < (EvtPool + POOL_SIZE))) {
        /*  Free the message from shared memory */
        TL_MM_EvtDone(hcievt);
    }
}

TL_Evt_t next_received_event()
{
    if (received_ble_evt_pos > 0) {
        TL_Evt_t result = received_ble_evt[0];
        received_ble_evt_pos--;
        memmove(received_ble_evt, received_ble_evt + 1, received_ble_evt_pos);
        return result;
    }
    else {
        return TL_Evt_t{.evtcode = 0xFF, .plen = 0, .payload = {}};
    }
}

static void acl_data_ack(void) {}

HCI_SharedMemory::HCI_SharedMemory()
{
    sys_event = false;
}

bool HCI_SharedMemory::init()
{
    if (!sysevt_check()) {
        start_ble_rf();
        /* Take BLE out of reset */
        stm32wb_reset();
        /* "C2 unlocking" */
        if (!transport_init()) return false;
    }

    return stm32wb_start_ble();
}

vector<uint8_t> HCI_SharedMemory::sendCommand(OpCodeCommand command, uint8_t nbArgs, const uint8_t* args)
{
    BleCmdBuffer.cmdserial.type        = 0x01;
    BleCmdBuffer.cmdserial.cmd.cmdcode = (uint16_t)command;
    BleCmdBuffer.cmdserial.cmd.plen    = nbArgs;
    memcpy(BleCmdBuffer.cmdserial.cmd.payload, args, nbArgs);
    TL_BLE_SendCmd(NULL, 0);

    for (uint32_t start = getCurrentMillis(); getCurrentMillis() - start < SEND_COMMAND_TIMEOUT;) {
        TL_Evt_t evt = next_received_event();

        if (evt.evtcode != 0xFF && evt.plen >= 4) {
            uint8_t* payload     = evt.payload + 3;
            uint8_t payload_size = evt.plen - 3;

            return vector<uint8_t>(payload, payload + payload_size);
        }
    }

    return vector<uint8_t>();
}

void HCI_SharedMemory::start_ble_rf()
{
    /* Set the DBP bit in the Power control register 1 (PWR_CR1) */
    LL_PWR_EnableBkUpAccess();

    /* LSE belongs to the back-up domain, enable access.*/
    while (!LL_PWR_IsEnabledBkUpAccess()) {
        /* Wait for Backup domain access */
    }
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();

    /* Enable LSE Oscillator (32.768 kHz) */
    LL_RCC_LSE_Enable();
    while (!LL_RCC_LSE_IsReady()) {
        /* Wait for LSE ready */
    }

    LL_PWR_DisableBkUpAccess();

    /* Switch OFF LSI as LSE is the source clock */
    LL_RCC_LSI2_Disable();
}

void HCI_SharedMemory::stm32wb_reset()
{
    // Reset IPCC
    LL_AHB3_GRP1_EnableClock(LL_AHB3_GRP1_PERIPH_IPCC);

    LL_C1_IPCC_ClearFlag_CHx(IPCC, LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4 |
                                       LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

    LL_C2_IPCC_ClearFlag_CHx(IPCC, LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4 |
                                       LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

    LL_C1_IPCC_DisableTransmitChannel(IPCC, LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 |
                                                LL_IPCC_CHANNEL_4 | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

    LL_C2_IPCC_DisableTransmitChannel(IPCC, LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 |
                                                LL_IPCC_CHANNEL_4 | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

    LL_C1_IPCC_DisableReceiveChannel(IPCC, LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 |
                                               LL_IPCC_CHANNEL_4 | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

    LL_C2_IPCC_DisableReceiveChannel(IPCC, LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 |
                                               LL_IPCC_CHANNEL_4 | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

    /* IPCC default IRQ handlers: IPCC_C1_TX_IRQHandler & IPCC_C1_RX_IRQHandler
     * are mapped in the flash mem area, so that NVIC does not need to SetVector
     */
}

bool HCI_SharedMemory::transport_init()
{
    TL_MM_Config_t tl_mm_config;
    TL_BLE_InitConf_t tl_ble_config;
    /* STM32WB offers a System Channel HCI interface for
       offering system services, with proprietary commands.
       System Channel must be used as well for starting up
       BLE service so we need to initialize it. */
    SHCI_TL_HciInitConf_t shci_init_config;

    /**< Reference table initialization */
    TL_Init();

    /**< System channel initialization */
    shci_init_config.p_cmdbuffer       = (uint8_t*)&SystemCmdBuffer;
    shci_init_config.StatusNotCallBack = syscmd_status_not;
    shci_init(sysevt_received, (void*)&shci_init_config);

    /**< Memory Manager channel initialization */
    tl_mm_config.p_BleSpareEvtBuffer    = BleSpareEvtBuffer;
    tl_mm_config.p_SystemSpareEvtBuffer = SystemSpareEvtBuffer;
    tl_mm_config.p_AsynchEvtPool        = EvtPool;
    tl_mm_config.AsynchEvtPoolSize      = POOL_SIZE;
    TL_MM_Init(&tl_mm_config);

    TL_Enable();

    /*  At this stage, we'll need to wait for ready event,
     *  passed thru TL_SYS_EvtReceived */
    if (!sysevt_wait()) {
        return false;
    }
    else {
        /**< BLE channel initialization */
        tl_ble_config.p_cmdbuffer       = (uint8_t*)&BleCmdBuffer;
        tl_ble_config.p_AclDataBuffer   = HciAclDataBuffer;
        tl_ble_config.IoBusEvtCallBack  = evt_received;
        tl_ble_config.IoBusAclDataTxAck = acl_data_ack;
        TL_BLE_Init((void*)&tl_ble_config);
    }

    return true;
}

bool HCI_SharedMemory::stm32wb_start_ble()
{
    SHCI_C2_Ble_Init_Cmd_Packet_t ble_init_cmd_packet = {0,
                                                         0,
                                                         0, /**< Header unused */
                                                         0, /** pBleBufferAddress not used */
                                                         0, /** BleBufferSize not used */
                                                         CFG_BLE_NUM_GATT_ATTRIBUTES,
                                                         CFG_BLE_NUM_GATT_SERVICES,
                                                         CFG_BLE_ATT_VALUE_ARRAY_SIZE,
                                                         CFG_BLE_NUM_LINK,
                                                         CFG_BLE_DATA_LENGTH_EXTENSION,
                                                         CFG_BLE_PREPARE_WRITE_LIST_SIZE,
                                                         CFG_BLE_MBLOCK_COUNT,
                                                         CFG_BLE_MAX_ATT_MTU,
                                                         CFG_BLE_PERIPHERAL_SCA,
                                                         CFG_BLE_CENTRAL_SCA,
                                                         CFG_BLE_LS_SOURCE,
                                                         CFG_BLE_MAX_CONN_EVENT_LENGTH,
                                                         CFG_BLE_HSE_STARTUP_TIME,
                                                         CFG_BLE_VITERBI_MODE,
                                                         CFG_BLE_OPTIONS,
                                                         0, /** TODO Should be read from HW */
                                                         CFG_BLE_MAX_COC_INITIATOR_NBR,
                                                         CFG_BLE_MIN_TX_POWER,
                                                         CFG_BLE_MAX_TX_POWER,
                                                         CFG_BLE_RX_MODEL_CONFIG,
                                                         CFG_BLE_MAX_ADV_SET_NBR,
                                                         CFG_BLE_MAX_ADV_DATA_LEN,
                                                         CFG_BLE_TX_PATH_COMPENS,
                                                         CFG_BLE_RX_PATH_COMPENS,
                                                         CFG_BLE_CORE_VERSION,
                                                         CFG_BLE_OPTIONS_EXT};
    /**
     * Starts the BLE Stack on CPU2
     */
    return SHCI_C2_BLE_Init(&ble_init_cmd_packet) == SHCI_Success;
}
#endif