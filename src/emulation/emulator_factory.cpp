#include "emulator_factory.hpp"

#include <emulation/runtime.hpp>
#include <emulation/interpreter.hpp>
#include <emulation/hybrid_runtime.hpp>
#include <utils/strtools.hpp>

namespace emulation
{
    Emulator* EmulatorFactory::create_from_str(const std::string& str)
    {
        if (str.starts_with("jit"))         return create_jit_from_str(str);
        if (str.starts_with("interpreter")) return create_interpreter_from_str(str);
        if (str.starts_with("hybrid"))      return create_hybrid_from_str(str);

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
