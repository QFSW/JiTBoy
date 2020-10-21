#pragma once
#include <compiler_config.hpp>

struct CompiledBlock
{
	typedef void(*func)();
	CompiledBlock();
	CompiledBlock(func code, size_t size, CompilerConfig config);
	
	func code;
	size_t size;
	CompilerConfig config;

	void operator()() const;
};