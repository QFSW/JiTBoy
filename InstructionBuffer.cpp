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

void InstructionBuffer::add_rm_32(const Register32 dst, const Register32 src)
{
	write_raw(ADD_32);
	write_rm_32(dst, src);
}

void InstructionBuffer::add_mr_32(const Register32 dst, const Register32 src)
{
	write_raw<uint8_t>(ADD_32 | 0b10);
	write_mr_32(dst, src);
}

void InstructionBuffer::mov_rm_32(const Register32 dst, const Register32 src)
{
	write_raw(MOV_32);
	write_rm_32(dst, src);
}

void InstructionBuffer::mov_mr_32(const Register32 dst, const Register32 src)
{
	write_raw<uint8_t>(MOV_32 | 0b10);
	write_mr_32(dst, src);
}

void InstructionBuffer::mov_ir_32(const Register32 dst, const uint32_t imm)
{
	write_raw<uint8_t>(MOV_I_32 | dst);
	write_raw(imm);
}

void InstructionBuffer::ret() { write_raw(RET); }

void InstructionBuffer::encode_regs(RegisterMode mode, Register dst, Register src)
{
	const uint8_t val = static_cast<uint8_t>(RegisterMode::Register) | static_cast<uint8_t>(src) << 3 | static_cast<uint8_t>(dst);
	write_raw(val);
}

inline void InstructionBuffer::write_rr_32(const Register32 dst, const Register32 src)
{
	write_raw<uint8_t>(static_cast<uint8_t>(RegisterMode::Register) | src << 3 | dst);
}

void InstructionBuffer::write_rm_32(const Register32 dst, const Register32 src)
{
	write_raw<uint8_t>(static_cast<uint8_t>(RegisterMode::MemoryDisp0) | src << 3 | dst);
}

void InstructionBuffer::write_mr_32(const Register32 dst, const Register32 src)
{
	write_raw<uint8_t>(static_cast<uint8_t>(RegisterMode::MemoryDisp0) | dst << 3 | src);
}
