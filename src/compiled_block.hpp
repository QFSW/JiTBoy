#pragma once

#include <cstdint>
#include <compiler_config.hpp>

struct CompiledBlock
{
    typedef uint32_t(__fastcall *func)();
    CompiledBlock();
    CompiledBlock(func code, size_t size, CompilerConfig config);
    
    func code;
    size_t size;
    CompilerConfig config;

    uint32_t operator()() const;
};
