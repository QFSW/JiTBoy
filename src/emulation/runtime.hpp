#pragma once

#include <config.hpp>
#include <emulation/runtime_config.hpp>
#include <emulation/emulator_state.hpp>
#include <emulation/compiler.hpp>
#include <emulation/jump_resolver.hpp>
#include <emulation/emulator.hpp>

namespace emulation
{
    class Runtime final : public Emulator
    {
    public:
        using Config = RuntimeConfig;
        Runtime();
        Runtime(Config config);

        void execute(mips::Program&& program) override;
        void load_source(mips::Program&& program);
        void execute(uint32_t addr);

        [[nodiscard]] EmulatorState& get_state() noexcept override { return _state; }
        [[nodiscard]] const EmulatorState& get_state() const noexcept override { return _state; }
        [[nodiscard]] const common::unordered_map<uint32_t, CompiledBlock>& get_blocks() const noexcept { return _blocks; }
        [[nodiscard]] std::string get_debug() const;
        [[nodiscard]] std::string get_debug_with_dumps() const;

    private:
        Config _config;
        EmulatorState _state;
        Compiler _compiler;
        JumpResolver _jump_resolver;
        common::unordered_map<uint32_t, CompiledBlock> _blocks;

        static constexpr bool debug = config::debug;
        std::stringstream _debug_stream;

        [[nodiscard]] SourceBlock partition_block(uint32_t addr) const;
        [[nodiscard]] const CompiledBlock& get_or_compile_block(uint32_t addr);
    };
}
