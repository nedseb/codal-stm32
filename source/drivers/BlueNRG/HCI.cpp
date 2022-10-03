#include "HCI.h"

#include <cstring>
#include <queue>

#include "CodalFiber.h"
#include "ble_utils.h"
#include "clock.h"

constexpr uint16_t MAX_BUFFER_POLL_SIZE  = 256;
constexpr uint32_t EXPIRATION_PACKETS_MS = 10000;
constexpr uint32_t TIMEOUT_COMMAND_MS    = 1000;

/*  BLUENRG SPI OPERATION */
constexpr uint8_t BLUENRG_WRITE_OP = 0x0A;
constexpr uint8_t BLUENRG_READ_OP  = 0x0B;

/*  TYPE OF PACKET  */
constexpr uint8_t HCI_COMMAND_PKT   = 0x01;
constexpr uint8_t HCI_ASYNCDATA_PKT = 0x02;
constexpr uint8_t HCI_EVENT_PKT     = 0x04;

void HCI::poll()
{
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
                eventPackets.push_front(pkt);

                if (isDebug) {
                    printf("[poll] received event : [0x%0.2x] - %d bytes\r\n", pkt.eventCode, pkt.params.size());
                    printf("\tParams: ");
                    for (auto v : pkt.params) printf("0x%0.2x ", v);
                    printf("\r\n");
                }
            } break;

            case HCI_ASYNCDATA_PKT:
                if (isDebug) printf("[poll] received data : not implemented\r\n");
                break;

            default:
                if (isDebug) printf("[poll] unknown message type : 0x%0.2X\r\n", typeByte);
                break;
        }
    }

    if (isDebug) printf("[poll] No data availables....\r\n\r\n");

    cleanPackets();
}

bool HCI::resetSoftware()
{
    auto result = sendCommand(OpCodeCommand::RESET);
    return result.size() > 0 && result.front() > 0;
}

bool HCI::setEventMask(uint64_t mask)
{
    return false;
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

bool HCI::leSetEventMask(uint64_t mask)
{
    return false;
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

bool HCI::leSetAdvertisingParameters(uint16_t advertisingIntervalMin, uint16_t advertisingIntervalMax,
                                     AdvertisingType advertisingType, OwnAddressType ownAddressType,
                                     PeerAddressType peerAddressType, uint64_t peerAddress,
                                     uint8_t advertisingChannelMap, AdvertisingFilterPolicy advertisingFilterPolicy)
{
    uint8_t data[15] = {
        BLE_Utils::getLsb(advertisingIntervalMin),
        BLE_Utils::getMsb(advertisingIntervalMin),

        BLE_Utils::getLsb(advertisingIntervalMax),
        BLE_Utils::getMsb(advertisingIntervalMax),

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

void HCI::cleanPackets()
{
    eventPackets.remove_if([](EventPacket e) -> bool { return getCurrentMillis() - e.time >= EXPIRATION_PACKETS_MS; });

    asyncDataPackets.remove_if(
        [](AsyncDataPacket e) -> bool { return getCurrentMillis() - e.time >= EXPIRATION_PACKETS_MS; });
}