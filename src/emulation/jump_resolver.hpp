#pragma once

#include <common/common.hpp>
#include <emulation/conditional_jump.hpp>
#include <emulation/unconditional_jump.hpp>
#include <emulation/compiled_block.hpp>
#include <x86/assembler.hpp>

namespace emulation
{
    class JumpResolver
    {
    public:
        void resolve_jumps(CompiledBlock& block, const common::unordered_map<uint32_t, CompiledBlock>& blocks);
        [[nodiscard]] std::string get_debug() const;

    private:
        x86::Assembler _assembler;

        static constexpr bool debug = config::debug;
        std::stringstream _debug_stream;

        bool resolve_jump(UnconditionalJump& jump, const common::unordered_map<uint32_t, CompiledBlock>& blocks);
        bool resolve_jump(ConditionalJump& jump, const common::unordered_map<uint32_t, CompiledBlock>& blocks);
    };
}
