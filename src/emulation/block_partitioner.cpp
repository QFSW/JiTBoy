#include "block_partitioner.hpp"

#include <utils/strtools.hpp>
#include <mips/utils.hpp>

namespace emulation
{
    SourceBlock BlockPartitioner::partition_block(const mips::Program& program, const uint32_t addr) const
    {
        if (!program.valid_addr(addr)) [[unlikely]]
            throw std::logic_error(strtools::catf("Address 0x%x is out of bounds", addr));

        const size_t start_index = (addr - program.start_addr) / 4;
        uint32_t end_index = start_index;
        for (; end_index < program.source.size(); end_index++)
        {
            const auto& instr = program.source[end_index];
            if (mips::utils::is_branch_instr(instr))
            {
                end_index++;
                break;
            }
        }

        const auto code = std::span<const mips::Instruction>(program.source.data() + start_index, end_index - start_index);
        return SourceBlock(code, addr);
    }
}
