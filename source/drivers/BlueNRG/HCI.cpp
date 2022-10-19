#include "HCI.h"

#include <cstring>
#include <queue>

#include "CodalFiber.h"
#include "ble_utils.h"
#include "clock.h"

constexpr uint16_t MAX_BUFFER_POLL_SIZE    = 64;
constexpr uint32_t EXPIRATION_PACKETS_MS   = 10000;
constexpr uint32_t TIMEOUT_COMMAND_MS      = 1000;
constexpr size_t MAX_ADV_REPORT_DEQUE_SIZE = 32;
constexpr size_t MAX_EVENT_LIST_SIZE       = 32;

/*  BLUENRG SPI OPERATION */
constexpr uint8_t BLUENRG_WRITE_OP = 0x0A;
constexpr uint8_t BLUENRG_READ_OP  = 0x0B;

/*  TYPE OF PACKET  */
constexpr uint8_t HCI_COMMAND_PKT   = 0x01;
constexpr uint8_t HCI_ASYNCDATA_PKT = 0x02;
constexpr uint8_t HCI_EVENT_PKT     = 0x04;

template <class T>
constexpr const T& constrain(const T& value, const T& min, const T& max)
{
    return std::max(std::min(value, max), min);
}

void HCI::poll()
{
    if (isPollLocked) {
        return;
    }

    isPollLocked = true;
    std::queue<uint8_t> buffer;

    for (uint8_t nbBytes = available(); nbBytes > 0 && buffer.size() < MAX_BUFFER_POLL_SIZE; nbBytes = available()) {
        if (isDebug) printf("[poll] Available bytes : %d\r\n", nbBytes);

        uint8_t tmpBuf[nbBytes] = {0x00};
        readBytes(tmpBuf, nbBytes);

        for (uint8_t i = 0; i < nbBytes; ++i) buffer.push(tmpBuf[i]);
    }

    while (!buffer.empty()) {
        uint8_t typeByte = buffer.front();
        buffer.pop();

        switch (typeByte) {
            case HCI_EVENT_PKT: {
                EventPacket pkt;
                pkt.eventCode = buffer.front();
                buffer.pop();
                pkt.length = buffer.front();
                buffer.pop();

                pkt.params.clear();
                for (uint8_t i = 0; i < pkt.length; ++i) {
                    pkt.params.push_back(buffer.front());
                    buffer.pop();
                }

                pkt.time = getCurrentMillis();

                if (pkt.eventCode == LE_EVENT_CODE) {
                    handleLeEvent(pkt);
                }
                else {
                    if (eventPackets.size() >= MAX_EVENT_LIST_SIZE) {
                        eventPackets.pop_front();
                    }

                    eventPackets.push_front(pkt);
                }

                if (isDebug) {
                    printf("[poll] received event : [0x%0.2x] - %d bytes\r\n", pkt.eventCode, pkt.params.size());
                    printf("\tParams: ");
                    for (auto v : pkt.params) printf("0x%0.2x ", v);
                    printf("\r\n");
                }
            } break;

            case HCI_ASYNCDATA_PKT: {  // HANDLE (12 bits) + PB Flag (2bits) + BC Flag (2 bits) = 16 bits (2 bytes)
                buffer.pop();
                buffer.pop();
                uint8_t lengthLSB = buffer.front();
                buffer.pop();
                uint8_t lengthMSB = buffer.front();
                buffer.pop();
                uint16_t length = (((uint16_t)lengthMSB) << 8) | lengthLSB;

                if (isDebug)
                    printf("[poll] received data : not implemented (%d bytes length / %d bytes remaining)\r\n", length,
                           buffer.size());

                for (uint16_t i = 0; i < length && !buffer.empty(); ++i) buffer.pop();
            } break;

            default:
                if (isDebug) printf("[poll] unknown message type : 0x%0.2X\r\n", typeByte);
                break;
        }
    }

    if (isDebug) printf("[poll] No data availables....\r\n\r\n");

    cleanPackets();
    isPollLocked = false;
}

bool HCI::resetSoftware()
{
    auto result = sendCommand(OpCodeCommand::RESET);
    return result.size() > 0 && result.front() > 0;
}

std::array<uint32_t, 2> HCI::readBD_ADDR()
{
    auto result                    = sendCommand(OpCodeCommand::READ_BD_ADDR);
    std::array<uint32_t, 2> values = {0x00, 0x00};

    if (result.empty() || result.size() != 7 || result[0] != 0x00) {
        return values;
    }

    for (uint8_t i = 0; i < 2; ++i) {
        values[0] |= (uint32_t)result[5 + i] << (8 * i);
    }

    for (uint8_t i = 0; i < 4; ++i) {
        values[1] |= (uint32_t)result[1 + i] << (8 * i);
    }

    return values;
}

bool HCI::setEventMask(uint32_t msbMask, uint32_t lsbMask)
{
    uint8_t data[8] = {
        BLE_Utils::getByte(lsbMask, 0), BLE_Utils::getByte(lsbMask, 1),
        BLE_Utils::getByte(lsbMask, 2), BLE_Utils::getByte(lsbMask, 3),

        BLE_Utils::getByte(msbMask, 0), BLE_Utils::getByte(msbMask, 1),
        BLE_Utils::getByte(msbMask, 2), BLE_Utils::getByte(msbMask, 3),
    };

    auto result = sendCommand(OpCodeCommand::SET_EVENT_MASK, 8, data);

    return !result.empty() && result.size() == 1 && result[0] == 0x00;
}

bool HCI::leSetEventMask(uint32_t msbMask, uint32_t lsbMask)
{
    uint8_t data[8] = {
        BLE_Utils::getByte(lsbMask, 0), BLE_Utils::getByte(lsbMask, 1),
        BLE_Utils::getByte(lsbMask, 2), BLE_Utils::getByte(lsbMask, 3),

        BLE_Utils::getByte(msbMask, 0), BLE_Utils::getByte(msbMask, 1),
        BLE_Utils::getByte(msbMask, 2), BLE_Utils::getByte(msbMask, 3),
    };

    auto result = sendCommand(OpCodeCommand::LE_SET_EVENT_MASK, 8, data);

    return !result.empty() && result.size() == 1 && result[0] == 0x00;
}

LeReadBufferSizeResult HCI::leReadBufferSize()
{
    auto result = sendCommand(OpCodeCommand::LE_READ_BUFFER_SIZE);
    LeReadBufferSizeResult values;

    if (result.empty() || result.size() != 4 || result[0] != 0x00) {
        values.success = false;
        return values;
    }

    values.success = true;

    values.leAclDataPacketLength   = ((uint16_t)result[2] << 8) | result[1];
    values.totalNumLEAclDataPacket = result[3];

    return values;
}

bool HCI::leSetAdvertisingParameters(float advertisingIntervalMin, float advertisingIntervalMax,
                                     AdvertisingType advertisingType, OwnAddressType ownAddressType,
                                     AdvertisingPeerAddressType peerAddressType, uint64_t peerAddress,
                                     uint8_t advertisingChannelMap, AdvertisingFilterPolicy advertisingFilterPolicy)
{
    uint16_t advIntervalMin = constrain(advertisingIntervalMin, 20.0f, 10240.0f) / 0.625f;
    uint16_t advIntervalMax = constrain(advertisingIntervalMax, 20.0f, 10240.0f) / 0.625f;

    if (advIntervalMin > advIntervalMax) {
        advIntervalMax = advIntervalMin;
    }

    uint8_t data[15] = {
        BLE_Utils::getLsb(advIntervalMin),
        BLE_Utils::getMsb(advIntervalMin),

        BLE_Utils::getLsb(advIntervalMax),
        BLE_Utils::getMsb(advIntervalMax),

        (uint8_t)advertisingType,
        (uint8_t)ownAddressType,
        (uint8_t)peerAddressType,

        BLE_Utils::getByte(peerAddress, 0),
        BLE_Utils::getByte(peerAddress, 1),
        BLE_Utils::getByte(peerAddress, 2),
        BLE_Utils::getByte(peerAddress, 3),
        BLE_Utils::getByte(peerAddress, 4),
        BLE_Utils::getByte(peerAddress, 5),

        advertisingChannelMap,

        (uint8_t)advertisingFilterPolicy,
    };
    auto result = sendCommand(OpCodeCommand::LE_SET_ADV_PARAM, 15, data);

    return !result.empty() && result.size() == 1 && result[0] == 0x00;
}

int8_t HCI::leReadAdvertisingPhysicalChannelTxPower()
{
    auto result = sendCommand(OpCodeCommand::LE_READ_TX_POWER);

    if (result.empty() || result.size() != 2 || result[0] != 0x00) {
        return 127;
    }

    return (int8_t)result[1];
}

bool HCI::leSetAdvertisingData(uint8_t significantSize, uint8_t* data)
{
    uint8_t tmp[32] = {significantSize, 0x00};
    memcpy(tmp + 1, data, significantSize);

    auto result = sendCommand(OpCodeCommand::LE_SET_ADV_DATA, 32, tmp);

    return !result.empty() && result.size() == 1 && result[0] == 0x00;
}

bool HCI::leSetScanResponseData(uint8_t significantSize, uint8_t* data)
{
    uint8_t tmp[32] = {significantSize, 0x00};
    memcpy(tmp + 1, data, significantSize);

    auto result = sendCommand(OpCodeCommand::LE_SET_SCAN_RESP_DATA, 32, tmp);

    return !result.empty() && result.size() == 1 && result[0] == 0x00;
}

bool HCI::leSetAdvertisingEnable(bool enable)
{
    uint8_t data[1] = {enable ? (uint8_t)0x01 : (uint8_t)0x00};
    auto result     = sendCommand(OpCodeCommand::LE_SET_ADV_ENABLE, 1, data);
    return !result.empty() && result.front() == 0x00;
}

bool HCI::leSetScanParameters(ScanType scanType, float scanInterval, float scanWindow, OwnAddressType addressType,
                              ScanningFilterPolicy filterPolicy)
{
    uint16_t interval = constrain(scanInterval, 2.5f, 10240.0f) / 0.625f;
    uint16_t window   = constrain(scanWindow, 2.5f, 10240.0f) / 0.625f;

    if (window > interval) {
        window = interval;
    }

    uint8_t data[7] = {
        (uint8_t)scanType,

        BLE_Utils::getLsb(interval), BLE_Utils::getMsb(interval),

        BLE_Utils::getLsb(window),   BLE_Utils::getMsb(window),

        (uint8_t)addressType,

        (uint8_t)filterPolicy,
    };
    auto result = sendCommand(OpCodeCommand::LE_SET_SCAN_PARAM, 7, data);

    return !result.empty() && result.size() == 1 && result[0] == 0x00;
}

bool HCI::leSetScanEnable(bool enable, bool filterDuplicate)
{
    uint8_t data[2] = {enable ? (uint8_t)0x01 : (uint8_t)0x00, filterDuplicate ? (uint8_t)0x01 : (uint8_t)0x00};
    auto result     = sendCommand(OpCodeCommand::LE_SET_SCAN_ENABLE, 2, data);
    return !result.empty() && result.front() == 0x00;
}

std::array<uint32_t, 2> HCI::leRand()
{
    auto result                    = sendCommand(OpCodeCommand::LE_RAND);
    std::array<uint32_t, 2> values = {0x00, 0x00};

    if (result.empty() || result.size() != 9 || result[0] != 0x00) {
        return values;
    }

    for (uint8_t i = 0; i < 4; ++i) {
        values[0] |= (uint32_t)result[5 + i] << (8 * i);
        values[1] |= (uint32_t)result[1 + i] << (8 * i);
    }

    return values;
}

void HCI::cleanPackets()
{
    eventPackets.remove_if([](EventPacket e) -> bool { return getCurrentMillis() - e.time >= EXPIRATION_PACKETS_MS; });

    asyncDataPackets.remove_if(
        [](AsyncDataPacket e) -> bool { return getCurrentMillis() - e.time >= EXPIRATION_PACKETS_MS; });
}

void HCI::handleLeEvent(EventPacket pkt)
{
    if (pkt.length == 0) {
        return;
    }

    switch (pkt.params[0]) {
        case LE_EVENT_ADVERTISING_REPORT: {
            if (advertisingReports.size() > MAX_ADV_REPORT_DEQUE_SIZE) {
                advertisingReports.pop_front();
            }

            advertisingReports.push_back(BLEAdvertisingReport(pkt.params));
        } break;

        default:
            break;
    }
}