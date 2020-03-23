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

void InstructionBuffer::mov_ir_32(const Register32 dst, const uint32_t imm)
{
	write_raw<uint8_t>(MOV_I_32 | dst);
	write_raw(imm);
}

void InstructionBuffer::encode_regs(RegisterMode mode, Register dst, Register src)
{
	const uint8_t val = static_cast<uint8_t>(mode) | static_cast<uint8_t>(src) << 3 | static_cast<uint8_t>(dst);
	write_raw(val);
}
