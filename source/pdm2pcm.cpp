#include "pdm2pcm.h"

#include <algorithm>

using namespace std;

PDM2PCM::PDM2PCM(uint16_t lowpass_order, uint16_t decimation_value, uint16_t hipass_value, uint16_t gain)
    : lowpass_order(lowpass_order), decimation(decimation_value), hipass_value(hipass_value), gain(gain)
{
}

vector<int16_t> PDM2PCM::convert(uint16_t* data, uint16_t size)
{
    auto result = lowpassAndDecimation(data, size);
    hipass(&result);
    applyGgain(&result);

    return result;
}

vector<int16_t> PDM2PCM::lowpassAndDecimation(uint16_t* data, uint16_t size)
{
    vector<int16_t> result;
    float ratio = 1.0 / (float(lowpass_order) * 16.0);

    for (uint16_t word = 0; (word + lowpass_order) < size; word += decimation) {
        double sum = 0;

        for (uint16_t order = 0; order < lowpass_order; ++order) {
            for (uint8_t b = 0; b < 16; ++b) {
                if ((data[word + order] & (1 << b)) > 0) {
                    sum += ratio;
                }
            }
        }

        result.push_back(int16_t(sum * 65'535) - 32'767);
    }

    return result;
}

void PDM2PCM::hipass(std::vector<int16_t>* vec)
{
    for_each(vec->begin(), vec->end(), [&](int16_t& v) { v -= this->hipass_value; });
}
void PDM2PCM::applyGgain(std::vector<int16_t>* vec)
{
    for_each(vec->begin(), vec->end(), [&](int16_t& v) { v *= this->gain; });
}