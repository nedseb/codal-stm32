#include "BLEDevice_Component.h"

#include "CodalFiber.h"
#include "Timer.h"

using namespace codal;

constexpr int BLE_DEVICE_UPDATE = 0x01;
constexpr int BLE_DEVICE_SWAP   = 0x02;

constexpr uint32_t BLE_DEVICE_UPDATE_PERIOD_MS = 100;

BLEDevice_Component::BLEDevice_Component(uint16_t id, HCI* hci, uint16_t swapPeriod)
    : BLEDevice(hci), CodalComponent(id, 0), isTimerEventConfigured(false), swapPeriod(swapPeriod)
{
}

int BLEDevice_Component::init()
{
    if (!BLEDevice::init()) {
        return DEVICE_INVALID_STATE;
    }

    if (EventModel::defaultEventBus && !isTimerEventConfigured) {
        EventModel::defaultEventBus->listen(this->id, BLE_DEVICE_UPDATE, this, &BLEDevice_Component::onUpdateRequest,
                                            MESSAGE_BUS_LISTENER_IMMEDIATE);
        EventModel::defaultEventBus->listen(this->id, BLE_DEVICE_SWAP, this, &BLEDevice_Component::onSwapRequest,
                                            MESSAGE_BUS_LISTENER_IMMEDIATE);

        system_timer_event_every(BLE_DEVICE_UPDATE_PERIOD_MS, this->id, BLE_DEVICE_UPDATE);
        system_timer_event_every(swapPeriod, this->id, BLE_DEVICE_SWAP);

        isTimerEventConfigured = true;
    }

    return DEVICE_OK;
}

void BLEDevice_Component::onUpdateRequest(Event)
{
    BLEDevice::poll();
}

void BLEDevice_Component::onSwapRequest(Event)
{
    BLEDevice::swapMode();
}