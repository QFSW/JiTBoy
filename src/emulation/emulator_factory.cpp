#include "emulator_factory.hpp"


#include <iostream>
#include <emulation/runtime.hpp>
#include <emulation/interpreter.hpp>
#include <emulation/hybrid_runtime.hpp>

namespace emulation
{
    Emulator* EmulatorFactory::create_from_str(const std::string& str)
    {
        return create_jit_from_str(str);
    }

    Emulator* EmulatorFactory::create_jit_from_str(const std::string& str)
    {
        return new Runtime();
    }

    Emulator* EmulatorFactory::create_interpreter_from_str(const std::string& str)
    {
        return new Interpreter();
    }

    Emulator* EmulatorFactory::create_hybrid_from_str(const std::string& str)
    {
        return new HybridRuntime();
    }
}
