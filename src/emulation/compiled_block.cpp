#include "compiled_block.hpp"

#include <utils/utils.hpp>

namespace emulation
{
    CompiledBlock::CompiledBlock()
        : code(nullptr)
        , size(0)
        , host_instr_count(0)
        , source_instr_count(0)
        , execution_count(0)
        , exec_mutex(nullptr)
    { }

    uint32_t CompiledBlock::operator()(bool locking) const
    {
        return utils::potentially_lock_return<uint32_t>(*exec_mutex, locking, [&]
        {
            execution_count++;
            return code();
        });
    }
}
