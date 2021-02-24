#include "compiler_config.hpp"

namespace emulation
{
    std::ostream& operator<<(std::ostream& o, const CompilerConfig& config)
    {
        o << "-O" << config.o_level;
        if (config.direct_linking)
            o << " -L";

        return o;
    }
}
