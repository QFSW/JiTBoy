#include "jump_resolver.hpp"

#include <emulation/compiler.hpp>
#include <x86/utils.hpp>

namespace emulation
{
    void JumpResolver::resolve_jumps(CompiledBlock& block, const common::unordered_map<uint32_t, CompiledBlock>& blocks)
    {
        if constexpr (debug)
        {
            std::stringstream().swap(_debug_stream);
        }

        auto& unresolved_jumps = block.unresolved_jumps;
        for (int i = unresolved_jumps.size() - 1; i >= 0; i--)
        {
            if (resolve_jump(unresolved_jumps[i], blocks))
            {
                unresolved_jumps.erase(unresolved_jumps.begin() + i);
            }
        }

        auto& unresolved_cond_jumps = block.unresolved_cond_jumps;
        for (int i = unresolved_cond_jumps.size() - 1; i >= 0; i--)
        {
            if (resolve_jump(unresolved_cond_jumps[i], blocks))
            {
                unresolved_cond_jumps.erase(unresolved_cond_jumps.begin() + i);
            }
        }
    }

    bool JumpResolver::resolve_jump(UnconditionalJump& jump, const common::unordered_map<uint32_t, CompiledBlock>& blocks)
    {
        if (const auto it = blocks.find(jump.dst_mips); it != blocks.end())
        {
            const auto& target_block = it->second;
            uint8_t* dst = reinterpret_cast<uint8_t*>(target_block.code);
            int32_t offset = static_cast<int32_t>(dst - jump.src_x86);

            _assembler.reset();
            _assembler.jump<x86::JumpAdjust::Always>(offset);
            _assembler.copy(jump.src_x86);

            jump.dst_x86 = dst;

            if constexpr (debug)
            {
                _debug_stream << strtools::catf("Directly linked jump to 0x%x (%p -> %p)\n", jump.dst_mips, jump.src_x86, dst);
            }

            return true;
        }

        return false;
    }

    bool JumpResolver::resolve_jump(ConditionalJump& jump, const common::unordered_map<uint32_t, CompiledBlock>& blocks)
    {
        const auto it_true = blocks.find(jump.dst_true_mips);
        const auto it_false = blocks.find(jump.dst_false_mips);

        if (!jump.dst_true_x86 && !jump.dst_false_x86)
        {
            if (it_true != blocks.end() && it_false == blocks.end())
            {
                const auto& target_true_block = it_true->second;
                uint8_t* dst_true = reinterpret_cast<uint8_t*>(target_true_block.code);
                int32_t offset_true = static_cast<int32_t>(dst_true - jump.src_x86);

                _assembler.reset();
                _assembler.jump_cond<x86::JumpAdjust::Always>(jump.cond, offset_true);
                _assembler.instr_imm<x86::Opcode::MOV_I, x86::OpcodeExt::MOV_I>(Compiler::return_reg, jump.dst_false_mips);
                _assembler.instr<x86::Opcode::RET>();
                _assembler.copy(jump.src_x86);

                jump.dst_true_x86 = dst_true;

                if constexpr (debug)
                {
                    _debug_stream << strtools::catf("Partially linked conditional jump to 0x%x/0x%x (%p -> %p/???)\n"
                        , jump.dst_true_mips, jump.dst_false_mips, jump.src_x86, dst_true);
                }
            }
            else if (it_true == blocks.end() && it_false != blocks.end())
            {
                const auto& target_false_block = it_false->second;
                uint8_t* dst_false = reinterpret_cast<uint8_t*>(target_false_block.code);
                int32_t offset_false = static_cast<int32_t>(dst_false - jump.src_x86);

                _assembler.reset();
                _assembler.jump_cond<x86::JumpAdjust::Always>(x86::utils::negate_cond(jump.cond), offset_false);
                _assembler.instr_imm<x86::Opcode::MOV_I, x86::OpcodeExt::MOV_I>(Compiler::return_reg, jump.dst_true_mips);
                _assembler.instr<x86::Opcode::RET>();
                _assembler.copy(jump.src_x86);

                jump.dst_false_x86 = dst_false;

                if constexpr (debug)
                {
                    _debug_stream << strtools::catf("Partially linked conditional jump to 0x%x/0x%x (%p -> ???/%p)\n"
                        , jump.dst_true_mips, jump.dst_false_mips, jump.src_x86, dst_false);
                }
            }
        }

        if (it_true != blocks.end() && it_false != blocks.end())
        {
            const auto& target_true_block = it_true->second;
            const auto& target_false_block = it_false->second;
            uint8_t* dst_true = reinterpret_cast<uint8_t*>(target_true_block.code);
            uint8_t* dst_false = reinterpret_cast<uint8_t*>(target_false_block.code);
            int32_t offset_true = static_cast<int32_t>(dst_true - jump.src_x86);
            int32_t offset_false = static_cast<int32_t>(dst_false - jump.src_x86);

            _assembler.reset();
            _assembler.jump_cond<x86::JumpAdjust::Always>(jump.cond, offset_true);
            _assembler.jump<x86::JumpAdjust::Always>(offset_false - _assembler.size());
            _assembler.copy(jump.src_x86);

            jump.dst_true_x86 = dst_true;
            jump.dst_false_x86 = dst_false;

            if constexpr (debug)
            {
                _debug_stream << strtools::catf("Directly linked conditional jump to 0x%x/0x%x (%p -> %p/%p)\n"
                    , jump.dst_true_mips, jump.dst_false_mips, jump.src_x86, dst_true, dst_false);
            }

            return true;
        }

        return false;
    }

    std::string JumpResolver::get_debug() const
    {
        return _debug_stream.str();
    }
}