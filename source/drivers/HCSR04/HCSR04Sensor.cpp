/*
The MIT License (MIT)

Copyright (c) 2017 Lancaster University.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "HCSR04Sensor.h"

/**
 * Class definition for a generic analog sensor, that takes the general form of a logarithmic response to a sensed value, in a potential divider.
 * Implements a base class for such a sensor, using the Steinhart-Hart equation to delineate a result.
 */

using namespace codal;


/**
 * Constructor.
 *
 * Creates a generic HCSR04Sensor.
 *
 * @param id The ID of this component e.g. DEVICE_ID_THERMOMETER
 */
HCSR04Sensor::HCSR04Sensor(STM32Pin& trig, STM32Pin& echo, uint16_t id) : HCSR04(trig, echo), Sensor(id, 1023), onNearDistance(nullptr), onFarDistance(nullptr)
{
    updateSample();
    EventModel::defaultEventBus->listen(id, SENSOR_THRESHOLD_HIGH, this, &HCSR04Sensor::onEvent);
    EventModel::defaultEventBus->listen(id, SENSOR_THRESHOLD_LOW, this, &HCSR04Sensor::onEvent);
}

/**
 * Read the value from pin.
 */
int HCSR04Sensor::readValue() {
    return getDistance(codal::HCSR04Unit::Mm);
}

void HCSR04Sensor::registerDistanceEvent(DistanceBehold fromDistanceIs, uint16_t distance, codal::HCSR04Unit type, HCSR04UserEvent handler) {
    uint16_t reelDistance;
    switch (type) {
    case codal::HCSR04Unit::M :
        reelDistance = distance * 1000;
        break;
    case codal::HCSR04Unit::Dm :
        reelDistance = distance * 100;
        break;
    case codal::HCSR04Unit::Cm :
        reelDistance = distance * 10;
        break;
    default:
        reelDistance = distance;
        break;
    }

    switch (fromDistanceIs) {
        case DistanceBehold::Near :
            setLowThreshold(reelDistance);
            onNearDistance = handler;
            break;
        case DistanceBehold::Far :
            setHighThreshold(reelDistance);
            onFarDistance = handler;
            break;
        default:
            break;
    }
}

void HCSR04Sensor::onEvent(Event event) {
    switch(event.value) {
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





/**
 * Destructor.
 */
// HCSR04Sensor::~HCSR04Sensor()
// {
// }