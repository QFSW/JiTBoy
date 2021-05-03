#pragma once

#include <emulation/source_block.hpp>
#include <mips/program.hpp>

namespace emulation
{
    class BlockPartitioner
    {
    public:
        [[nodiscard]] SourceBlock partition_block(const mips::Program& program, uint32_t addr) const;
    };
}