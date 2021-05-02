#include "emulator_state.hpp"

namespace emulation
{
    bool EmulatorState::valid_pc() const noexcept
    {
        return program.valid_addr(pc);
    }
}