#pragma once

#include <map>

#include <mips/register_file.hpp>
#include <compiler.hpp>

class Runtime
{
public:
	Runtime();

	void execute(const std::vector<mips::Instruction>& code);
	[[nodiscard]] const mips::RegisterFile& get_regs() const noexcept { return _regs; }
	
private:
	mips::RegisterFile _regs;
	Compiler::Allocator _allocator;
	Compiler _compiler;
	std::map<uint64_t, Compiler::func> _blocks;
};

