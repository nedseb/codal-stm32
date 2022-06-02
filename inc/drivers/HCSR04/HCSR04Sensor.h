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

    class HCSR04Sensor : public HCSR04, public Sensor
    {
        public:
            HCSR04Sensor(STM32Pin& trig, STM32Pin& echo, uint16_t id);

            void registerDistanceEvent(DistanceBehold oui, uint16_t distance, codal::HCSR04Unit type, HCSR04UserEvent handler);
          
            virtual int readValue() override final;

        private:
            HCSR04UserEvent onNearDistance;
            HCSR04UserEvent onFarDistance;

            void onEvent(Event event);
    };
}
