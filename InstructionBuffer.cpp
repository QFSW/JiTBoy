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
	write_raw<uint8_t>(MOV_IR_32 | dst);
	write_raw(imm);
}

void InstructionBuffer::mov_ir_8(const Register8 dst, const uint8_t imm)
{
	write_raw<uint8_t>(MOV_IR_8 | dst);
	write_raw(imm);
}

void InstructionBuffer::ret() { write_raw(RET); }
