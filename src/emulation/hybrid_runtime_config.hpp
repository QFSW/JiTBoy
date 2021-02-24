#pragma once

#include <ostream>

namespace emulation
{
    struct HybridRuntimeConfig
    {
        size_t compilation_threshold = 1;
        bool direct_linking = false;
    };

    std::ostream& operator<<(std::ostream& o, const HybridRuntimeConfig& config);
}
