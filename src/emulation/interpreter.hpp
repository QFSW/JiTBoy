#pragma once

#include <sstream>

#include <config.hpp>
#include <mips/program.hpp>
#include <emulation/interpreter_config.hpp>
#include <emulation/interpreter_core.hpp>
#include <emulation/emulator_state.hpp>
#include <emulation/emulator.hpp>

namespace emulation
{
    class Interpreter final : public Emulator
    {
    public:
        using Config = InterpreterConfig;
        Interpreter();
        Interpreter(Config config);

        void execute(mips::Program&& program) override;
        void load_source(mips::Program&& program);
        void execute(uint32_t addr);

        [[nodiscard]] EmulatorState& get_state() noexcept override { return _state; }
        [[nodiscard]] const EmulatorState& get_state() const noexcept override { return _state; }
        [[nodiscard]] std::string get_debug() const;
        [[nodiscard]] std::string get_debug_with_dumps() const;
        [[nodiscard]] size_t get_instruction_count() const noexcept { return _executed_instructions; }

    private:
        Config _config;
        EmulatorState _state;
        InterpreterCore _core;
        size_t _executed_instructions;

        static constexpr bool debug = config::debug;
        std::stringstream _debug_stream;
    };
}