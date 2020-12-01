#include "loader.hpp"

#include <utils/io.hpp>
#include <mips/encoding.hpp>

namespace mips
{
    std::vector<Instruction> Loader::load_assembly(const std::string& filepath)
    {
        const std::string raw = io::read_text_file(filepath);
        return _parser.parse_instructions(raw);
    }

    std::vector<Instruction> Loader::load_binary(const std::string& filepath)
    {
        const std::vector<uint32_t> raw = io::read_binary_file<uint32_t, io::Endianness::Big>(filepath);

        std::vector<Instruction> result;
        result.reserve(raw.size());

        for (const auto word : raw)
            result.push_back(decode_instruction(word));

        return result;
    }

    std::vector<Instruction> Loader::load_auto(const std::string& filepath)
    {
        const std::filesystem::path path = filepath;
        const auto ext = path.extension();

        if (ext == ".s")   return load_assembly(filepath);
        if (ext == ".bin") return load_binary(filepath);
        if (ext == "")     return load_binary(filepath);
        throw std::runtime_error(strtools::catf("%s files are not supported", ext.generic_string().c_str()));
    }
}
