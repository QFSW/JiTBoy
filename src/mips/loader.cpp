#include "loader.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

#include <mips/encoding.hpp>
#include <mips/parser.hpp>

namespace mips
{
    std::vector<Instruction> Loader::load_assembly(const std::string& filepath)
    {
        const std::ifstream file(filepath);
        std::stringstream buf;
        buf << file.rdbuf();

        return Parser::parse_instructions(buf.str());
    }

    std::vector<Instruction> Loader::load_binary(const std::string& filepath)
    {
        throw std::logic_error("Binary loader not implemented");
    }
}
