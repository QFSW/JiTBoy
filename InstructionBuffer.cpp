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

void InstructionBuffer::add_rr_16(const Register16 dst, const Register16 src)
{
	write_rr<ADD, RegisterSize::Reg16>(static_cast<Register32>(dst), static_cast<Register32>(src));
}

void InstructionBuffer::add_rr_8(const Register8 dst, const Register8 src)
{
	write_raw(ADD_8);
	write_rr_8(dst, src);
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

void InstructionBuffer::mov_rr_16(const Register16 dst, const Register16 src)
{
	write_raw(OpcodePrefix::Size16);
	write_rr<MOV, RegisterSize::Reg16>(static_cast<Register32>(dst), static_cast<Register32>(src));
}

void InstructionBuffer::mov_rr_8(const Register8 dst, const Register8 src)
{
	write_raw(MOV_8);
	write_rr_8(dst, src);
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

void InstructionBuffer::mov_ir_8(const Register8 dst, const uint8_t imm)
{
	write_raw<uint8_t>(MOV_I_8 | dst);
	write_raw(imm);
}

void InstructionBuffer::ret() { write_raw(RET); }

inline void InstructionBuffer::write_rr_32(const Register32 dst, const Register32 src)
{
	write_raw<uint8_t>(Register | src << 3 | dst);
}

inline void InstructionBuffer::write_rr_8(const Register8 dst, const Register8 src)
{
	write_raw<uint8_t>(Register | src << 3 | dst);
}

void InstructionBuffer::write_rm_32(const Register32 dst, const Register32 src)
{
	write_raw<uint8_t>(MemoryDisp0 | src << 3 | dst);
}

void InstructionBuffer::write_mr_32(const Register32 dst, const Register32 src)
{
	write_raw<uint8_t>(MemoryDisp0 | dst << 3 | src);
}

void InstructionBuffer::write_rm_8(const Register8 dst, const Register8 src)
{
	write_raw<uint8_t>(MemoryDisp0 | src << 3 | dst);
}

void InstructionBuffer::write_mr_8(const Register8 dst, const Register8 src)
{
	write_raw<uint8_t>(MemoryDisp0 | dst << 3 | src);
}
