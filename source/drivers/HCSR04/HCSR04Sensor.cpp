#include "HCSR04Sensor.h"

using namespace codal;

/**
 * Constructor.
 *
 * Creates a generic HCSR04Sensor.
 *
 * @param id The ID of this component e.g. DEVICE_ID_THERMOMETER
 */
HCSR04Sensor::HCSR04Sensor(STM32Pin& trig, STM32Pin& echo, uint16_t id)
    : HCSR04(trig, echo), Sensor(id, 1'023), onNearDistance(nullptr), onFarDistance(nullptr)
{
    updateSample();
    EventModel::defaultEventBus->listen(id, SENSOR_THRESHOLD_HIGH, this, &HCSR04Sensor::onEvent);
    EventModel::defaultEventBus->listen(id, SENSOR_THRESHOLD_LOW, this, &HCSR04Sensor::onEvent);
}

/**
 * Read the value from pin.
 */
int HCSR04Sensor::readValue()
{
    return getDistance(codal::HCSR04Unit::Mm);
}

void HCSR04Sensor::registerDistanceEvent(DistanceBehold fromDistanceIs, uint16_t distance, codal::HCSR04Unit type,
                                         HCSR04UserEvent handler)
{
    uint16_t reelDistance;
    switch (type) {
        case codal::HCSR04Unit::M:
            reelDistance = distance * 1'000;
            break;
        case codal::HCSR04Unit::Dm:
            reelDistance = distance * 100;
            break;
        case codal::HCSR04Unit::Cm:
            reelDistance = distance * 10;
            break;
        default:
            reelDistance = distance;
            break;
    }

    switch (fromDistanceIs) {
        case DistanceBehold::Near:
            setLowThreshold(reelDistance);
            onNearDistance = handler;
            break;
        case DistanceBehold::Far:
            setHighThreshold(reelDistance);
            onFarDistance = handler;
            break;
        default:
            break;
    }
}

void HCSR04Sensor::onEvent(Event event)
{
    switch (event.value) {
        case SENSOR_THRESHOLD_LOW:
            onNearDistance();
            break;
        case SENSOR_THRESHOLD_HIGH:
            onFarDistance();
            break;
        default:
            break;
    }
}