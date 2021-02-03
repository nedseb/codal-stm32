#include "STM32SPI.h"

#include "CodalConfig.h"
#include "CodalDmesg.h"
#include "ErrorNo.h"

using namespace codal;

void STM32SPI::complete() {}

void STM32SPI::_complete(uint32_t instance) {}

void STM32SPI::_irq(uint32_t instance) {}

extern "C" void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi) {
    STM32SPI::_complete((uint32_t)hspi->Instance);
}

extern "C" void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hspi) {
    STM32SPI::_complete((uint32_t)hspi->Instance);
}

extern "C" void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef* hspi) {
    STM32SPI::_complete((uint32_t)hspi->Instance);
}

#define DEFIRQ(nm, id) \
    extern "C" void nm() { STM32SPI::_irq(id); }

DEFIRQ(SPI1_IRQHandler, SPI1_BASE)
DEFIRQ(SPI2_IRQHandler, SPI2_BASE)
#ifdef SPI3_BASE
DEFIRQ(SPI3_IRQHandler, SPI3_BASE)
#endif
#ifdef SPI4_BASE
DEFIRQ(SPI4_IRQHandler, SPI4_BASE)
#endif
#ifdef SPI5_BASE
DEFIRQ(SPI5_IRQHandler, SPI5_BASE)
#endif
#ifdef SPI6_BASE
DEFIRQ(SPI6_IRQHandler, SPI6_BASE)
#endif

void STM32SPI::init_internal() {}

STM32SPI::STM32SPI(codal::Pin& mosi, codal::Pin& miso, codal::Pin& sclk, codal::Pin* cs) : codal::SPI() {}

int STM32SPI::setFrequency(uint32_t frequency) {
    return DEVICE_SPI_ERROR;
}

int STM32SPI::setMode(int mode, int bits) {
    return DEVICE_SPI_ERROR;
}

int STM32SPI::write(int data) {
    return DEVICE_SPI_ERROR;
}

int STM32SPI::transfer(const uint8_t* txBuffer, uint32_t txSize, uint8_t* rxBuffer, uint32_t rxSize) {
    return DEVICE_SPI_ERROR;
}

int STM32SPI::startTransfer(const uint8_t* txBuffer,
                            uint32_t txSize,
                            uint8_t* rxBuffer,
                            uint32_t rxSize,
                            PVoidCallback doneHandler,
                            void* arg) {
    return DEVICE_SPI_ERROR;
}
