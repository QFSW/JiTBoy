#include "interpreter.hpp"

#include <mips/utils.hpp>

namespace emulation
{
    using namespace mips;

    Interpreter::Interpreter()
        : Interpreter(Config())
    { }

    Interpreter::Interpreter(Config config)
        : _config(config)
        , _core(_state)
        , _executed_instructions(0)
    { }

    void Interpreter::load_source(Program&& program)
    {
        _state.program = std::move(program);
    }

    void Interpreter::execute(Program&& program)
    {
        load_source(std::move(program));
        execute(_state.program.entry_point());
    }

    void Interpreter::execute(const uint32_t addr)
    {
        _state.pc = addr;
        while (_state.valid_pc())
        {
            _core.execute_current();
            _executed_instructions++;
        }
    }

    std::string Interpreter::get_debug() const
    {
        return _debug_stream.str();
    }

    std::string Interpreter::get_debug_with_dumps() const
    {
        std::stringstream ss;
        ss << get_debug() << "\n" << _state.regs.generate_dump() << "\n";
        return ss.str();
    }
}
