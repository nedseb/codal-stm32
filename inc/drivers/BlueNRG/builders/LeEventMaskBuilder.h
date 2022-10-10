#pragma once

#include <array>
#include <cstdint>

class LeEventMaskBuilder {
  public:
    LeEventMaskBuilder() : maskMSB(0x00000000), maskLSB(0x00000000) {}

    /**
     * @brief The HCI_LE_Connection_Complete event indicates to both of the Hosts forming the connection that a new
     * connection has been created. Upon the creation of the connection a Connection_Handle shall be assigned by the
     * Controller, and passed to the Host in this event. If the connection creation fails this event shall be provided
     * to the Host that had issued the HCI_LE_Create_Connection command.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setConnectionComplete()
    {
        maskLSB |= (1 << 0);
        return *this;
    }

    /**
     * @brief The HCI_LE_Advertising_Report event indicates that one or more Bluetooth devices have responded to an
     * active scan or have broadcast advertisements that were received during a passive scan. The Controller may queue
     * these advertising reports and send information from multiple devices in one HCI_LE_Advertising_Report event.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setAdvertisingReport()
    {
        maskLSB |= (1 << 1);
        return *this;
    }

    /**
     * @brief The HCI_LE_Connection_Update_Complete event is used to indicate that the Connection Update procedure has
     * completed.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setConnectionUpdateComplete()
    {
        maskLSB |= (1 << 2);
        return *this;
    }

    /**
     * @brief The HCI_LE_Read_Remote_Features_Complete event is used to indicate the completion of the process of the
     * Controller obtaining the features used on the connection and the features supported by the remote Bluetooth
     * device specified by the Connection_Handle event parameter.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setReadRmoteFeaturesComplete()
    {
        maskLSB |= (1 << 3);
        return *this;
    }

    /**
     * @brief The HCI_LE_Long_Term_Key_Request event indicates that the peer device, in the Central role, is attempting
     * to encrypt or re-encrypt the link and is requesting the Long Term Key from the Host.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setLongTermKeyRequest()
    {
        maskLSB |= (1 << 4);
        return *this;
    }

    /**
     * @brief This event indicates to the Central’s Host or the Peripheral’s Host that the remote device is requesting a
     * change in the connection parameters using the Connection Update procedure. The Host replies either with the
     * HCI_LE_Remote_Connection_Parameter_Request_Reply command or the
     * HCI_LE_Remote_Connection_Parameter_Request_Negative_Reply command.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setRemoteConnectionParameterRequest()
    {
        maskLSB |= (1 << 5);
        return *this;
    }

    /**
     * @brief The HCI_LE_Data_Length_Change event notifies the Host of a change to either the maximum LL Data PDU
     * Payload length or the maximum transmission time of packets containing LL Data PDUs in either direction. The
     * values reported are the limits imposed on the connection by the Link Layer following the change (see [Vol 6] Part
     * B, Section 4.5.10); the actual maximum used on the connection may be less for other reasons. This event shall not
     * be generated if the values have not changed.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setDataLengthChange()
    {
        maskLSB |= (1 << 6);
        return *this;
    }

    /**
     * @brief This event is generated when local P-256 key generation is complete.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setReadLocalP256PublicKeyComplete()
    {
        maskLSB |= (1 << 7);
        return *this;
    }

    /**
     * @brief This event indicates that LE Diffie Hellman key generation has been completed by the Controller.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setGenerateDHKeyComplete()
    {
        maskLSB |= (1 << 8);
        return *this;
    }

    /**
     * @brief The HCI_LE_Enhanced_Connection_Complete event indicates to both of the Hosts forming the connection that a
     * new connection has been created. Upon the creation of the connection a Connection_Handle shall be assigned by the
     * Controller, and passed to the Host in this event. If the connection creation fails, this event shall be provided
     * to the Host that had issued the HCI_LE_Create_Connection or HCI_LE_Extended_Create_Connection command.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setEnhanceConnectionComplete()
    {
        maskLSB |= (1 << 9);
        return *this;
    }

    /**
     * @brief The HCI_LE_Directed_Advertising_Report event indicates that directed advertisements have been received
     * where the advertiser is using a resolvable private address for the TargetA field of the advertising PDU which the
     * Controller is unable to resolve and the Scanning_Filter_Policy is equal to 0x02 or 0x03, see Section 7.8.10.
     * Direct_Address_Type and Direct_Address specify the address the directed advertisements are being directed to.
     * Address_Type and Address specify the address of the advertiser sending the directed advertisements. The
     * Controller may queue these advertising reports and send information from multiple advertisers in one
     * HCI_LE_Directed_Advertising_Report event.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setDirectedAdvertisingReport()
    {
        maskLSB |= (1 << 10);
        return *this;
    }

    /**
     * @brief The HCI_LE_PHY_Update_Complete event is used to indicate that the Controller has changed the transmitter
     * PHY or receiver PHY in use.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setPhyUpdateComplete()
    {
        maskLSB |= (1 << 11);
        return *this;
    }

    /**
     * @brief The HCI_LE_Extended_Advertising_Report event indicates that one or more Bluetooth devices have responded
     * to an active scan or have broadcast advertisements that were received during a passive scan. The Controller may
     * coalesce multiple advertising reports from the same or different advertisers into a single
     * HCI_LE_Extended_Advertising_Report event, provided all the parameters from all the advertising reports fit in a
     * single HCI event.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setExtendedAdvertisingReport()
    {
        maskLSB |= (1 << 12);
        return *this;
    }

    /**
     * @brief The HCI_LE_Periodic_Advertising_Sync_Established event indicates that the Controller has received the
     * first periodic advertising packet from an advertiser after the HCI_LE_Periodic_Advertising_Create_Sync command
     * has been sent to the Controller.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setPeriodicAdvertisingSyncEstablished()
    {
        maskLSB |= (1 << 13);
        return *this;
    }

    /**
     * @brief The HCI_LE_Periodic_Advertising_Report event indicates that the Controller has received a periodic
     * advertisement.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setPeriodicAdvertisingReport()
    {
        maskLSB |= (1 << 14);
        return *this;
    }

    /**
     * @brief The HCI_LE_Periodic_Advertising_Sync_Lost event indicates that the Controller has not received a Periodic
     * Advertising packet from the train identified by Sync_Handle within the timeout period.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setPeriodicAdvertisingSyncLost()
    {
        maskLSB |= (1 << 15);
        return *this;
    }

    /**
     * @brief The HCI_LE_Scan_Timeout event indicates that scanning has ended because the duration has expired.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setScanTimeout()
    {
        maskLSB |= (1 << 16);
        return *this;
    }

    /**
     * @brief The HCI_LE_Advertising_Set_Terminated event indicates that the Controller has terminated advertising in
     * the advertising sets specified by the Advertising_Handle parameter.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setAdvertisingSetTerminated()
    {
        maskLSB |= (1 << 17);
        return *this;
    }

    /**
     * @brief The HCI_LE_Scan_Request_Received event indicates that a SCAN_REQ PDU or an AUX_SCAN_REQ PDU has been
     * received by the advertiser. The request contains a device address from a scanner that is allowed by the
     * advertising filter policy. The advertising set is identified by Advertising_Handle.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setScanRequestReceived()
    {
        maskLSB |= (1 << 18);
        return *this;
    }

    /**
     * @brief The HCI_LE_Channel_Selection_Algorithm event indicates which channel selection algorithm is used on a data
     * physical channel connection
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setChannelSelectionAlgorithm()
    {
        maskLSB |= (1 << 19);
        return *this;
    }

    /**
     * @brief The HCI_LE_Connectionless_IQ_Report event is used by the Controller to report IQ information from the
     * Constant Tone Extension of a received advertising packet forming part of the periodic advertising train
     * identified by Sync_Handle and to report IQ information from the Constant Tone Extension of a received Test Mode
     * packet
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setConnectionlessIQReport()
    {
        maskLSB |= (1 << 20);
        return *this;
    }

    /**
     * @brief The HCI_LE_Connection_IQ_Report event is used by the Controller to report the IQ samples from the Constant
     * Tone Extension of a received packet
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setConnectionIQReport()
    {
        maskLSB |= (1 << 21);
        return *this;
    }

    /**
     * @brief This event is used by the Controller to report an issue following a request to a peer device to reply with
     * a packet containing an LL_CTE_RSP PDU and a Constant Tone Extension. It shall be generated if the packet
     * containing the LL_CTE_RSP PDU sent in response did not contain a Constant Tone Extension or if the peer rejected
     * the request. It shall not be generated if the packet containing the LL_CTE_RSP PDU had a CRC error or if the
     * procedure response timeout timer (see [Vol 6] Part B, Section 5.2) expired.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setCTERequestFailed()
    {
        maskLSB |= (1 << 22);
        return *this;
    }

    /**
     * @brief The HCI_LE_Periodic_Advertising_Sync_Transfer_Received event is used by the Controller to report that it
     * has received periodic advertising synchronization information from the device referred to by the
     * Connection_Handle parameter and either successfully synchronized to the periodic advertising train or timed out
     * while attempting to synchronize. The Status will be zero if it successfully synchronized and non-zero otherwise.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setPeriodicAdvertisingSyncTransferReceived()
    {
        maskLSB |= (1 << 23);
        return *this;
    }

    /**
     * @brief The HCI_LE_CIS_Established event indicates that a CIS has been established, was considered lost before
     * being established, or—on the Central—was rejected by the Peripheral. It is generated by the Controller in the
     * Central and Peripheral. The Connection_Handle parameter shall be set to the value provided in the
     * HCI_LE_Create_CIS command on the Central and in the HCI_LE_CIS_Request event on the Peripheral.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setCISEstablished()
    {
        maskLSB |= (1 << 24);
        return *this;
    }

    /**
     * @brief The HCI_LE_CIS_Request event indicates that a Controller has received a request to establish a CIS. If the
     * Controller receives such a request while the HCI_LE_CIS_Request event is masked away, it shall reject it.
     * Otherwise the Controller shall assign a connection handle for the requested CIS and send the handle in the
     * CIS_Connection_Handle parameter of the event.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setCISRequest()
    {
        maskLSB |= (1 << 25);
        return *this;
    }

    /**
     * @brief The HCI_LE_Create_BIG_Complete event indicates that the HCI_LE_Create_BIG command has completed.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setCreateBIGComplete()
    {
        maskLSB |= (1 << 26);
        return *this;
    }

    /**
     * @brief The HCI_LE_Terminate_BIG_Complete event indicates that the transmission of all the BISes in the BIG are
     * terminated.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setTerminateBIGComplete()
    {
        maskLSB |= (1 << 27);
        return *this;
    }

    /**
     * @brief The HCI_LE_BIG_Sync_Established event indicates that the HCI_LE_BIG_Create_Sync command has completed.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setBIGSyncEstablished()
    {
        maskLSB |= (1 << 28);
        return *this;
    }

    /**
     * @brief The HCI_LE_BIG_Sync_Lost event indicates that the Controller has not received any PDUs on a BIG within the
     * timeout period BIG_Sync_Timeout or the BIG has been terminated by the remote device.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setBIGSyncLost()
    {
        maskLSB |= (1 << 29);
        return *this;
    }

    /**
     * @brief The HCI_LE_Request_Peer_SCA_Complete event indicates that the HCI_LE_Request_Peer_SCA command has been
     * completed.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setRequestPeerSCAComplete()
    {
        maskLSB |= (1 << 30);
        return *this;
    }

    /**
     * @brief The HCI_LE_Path_Loss_Threshold event is used to report a path loss threshold crossing (see [Vol 6] Part B,
     * Section 4.5.16) on the ACL connection identified by the Connection_Handle parameter.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setPathLossThreshold()
    {
        maskLSB |= (1 << 31);
        return *this;
    }

    /**
     * @brief The HCI_LE_Transmit_Power_Reporting event is used to report the transmit power level on the ACL connection
     * identified by the Connection_Handle parameter.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setTransmitPowerReporting()
    {
        maskMSB |= (1 << 0);
        return *this;
    }

    /**
     * @brief The HCI_LE_BIGInfo_Advertising_Report event indicates that the Controller has received an Advertising PDU
     * that contained a BIGInfo field. If the Controller also generates an HCI_LE_Periodic_Advertising_Report event,
     * the HCI_LE_BIGInfo_Advertising_Report event shall immediately follow that event.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setBIGInfoAdvertisingReport()
    {
        maskMSB |= (1 << 1);
        return *this;
    }

    /**
     * @brief The HCI_LE_Subrate_Change event is used to indicate that a Connection Subrate Update procedure has
     * completed and some parameters of the specified connection have changed.
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setSubrateChange()
    {
        maskMSB |= (1 << 2);
        return *this;
    }

    /**
     * @brief Set the default event (LE Connection Complete event, LE Advertising Report event, LE Connection Update
     * Complete event, LE Read Remote Features Complete event, LE Long Term Key Request event)
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setDefault()
    {
        return setConnectionComplete()
            .setAdvertisingReport()
            .setConnectionUpdateComplete()
            .setReadRmoteFeaturesComplete()
            .setLongTermKeyRequest();
    }

    /**
     * @brief Set all mask bits
     *
     * @return LeEventMaskBuilder&
     */
    LeEventMaskBuilder& setAll()
    {
        maskMSB = 0x00000007;
        maskLSB = 0xFFFFFFFF;
        return *this;
    }

    /**
     * @brief Get the first 32 bits (MSB) from the 64-bits mask
     *
     * @return uint32_t
     */
    uint32_t getMSBMask() { return maskMSB; }

    /**
     * @brief Get the last 32 bits (LSB) from the 64-bits mask
     *
     * @return uint32_t
     */
    uint32_t getLSBMask() { return maskLSB; }

  private:
    uint32_t maskMSB;
    uint32_t maskLSB;
};