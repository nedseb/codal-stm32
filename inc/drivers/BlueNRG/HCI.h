#pragma once

#include <array>
#include <cstdint>
#include <list>
#include <string>

#include "data_format.h"
#include "le_parameters.h"
#include "opcode.h"

constexpr uint8_t ADVERTISING_CHANNEL_37 = 0x01;
constexpr uint8_t ADVERTISING_CHANNEL_38 = 0x02;
constexpr uint8_t ADVERTISING_CHANNEL_39 = 0x04;

class HCI {
  public:
    HCI() : isDebug(false) {}
    virtual ~HCI() {}

    void enableDebug() { isDebug = true; }
    void disableDebug() { isDebug = false; }

    virtual void init()          = 0;
    virtual void resetHardware() = 0;

    void poll();

    /**
     * @brief The HCI_Reset command will reset the Controller and the Link Manager on the BR/EDR Controller or the Link
      Layer on an LE Controller. If the Controller supports both BR/EDR and LE then the HCI_Reset command shall reset
     the Link Manager, Baseband and Link Layer. The HCI_Reset command shall not affect the used HCI transport layer
     since the HCI transport layers may have reset mechanisms of their own. After the reset is completed, the current
     operational state will be lost, the Controller will enter standby mode and the Controller will automatically revert
     to the default values for the parameters for which default values are defined in the specification.

      Note: The HCI_Reset command will not necessarily perform a hardware reset. This is implementation defined. The
     Host shall not send additional HCI commands before the

      HCI_Command_Complete event related to the HCI_Reset command has been received.
     *
     * @return true if succes, false otherwise
     */
    bool resetSoftware();

    /**
     * @brief
     *
     * @return std::array<uint32_t, 2>
     */
    std::array<uint32_t, 2> readBD_ADDR();

    /**
     * @brief The HCI_LE_Rand command is used to request the Controller to generate 8 octets of random data to be sent
     * to the Host.
     *
     * @return std::array<uint32_t, 2>
     */
    std::array<uint32_t, 2> leRand();

    /**
     * @brief The HCI_LE_Set_Advertising_Parameters command is used by the Host to set the advertising parameters.
     *
     * @param advertisingIntervalMin Minimum advertising interval for undirected and low duty cycle directed
     * advertising. (Time Range: 20 ms to 10240 ms (10.24 s) )
     * @param advertisingIntervalMax Maximum advertising interval for undirected and low duty cycle directed
     * advertising. (Time Range: 20 ms to 10240 ms (10.24 s) )
     * @param advertisingType is used to determine the packet type that is used for advertising when advertising is
     * enabled.
     * @param ownAddressType parameter indicates the type of address being used in the advertising packets.
     * @param peerAddressType contains the Peer’s Identity Type
     * @param peerAddress Public Device Address, Random Device Address, Public Identity Address, or Random (static)
     * Identity Address of the device to be connected
     * @param advertisingChannelMap is a bit field that indicates the advertising channel indices that shall be used
     * when transmitting advertising packets. At least one channel bit shall be set
     * @param advertisingFilterPolicy parameter shall be ignored when directed advertising is enabled
     * @return true if succes, false otherwise
     */
    bool leSetAdvertisingParameters(
        float advertisingIntervalMin = 1280, float advertisingIntervalMax = 1280,
        AdvertisingType advertisingType = AdvertisingType::ADV_IND,
        OwnAddressType ownAddressType   = OwnAddressType::PUBLIC,
        PeerAddressType peerAddressType = PeerAddressType::PUBLIC, uint64_t peerAddress = 0x0000000000000000,
        uint8_t advertisingChannelMap = ADVERTISING_CHANNEL_37 | ADVERTISING_CHANNEL_38 | ADVERTISING_CHANNEL_39,
        AdvertisingFilterPolicy advertisingFilterPolicy = AdvertisingFilterPolicy::ALL_DEVICE);

    /**
     * @brief The HCI_LE_Read_Advertising_Physical_Channel_Tx_Power command is used by the Host to read the transmit
     * power level used for LE advertising physical channel packets
     *
     * @return int8_t Range: -127 to 20 (dBm). Accuracy: ±4 dB
     */
    int8_t leReadAdvertisingPhysicalChannelTxPower();

    /**
     * @brief The HCI_LE_Set_Advertising_Data command is used to set the data used in advertising packets that have a
     * data field.
     *
     * @param significantSize The number of significant octets in the Advertising_Data.
     * @param data 31 octets of Scan_Response_Data formatted as defined in [Vol 3] PartC, Section 11. All octets zero
     * (default)
     * @return true if succes, false otherwise
     */
    bool leSetAdvertisingData(uint8_t significantSize, uint8_t* data);

    /**
     * @brief This command is used to provide data used in Scanning Packets that have a data field.
     *
     * @param significantSize The number of significant octets in the Scan_Response_Data.
     * @param data 31 octets of Scan_Response_Data formatted as defined in [Vol 3] PartC, Section 11. All octets zero
     * (default)
     * @return true if succes, false otherwise
     */
    bool leSetScanResponseData(uint8_t significantSize, uint8_t* data);

    /**
     * @brief The HCI_LE_Set_Advertising_Enable command is used to request the Controller to start or stop advertising.
     *
     * @param enable true to enable advertising, false otherwise.
     * @return true if succes, false otherwise
     */
    bool leSetAdvertisingEnable(bool enable);

  protected:
    bool isDebug;
    std::list<EventPacket> eventPackets;
    std::list<AsyncDataPacket> asyncDataPackets;

    virtual std::vector<uint8_t> sendCommand(OpCodeCommand command, uint8_t nbArgs, const uint8_t* args) = 0;
    virtual std::vector<uint8_t> sendCommand(OpCodeCommand command) { return sendCommand(command, 0, NULL); }
    virtual uint8_t available()                          = 0;
    virtual uint8_t readByte()                           = 0;
    virtual void readBytes(uint8_t* data, uint8_t size)  = 0;
    virtual bool writeBytes(uint8_t* data, uint8_t size) = 0;

  private:
    void cleanPackets();
};