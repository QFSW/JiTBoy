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
            case OpcodeR::ADDU: execute_addu(instr); break;
            case OpcodeR::SUB:  execute_sub(instr); break;
            case OpcodeR::SUBU: execute_subu(instr); break;
            case OpcodeR::AND:  execute_and(instr); break;
            case OpcodeR::OR:   execute_or(instr); break;
            case OpcodeR::NOR:  execute_nor(instr); break;
            case OpcodeR::XOR:  execute_xor(instr); break;
            case OpcodeR::JR:   execute_jr(instr); break;
            case OpcodeR::JALR: execute_jalr(instr); break;
            // case OpcodeR::MULT:
            // case OpcodeR::MULTU:
            case OpcodeR::DIV:  execute_div(instr); break;
            case OpcodeR::DIVU: execute_divu(instr); break;
            case OpcodeR::MFHI: execute_mfhi(instr); break;
            case OpcodeR::MFLO: execute_mflo(instr); break;
            case OpcodeR::MTHI: execute_mthi(instr); break;
            case OpcodeR::MTLO: execute_mtlo(instr); break;
            case OpcodeR::SLT:  execute_slt(instr); break;
            case OpcodeR::SLTU: execute_sltu(instr); break;
            case OpcodeR::SLL:  execute_sll(instr); break;
            // case OpcodeR::SLLV:
            case OpcodeR::SRA:  execute_sra(instr); break;
            // case OpcodeR::SRAV:
            case OpcodeR::SRL:  execute_srl(instr); break;
            // case OpcodeR::SRLV:
            default: throw_invalid_instr(instr);
        }
    }

    void Interpreter::execute(const InstructionI instr)
    {
        switch (instr.op)
        {
            case OpcodeI::ADDI:   execute_addi(instr); break;
            case OpcodeI::ADDIU:  execute_addiu(instr); break;
            case OpcodeI::ANDI:   execute_andi(instr); break;
            case OpcodeI::ORI:    execute_ori(instr); break;
            case OpcodeI::XORI:   execute_xori(instr); break;
            case OpcodeI::SLTI:   execute_slti(instr); break;
            case OpcodeI::SLTIU:  execute_sltiu(instr); break;
            case OpcodeI::LUI:    execute_lui(instr); break;
            case OpcodeI::LW:     execute_lw(instr); break;
            case OpcodeI::LB:     execute_lb(instr); break;
            case OpcodeI::LBU:    execute_lbu(instr); break;
            case OpcodeI::LH:     execute_lh(instr); break;
            case OpcodeI::LHU:    execute_lhu(instr); break;
            case OpcodeI::SW:     execute_sw(instr); break;
            case OpcodeI::SB:     execute_sb(instr); break;
            case OpcodeI::SH:     execute_sh(instr); break;
            case OpcodeI::BEQ:    execute_beq(instr); break;
            case OpcodeI::BGTZ:   execute_bgtz(instr); break;
            case OpcodeI::BLEZ:   execute_blez(instr); break;
            case OpcodeI::BNE:    execute_bne(instr); break;
            // case OpcodeI::LWL:
            // case OpcodeI::LWR:
            case OpcodeI::BGEZ:   execute_bgez(instr); break;
            case OpcodeI::BGEZAL: execute_bgezal(instr); break;
            case OpcodeI::BLTZ:   execute_bltz(instr); break;
            case OpcodeI::BLTZAL: execute_bltzal(instr); break;
            default: throw_invalid_instr(instr);
        }
    }

    void Interpreter::execute(const mips::InstructionJ instr)
    {
        switch (instr.op)
        {
            case OpcodeJ::J:   execute_j(instr); break;
            case OpcodeJ::JAL: execute_jal(instr); break;
            default: throw_invalid_instr(instr);
        }
    }

    void Interpreter::throw_invalid_instr(const Instruction instr)
    {
        std::visit(functional::overload{
        [&](const auto& x) { throw std::logic_error(std::string("Instruction ") + mips::opcode_to_string(x.op) + " is not supported"); }
        }, instr);
    }

    void Interpreter::link(const Register reg)
    {
        _regs[reg] = _pc + 4;
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

    uint32_t Interpreter::calc_mem_target(const InstructionI instr)
    {
        return _regs[instr.rs] + instr.constant;
    }

    void Interpreter::execute_add(const InstructionR instr)
    {
        _regs[instr.rd] = _regs[instr.rs] + _regs[instr.rt];
    }

    void Interpreter::execute_addu(const InstructionR instr)
    {
        _regs[instr.rd] = _regs[instr.rs] + _regs[instr.rt];
    }

    void Interpreter::execute_sub(const InstructionR instr)
    {
        _regs[instr.rd] = _regs[instr.rs] - _regs[instr.rt];
    }

    void Interpreter::execute_subu(const InstructionR instr)
    {
        _regs[instr.rd] = _regs[instr.rs] - _regs[instr.rt];
    }

    void Interpreter::execute_and(const InstructionR instr)
    {
        _regs[instr.rd] = _regs[instr.rs] & _regs[instr.rt];
    }

    void Interpreter::execute_or(const InstructionR instr)
    {
        _regs[instr.rd] = _regs[instr.rs] | _regs[instr.rt];
    }

    void Interpreter::execute_nor(const InstructionR instr)
    {
        _regs[instr.rd] = ~(_regs[instr.rs] | _regs[instr.rt]);
    }

    void Interpreter::execute_xor(const InstructionR instr)
    {
        _regs[instr.rd] = _regs[instr.rs] ^ _regs[instr.rt];
    }

    void Interpreter::execute_jr(const InstructionR instr)
    {
        jump(_regs[instr.rs]);
    }

    void Interpreter::execute_jalr(const InstructionR instr)
    {
        link(instr.rd);
        execute_jr(instr);
    }

    void Interpreter::execute_div(const InstructionR instr)
    {
        _regs.lo() = static_cast<int32_t>(_regs[instr.rs]) / static_cast<int32_t>(_regs[instr.rt]);
        _regs.hi() = static_cast<int32_t>(_regs[instr.rs]) % static_cast<int32_t>(_regs[instr.rt]);
    }

    void Interpreter::execute_divu(const InstructionR instr)
    {
        _regs.lo() = static_cast<uint32_t>(_regs[instr.rs]) / static_cast<uint32_t>(_regs[instr.rt]);
        _regs.hi() = static_cast<uint32_t>(_regs[instr.rs]) % static_cast<uint32_t>(_regs[instr.rt]);
    }

    void Interpreter::execute_mfhi(const InstructionR instr)
    {
        _regs[instr.rd] = _regs.hi();
    }

    void Interpreter::execute_mflo(const InstructionR instr)
    {
        _regs[instr.rd] = _regs.lo();
    }

    void Interpreter::execute_mthi(const InstructionR instr)
    {
        _regs.hi() = _regs[instr.rd];
    }

    void Interpreter::execute_mtlo(const InstructionR instr)
    {
        _regs.lo() = _regs[instr.rd];
    }

    void Interpreter::execute_slt(const InstructionR instr)
    {
        _regs[instr.rd] =
            static_cast<int32_t>(_regs[instr.rs]) < static_cast<int32_t>(_regs[instr.rt])
            ? 1
            : 0;
    }

    void Interpreter::execute_sltu(const InstructionR instr)
    {
        _regs[instr.rd] =
            static_cast<uint32_t>(_regs[instr.rs]) < static_cast<uint32_t>(_regs[instr.rt])
            ? 1
            : 0;
    }

    void Interpreter::execute_sll(const InstructionR instr)
    {
        _regs[instr.rd] = _regs[instr.rt] << instr.sa;
    }

    void Interpreter::execute_sra(const InstructionR instr)
    {
        _regs[instr.rd] = static_cast<int32_t>(_regs[instr.rt]) >> instr.sa;
    }

    void Interpreter::execute_srl(const InstructionR instr)
    {
        _regs[instr.rd] = static_cast<uint32_t>(_regs[instr.rt]) >> instr.sa;
    }

    void Interpreter::execute_addi(const InstructionI instr)
    {
        _regs[instr.rt] = _regs[instr.rs] + instr.constant;
    }

    void Interpreter::execute_addiu(const InstructionI instr)
    {
        _regs[instr.rt] = _regs[instr.rs] + instr.constant;
    }

    void Interpreter::execute_andi(const InstructionI instr)
    {
        _regs[instr.rt] = _regs[instr.rs] & instr.constant;
    }

    void Interpreter::execute_ori(const InstructionI instr)
    {
        _regs[instr.rt] = _regs[instr.rs] | instr.constant;
    }

    void Interpreter::execute_xori(const InstructionI instr)
    {
        _regs[instr.rt] = _regs[instr.rs] ^ instr.constant;
    }

    void Interpreter::execute_slti(const InstructionI instr)
    {
        _regs[instr.rt] =
            static_cast<int32_t>(_regs[instr.rs]) < static_cast<int32_t>(instr.constant)
            ? 1
            : 0;
    }

    void Interpreter::execute_sltiu(const InstructionI instr)
    {
        _regs[instr.rt] =
            static_cast<uint32_t>(_regs[instr.rs]) < static_cast<uint32_t>(instr.constant)
            ? 1
            : 0;
    }

    void Interpreter::execute_lui(const InstructionI instr)
    {
        _regs[instr.rt] = instr.constant << 16;
    }

    void Interpreter::execute_lw (const InstructionI instr)
    {
        _regs[instr.rt] = _mem.read<uint32_t>(calc_mem_target(instr));
    }

    void Interpreter::execute_lb (const InstructionI instr)
    {
        _regs[instr.rt] = _mem.read<int8_t>(calc_mem_target(instr));
    }

    void Interpreter::execute_lbu(const InstructionI instr)
    {
        _regs[instr.rt] = _mem.read<uint8_t>(calc_mem_target(instr));
    }

    void Interpreter::execute_lh (const InstructionI instr)
    {
        _regs[instr.rt] = _mem.read<int16_t>(calc_mem_target(instr));
    }

    void Interpreter::execute_lhu(const InstructionI instr)
    {
        _regs[instr.rt] = _mem.read<uint16_t>(calc_mem_target(instr));
    }

    void Interpreter::execute_sw (const InstructionI instr)
    {
        _mem.write<uint32_t>(calc_mem_target(instr), _regs[instr.rt]);
    }

    void Interpreter::execute_sb (const InstructionI instr)
    {
        _mem.write<uint8_t>(calc_mem_target(instr), _regs[instr.rt]);
    }

    void Interpreter::execute_sh(const InstructionI instr)
    {
        _mem.write<uint16_t>(calc_mem_target(instr), _regs[instr.rt]);
    }

    void Interpreter::execute_beq(const InstructionI instr)
    {
        if (_regs[instr.rs] == _regs[instr.rt])
            branch(instr);
    }

    void Interpreter::execute_bgtz(const InstructionI instr)
    {
        if (static_cast<int32_t>(_regs[instr.rs]) > 0)
            branch(instr);
    }

    void Interpreter::execute_blez(const InstructionI instr)
    {
        if (static_cast<int32_t>(_regs[instr.rs]) <= 0)
            branch(instr);
    }

    void Interpreter::execute_bne(const InstructionI instr)
    {
        if (_regs[instr.rs] != _regs[instr.rt])
            branch(instr);
    }

    void Interpreter::execute_bgez(const InstructionI instr)
    {
        if (static_cast<int32_t>(_regs[instr.rs]) >= 0)
            branch(instr);
    }

    void Interpreter::execute_bgezal(const InstructionI instr)
    {
        link();
        execute_bgez(instr);
    }

    void Interpreter::execute_bltz(const InstructionI instr)
    {
        if (static_cast<int32_t>(_regs[instr.rs]) < 0)
            branch(instr);
    }

    void Interpreter::execute_bltzal(const InstructionI instr)
    {
        link();
        execute_bltz(instr);
    }

    void Interpreter::execute_j(const InstructionJ instr)
    {
        const uint32_t target = (0xF0000000 & _pc) | (instr.target << 2);
        jump(target);
    }

    void Interpreter::execute_jal(const InstructionJ instr)
    {
        link();
        execute_j(instr);
    }
}
