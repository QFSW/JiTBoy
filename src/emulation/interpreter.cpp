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

    void Interpreter::execute(const Instruction instr)
    {
        std::visit(functional::overload{
        [&](const auto& x) { execute(x); }
        }, instr);
    }

    void Interpreter::execute(const InstructionR instr)
    {
        switch (instr.op)
        {
            case OpcodeR::ADD:  execute_add(instr); break;
            case OpcodeR::ADDU: execute_add(instr); break;
            case OpcodeR::SUB:
            case OpcodeR::SUBU:
            case OpcodeR::AND:
            case OpcodeR::OR:
            case OpcodeR::NOR:
            case OpcodeR::XOR:
            case OpcodeR::JR:
            case OpcodeR::JALR:
            case OpcodeR::MULT:
            case OpcodeR::MULTU:
            case OpcodeR::DIV:
            case OpcodeR::DIVU:
            case OpcodeR::MFHI:
            case OpcodeR::MFLO:
            case OpcodeR::MTHI:
            case OpcodeR::MTLO:
            case OpcodeR::SLT:
            case OpcodeR::SLTU:
            case OpcodeR::SLL:
            case OpcodeR::SLLV:
            case OpcodeR::SRA:
            case OpcodeR::SRAV:
            case OpcodeR::SRL:
            case OpcodeR::SRLV:
            default: throw_invalid_instr(instr);
        }
    }

    void Interpreter::execute(const InstructionI instr)
    {
        switch (instr.op)
        {
            case OpcodeI::ADDI:
            case OpcodeI::ADDIU:
            case OpcodeI::ANDI:
            case OpcodeI::ORI:
            case OpcodeI::XORI:
            case OpcodeI::SLTI:
            case OpcodeI::SLTIU:
            case OpcodeI::LUI:
            case OpcodeI::LW:
            case OpcodeI::LB:
            case OpcodeI::LBU:
            case OpcodeI::LH:
            case OpcodeI::LHU:
            case OpcodeI::SW:
            case OpcodeI::SB:
            case OpcodeI::SH:     break;
            case OpcodeI::BEQ:    execute_beq(instr); break;
            case OpcodeI::BGTZ:
            case OpcodeI::BLEZ:
            case OpcodeI::BNE:
            case OpcodeI::LWL:
            case OpcodeI::LWR:
            case OpcodeI::BGEZ:
            case OpcodeI::BGEZAL:
            case OpcodeI::BLTZ:
            case OpcodeI::BLTZAL:
            default: throw_invalid_instr(instr);
        }
    }

    void Interpreter::execute(const mips::InstructionJ instr)
    {
        switch (instr.op)
        {
            case OpcodeJ::J:
            {
                const uint32_t target = (0xF0000000 & _pc) | (instr.target << 2);
                jump(target);
                break;
            }
            case OpcodeJ::JAL:
            default: throw_invalid_instr(instr);
        }
    }

    void Interpreter::throw_invalid_instr(const Instruction instr)
    {
        std::visit(functional::overload{
        [&](const auto& x) { throw std::logic_error(std::string("Instruction ") + mips::opcode_to_string(x.op) + " is not supported"); }
        }, instr);
    }

    void Interpreter::jump(const uint32_t target)
    {
        _pc = target - 4;
    }

    void Interpreter::branch(const InstructionI instr)
    {
        const uint32_t target = _pc + (instr.constant << 2);
        jump(target);
    }

    void Interpreter::execute_add(const InstructionR instr)
    {
        _regs[instr.rd] = _regs[instr.rs] + _regs[instr.rt];
    }

    void Interpreter::execute_beq(const InstructionI instr)
    {
        if (_regs[instr.rs] == _regs[instr.rt])
            branch(instr);
    }
}
