#pragma once

#include <emulation/emulator.hpp>

#include <memory>

namespace emulation
{
    class EmulatorFactory
    {
    public:
        static Emulator* create_from_str(const std::string& str);

    private:
        static Emulator* create_jit_from_str(const std::string& str);
        static Emulator* create_interpreter_from_str(const std::string& str);
        static Emulator* create_hybrid_from_str(const std::string& str);
    };
}
