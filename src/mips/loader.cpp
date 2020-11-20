#include "loader.hpp"

#include <utils/io.hpp>
#include <mips/encoding.hpp>

namespace mips
{
    std::vector<Instruction> Loader::load_assembly(const std::string& filepath) const
    {
        const std::string raw = io::read_text_file(filepath);
        return _parser.parse_instructions(raw);
    }

    std::vector<Instruction> Loader::load_binary(const std::string& filepath) const
    {
        const std::vector<uint8_t> raw = io::read_binary_file(filepath);
        if (raw.size() % 4 > 0)
            throw std::runtime_error("MIPS instruction binary must be word aligned");

        std::vector<Instruction> result;
        result.reserve(raw.size() / 4);

        // Interpret byte vector

        throw std::logic_error("Binary loader not implemented");
    }
}
