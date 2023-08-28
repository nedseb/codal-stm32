#pragma once

#include <cmath>
#include <cstdint>
#include <vector>

namespace PCMUtils {
float toDecibel(std::vector<int16_t>& data)
{
    uint64_t sum           = 0;
    uint32_t avgSquaredRMS = 0;

    for (auto val : data) {
        sum += std::pow(val, 2);
    }

    avgSquaredRMS = sum / uint64_t(data.size());

    return 10.0 * log10(double(avgSquaredRMS));
}
};  // namespace PCMUtils