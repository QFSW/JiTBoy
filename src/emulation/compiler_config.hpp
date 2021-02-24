#pragma once

#include <ostream>

namespace emulation
{
    struct CompilerConfig
    {
        int o_level = 0;
        bool direct_linking = false;
    };

    std::ostream& operator<<(std::ostream& o, const CompilerConfig& config);
}