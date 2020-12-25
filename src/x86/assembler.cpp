#include "assembler.hpp"

namespace x86
{
    Assembler::Assembler()
        : _buffer()
        , _instr_count()
        , _debug_stream()
    { }

    void Assembler::reset()
    {
        _buffer.reset();
        _instr_count = 0;
        if constexpr (debug) std::stringstream().swap(_debug_stream);
    }

    size_t Assembler::size() const noexcept
    {
        return _buffer.size();
    }

    size_t Assembler::instr_count() const noexcept
    {
        return _instr_count;
    }

    void Assembler::copy(void* ptr) const
    {
        _buffer.copy(ptr);
    }

    void Assembler::bswap(Register dst)
    {
        _instr_count++;

        _buffer.write<uint8_t>(0x0F);
        _buffer.write<uint8_t>(0xC8 | static_cast<uint8_t>(dst));

        if constexpr (debug)
        {
            _debug_stream << strtools::catf("BSWAP %s\n", reg_to_string(dst, RegisterSize::Reg32));
        }
    }

    void Assembler::enter(const uint16_t size, const uint8_t nesting)
    {
        _instr_count++;

        _buffer.write(Opcode::ENTER);
        _buffer.write(size);
        _buffer.write(nesting);

        if constexpr (debug)
        {
            _debug_stream << strtools::catf("ENTER %d, %d\n", size, nesting);
        }
    }

    void Assembler::encode_regs(RegisterMode mode, Register dst, Register src)
    {
        const uint8_t val = static_cast<uint8_t>(mode) | static_cast<uint8_t>(src) << 3 | static_cast<uint8_t>(dst);
        _buffer.write(val);
    }

    void Assembler::encode_regs_offset(const Register dst, const Register src, const int32_t addr_offset)
    {
        if (addr_offset == 0)
        {
            encode_regs(RegisterMode::MemoryDisp0, dst, src);
        }
        else if (is_8_bit(addr_offset))
        {
            encode_regs(RegisterMode::MemoryDisp1, dst, src);
            _buffer.write<uint8_t>(addr_offset);
        }
        else
        {
            encode_regs(RegisterMode::MemoryDisp4, dst, src);
            _buffer.write<uint32_t>(addr_offset);
        }
    }

    std::string Assembler::get_debug() const
    {
        return _debug_stream.str();
    }

    std::string Assembler::debug_offset(const int32_t offset)
    {
        std::stringstream ss;
        if (offset)
        {
            ss << " " << (offset > 0 ? '+' : '-')
               << " "
               << abs(offset);
        }

        return ss.str();
    }
}
