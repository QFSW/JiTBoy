#pragma once
#include <cstdint>
#include <vector>
#include "x86_64.h"

class InstructionBuffer
{
public:
	void reset();
	size_t size() const noexcept;
	void copy(void* ptr) const;

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
			*buffer_ptr = data;
		}
	}
	
	void write_raw(uint8_t data);

	template <Opcode8 Op, RegisterSize Size = RegisterSize::Reg32>
	void write_rr(const Register dst, const Register src)
	{
		constexpr uint8_t opcode = encode_opcode<Op, Size>();
		if constexpr (Size == RegisterSize::Reg16)
		{
			write_raw(OpcodePrefix::Size16);
		}

		write_raw(opcode);
		encode_regs(RegisterMode::Register, dst, src);
	}

	void add_rm_32(Register32 dst, Register32 src);
	void add_mr_32(Register32 dst, Register32 src);

	void mov_rm_32(Register32 dst, Register32 src);
	void mov_mr_32(Register32 dst, Register32 src);
	
	void mov_ir_32(Register32 dst, uint32_t imm);
	void ret();

private:
	std::vector<uint8_t> _buffer;

	template <Opcode8 Op, RegisterSize Size>
	static constexpr uint8_t encode_opcode()
	{
		auto op = static_cast<uint8_t>(Op);
		switch (Size)
		{
			case RegisterSize::Reg16:
			case RegisterSize::Reg32: op |= 0b1; break;
			default: break;
		}

		return op;
	}

	void encode_regs(RegisterMode mode, Register dst, Register src);

	void write_rr_32(Register32 dst, Register32 src);

	void write_rm_32(Register32 dst, Register32 src);
	void write_mr_32(Register32 dst, Register32 src);
};

