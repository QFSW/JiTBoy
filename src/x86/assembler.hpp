#pragma once
#include <cstdint>
#include <stdexcept>
#include <sstream>

#include <config.hpp>
#include <vector_buffer.hpp>
#include <utils/strtools.hpp>
#include <x86/x86.hpp>

namespace x86
{
    enum class InstrMode
    {
        RR,
        RM,
        MR,
        IR,
        IM
    };

    enum class JumpAdjust
    {
        None,
        Always,
        Auto
    };

    class Assembler
    {
    public:
        Assembler();

        void reset();
        void copy(void* ptr) const;
        [[nodiscard]] size_t size() const noexcept;
        [[nodiscard]] size_t instr_count() const noexcept;
        [[nodiscard]] std::string get_debug() const;

        #pragma region Non Immediate Instructions
        
        template <Opcode Op, InstrMode Mode = InstrMode::RR, RegisterSize Size = RegisterSize::Reg32>
        void instr(Register dst, Register src);

        template <Opcode Op, InstrMode Mode, RegisterSize Size = RegisterSize::Reg32>
        void instr(Register dst, Register src, int32_t addr_offset);

        template <Opcode Op, OpcodeExt Ext, InstrMode Mode = InstrMode::RR, RegisterSize Size = RegisterSize::Reg32>
        void instr(Register reg);

        template <Opcode Op, OpcodeExt Ext, InstrMode Mode = InstrMode::RR, RegisterSize Size = RegisterSize::Reg32>
        void instr(Register reg, int32_t addr_offset);

        template <Opcode Op>
        void instr()
        {
            _buffer.write(Op);
            if constexpr (debug) _debug_stream << opcode_to_string(Op) << "\n";
        }
        
        #pragma endregion 

        #pragma region Immediate Instructions
        
        template <Opcode Op, OpcodeExt Ext, InstrMode Mode = InstrMode::IR, RegisterSize Size = RegisterSize::Reg32>
        void instr_imm(Register dst, uint32_t imm);

        template <Opcode Op, OpcodeExt Ext, InstrMode Mode = InstrMode::IR, RegisterSize Size = RegisterSize::Reg32>
        void instr_imm(Register dst, uint32_t imm, int32_t addr_offset);

        #pragma endregion

        #pragma region Jump Instructions

        template <JumpAdjust Adjust = JumpAdjust::Auto>
        void jump(int32_t offset);

        template <CondCode Cond, JumpAdjust Adjust = JumpAdjust::Auto>
        void jump_cond(int32_t offset);

        template <JumpAdjust Adjust = JumpAdjust::Auto>
        void call(int32_t offset);
        
        #pragma endregion

        #pragma region Misc Intructions

        template <CondCode Cond, InstrMode Mode = InstrMode::RR, RegisterSize Size = RegisterSize::Reg32>
        void move_cond(Register dst, Register src);

        template <CondCode Cond, InstrMode Mode, RegisterSize Size = RegisterSize::Reg32>
        void move_cond(Register dst, Register src, int32_t addr_offset);

        template <CondCode Cond, InstrMode Mode = InstrMode::RR>
        void set_cond(Register dst);

        template <CondCode Cond, InstrMode Mode = InstrMode::RR>
        void set_cond(Register dst, int32_t addr_offset);

        void bswap(Register dst);
        void enter(uint16_t size, uint8_t nesting = 0);
        
        #pragma endregion 

        #pragma region Debug

        std::string debug_offset(int32_t offset);
        
        #pragma endregion 
        
    private:
        VectorBuffer _buffer;
        size_t _instr_count;

        template <Opcode Op, InstrMode Mode, RegisterSize Size>
        static consteval uint8_t encode_opcode();

        template <Opcode Op, RegisterSize Size>
        static constexpr uint8_t encode_imm_opcode(uint32_t imm);

        template <Opcode Op, InstrMode Mode, RegisterSize Size>
        void write_opcode();

        template <Opcode Op, RegisterSize Size>
        void write_imm_opcode(uint32_t imm);

        template <Opcode Op, RegisterSize Size>
        void write_immediate(uint32_t imm);

        void encode_regs(RegisterMode mode, Register dst, Register src);
        void encode_regs_offset(Register dst, Register src, int32_t addr_offset);

        template <JumpAdjust Adjust>
        static constexpr int32_t adjust_offset(int32_t offset, uint8_t instr_size);

        template <Opcode Op>
        static consteval bool is_fast_imm_instr();

        template <Opcode Op>
        static consteval bool is_imm8_only_instr();

        template <JumpAdjust Adjust, uint8_t NearSize = 2>
        static constexpr bool is_near_jump(int32_t offset);

        static constexpr bool is_8_bit(int32_t val);

        static constexpr bool debug = config::debug;
        std::stringstream _debug_stream;
    };

    #pragma region Non Immediate Instruction Implementations

    template <Opcode Op, InstrMode Mode, RegisterSize Size>
    void Assembler::instr(const Register dst, const Register src)
    {
        _instr_count++;

        write_opcode<Op, Mode, Size>();
        
        if constexpr (Mode == InstrMode::RR) encode_regs(RegisterMode::Register, dst, src);
        else if constexpr (Mode == InstrMode::RM) encode_regs(RegisterMode::MemoryDisp0, dst, src);
        else if constexpr (Mode == InstrMode::MR) encode_regs(RegisterMode::MemoryDisp0, src, dst);
        else throw std::logic_error("Invalid instruction mode encountered");

        if constexpr (debug)
        {
            _debug_stream << opcode_to_string(Op) << " ";

            const auto reg1 = reg_to_string(dst, Size);
            const auto reg2 = reg_to_string(src, Size);

            using namespace strtools;
            if constexpr (Mode == InstrMode::RR) _debug_stream << catf("%s, %s", reg1, reg2);
            else if constexpr (Mode == InstrMode::RM) _debug_stream << catf("[%s], %s",reg1, reg2);
            else if constexpr (Mode == InstrMode::MR) _debug_stream << catf("%s, [%s]",reg1, reg2);
            
            _debug_stream << "\n";
        }
    }

    template <Opcode Op, InstrMode Mode, RegisterSize Size>
    void Assembler::instr(const Register dst, const Register src, const int32_t addr_offset)
    {
        _instr_count++;

        write_opcode<Op, Mode, Size>();
        
        if constexpr (Mode == InstrMode::RM) encode_regs_offset(dst, src, addr_offset);
        else if constexpr (Mode == InstrMode::MR) encode_regs_offset(src, dst, addr_offset);
        else throw std::logic_error("Invalid instruction mode encountered");

        if constexpr (debug)
        {
            _debug_stream << opcode_to_string(Op) << " ";

            const auto reg1 = reg_to_string(dst, Size);
            const auto reg2 = reg_to_string(src, Size);

            const std::string offset = debug_offset(addr_offset);

            using namespace strtools;
            if constexpr (Mode == InstrMode::RM) _debug_stream << catf("[%s%s], %s", reg1, offset.c_str(), reg2);
            else if constexpr (Mode == InstrMode::MR) _debug_stream << catf("%s, [%s%s]", reg1, reg2, offset.c_str());

            _debug_stream << "\n";
        }
    }

    template <Opcode Op, OpcodeExt Ext, InstrMode Mode, RegisterSize Size>
    void Assembler::instr(const Register reg)
    {
        instr<Op, Mode, Size>(reg, static_cast<Register>(Ext));

        if constexpr (debug)
        {
            _debug_stream << "???\n";
        }
    }

    template <Opcode Op, OpcodeExt Ext, InstrMode Mode, RegisterSize Size>
    void Assembler::instr(const Register reg, const int32_t addr_offset)
    {
        if constexpr (Mode == InstrMode::RR) { }
        else if constexpr (Mode == InstrMode::RM) { }
        else throw std::logic_error("Invalid instruction mode encountered");

        instr<Op, Mode, Size>(reg, static_cast<Register>(Ext), addr_offset);

        if constexpr (debug)
        {
            _debug_stream << "???\n";
        }
    }

    #pragma endregion

    #pragma region Immediate Instruction Implementations

    template <Opcode Op, OpcodeExt Ext, InstrMode Mode, RegisterSize Size>
    void Assembler::instr_imm(const Register dst, const uint32_t imm)
    {
        _instr_count++;

        write_imm_opcode<Op, Size>(imm);

        if constexpr (Mode == InstrMode::IR) encode_regs(RegisterMode::Register, dst, static_cast<Register>(Ext));
        else if constexpr (Mode == InstrMode::IM) encode_regs(RegisterMode::MemoryDisp0, dst, static_cast<Register>(Ext));
        else throw std::logic_error("Invalid instruction mode encountered");

        write_immediate<Op, Size>(imm);

        if constexpr (debug)
        {
            _debug_stream << opcode_imm_to_string(Op, Ext) << " ";

            const auto reg = reg_to_string(dst, Size);

            using namespace strtools;
            if constexpr (Mode == InstrMode::IR) _debug_stream << catf("%s, %d", reg, imm);
            else if constexpr (Mode == InstrMode::IM) _debug_stream << catf("[%s], %d", reg, imm);

            _debug_stream << "\n";
        }
    }

    template <Opcode Op, OpcodeExt Ext, InstrMode Mode, RegisterSize Size>
    void Assembler::instr_imm(const Register dst, const uint32_t imm, const int32_t addr_offset)
    {
        _instr_count++;

        write_imm_opcode<Op, Size>(imm);

        if constexpr (Mode == InstrMode::IM) encode_regs_offset(dst, static_cast<Register>(Ext), addr_offset);
        else throw std::logic_error("Invalid instruction mode encountered");

        write_immediate<Op, Size>(imm);

        if constexpr (debug)
        {
            _debug_stream << opcode_imm_to_string(Op, Ext) << " ";

            const auto reg = reg_to_string(dst, Size);
            const auto offset = debug_offset(addr_offset);

            _debug_stream << strtools::catf("[%s%s], %d", reg, offset.c_str(), imm) << "\n";
        }
    }

    #pragma endregion 

    #pragma region Jump Instruction Implementations

    template <JumpAdjust Adjust>
    constexpr int32_t Assembler::adjust_offset(const int32_t offset, const uint8_t instr_size)
    {
        if constexpr (Adjust == JumpAdjust::Always) return offset - instr_size;
        else if constexpr (Adjust == JumpAdjust::Auto)
        {
            if (offset < 0) return offset - instr_size;
        }

        return offset;
    }

    template <JumpAdjust Adjust>
    void Assembler::jump(int32_t offset)
    {
        _instr_count++;

        const bool is_near = is_near_jump<Adjust>(offset);
        const uint8_t instr_size = 1 + (is_near ? 1 : 4);
        
        offset = adjust_offset<Adjust>(offset, instr_size);

        if (is_near) [[likely]]
        {
            _buffer.write(Opcode::JMP_8);
            _buffer.write<int8_t>(offset);
        }
        else
        {
            _buffer.write(Opcode::JMP_32);
            _buffer.write<int32_t>(offset);
        }

        if constexpr (debug)
        {
            _debug_stream << "???\n";
        }
    }

    template <CondCode Cond, JumpAdjust Adjust>
    void Assembler::jump_cond(int32_t offset)
    {
        _instr_count++;

        const bool is_near = is_near_jump<Adjust>(offset);
        const uint8_t instr_size = 1 + (is_near ? 1 : 5);

        offset = adjust_offset<Adjust>(offset, instr_size);

        if (is_near) [[likely]]
        {
            constexpr uint8_t op = static_cast<uint8_t>(Cond) | static_cast<uint8_t>(Opcode::Jcc_8);
            
            _buffer.write(op);
            _buffer.write<int8_t>(offset);
        }
        else
        {
            constexpr uint8_t op = static_cast<uint8_t>(Cond) | static_cast<uint8_t>(Opcode::Jcc_32);
            
            _buffer.write(OpcodePrefix::Jcc_32);
            _buffer.write(op);
            _buffer.write<int32_t>(offset);
        }

        if constexpr (debug)
        {
            _debug_stream << strtools::catf("J%s %d\n", cond_to_string(Cond), offset);
        }
    }

    template <JumpAdjust Adjust>
    void Assembler::call(int32_t offset)
    {
        _instr_count++;

        offset = adjust_offset<Adjust>(offset, 5);

        _buffer.write(Opcode::CALL);
        _buffer.write<int32_t>(offset);

        if constexpr (debug)
        {
            _debug_stream << strtools::catf("CALL %d\n", offset);
        }
    }

    #pragma endregion 

    #pragma region Misc Instruction Implementations

    template <CondCode Cond, InstrMode Mode, RegisterSize Size>
    void Assembler::move_cond(const Register dst, const Register src)
    {
        constexpr auto op = static_cast<Opcode>(static_cast<uint8_t>(Opcode::CMOVcc) | static_cast<uint8_t>(Cond));

        if constexpr (Size == RegisterSize::Reg8) throw std::logic_error("CMOVcc does not support 8 bit operands");
        else if constexpr (Size == RegisterSize::Reg16) _buffer.write(OpcodePrefix::Size16);

        _buffer.write(OpcodePrefix::CMOVcc);

        if constexpr (Mode == InstrMode::RR) instr<op, InstrMode::RR, RegisterSize::Reg8>(src, dst);
        else if constexpr (Mode == InstrMode::MR) instr<op, InstrMode::RM, RegisterSize::Reg8>(src, dst);
        else throw std::logic_error("Unsupported instruction mode");

        if constexpr (debug)
        {
            _debug_stream << strtools::catf("CMOV%s ", cond_to_string(Cond));

            const auto reg1 = reg_to_string(dst, Size);
            const auto reg2 = reg_to_string(src, Size);

            using namespace strtools;
            if constexpr (Mode == InstrMode::RR) _debug_stream << catf("%s, %s", reg1, reg2);
            else if constexpr (Mode == InstrMode::MR) _debug_stream << catf("%s, [%s]", reg1, reg2);

            _debug_stream << "\n";
        }
    }

    template <CondCode Cond, InstrMode Mode, RegisterSize Size>
    void Assembler::move_cond(const Register dst, const Register src, const int32_t addr_offset)
    {
        constexpr auto op = static_cast<Opcode>(static_cast<uint8_t>(Opcode::CMOVcc) | static_cast<uint8_t>(Cond));

        if constexpr (Size == RegisterSize::Reg8) throw std::logic_error("CMOVcc does not support 8 bit operands");
        else if constexpr (Size == RegisterSize::Reg16) _buffer.write(OpcodePrefix::Size16);

        _buffer.write(OpcodePrefix::CMOVcc);

        if constexpr (Mode == InstrMode::MR) instr<op, InstrMode::RM, RegisterSize::Reg8>(src, dst, addr_offset);
        else throw std::logic_error("Unsupported instruction mode");

        if constexpr (debug)
        {
            _debug_stream << "???\n";
        }
    }

    template <CondCode Cond, InstrMode Mode>
    void Assembler::set_cond(const Register dst)
    {
        constexpr auto src = static_cast<Register>(0);
        constexpr auto op = static_cast<Opcode>(static_cast<uint8_t>(Opcode::SETcc) | static_cast<uint8_t>(Cond));

        _buffer.write(OpcodePrefix::SETcc);

        if constexpr (Mode == InstrMode::RR) instr<op, InstrMode::RR, RegisterSize::Reg8>(dst, src);
        else if constexpr (Mode == InstrMode::RM) instr<op, InstrMode::RM, RegisterSize::Reg8>(dst, src);
        else throw std::logic_error("Unsupported instruction mode");

        if constexpr (debug)
        {
            _debug_stream << strtools::catf("SET%s ", cond_to_string(Cond));

            const auto reg = reg_to_string(dst, RegisterSize::Reg8);

            using namespace strtools;
            if constexpr (Mode == InstrMode::RR) _debug_stream << catf("%s", reg);
            else if constexpr (Mode == InstrMode::MR) _debug_stream << catf("[%s]", reg);

            _debug_stream << "\n";
        }
    }

    template <CondCode Cond, InstrMode Mode>
    void Assembler::set_cond(const Register dst, const int32_t addr_offset)
    {
        constexpr auto src = static_cast<Register>(0);
        constexpr auto op = static_cast<Opcode>(static_cast<uint8_t>(Opcode::SETcc) | static_cast<uint8_t>(Cond));

        _buffer.write(OpcodePrefix::SETcc);

        if constexpr (Mode == InstrMode::RM) instr<op, InstrMode::RM, RegisterSize::Reg8>(dst, src, addr_offset);
        else throw std::logic_error("Unsupported instruction mode");

        if constexpr (debug)
        {
            const auto cond = cond_to_string(Cond);
            const auto reg = reg_to_string(dst, RegisterSize::Reg8);
            const auto offset = debug_offset(addr_offset);

            _debug_stream << strtools::catf("SET%s [%s%s]\n", cond, reg, offset.c_str());
        }
    }

    #pragma endregion

    template <Opcode Op, InstrMode Mode, RegisterSize Size>
    consteval uint8_t Assembler::encode_opcode()
    {
        auto op = static_cast<uint8_t>(Op);
        switch (Size)
        {
            case RegisterSize::Reg16:
            case RegisterSize::Reg32: op |= 0b1; break;
            default: break;
        }

        switch (Mode)
        {
            case InstrMode::MR: op |= 0b10; break;
            default: break;
        }

        return op;
    }

    template <Opcode Op, RegisterSize Size>
    constexpr uint8_t Assembler::encode_imm_opcode(const uint32_t imm)
    {
        auto opcode = static_cast<uint8_t>(Op);
        if constexpr (Size == RegisterSize::Reg8)
        {
            return opcode;
        }
        else
        {
            opcode |= 0b1;
            if constexpr (is_fast_imm_instr<Op>())
            {
                if (is_8_bit(imm))
                {
                    opcode |= 0b10;
                }
            }

            return opcode;
        }
    }

    template <Opcode Op>
    consteval bool Assembler::is_fast_imm_instr()
    {
        switch (Op)
        {
        case Opcode::MOV_I:
        case Opcode::CALL:
        case Opcode::RET:
        case Opcode::SAL_I:
            return false;
        default:
            return true;
        }
    }

    template <Opcode Op>
    consteval bool Assembler::is_imm8_only_instr()
    {
        switch (Op)
        {
        case Opcode::SAL_I:
            return true;
        default:
            return false;
        }
    }

    template <JumpAdjust Adjust, uint8_t NearSize>
    constexpr bool Assembler::is_near_jump(const int32_t offset)
    {
        if constexpr (Adjust == JumpAdjust::None)
        {
            return is_8_bit(offset);
        }
        else if constexpr (Adjust == JumpAdjust::Auto)
        {
            return offset >= -128 + NearSize && offset <= 127;
        }
        else
        {
            return offset >= -128 + NearSize && offset <= 127 + NearSize;
        }
    }

    template <Opcode Op, InstrMode Mode, RegisterSize Size>
    void Assembler::write_opcode()
    {
        constexpr uint8_t opcode = encode_opcode<Op, Mode, Size>();
        if constexpr (Size == RegisterSize::Reg16)
        {
            _buffer.write(OpcodePrefix::Size16);
        }

        _buffer.write(opcode);
    }

    template <Opcode Op, RegisterSize Size>
    void Assembler::write_imm_opcode(const uint32_t imm)
    {
        const uint8_t opcode = encode_imm_opcode<Op, Size>(imm);
        if constexpr (Size == RegisterSize::Reg16)
        {
            _buffer.write(OpcodePrefix::Size16);
        }

        _buffer.write(opcode);
    }

    template <Opcode Op, RegisterSize Size>
    void Assembler::write_immediate(const uint32_t imm)
    {
        if constexpr (Size == RegisterSize::Reg8) _buffer.write<uint8_t>(imm);
        else
        {
            constexpr bool fast = is_fast_imm_instr<Op>();

            if (fast && is_8_bit(imm))                      _buffer.write<uint8_t>(imm);
            else if constexpr (is_imm8_only_instr<Op>())    _buffer.write<uint8_t>(imm);
            else if constexpr (Size == RegisterSize::Reg16) _buffer.write<uint16_t>(imm);
            else if constexpr (Size == RegisterSize::Reg32) _buffer.write<uint32_t>(imm);
            else throw std::logic_error("Unsupported register size");
        }
    }

    constexpr bool Assembler::is_8_bit(const int32_t val)
    {
        return val == static_cast<int8_t>(val);
    }
}

