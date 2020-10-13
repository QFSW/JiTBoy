#pragma once
#include <cstdint>
#include <vector>

class VectorBuffer
{
public:
	explicit VectorBuffer(size_t capacity = 0);
	
	template <typename T, int Size = sizeof(T)>
	void write(T data);

	void reset();
	void copy(void* ptr) const;
	[[nodiscard]] size_t size() const noexcept;

private:
	std::vector<uint8_t> _buffer;
};

template <typename T, int Size>
void VectorBuffer::write(T data)
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

