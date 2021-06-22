#include "emulator_factory.hpp"

#include <emulation/runtime.hpp>
#include <emulation/interpreter.hpp>
#include <emulation/hybrid_runtime.hpp>
#include <utils/strtools.hpp>

#include <regex>

namespace emulation
{
    Emulator* EmulatorFactory::create_from_str(const std::string& str)
    {
        if (str.starts_with("jit"))         return create_jit_from_str(str);
        if (str.starts_with("interpreter")) return create_interpreter_from_str(str);
        if (str.starts_with("hybrid"))      return create_hybrid_from_str(str);

        throw std::runtime_error(strtools::catf("Cannot parse emulator config %s", str.c_str()));
    }

    Emulator* EmulatorFactory::create_jit_from_str(const std::string& str)
    {
        Runtime::Config config;
        if (strtools::str_contains(str, "-L")) config.direct_linking = true;

        return new Runtime(config);
    }

    Emulator* EmulatorFactory::create_interpreter_from_str(const std::string& str)
    {
        Interpreter::Config config;

        return new Interpreter(config);
    }

    Emulator* EmulatorFactory::create_hybrid_from_str(const std::string& str)
    {
        HybridRuntime::Config config;
        if (strtools::str_contains(str, "-L")) config.direct_linking = true;
        if (strtools::str_contains(str, "-S")) config.speculative_compilation = true;

        std::smatch matches;
        if (std::regex_search(str, matches, std::regex(".*-T([0-9]+)")))
        {
            config.compilation_threshold = std::stoi(matches[1]);
        }

        return new HybridRuntime(config);
    }
}
