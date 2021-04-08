#ifndef __STM32WEBUSB_H__
#define __STM32WEBUSB_H__

namespace codal {
    class STM32WebUSB {

        public:
            STM32WebUSB();
            ~STM32WebUSB() {}

            void init();
            void deInit();
    };
}

#endif /* __STM32WEBUSB_H__ */