#pragma once

#include <cstdint>
#include <compiler_config.hpp>
#include <config.hpp>

struct CompiledBlock
{
    typedef uint32_t(CALLING_CONV *func)();
    CompiledBlock();
    CompiledBlock(func code, size_t size, CompilerConfig config);
    
    func code;
    size_t size;
    CompilerConfig config;

    uint32_t operator()() const;
};
