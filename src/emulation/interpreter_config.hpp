#pragma once

#include <ostream>

namespace emulation
{
    struct InterpreterConfig
    {
        
    };

    std::ostream& operator<<(std::ostream& o, const InterpreterConfig& config);
}
