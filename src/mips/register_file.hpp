#pragma once
#include <mips/mips.hpp>

namespace mips
{
	class RegisterFile
	{
	public:
		[[nodiscard]] size_t size() const noexcept { return reg_count; }
		[[nodiscard]] uint32_t* data() noexcept { return _regs; }
		[[nodiscard]] const uint32_t* data() const noexcept { return _regs; }
		
	private:
		static constexpr size_t reg_count = 32;
		uint32_t _regs[reg_count] = {};
	};
}

