#include "block_partitioner.hpp"

#include <utils/strtools.hpp>
#include <mips/utils.hpp>

namespace emulation
{
    SourceBlock BlockPartitioner::partition_block(const mips::Program& program, const uint32_t addr) const
    {
        if (!program.valid_addr(addr)) [[unlikely]]
            throw std::logic_error(strtools::catf("Address 0x%x is out of bounds", addr));

        uint32_t start_addr = addr;
        uint32_t end_addr = start_addr;

        while (program.valid_addr(end_addr))
        {
            if (mips::utils::is_branch_instr(program.at(end_addr)))
            {
                end_addr += 4;
                break;
            }

            end_addr += 4;
        }

        // Include delay slot instruction
        if (program.valid_addr(end_addr))
            end_addr += 4;

        return SourceBlock(program, start_addr, end_addr);
    }
}
