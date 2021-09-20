#ifndef __LIS3MDL_MAGNETOMETER_H__
#define __LIS3MDL_MAGNETOMETER_H__

#include "LIS3MDL.h"

namespace codal {

class LIS3MDL_Magnetometer : public LIS3MDL, public Compass {
  public:
    LIS3MDL_Magnetometer(STM32I2C* i2c, uint8_t address, CoordinateSpace& coordinateSpace,
                         uint16_t id = DEVICE_ID_COMPASS);

    /**
     * Configures the device for the sample rate defined
     * in this object. The nearest values are chosen to those defined
     * that are supported by the hardware. The instance variables are then
     * updated to reflect reality.
     *
     * @return DEVICE_OK on success, DEVICE_I2C_ERROR if the compass could not be configured.
     */
    virtual int configure() override final;

    /**
     * Poll to see if new data is available from the hardware. If so, update it.
     * n.b. it is not necessary to explicitly call this function to update data
     * (it normally happens in the background when the scheduler is idle), but a check is performed
     * if the user explicitly requests up to date data.
     *
     * @return DEVICE_OK on success, DEVICE_I2C_ERROR if the update fails.
     *
     * @note This method should be overidden by the hardware driver to implement the requested
     * changes in hardware.
     */
    virtual int requestUpdate() override final;

    /**
     * A periodic callback invoked by the fiber scheduler idle thread.
     *
     * Internally calls updateSample().
     */
    virtual void idleCallback() override final { requestUpdate(); }

    /**
     * Puts the component in (or out of) sleep (low power) mode.
     */
    virtual int setSleep(bool doSleep) override final
    {
        doSleep ? disable() : enable();
        return DEVICE_OK;
    }
};

}  // namespace codal

#endif