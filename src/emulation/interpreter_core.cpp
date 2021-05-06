#include "interpreter_core.hpp"

#include <stdexcept>
#include <mips/utils.hpp>
#include <utils/functional.hpp>

namespace emulation
{
    using namespace mips;

    InterpreterCore::InterpreterCore(EmulatorState& state)
        : _state(state)
    { }

    void InterpreterCore::execute_current()
    {
        execute(_state.pc);
        _state.pc += 4;
    }

    void InterpreterCore::execute(const uint32_t pc)
    {
        execute(_state.program.source[pc / 4]);
    }

    void InterpreterCore::execute(const Instruction instr)
    {
        std::visit(functional::overload{
            [&](const auto& x) { execute(x); }
        }, instr);
    }

    void InterpreterCore::execute(const InstructionR instr)
    {
        switch (instr.op)
        {
            case OpcodeR::ADD:   execute_add(instr); break;
            case OpcodeR::ADDU:  execute_addu(instr); break;
            case OpcodeR::SUB:   execute_sub(instr); break;
            case OpcodeR::SUBU:  execute_subu(instr); break;
            case OpcodeR::AND:   execute_and(instr); break;
            case OpcodeR::OR:    execute_or(instr); break;
            case OpcodeR::NOR:   execute_nor(instr); break;
            case OpcodeR::XOR:   execute_xor(instr); break;
            case OpcodeR::JR:    execute_jr(instr); break;
            case OpcodeR::JALR:  execute_jalr(instr); break;
            case OpcodeR::MULT:  execute_mult(instr); break;
            case OpcodeR::MULTU: execute_multu(instr); break;
            case OpcodeR::DIV:   execute_div(instr); break;
            case OpcodeR::DIVU:  execute_divu(instr); break;
            case OpcodeR::MFHI:  execute_mfhi(instr); break;
            case OpcodeR::MFLO:  execute_mflo(instr); break;
            case OpcodeR::MTHI:  execute_mthi(instr); break;
            case OpcodeR::MTLO:  execute_mtlo(instr); break;
            case OpcodeR::SLT:   execute_slt(instr); break;
            case OpcodeR::SLTU:  execute_sltu(instr); break;
            case OpcodeR::SLL:   execute_sll(instr); break;
            case OpcodeR::SRA:   execute_sra(instr); break;
            case OpcodeR::SRL:   execute_srl(instr); break;
            case OpcodeR::SLLV:  execute_sllv(instr); break;
            case OpcodeR::SRAV:  execute_srav(instr); break;
            case OpcodeR::SRLV:  execute_srlv(instr); break;
            default: throw_invalid_instr(instr);
        }
    }

    void InterpreterCore::execute(const InstructionI instr)
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
            case OpcodeI::LWL:    execute_lwl(instr); break;
            case OpcodeI::LWR:    execute_lwr(instr); break;
            case OpcodeI::BGEZ:   execute_bgez(instr); break;
            case OpcodeI::BGEZAL: execute_bgezal(instr); break;
            case OpcodeI::BLTZ:   execute_bltz(instr); break;
            case OpcodeI::BLTZAL: execute_bltzal(instr); break;
            default: throw_invalid_instr(instr);
        }
    }

    void InterpreterCore::execute(const mips::InstructionJ instr)
    {
        switch (instr.op)
        {
            case OpcodeJ::J:   execute_j(instr); break;
            case OpcodeJ::JAL: execute_jal(instr); break;
            default: throw_invalid_instr(instr);
        }
    }

    void InterpreterCore::throw_invalid_instr(const Instruction instr)
    {
        std::visit(functional::overload{
            [&](const auto& x) { throw std::logic_error(std::string("Instruction ") + mips::opcode_to_string(x.op) + " is not supported"); }
        }, instr);
    }

    void InterpreterCore::link(const Register reg)
    {
        _state.regs.write(reg, _state.pc + 8);
    }

    void InterpreterCore::jump(const uint32_t target)
    {
        const uint32_t delay_slot = _state.pc + 4;
        if (_state.program.valid_addr(delay_slot))
            execute(delay_slot);

        _state.pc = target - 4;
    }

    void InterpreterCore::branch(const InstructionI instr)
    {
        const uint32_t target = _state.pc + (instr.constant << 2);
        jump(target);
    }

    uint32_t InterpreterCore::calc_mem_target(const InstructionI instr)
    {
        return _state.regs[instr.rs] + instr.constant;
    }

    void InterpreterCore::execute_add(const InstructionR instr)
    {
        _state.regs.write(instr.rd, _state.regs[instr.rs] + _state.regs[instr.rt]);
    }

    void InterpreterCore::execute_addu(const InstructionR instr)
    {
        _state.regs.write(instr.rd, _state.regs[instr.rs] + _state.regs[instr.rt]);
    }

    void InterpreterCore::execute_sub(const InstructionR instr)
    {
        _state.regs.write(instr.rd, _state.regs[instr.rs] - _state.regs[instr.rt]);
    }

    void InterpreterCore::execute_subu(const InstructionR instr)
    {
        _state.regs.write(instr.rd, _state.regs[instr.rs] - _state.regs[instr.rt]);
    }

    void InterpreterCore::execute_and(const InstructionR instr)
    {
        _state.regs.write(instr.rd, _state.regs[instr.rs] & _state.regs[instr.rt]);
    }

    void InterpreterCore::execute_or(const InstructionR instr)
    {
        _state.regs.write(instr.rd, _state.regs[instr.rs] | _state.regs[instr.rt]);
    }

    void InterpreterCore::execute_nor(const InstructionR instr)
    {
        _state.regs.write(instr.rd, ~(_state.regs[instr.rs] | _state.regs[instr.rt]));
    }

    void InterpreterCore::execute_xor(const InstructionR instr)
    {
        _state.regs.write(instr.rd, _state.regs[instr.rs] ^ _state.regs[instr.rt]);
    }

    void InterpreterCore::execute_jr(const InstructionR instr)
    {
        jump(_state.regs[instr.rs]);
    }

    void InterpreterCore::execute_jalr(const InstructionR instr)
    {
        link(instr.rd);
        execute_jr(instr);
    }

    void InterpreterCore::execute_mult(const InstructionR instr)
    {
        const int64_t left = static_cast<int64_t>(static_cast<int32_t>(_state.regs[instr.rs]));
        const int64_t right = static_cast<int64_t>(static_cast<int32_t>(_state.regs[instr.rt]));
        const int64_t res = left * right;
        _state.regs.lo() = res & 0xFFFFFFFF;
        _state.regs.hi() = res >> 32;
    }

    void InterpreterCore::execute_multu(const InstructionR instr)
    {
        const uint64_t left = static_cast<uint64_t>(_state.regs[instr.rs]);
        const uint64_t right = static_cast<uint64_t>(_state.regs[instr.rt]);
        const uint64_t res = left * right;
        _state.regs.lo() = res & 0xFFFFFFFF;
        _state.regs.hi() = res >> 32;
    }

    void InterpreterCore::execute_div(const InstructionR instr)
    {
        _state.regs.lo() = static_cast<int32_t>(_state.regs[instr.rs]) / static_cast<int32_t>(_state.regs[instr.rt]);
        _state.regs.hi() = static_cast<int32_t>(_state.regs[instr.rs]) % static_cast<int32_t>(_state.regs[instr.rt]);
    }

    void InterpreterCore::execute_divu(const InstructionR instr)
    {
        _state.regs.lo() = static_cast<uint32_t>(_state.regs[instr.rs]) / static_cast<uint32_t>(_state.regs[instr.rt]);
        _state.regs.hi() = static_cast<uint32_t>(_state.regs[instr.rs]) % static_cast<uint32_t>(_state.regs[instr.rt]);
    }

    void InterpreterCore::execute_mfhi(const InstructionR instr)
    {
        _state.regs.write(instr.rd, _state.regs.hi());
    }

    void InterpreterCore::execute_mflo(const InstructionR instr)
    {
        _state.regs.write(instr.rd, _state.regs.lo());
    }

    void InterpreterCore::execute_mthi(const InstructionR instr)
    {
        _state.regs.hi() = _state.regs[instr.rs];
    }

    void InterpreterCore::execute_mtlo(const InstructionR instr)
    {
        _state.regs.lo() = _state.regs[instr.rs];
    }

    void InterpreterCore::execute_slt(const InstructionR instr)
    {
        const uint32_t val =
            static_cast<int32_t>(_state.regs[instr.rs]) < static_cast<int32_t>(_state.regs[instr.rt])
            ? 1
            : 0;

        _state.regs.write(instr.rd, val);
    }

    void InterpreterCore::execute_sltu(const InstructionR instr)
    {
        const uint32_t val =
            static_cast<uint32_t>(_state.regs[instr.rs]) < static_cast<uint32_t>(_state.regs[instr.rt])
            ? 1
            : 0;

        _state.regs.write(instr.rd, val);
    }

    void InterpreterCore::execute_sll(const InstructionR instr)
    {
        _state.regs.write(instr.rd, _state.regs[instr.rt] << instr.sa);
    }

    void InterpreterCore::execute_sra(const InstructionR instr)
    {
        _state.regs.write(instr.rd, static_cast<int32_t>(_state.regs[instr.rt]) >> instr.sa);
    }

    void InterpreterCore::execute_srl(const InstructionR instr)
    {
        _state.regs.write(instr.rd, static_cast<uint32_t>(_state.regs[instr.rt]) >> instr.sa);
    }

    void InterpreterCore::execute_sllv(const InstructionR instr)
    {
        const uint8_t sa = _state.regs[instr.rs] & 0b11111;
        _state.regs.write(instr.rd, _state.regs[instr.rt] << sa);
    }

    void InterpreterCore::execute_srav(const InstructionR instr)
    {
        const uint8_t sa = _state.regs[instr.rs] & 0b11111;
        _state.regs.write(instr.rd, static_cast<int32_t>(_state.regs[instr.rt]) >> sa);
    }

    void InterpreterCore::execute_srlv(const InstructionR instr)
    {
        const uint8_t sa = _state.regs[instr.rs] & 0b11111;
        _state.regs.write(instr.rd, static_cast<uint32_t>(_state.regs[instr.rt]) >> sa);
    }

    void InterpreterCore::execute_addi(const InstructionI instr)
    {
        _state.regs.write(instr.rt, _state.regs[instr.rs] + instr.constant);
    }

    void InterpreterCore::execute_addiu(const InstructionI instr)
    {
        _state.regs.write(instr.rt, _state.regs[instr.rs] + instr.constant);
    }

    void InterpreterCore::execute_andi(const InstructionI instr)
    {
        _state.regs.write(instr.rt, _state.regs[instr.rs] & instr.constant);
    }

    void InterpreterCore::execute_ori(const InstructionI instr)
    {
        _state.regs.write(instr.rt, _state.regs[instr.rs] | instr.constant);
    }

    void InterpreterCore::execute_xori(const InstructionI instr)
    {
        _state.regs.write(instr.rt, _state.regs[instr.rs] ^ instr.constant);
    }

    void InterpreterCore::execute_slti(const InstructionI instr)
    {
        const uint32_t val =
            static_cast<int32_t>(_state.regs[instr.rs]) < static_cast<int32_t>(instr.constant)
            ? 1
            : 0;

        _state.regs.write(instr.rt, val);
    }

    void InterpreterCore::execute_sltiu(const InstructionI instr)
    {
        const uint32_t val =
            static_cast<uint32_t>(_state.regs[instr.rs]) < static_cast<uint32_t>(instr.constant)
            ? 1
            : 0;

        _state.regs.write(instr.rt, val);
    }

    void InterpreterCore::execute_lui(const InstructionI instr)
    {
        _state.regs.write(instr.rt, instr.constant << 16);
    }

    void InterpreterCore::execute_lw(const InstructionI instr)
    {
        _state.regs.write(instr.rt, _state.mem.read<uint32_t>(calc_mem_target(instr)));
    }

    void InterpreterCore::execute_lb(const InstructionI instr)
    {
        _state.regs.write(instr.rt, _state.mem.read<int8_t>(calc_mem_target(instr)));
    }

    void InterpreterCore::execute_lbu(const InstructionI instr)
    {
        _state.regs.write(instr.rt, _state.mem.read<uint8_t>(calc_mem_target(instr)));
    }

    void InterpreterCore::execute_lh(const InstructionI instr)
    {
        _state.regs.write(instr.rt, _state.mem.read<int16_t>(calc_mem_target(instr)));
    }

    void InterpreterCore::execute_lhu(const InstructionI instr)
    {
        _state.regs.write(instr.rt, _state.mem.read<uint16_t>(calc_mem_target(instr)));
    }

    void InterpreterCore::execute_sw(const InstructionI instr)
    {
        _state.mem.write<uint32_t>(calc_mem_target(instr), _state.regs[instr.rt]);
    }

    void InterpreterCore::execute_sb(const InstructionI instr)
    {
        _state.mem.write<uint8_t>(calc_mem_target(instr), _state.regs[instr.rt]);
    }

    void InterpreterCore::execute_sh(const InstructionI instr)
    {
        _state.mem.write<uint16_t>(calc_mem_target(instr), _state.regs[instr.rt]);
    }

    void InterpreterCore::execute_beq(const InstructionI instr)
    {
        if (_state.regs[instr.rs] == _state.regs[instr.rt])
            branch(instr);
    }

    void InterpreterCore::execute_bgtz(const InstructionI instr)
    {
        if (static_cast<int32_t>(_state.regs[instr.rs]) > 0)
            branch(instr);
    }

    void InterpreterCore::execute_blez(const InstructionI instr)
    {
        if (static_cast<int32_t>(_state.regs[instr.rs]) <= 0)
            branch(instr);
    }

    void InterpreterCore::execute_bne(const InstructionI instr)
    {
        if (_state.regs[instr.rs] != _state.regs[instr.rt])
            branch(instr);
    }

    void InterpreterCore::execute_lwl(const InstructionI instr)
    {
        const uint32_t addr = calc_mem_target(instr);
        const uint32_t result = _state.mem.load_word_left(addr, _state.regs[instr.rt]);
        _state.regs.write(instr.rt, result);
    }

    void InterpreterCore::execute_lwr(const InstructionI instr)
    {
        const uint32_t addr = calc_mem_target(instr);
        const uint32_t result = _state.mem.load_word_right(addr, _state.regs[instr.rt]);
        _state.regs.write(instr.rt, result);
    }

    void InterpreterCore::execute_bgez(const InstructionI instr)
    {
        if (static_cast<int32_t>(_state.regs[instr.rs]) >= 0)
            branch(instr);
    }

    void InterpreterCore::execute_bgezal(const InstructionI instr)
    {
        link();
        execute_bgez(instr);
    }

    void InterpreterCore::execute_bltz(const InstructionI instr)
    {
        if (static_cast<int32_t>(_state.regs[instr.rs]) < 0)
            branch(instr);
    }

    void InterpreterCore::execute_bltzal(const InstructionI instr)
    {
        link();
        execute_bltz(instr);
    }

    void InterpreterCore::execute_j(const InstructionJ instr)
    {
        const uint32_t target = (0xF0000000 & _state.pc) | (instr.target << 2);
        jump(target);
    }

    void InterpreterCore::execute_jal(const InstructionJ instr)
    {
        link();
        execute_j(instr);
    }
}
