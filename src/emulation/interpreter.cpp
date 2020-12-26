#include "interpreter.hpp"

#include <stdexcept>
#include <mips/utils.hpp>
#include <utils/functional.hpp>

namespace emulation
{
    using namespace mips;

    Interpreter::Interpreter()
        : _pc(instruction_mem_addr)
        , _executed_instructions(0)
    { }

    void Interpreter::load_source(std::vector<mips::Instruction>&& code, const uint32_t addr)
    {
        _source = std::move(code);
    }

    bool Interpreter::valid_pc(const uint32_t addr) const noexcept
    {
        if (addr < instruction_mem_addr)
            return false;
        if (addr >= instruction_mem_addr + _source.size() * 4)
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
        _pc = addr;
        while (valid_pc(_pc))
        {
            execute(_source[_pc / 4]);

            _executed_instructions++;
            _pc += 4;
        }
    }

    std::string Interpreter::get_debug() const
    {
        return _debug_stream.str();
    }

    std::string Interpreter::get_debug_with_dumps() const
    {
        std::stringstream ss;
        ss << get_debug() << "\n" << _regs.generate_dump() << "\n";
        return ss.str();
    }

    void Interpreter::execute(const mips::Instruction instr)
    {
        std::visit(functional::overload{
        [&](const auto& x) { execute(x); }
        }, instr);
    }

    void Interpreter::execute(const mips::InstructionR instr)
    {
        switch (instr.op)
        {
            case OpcodeR::ADD: execute_add(instr); break;
            default: throw_invalid_instr(instr);
        }
    }

    void Interpreter::execute(const mips::InstructionI instr)
    {
        switch (instr.op)
        {
            default: throw_invalid_instr(instr);
        }
    }

    void Interpreter::execute(const mips::InstructionJ instr)
    {
        switch (instr.op)
        {
            default: throw_invalid_instr(instr);
        }
    }

    void Interpreter::throw_invalid_instr(const mips::Instruction instr)
    {
        std::visit(functional::overload{
        [&](const auto& x) { throw std::logic_error(std::string("Instruction ") + mips::opcode_to_string(x.op) + " is not supported"); }
        }, instr);
    }

    void Interpreter::execute_add(const mips::InstructionR instr)
    {
        _regs[instr.rd] = _regs[instr.rs] + _regs[instr.rt];
    }
}
