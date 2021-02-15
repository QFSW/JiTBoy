#pragma once

#include <emulation/emulator_state.hpp>

namespace emulation
{
    class Emulator
    {
    public:
        virtual void execute(std::vector<mips::Instruction>&& code) = 0;

        virtual [[nodiscard]] EmulatorState& get_state() noexcept = 0;
        virtual [[nodiscard]] const EmulatorState& get_state() const noexcept = 0;
    };
}