#include "runtime_config.hpp"

namespace emulation
{
    std::ostream& operator<<(std::ostream& o, const RuntimeConfig& config)
    {
        if (config.direct_linking)
            o << "-L";

        return o;
    }
}
