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

#pragma once

#include "HCSR04.h"

#include "CodalConfig.h"
#include "Pin.h"
#include "Event.h"
#include "Sensor.h"
#include "EventModel.h"

namespace codal
{
    typedef void (*HCSR04UserEvent)();

    enum class DistanceBehold : uint8_t {Near = 0, Far = 1};

    /**
     * Class definition for a generic analog sensor, and performs periodic sampling, buffering and low pass filtering of the data.
     */
    class HCSR04Sensor : public HCSR04, public Sensor
    {
        private:
        HCSR04UserEvent onNearDistance;
        HCSR04UserEvent onFarDistance;


        public:

        /**
          * Constructor.
          *
          * Creates a generic HCSR04Sensor.
          *
          * @param id The ID of this compoenent e.g. DEVICE_ID_THERMOMETER
         */
        HCSR04Sensor(STM32Pin& trig, STM32Pin& echo, uint16_t id);

        void registerDistanceEvent(DistanceBehold oui, uint16_t distance, codal::HCSR04Unit type, HCSR04UserEvent handler);

        void onEvent(Event event);
        

        /**
         * Read the value from pin.
         */
        virtual int readValue() override final;

        /**
          * Destructor.
          */
        // ~HCSR04Sensor();
    };
}
