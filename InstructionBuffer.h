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

	void add_rr_32(Register32 dst, Register32 src);
	void add_rr_8(Register8 dst, Register8 src);
	void add_rm_32(Register32 dst, Register32 src);
	void add_mr_32(Register32 dst, Register32 src);
	
	void mov_ir_32(Register32 dst, uint32_t imm);
	void mov_ir_8(Register8 dst, uint8_t imm);
	void ret();

private:
	std::vector<uint8_t> _buffer;

	void write_rr_32(Register32 dst, Register32 src);
	void write_rr_8(Register8 dst, Register8 src);

	void write_rm_32(Register32 dst, Register32 src);
	void write_mr_32(Register32 dst, Register32 src);
	void write_rm_8(Register8 dst, Register8 src);
	void write_mr_8(Register8 dst, Register8 src);
};

