#pragma once

#include <ostream>

namespace emulation
{
    struct RuntimeConfig
    {
        bool direct_linking = false;
    };

    std::ostream& operator<<(std::ostream& o, const RuntimeConfig& config);
}
