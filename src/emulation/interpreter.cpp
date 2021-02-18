#include "interpreter.hpp"

#include <mips/utils.hpp>

namespace emulation
{
    using namespace mips;

    Interpreter::Interpreter()
        : _core(_state)
        , _executed_instructions(0)
    {
        _state.pc = instruction_mem_addr;
    }

    void Interpreter::load_source(std::vector<mips::Instruction>&& code, const uint32_t addr)
    {
        _state.source = std::move(code);
    }

    bool Interpreter::valid_pc(const uint32_t addr) const noexcept
    {
        if (addr < instruction_mem_addr)
            return false;
        if (addr >= instruction_mem_addr + _state.source.size() * 4)
            return false;
        return true;
    }

    void Interpreter::execute(std::vector<mips::Instruction>&& code)
    {
        load_source(std::move(code));
        execute(instruction_mem_addr);
    }

    void Interpreter::execute(const uint32_t addr)
    {
        _state.pc = addr;
        while (valid_pc(_state.pc))
        {
            _core.execute_current();
            _executed_instructions++;
        }
    }

    std::string Interpreter::get_debug() const
    {
        return _debug_stream.str();
    }

    std::string Interpreter::get_debug_with_dumps() const
    {
        std::stringstream ss;
        ss << get_debug() << "\n" << _state.regs.generate_dump() << "\n";
        return ss.str();
    }
}
