#include "Assembler.h"

void Assembler::reset()
{
	_buffer.reset();
}

size_t Assembler::size() const noexcept
{
	return _buffer.size();
}

void Assembler::copy(void* ptr) const
{
	_buffer.copy(ptr);
}

void Assembler::bswap(Register dst)
{
	_buffer.write_raw<uint8_t>(0x0F);
	_buffer.write_raw<uint8_t>(0xC8 | static_cast<uint8_t>(dst));
}

void Assembler::enter(const uint16_t size, const uint8_t nesting)
{
	_buffer.write_raw(Opcode::ENTER);
	_buffer.write_raw(size);
	_buffer.write_raw(nesting);
}

void Assembler::encode_regs(RegisterMode mode, Register dst, Register src)
{
	const uint8_t val = static_cast<uint8_t>(mode) | static_cast<uint8_t>(src) << 3 | static_cast<uint8_t>(dst);
	_buffer.write_raw(val);
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
		_buffer.write_raw<uint8_t>(addr_offset);
	}
	else
	{
		encode_regs(RegisterMode::MemoryDisp4, dst, src);
		_buffer.write_raw<uint32_t>(addr_offset);
	}
}
