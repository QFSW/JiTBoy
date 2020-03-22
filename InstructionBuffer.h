#pragma once
#include <cstdint>
#include <vector>
#include "x86_64.h"

class InstructionBuffer
{
public:
	void reset();
	size_t size() const;
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

	void write_mov_ir_32(Register32 dst, uint32_t imm);
	void write_mov_ir_8(Register8 dst, uint8_t imm);

private:
	std::vector<uint8_t> _buffer;
};

