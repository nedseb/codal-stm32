#pragma once

#include <cstdint>
#include <vector>

class PDM2PCM {
  public:
    /**
     * @brief Construct a new PDM2PCM object
     *
     * @param lowpass_order order used for FIR algorithme. This value will be multyplied by 16. So it represents the
     * 16-bits word order.
     * @param decimation_value decimation value for dropping sample data. The value reprents 16-bits words (e.g
     * decimation_value = 2, means dropping 32 bits).
     * @param hipass_value DC offset value.
     * @param gain  gain apply to final values.
     */
    PDM2PCM(uint16_t lowpass_order, uint16_t decimation_value, uint16_t hipass_value, uint16_t gain);

    std::vector<int16_t> convert(uint16_t* data, uint16_t size);

  private:
    uint16_t lowpass_order;
    uint16_t decimation;
    uint16_t hipass_value;
    uint16_t gain;

    std::vector<int16_t> lowpassAndDecimation(uint16_t* data, uint16_t size);
    void hipass(std::vector<int16_t>* vec);
    void applyGgain(std::vector<int16_t>* vec);
};
