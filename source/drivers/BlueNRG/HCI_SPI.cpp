#include "HCI_SPI.h"

#include "ble_utils.h"

using namespace codal;

constexpr uint32_t EXPIRATION_PACKETS_MS = 10000;
constexpr uint32_t TIMEOUT_COMMAND_MS    = 5000;

/*  BLUENRG SPI OPERATION */
constexpr uint8_t BLUENRG_WRITE_OP = 0x0A;
constexpr uint8_t BLUENRG_READ_OP  = 0x0B;

/*  TYPE OF PACKET  */
constexpr uint8_t HCI_COMMAND_PKT   = 0x01;
constexpr uint8_t HCI_ASYNCDATA_PKT = 0x02;
constexpr uint8_t HCI_EVENT_PKT     = 0x04;

HCI_SPI::HCI_SPI(STM32SPI& spi, STM32Pin& cs, STM32Pin& irq, STM32Pin& rst, uint32_t freq)
    : HCI(), spi(spi), cs(cs), irq(irq), rst(rst)
{
    spi.setFrequency(freq);
    spi.setMode(0);
}

void HCI_SPI::init()
{
    disableCS();

    irq.setDigitalValue(0);
    irq.setPull(PullMode::Down);
    target_wait(500);
    irq.getDigitalValue();

    resetHardware();
    waitForInit();
}

void HCI_SPI::resetHardware()
{
    rst.setDigitalValue(0);
    target_wait(5);
    rst.setDigitalValue(1);
    target_wait(5);
}

void HCI_SPI::poll()
{
    if (irq.getDigitalValue() == 0) {
        if (isDebug) printf("[poll HCI_SPI] IRQ low, no data available...\r\n");
        return;
    }

    HCI::poll();
}

void HCI_SPI::waitForInit()
{
    bool wait = true;

    if (isDebug) printf("[waitForInit] Start initialization...\r\n");

    while (irq.getDigitalValue() == 0)
        ;

    while (wait) {
        uint8_t bytesToRead = available();

        if (bytesToRead > 0) {
            if (bytesToRead == 6) {
                uint8_t buffer[6] = {0x00};
                readBytes(buffer, 6);

                if (buffer[0] == 0x04 && buffer[1] == 0xFF && buffer[2] == 0x03 && buffer[3] == 0x01 &&
                    buffer[4] == 0x00 && buffer[5] == 0x01) {
                    wait = false;
                }
            }
            else {
                readBytes(NULL, bytesToRead);
            }
        }
    }

    if (isDebug) printf("[waitForInit] Initialization done...\r\n");
}

std::vector<uint8_t> HCI_SPI::sendCommand(OpCodeCommand command, uint8_t nbArgs, const uint8_t* args)
{
    uint8_t opcodeHigh           = BLE_Utils::getMsb((uint16_t)command);
    uint8_t opcodeLow            = BLE_Utils::getLsb((uint16_t)command);
    uint8_t payloadSize          = 4 + nbArgs;
    uint8_t payload[payloadSize] = {HCI_COMMAND_PKT, opcodeLow, opcodeHigh, nbArgs};
    auto result                  = std::vector<uint8_t>();

    memcpy(&payload[4], args, nbArgs);

    if (isDebug) {
        printf("[sendCommand] Send command 0x%0.4X\r\n", (uint16_t)command);
    }

    // Wait device ready to send data
    for (uint32_t start = getCurrentMillis(); !writeBytes(payload, payloadSize);) {
        if (getCurrentMillis() - start > TIMEOUT_COMMAND_MS) {
            if (isDebug) {
                printf("[sendCommand] Command (0x%0.4X) timeout (write timeout)...\r\n", (uint16_t)command);
            }
            return result;
        }
    }
    // Test command received
    uint32_t start = getCurrentMillis();

    while ((getCurrentMillis() - start) < TIMEOUT_COMMAND_MS) {
        poll();
        for (auto it = eventPackets.begin(); it != eventPackets.end(); ++it) {
            if (it->eventCode == EVT_COMMAND_COMPLETE && it->isFromCommand(command)) {
                if (it->params.size() > 3 && it->params[3] == 0x00) {
                    // Skip the 3 first bytes
                    for (uint8_t i = 3; i < it->length; i++) {
                        result.push_back(it->params[i]);
                    }

                    if (isDebug) {
                        printf("[sendCommand] Command (0x%0.4X) success\r\n", (uint16_t)command);
                    }
                }

                it = eventPackets.erase(it);
                return result;
            }
        }

        target_wait(5);
    }

    if (isDebug) {
        printf("[sendCommand] Command (0x%0.4X) timeout (read timout)...\r\n", (uint16_t)command);
    }

    return result;
}

uint8_t HCI_SPI::available()
{
    uint8_t availableData = 0;
    for (uint8_t i = 0; i < 5; ++i) {
        uint8_t header[5] = {BLUENRG_READ_OP, 0x00, 0x00, 0x00, 0x00};

        enableCS();
        spi.transfer(header, 5, header, 5);
        disableCS();

        if (header[0] == 0x02) {
            availableData = header[3];
            break;
        }
    }

    return availableData;
}

uint8_t HCI_SPI::readByte()
{
    uint8_t b = 0xff;
    enableCS();
    uint8_t header[5] = {BLUENRG_READ_OP, 0x00, 0x00, 0x00, 0x00};
    spi.transfer(header, 5, header, 5);

    if (header[0] != 0x02) {
        if (isDebug) printf("[readByte] Device is not ready !\r\n");
    }
    else if (header[3] < 1) {
        if (isDebug) printf("[readByte] Notyhing to read...\r\n");
    }
    else {
        spi.transfer(&b, 1, &b, 1);
    }

    disableCS();

    return b;
}

void HCI_SPI::readBytes(uint8_t* data, uint8_t size)
{
    uint8_t t[size]   = {0xff};
    uint8_t header[5] = {BLUENRG_READ_OP, 0x00, 0x00, 0x00, 0x00};

    enableCS();
    spi.transfer(header, 5, header, 5);

    if (isDebug) {
        printf("[readBytes (%d)] ", size);
        for (auto e : header) {
            printf("0x%02x ", e);
        }

        printf("\r\n");
    }

    if (header[0] != 0x02) {
        if (isDebug) printf("[readBytes] Device is not ready !\r\n");
        codal::fiber_sleep(5);
    }
    else if (header[3] < size) {
        if (isDebug) printf("[readBytes] Read buffer smaller than the asked size. Available: %d\r\n", header[3]);
        codal::fiber_sleep(5);
    }
    else {
        spi.transfer(t, size, data, size);
    }

    disableCS();
}

bool HCI_SPI::writeBytes(uint8_t* data, uint8_t size)
{
    bool result = false;
    enableCS();

    uint8_t header[5] = {BLUENRG_WRITE_OP, 0x00, 0x00, 0x00, 0x00};
    spi.transfer(header, 5, header, 5);

    if (header[0] != 0x02) {
        result = false;
    }
    else if (header[1] < size) {
        result = false;
    }
    else {
        spi.transfer(data, size, NULL, 0);
        result = true;
    }

    disableCS();

    return result;
}