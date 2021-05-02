#pragma once

#include <emulation/emulator_state.hpp>
#include <mips/program.hpp>

namespace emulation
{
    class Emulator
    {
    public:
        virtual void execute(mips::Program&& program) = 0;

        virtual [[nodiscard]] EmulatorState& get_state() noexcept = 0;
        virtual [[nodiscard]] const EmulatorState& get_state() const noexcept = 0;
    };
}