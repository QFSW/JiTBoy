#include "InstructionBuffer.h"

void InstructionBuffer::reset()
{
	_buffer.clear();
}

inline size_t InstructionBuffer::size() const noexcept
{
	return _buffer.size();
}

void InstructionBuffer::copy(void* ptr) const
{
	memcpy(ptr, _buffer.data(), _buffer.size());
}

inline void InstructionBuffer::write_raw(const uint8_t data)
{
	_buffer.push_back(data);
}

void InstructionBuffer::encode_regs(RegisterMode mode, Register dst, Register src)
{
	const uint8_t val = static_cast<uint8_t>(mode) | static_cast<uint8_t>(src) << 3 | static_cast<uint8_t>(dst);
	write_raw(val);
}

void InstructionBuffer::encode_regs_offset(const Register dst, const Register src, const uint32_t addr_offset)
{
	if (addr_offset == 0)
	{
		encode_regs(RegisterMode::MemoryDisp0, dst, src);
	}
	else if ((addr_offset & ~0xFF) == 0)
	{
		encode_regs(RegisterMode::MemoryDisp1, dst, src);
		write_raw<uint8_t>(addr_offset);
	}
	else
	{
		encode_regs(RegisterMode::MemoryDisp4, dst, src);
		write_raw<uint32_t>(addr_offset);
	}
}
