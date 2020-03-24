#pragma once
#include <cstdint>
#include <vector>
#include "x86_64.h"

enum class InstrMode : uint8_t
{
	RR,
	RM,
	MR
};

class InstructionBuffer
{
public:
	void reset();
	size_t size() const noexcept;
	void copy(void* ptr) const;

	#pragma region Non Immediate Instructions
	
	template <Opcode Op, InstrMode Mode = InstrMode::RR, RegisterSize Size = RegisterSize::Reg32>
	void instr(Register dst, Register src);

	template <Opcode Op, InstrMode Mode, RegisterSize Size = RegisterSize::Reg32>
	void instr(Register dst, Register src, uint32_t addr_offset);

	template <Opcode Op, OpcodeExt Ext, InstrMode Mode = InstrMode::RR, RegisterSize Size = RegisterSize::Reg32>
	void instr(Register reg);

	template <Opcode Op, OpcodeExt Ext, InstrMode Mode = InstrMode::RR, RegisterSize Size = RegisterSize::Reg32>
	void instr(Register reg, uint32_t addr_offset);

	template <Opcode Op>
	void instr() { write_raw(Op); }
	
	#pragma endregion 

	#pragma region Immediate Instructions
	
	template <Opcode Op, OpcodeExt Ext, InstrMode Mode = InstrMode::RR, RegisterSize Size = RegisterSize::Reg32>
	void instr_imm(Register dst, uint32_t imm);

	template <Opcode Op, OpcodeExt Ext, InstrMode Mode = InstrMode::RR, RegisterSize Size = RegisterSize::Reg32>
	void instr_imm(Register dst, uint32_t imm, uint32_t addr_offset);

	#pragma endregion 

private:
	std::vector<uint8_t> _buffer;

	template <typename T, int Size = sizeof(T)>
	void write_raw(T data);

	void write_raw(uint8_t data);

	template <Opcode Op, InstrMode Mode, RegisterSize Size>
	static constexpr uint8_t encode_opcode();

	template <Opcode Op, InstrMode Mode = InstrMode::RR, RegisterSize Size = RegisterSize::Reg32>
	void write_opcode();

	void encode_regs(RegisterMode mode, Register dst, Register src);
	void encode_regs_offset(Register dst, Register src, uint32_t addr_offset);
};

template <Opcode Op, InstrMode Mode, RegisterSize Size>
constexpr uint8_t InstructionBuffer::encode_opcode()
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

#pragma region Non Immediate Instruction Implementations

template <Opcode Op, InstrMode Mode, RegisterSize Size>
void InstructionBuffer::instr(const Register dst, const Register src)
{
	write_opcode<Op, Mode, Size>();
	
	if constexpr (Mode == InstrMode::RR) encode_regs(RegisterMode::Register, dst, src);
	else if constexpr (Mode == InstrMode::RM) encode_regs(RegisterMode::MemoryDisp0, dst, src);
	else if constexpr (Mode == InstrMode::MR) encode_regs(RegisterMode::MemoryDisp0, src, dst);
	else throw "Invalid instruction mode encountered";
}

template <Opcode Op, InstrMode Mode, RegisterSize Size>
void InstructionBuffer::instr(const Register dst, const Register src, const uint32_t addr_offset)
{
	write_opcode<Op, Mode, Size>();
	
	if constexpr (Mode == InstrMode::RM) encode_regs_offset(dst, src, addr_offset);
	else if constexpr (Mode == InstrMode::MR) encode_regs_offset(src, dst, addr_offset);
	else throw "Invalid instruction mode encountered";
}

template <Opcode Op, OpcodeExt Ext, InstrMode Mode, RegisterSize Size>
void InstructionBuffer::instr(const Register reg)
{
	instr<Op, Mode, Size>(reg, static_cast<Register>(Ext));
}

template <Opcode Op, OpcodeExt Ext, InstrMode Mode, RegisterSize Size>
void InstructionBuffer::instr(const Register reg, const uint32_t addr_offset)
{
	instr<Op, Mode, Size>(reg, static_cast<Register>(Ext), addr_offset);
}

#pragma endregion

#pragma region Immediate Instruction Implementations

template <Opcode Op, OpcodeExt Ext, InstrMode Mode, RegisterSize Size>
void InstructionBuffer::instr_imm(const Register dst, const uint32_t imm)
{
	write_raw<uint8_t>(static_cast<uint8_t>(Op) | 0b1);

	if constexpr (Mode == InstrMode::RR) encode_regs(RegisterMode::Register, dst, static_cast<Register>(Ext));
	else if constexpr (Mode == InstrMode::RM) encode_regs(RegisterMode::MemoryDisp0, dst, static_cast<Register>(Ext));
	else throw "Invalid instruction mode encountered";

	write_raw(imm);
}

template <Opcode Op, OpcodeExt Ext, InstrMode Mode, RegisterSize Size>
void InstructionBuffer::instr_imm(const Register dst, const uint32_t imm, const uint32_t addr_offset)
{
	write_raw<uint8_t>(static_cast<uint8_t>(Op) | 0b1);

	if constexpr (Mode == InstrMode::RM) encode_regs_offset(dst, static_cast<Register>(Ext), addr_offset);
	else throw "Invalid instruction mode encountered";

	write_raw(imm);
}

#pragma endregion 

template <typename T, int Size>
void InstructionBuffer::write_raw(const T data)
{
	if constexpr (Size == 1)
	{
		_buffer.push_back(static_cast<uint8_t>(data));
	}
	else
	{
		_buffer.resize(size() + Size);
		if constexpr (Size == sizeof(T))
		{
			T* buffer_ptr = reinterpret_cast<T*>(&_buffer.back() - Size + 1);
			*buffer_ptr = data;
		}
		else
		{
			void* buffer_ptr = &_buffer.back() - Size + 1;
			memcpy(buffer_ptr, &data, Size);
		}
	}
}

template <Opcode Op, InstrMode Mode, RegisterSize Size>
void InstructionBuffer::write_opcode()
{
	constexpr uint8_t opcode = encode_opcode<Op, Mode, Size>();
	if constexpr (Size == RegisterSize::Reg16)
	{
		write_raw(OpcodePrefix::Size16);
	}

	write_raw(opcode);
}

