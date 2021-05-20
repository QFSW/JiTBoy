#include "hybrid_runtime_config.hpp"

namespace emulation
{
    std::ostream& operator<<(std::ostream& o, const HybridRuntimeConfig& config)
    {
        o << "-T" << config.compilation_threshold;

        if (config.direct_linking)          o << " -L";
        if (config.speculative_compilation) o << " -S";

        return o;
    }
}
