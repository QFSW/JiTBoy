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

	template <Opcode8 Op, InstrMode Mode = InstrMode::RR, RegisterSize Size = RegisterSize::Reg32>
	void instr(const Register dst, const Register src)
	{
		constexpr uint8_t opcode = encode_opcode<Op, Mode, Size>();
		if constexpr (Size == RegisterSize::Reg16)
		{
			write_raw(OpcodePrefix::Size16);
		}

		write_raw(opcode);
		if constexpr (Mode == InstrMode::RR) encode_regs(RegisterMode::Register, dst, src);
		else if constexpr (Mode == InstrMode::RM) encode_regs(RegisterMode::MemoryDisp0, dst, src);
		else if constexpr (Mode == InstrMode::MR) encode_regs(RegisterMode::MemoryDisp0, src, dst);
	}

	template <Opcode8 Op, InstrMode Mode = InstrMode::RR, RegisterSize Size = RegisterSize::Reg32>
	void instr(const Register reg, const UnaryOpcodeExt ext)
	{
		instr<Op, Mode, Size>(reg, static_cast<Register>(ext));
	}

	template <Opcode8 Op>
	void instr() { write_raw(Op); }

	void mov_ir_32(Register32 dst, uint32_t imm);

private:
	std::vector<uint8_t> _buffer;

	template <typename T, int Size = sizeof(T)>
	void write_raw(const T data)
	{
		if constexpr (Size == 1)
		{
			_buffer.push_back(static_cast<uint8_t>(data));
		}
		else
		{
			_buffer.resize(size() + Size);
			T* buffer_ptr = reinterpret_cast<T*>(&_buffer.back() - Size + 1);
			memcpy(buffer_ptr, &data, Size);
		}
	}

	void write_raw(uint8_t data);

	template <Opcode8 Op, InstrMode Mode, RegisterSize Size>
	static constexpr uint8_t encode_opcode()
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

	void encode_regs(RegisterMode mode, Register dst, Register src);
};

