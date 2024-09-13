#pragma once

#include <stdint.h>

class EventMaskBuilder {
  public:
    EventMaskBuilder() : maskMSB(0x00000000), maskLSB(0x00000000) {}

    /**
     * @brief The HCI_Inquiry_Complete event indicates that the Inquiry is finished. This event contains a Status
     * parameter, which is used to indicate if the Inquiry completed successfully or if the Inquiry was not completed.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setInquiryComplete()
    {
        maskLSB |= (1 << 0);
        return *this;
    }

    /**
     * @brief The HCI_Inquiry_Result event indicates that a BR/EDR Controller or multiple BR/EDR Controllers have
     * responded so far during the current Inquiry process. This event will be sent from the BR/EDR Controller to the
     * Host as soon as an Inquiry Response from a remote device is received if the remote device supports only mandatory
     * paging scheme. The BR/EDR Controller may queue these Inquiry Responses and send multiple BR/EDR Controllers
     * information in one HCI_Inquiry_Result event. The event can be used to return one or more Inquiry responses in one
     * event.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setInquiryResult()
    {
        maskLSB |= (1 << 1);
        return *this;
    }

    /**
     * @brief The HCI_Connection_Complete event indicates to both of the Hosts forming the connection that a new
     * connection has been established. This event also indicates to the Host which issued the HCI_Create_Connection,
     * HCI_Accept_- Connection_Request, or HCI_Reject_Connection_Request command, and then received an
     * HCI_Command_Status event, if the issued command failed or was successful.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setConnectionComplete()
    {
        maskLSB |= (1 << 2);
        return *this;
    }

    /**
     * @brief The HCI_Connection_Request event is used to indicate that a new incoming connection is trying to be
     * established. The connection may either be accepted or rejected. When the Host receives this event and the link
     * type parameter is ACL, it should respond with either an HCI_Accept_Connection_Request or HCI_Reject_-
     * Connection_Request command before the timer Connection_Accept_Timeout expires. If the link type is SCO or eSCO,
     * the Host should reply with the HCI_Accept_Synchronous_Connection_Request command, the
     * HCI_Enhanced_Accept_Synchronous_Connection_Request command, or the HCI_Reject_Synchronous_Connection_Request
     * command. If the link type is SCO, the Host may respond with the HCI_Accept_Connection_Request command. If the
     * event is responded to with the HCI_Accept_Connection_Request command, then the default parameter settings of the
     * HCI_Accept_Synchronous_- Connection_Request command (see Section 7.1.27) should be used by the local Link Manager
     * when negotiating the SCO link parameters. In that case, the HCI_Connection_Complete event and not the
     * HCI_Synchronous_Connection_- Complete event, shall be returned on completion of the connection.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setConnectionRequest()
    {
        maskLSB |= (1 << 3);
        return *this;
    }

    /**
     * @brief The HCI_Disconnection_Complete event occurs when a connection is terminated. The status parameter
     * indicates if the disconnection was successful or not. The reason parameter indicates the reason for the
     * disconnection if the disconnection was successful. If the disconnection was not successful, the value of the
     * reason parameter shall be ignored by the Host. For example, this can be the case if the Host has issued the
     * HCI_Disconnect command and there was a parameter error, or the command was not presently allowed, or a
     * Connection_Handle that didn’t correspond to a connection was given.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setDisconnectionComplete()
    {
        maskLSB |= (1 << 4);
        return *this;
    }

    /**
     * @brief The HCI_Authentication_Complete event occurs when authentication has been completed for the specified
     * connection. The Connection_Handle shall be a Connection_Handle for an ACL connection. Note: This event is only
     * generated on the initiator of the authentication and not on the responder.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setAuthenticationComplete()
    {
        maskLSB |= (1 << 5);
        return *this;
    }

    /**
     * @brief The HCI_Remote_Name_Request_Complete event is used to indicate that a remote name request has been
     * completed. The Remote_Name event parameter is a UTF-8 encoded string with the type utf8{248} defined in [Vol 1]
     * Part E, Section 2.9.3. The BD_ADDR event parameter is used to identify which device the user-friendly name was
     * obtained from.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setRemoteNameRequestComplete()
    {
        maskLSB |= (1 << 6);
        return *this;
    }

    /**
     * @brief The HCI_Encryption_Change event is used to indicate that the change of the encryption mode has been
     * completed. The Connection_Handle event parameter will be a Connection_Handle for an ACL connection and is used to
     * identify the remote device. The Encryption_Enabled event parameter specifies the new encryption state for the
     * connection specified by Connection_Handle. The Encryption_Key_Size event parameter specifies the size, in octets,
     * of the key used to encrypt the link. This event will occur on both devices to notify the Hosts when encryption
     * has changed for all connections between the two devices. This event shall not be generated if encryption is
     * paused or resumed; during a role switch, for example.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setEncryptionChange()
    {
        maskLSB |= (1 << 7);
        return *this;
    }

    /**
     * @brief The HCI_Change_Connection_Link_Key_Complete event is used to indicate that the change in the Link Key for
     * all connections to a given remote BR/EDR Controller has been completed.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setChangeConnectionLinkKeyComplete()
    {
        maskLSB |= (1 << 8);
        return *this;
    }

    /**
     * @brief The HCI_Link_Key_Type_Changed event is used to indicate that the Link Key managed by the Central of the
     * piconet has been changed. The Connection_- Handle will be a Connection_Handle for an ACL connection within that
     * piconet. The link key used for the connection will be the temporary link key or the semi- permanent link key
     * indicated by the Key_Flag. The Key_Flag event parameter is used to indicate which Link Key (temporary link key or
     * the semi-permanent link keys) is now being used in the piconet.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setLinkKeyTypeChanged()
    {
        maskLSB |= (1 << 9);
        return *this;
    }

    /**
     * @brief The HCI_Read_Remote_Supported_Features_Complete event is used to indicate the completion of the process of
     * the Link Manager obtaining the supported features of the remote BR/EDR Controller specified by the
     * Connection_Handle event parameter. The Connection_Handle will be a Connection_Handle for an ACL connection. The
     * event parameters include a list of LMP features.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setReadRemoteSupportedFeaturesComplete()
    {
        maskLSB |= (1 << 10);
        return *this;
    }

    /**
     * @brief The HCI_Read_Remote_Version_Information_Complete event is used to indicate the completion of the process
     * obtaining the version information of the remote Controller specified by the Connection_Handle event parameter.
     * The Connection_Handle shall be for an ACL connection.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setReadRemoteVersionInformationComplete()
    {
        maskLSB |= (1 << 11);
        return *this;
    }

    /**
     * @brief The HCI_QoS_Setup_Complete event is used to indicate the completion of the process of the Link Manager
     * setting up QoS with the remote BR/EDR Controller specified by the Connection_Handle event parameter. The
     * Connection_Handle will be a Connection_Handle for an ACL connection.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setQoSSetupComplete()
    {
        maskLSB |= (1 << 12);
        return *this;
    }

    /**
     * @brief The HCI_Hardware_Error event is used to notify the Host that a hardware failure has occurred in the
     * Controller.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setHardwareError()
    {
        maskLSB |= (1 << 15);
        return *this;
    }

    /**
     * @brief The HCI_Flush_Occurred event is used to indicate that, for the specified Handle, the current user data to
     * be transmitted has been removed. The Handle shall be a Connection_Handle for an ACL connection. This could result
     * from the HCI_Flush command, or be due to the automatic flush. Multiple blocks of an L2CAP packet could have been
     * pending in the Controller. If one Baseband packet part of an L2CAP PDU is flushed, then the rest of the HCI ACL
     * Data packets for the L2CAP PDU shall also be flushed.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setFlushOccurred()
    {
        maskLSB |= (1 << 16);
        return *this;
    }

    /**
     * @brief The HCI_Role_Change event is used to indicate that the current role of the BR/ EDR Controller related to
     * the particular connection has changed. This event occurs (with Status set to zero) when both the remote and local
     * BR/EDR Controllers have completed their role change for the BR/EDR Controller associated with the BD_ADDR event
     * parameter, allowing both affected Hosts to be notified when the Role has been changed.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setRoleChange()
    {
        maskLSB |= (1 << 17);
        return *this;
    }

    /**
     * @brief The HCI_Mode_Change event is used to indicate when the device associated with the Connection_Handle
     * changes between Active mode, Hold mode, and Sniff mode. The Connection_Handle will be a Connection_Handle for an
     * ACL connection. The Connection_Handle event parameter is used to indicate which connection the HCI_Mode_Change
     * event is for. The Current_Mode event parameter is used to indicate which state the connection is currently in.
     * The Interval parameter is used to specify a time amount specific to each state. Each Controller that is
     * associated with the Connection_Handle which has changed modes shall send the HCI_Mode_Change event to its Host.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setModeChange()
    {
        maskLSB |= (1 << 19);
        return *this;
    }

    /**
     * @brief The HCI_Return_Link_Keys event is used by the BR/EDR Controller to send the Host the BD_ADDRs associated
     * with one or more stored Link Keys. Zero or more instances of this event will occur after the
     * HCI_Read_Stored_Link_Key command. When there are no link keys stored, no HCI_Return_Link_Keys events shall be
     * returned. When there are link keys stored, the number of link keys returned in each HCI_Return_Link_Keys event is
     * implementation specific. This event shall never return the value of the link keys. The link keys value parameter
     * shall always contain the value of zero.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setReturnLinkKeys()
    {
        maskLSB |= (1 << 20);
        return *this;
    }

    /**
     * @brief The HCI_PIN_Code_Request event is used to indicate that a PIN code is required to create a new link key.
     * The Host shall respond using either the HCI_PIN_Code_Request_Reply or the HCI_PIN_Code_Request_Negative_- Reply
     * command, depending on whether the Host can provide the Controller with a PIN code or not.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setPINCodeRequest()
    {
        maskLSB |= (1 << 21);
        return *this;
    }

    /**
     * @brief The HCI_Link_Key_Request event is used to indicate that a Link Key is required for the connection with the
     * device specified in BD_ADDR. If the Host has the requested stored Link Key, then the Host shall pass the
     * requested Key to the Controller using the HCI_Link_Key_Request_Reply command. If the Host does not have the
     * requested stored Link Key, or the stored Link Key does not meet the security requirements for the requested
     * service, then the Host shall use the HCI_Link_Key_Request_Negative_Reply command to indicate to the Controller
     * that the Host does not have the requested key.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setLinkKeyRequest()
    {
        maskLSB |= (1 << 22);
        return *this;
    }

    /**
     * @brief The HCI_Link_Key_Notification event is used to indicate to the Host that a new Link Key has been created
     * for the connection with the device specified in BD_ADDR. The Host can save this new Link Key in its own storage
     * for future use. Also, the Host can decide to store the Link Key in the BR/EDR Controller’s Link Key Storage by
     * using the HCI_Write_Stored_Link_Key command. The Key_Type event parameter informs the Host about which key type
     * (combination key, debug combination key, unauthenticated combination key, authenticated combination key or
     * changed combination key) was used during pairing. If the key type is not supported or is reserved for future use,
     * the Host may discard the key or disconnect the link.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setLinkKeyNotification()
    {
        maskLSB |= (1 << 23);
        return *this;
    }

    /**
     * @brief When in Local Loopback mode, the BR/EDR Controller loops back commands and data to the Host. The
     * HCI_Loopback_Command event is used to loop back all commands that the Host sends to the Controller with some
     * exceptions. See Section 7.6.1 for a description of which commands that are not looped back. The
     * HCI_Command_Packet event parameter contains the entire HCI Command Packet including the header.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setLoopbackCommand()
    {
        maskLSB |= (1 << 24);
        return *this;
    }

    /**
     * @brief This event is used to indicate that the Controller’s data buffers have been overflowed. This can occur if
     * the Host has sent more packets than allowed. The Link_Type parameter is used to indicate the type of data whose
     * buffers overflowed.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setDataBufferOverflow()
    {
        maskLSB |= (1 << 25);
        return *this;
    }

    /**
     * @brief This event is used to notify the Host about the LMP_Max_Slots parameter when the value of this parameter
     * changes. It shall be sent each time the maximum allowed length, in number of slots, for Baseband packets
     * transmitted by the local device, changes. The Connection_Handle will be a Connection_Handle for an ACL
     * connection.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setMaxSlotsChange()
    {
        maskLSB |= (1 << 26);
        return *this;
    }

    /**
     * @brief The HCI_Read_Clock_Offset_Complete event is used to indicate the completion of the process of the Link
     * Manager obtaining the Clock Offset information of the BR/EDR Controller specified by the Connection_Handle event
     * parameter. The Connection_Handle will be a Connection_Handle for an ACL connection.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setReadClockOffsetComplete()
    {
        maskLSB |= (1 << 27);
        return *this;
    }

    /**
     * @brief The HCI_Connection_Packet_Type_Changed event is used to indicate that the process has completed of the
     * Link Manager changing which packet types can be used for the connection. This allows current connections to be
     * dynamically modified to support different types of user data. The Packet_Type event parameter specifies which
     * packet types the Link Manager can use for the connection identified by the Connection_Handle event parameter for
     * sending L2CAP data or voice. The Packet_Type event parameter does not decide which packet types the LM is allowed
     * to use for sending LMP PDUs.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setConnectionPacketTypeChanged()
    {
        maskLSB |= (1 << 28);
        return *this;
    }

    /**
     * @brief The HCI_QoS_Violation event is used to indicate the Controller is unable to provide the current QoS
     * requirement for the Connection identified by the Handle. This event indicates that the Controller is unable to
     * provide one or more of the agreed QoS parameters.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setQoSViolation()
    {
        maskLSB |= (1 << 29);
        return *this;
    }

    /**
     * @brief The HCI_Page_Scan_Repetition_Mode_Change event indicates that the remote BR/EDR Controller with the
     * specified BD_ADDR has successfully changed the Page Scan Repetition Mode (see [Vol 2] Part B, Section 8.3.1).
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setPageScanRepetitionModeChange()
    {
        maskLSB |= (1 << 31);
        return *this;
    }

    /**
     * @brief The HCI_Flow_Specification_Complete event is used to inform the Host about the Quality of Service for the
     * ACL connection the Controller is able to support. The Connection_Handle will be a Connection_Handle for an ACL
     * connection. The flow parameters refer to the outgoing or incoming traffic of the ACL link, as indicated by the
     * Flow_Direction field. The flow parameters are defined in the L2CAP specification [Vol 3] Part A, Section 5.3.
     * When the Status parameter indicates a successful completion, the flow parameters specify the agreed values by the
     * Controller. When the Status parameter indicates a failed completion with the error code QoS Unacceptable
     * Parameter (0x2C), the flow parameters specify the acceptable values of the Controller. This enables the Host to
     * continue the 'QoS negotiation' with a new HCI_Flow_Specification command with flow parameter values that are
     * acceptable for the Controller. When the Status parameter indicates a failed completion with the error code QoS
     * Rejected (0x2D), this indicates a request of the Controller to discontinue the 'QoS negotiation'. When the Status
     * parameter indicates a failed completion, the flow parameter values of the most recently successful completion
     * shall be assumed (or the default values when there was no success completion).
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setFlowSpecificationComplete()
    {
        maskMSB |= (1 << 0);
        return *this;
    }

    /**
     * @brief The HCI_Inquiry_Result_with_RSSI event indicates that a BR/EDR Controller or multiple BR/EDR Controllers
     * have responded so far during the current Inquiry process. This event will be sent from the BR/EDR Controller to
     * the Host as soon as an Inquiry Response from a remote device is received if the remote device supports only
     * mandatory paging scheme. This BR/EDR Controller may queue these Inquiry Responses and send multiple BR/EDR
     * Controllers information in one HCI_Inquiry_Result event. The event can be used to return one or more Inquiry
     * responses in one event. The RSSI parameter is measured during the FHS packet returned by each responding
     * Peripheral.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setInquiryResultwithRSSI()
    {
        maskMSB |= (1 << 1);
        return *this;
    }

    /**
     * @brief The HCI_Read_Remote_Extended_Features_Complete event is used to indicate the completion of the process of
     * the Link Manager obtaining the remote extended LMP features of the remote device specified by the
     * Connection_Handle event parameter. The Connection_Handle will be a Connection_Handle for an ACL connection. The
     * event parameters include a page of the remote devices extended LMP features.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setReadRemoteExtendedFeaturesComplete()
    {
        maskMSB |= (1 << 2);
        return *this;
    }

    /**
     * @brief The HCI_Synchronous_Connection_Complete event is sent to indicate completion of any of the following
     * commands: HCI_Setup_Synchronous_Connection, HCI_Accept_Synchronous_Connection_Request,
     * HCI_Reject_Synchronous_Connection_Request, HCI_Enhanced_Setup_Synchronous_Connection,
     * HCI_Enhanced_Accept_Synchronous_Connection_Request
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setSynchronousConnectionComplete()
    {
        maskMSB |= (1 << 11);
        return *this;
    }

    /**
     * @brief The HCI_Synchronous_Connection_Changed event indicates to the Host that an existing synchronous connection
     * has been reconfigured. This event also indicates to the initiating Host (if the change was Host initiated) if the
     * issued command failed or was successful.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setSynchronousConnectionChanged()
    {
        maskMSB |= (1 << 12);
        return *this;
    }

    /**
     * @brief The HCI_Sniff_Subrating event indicates that the device associated with the Connection_Handle has either
     * enabled sniff subrating or the sniff subrating parameters have been renegotiated by the link manager. The
     * Connection_Handle will be a Connection_Handle for an ACL connection. The Connection_Handle event parameter
     * indicates which connection the HCI_Sniff_Subrating event is for.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setSniffSubrating()
    {
        maskMSB |= (1 << 13);
        return *this;
    }

    /**
     * @brief The HCI_Extended_Inquiry_Result event indicates that a BR/EDR Controller has responded during the current
     * inquiry process with extended inquiry response data. This event will be sent from the BR/EDR Controller to the
     * Host upon reception of an Extended Inquiry Response from a remote device. One single Extended Inquiry Response is
     * returned per event. This event contains RSSI and inquiry response data for the BR/EDR Controller that responded
     * to the latest inquiry. The RSSI parameter is measured during the FHS packet returned by each responding
     * Peripheral. The Num_Responses parameter shall be set to one.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setExtendedInquiryResult()
    {
        maskMSB |= (1 << 14);
        return *this;
    }

    /**
     * @brief The HCI_Encryption_Key_Refresh_Complete event is used to indicate to the Host that the encryption key was
     * refreshed on the given Connection_Handle any time encryption is paused and then resumed. The Controller shall
     * send this event when the encryption key has been refreshed due to encryption being started or resumed.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setEncryptionKeyRefreshComplete()
    {
        maskMSB |= (1 << 15);
        return *this;
    }

    /**
     * @brief The HCI_IO_Capability_Request event is used to indicate that the IO capabilities of the Host are required
     * for a Secure Simple Pairing process. The Host shall respond with an HCI_IO_Capability_Request_Reply command or
     * HCI_IO_Capability_Request_Negative_Reply command. This event shall only be generated if Secure Simple Pairing has
     * been enabled with the HCI_Write_Simple_Pairing_Mode command.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setIOCapabilityRequest()
    {
        maskMSB |= (1 << 16);
        return *this;
    }

    /**
     * @brief The HCI_IO_Capability_Response event is used to indicate to the Host that IO capabilities from a remote
     * device specified by BD_ADDR have been received during a Secure Simple Pairing process. This event will only be
     * generated if Secure Simple Pairing has been enabled with the HCI_Write_Simple_Pairing_- Mode command.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setIOCapabilityResponse()
    {
        maskMSB |= (1 << 17);
        return *this;
    }

    /**
     * @brief The HCI_User_Confirmation_Request event is used to indicate that user confirmation of a numeric value is
     * required. The Host shall reply with either the HCI_User_Confirmation_Request_Reply or the HCI_User_Confirmation_-
     * Request_Negative_Reply command. If the Host has output capability (DisplayYesNo or KeyboardOnly), it shall
     * display the Numeric_Value until the HCI_Simple_Pairing_Complete event is received. It shall reply based on the
     * yes/no response from the user. If the Host has no input and no output it shall reply with the
     * HCI_User_Confirmation_Request_Reply command. When the Controller generates an HCI_User_Confirmation_Request
     * event, in order for the local Link Manager to respond to the request from the remote Link Manager, the local Host
     * shall respond with either an HCI_User_Confirmation_- Request_Reply or an
     * HCI_User_Confirmation_Request_Negative_Reply command before the remote Link Manager detects LMP response timeout.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setUserConfirmationRequest()
    {
        maskMSB |= (1 << 18);
        return *this;
    }

    /**
     * @brief The HCI_User_Passkey_Request event is used to indicate that a passkey is required as part of a Secure
     * Simple Pairing process. The Host shall respond with either an HCI_User_Passkey_Request_Reply or
     * HCI_User_Passkey_- Request_Negative_Reply command. This event will only be generated if Secure Simple Pairing has
     * been enabled with the HCI_Write_Simple_Pairing_- Mode command. When the Controller generates an
     * HCI_User_Passkey_- Request event, in order for the local Link Manager to respond to the request from the remote
     * Link Manager, the local Host shall respond with either an HCI_User_Passkey_Request_Reply or
     * HCI_User_Passkey_Request_- Negative_Reply command before the remote Link Manager detects LMP response timeout.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setUserPasskeyRequest()
    {
        maskMSB |= (1 << 19);
        return *this;
    }

    /**
     * @brief The HCI_Remote_OOB_Data_Request event is used to indicate that the Secure Simple Pairing Hash C and
     * Randomizer R are required for the Secure Simple Pairing process involving the device identified by BD_ADDR. The C
     * and R values were transferred to the Host from the remote device via an OOB mechanism. This event is sent by the
     * Controller because the Host previously set the OOB Data Present parameter to "OOB authentication data from remote
     * device present" in an HCI_IO_Capability_Request_Reply command. If both the Host and Controller support Secure
     * Connections the Host shall respond with the values using the HCI_Remote_OOB_Extended_Data_Request_Reply command.
     * Otherwise, the Host shall respond with the values using the HCI_Remote_OOB_Data_Request_Reply command. In either
     * case, if the Host does not have the C and R values for the device, it shall respond with the
     * HCI_Remote_OOB_Data_Request_Negative_Reply command.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setRemoteOOBDataRequest()
    {
        maskMSB |= (1 << 20);
        return *this;
    }

    /**
     * @brief The HCI_Simple_Pairing_Complete event is used to indicate that the Secure Simple Pairing process has
     * completed. A Host that is displaying a numeric value can use this event to change its UI.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setSimplePairingComplete()
    {
        maskMSB |= (1 << 21);
        return *this;
    }

    /**
     * @brief The HCI_Link_Supervision_Timeout_Changed event is used to notify the Peripheral's Host when the
     * Link_Supervision_Timeout parameter is changed in the Peripheral’s Controller. This event shall only be sent to
     * the Host by the Peripheral’s Controller upon receiving an LMP_SUPERVISION_TIMEOUT PDU from the Central.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setLinkSupervisionTimeoutChanged()
    {
        maskMSB |= (1 << 23);
        return *this;
    }

    /**
     * @brief The HCI_Enhanced_Flush_Complete event is used to indicate that an Enhanced Flush is complete for the
     * specified Handle.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setEnhancedFlushComplete()
    {
        maskMSB |= (1 << 24);
        return *this;
    }

    /**
     * @brief The HCI_User_Passkey_Notification event is used to provide a passkey for the Host to display to the user
     * as required as part of a Secure Simple Pairing process. The Passkey parameter shall be a randomly generated
     * number (see [Vol 2] Part H, Section 2) generated by the Controller mod 1,000,000
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setUserPasskeyNotification()
    {
        maskMSB |= (1 << 26);
        return *this;
    }

    /**
     * @brief The HCI_Keypress_Notification event is sent to the Host after a passkey notification has been received by
     * the Link Manager on the given BD_ADDR. The Notification_Type parameter may be used by the Host's user interface.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setKeypressNotification()
    {
        maskMSB |= (1 << 27);
        return *this;
    }

    /**
     * @brief The HCI_Remote_Host_Supported_Features_Notification event is used to return the LMP extended features page
     * containing the Host features. The BD_ADDR shall be the address of the remote device.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setRemoteHostSupportedFeaturesNotification()
    {
        maskMSB |= (1 << 28);
        return *this;
    }

    /**
     * @brief The LE Meta event is used to encapsulate all LE Controller specific events. The Event Code of all LE Meta
     * events shall be 0x3E. The Subevent_Code is the first octet of the event parameters. The Subevent_Code shall be
     * set to one of the valid Subevent_Codes from an LE specific event. All other Subevent_Parameters are defined in
     * the LE Controller specific events.
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setLEMeta()
    {
        maskMSB |= (1 << 29);
        return *this;
    }

    /**
     * @brief Set tdefault event recommended by the BLE Core specification (first 44 bits)
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setDefault()
    {
        maskMSB |= 0x00001FFF;
        maskLSB |= 0xFFFFFFFF;
        return *this;
    }

    /**
     * @brief Set all mask bits
     *
     * @return EventMaskBuilder&
     */
    EventMaskBuilder& setAll()
    {
        maskMSB = 0x3FFFFFFF;
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