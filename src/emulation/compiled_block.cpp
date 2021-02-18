#include "compiled_block.hpp"

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

    uint32_t CompiledBlock::operator()() const
    {
        std::lock_guard lock(*exec_mutex);
        execution_count++;
        return code();
    }
}
