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
        const std::vector<uint32_t> raw = io::read_binary_file<uint32_t, io::Endianness::Big>(filepath);

        std::vector<Instruction> result;
        result.reserve(raw.size());

        for (const auto word : raw)
            result.push_back(decode_instruction(word));

        return result;
    }
}
