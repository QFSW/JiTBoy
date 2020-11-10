#include "loader.hpp"

#include <stdexcept>

#include <mips/encoding.hpp>
#include <mips/parser.hpp>

namespace mips
{
    std::vector<Instruction> Loader::load_assembly(const std::string& filepath)
    {
        throw std::logic_error("Assembly loader not implemented");
    }

    std::vector<Instruction> Loader::load_binary(const std::string& filepath)
    {
        throw std::logic_error("Binary loader not implemented");
    }
}
