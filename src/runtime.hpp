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
	[[nodiscard]] std::string get_debug() const;
	
private:
	mips::RegisterFile _regs;
	Compiler::Allocator _allocator;
	Compiler _compiler;
	std::map<uint64_t, Compiler::func> _blocks;

	static constexpr bool debug = true;
	std::stringstream _debug_stream;
};

