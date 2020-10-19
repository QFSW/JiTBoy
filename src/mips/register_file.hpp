#pragma once
#include <mips/mips.hpp>

namespace mips
{
	class RegisterFile
	{
	public:
		[[nodiscard]] size_t size() const noexcept { return reg_count; }
		[[nodiscard]] Register* data() noexcept { return _regs; }
		[[nodiscard]] const Register* data() const noexcept { return _regs; }
		
	private:
		static constexpr size_t reg_count = 32;
		Register _regs[reg_count] = {};
	};
}

