#include "loader.hpp"

#include <utils/io.hpp>
#include <mips/encoding.hpp>

namespace mips
{
    Program Loader::load_assembly(const std::string& filepath)
    {
        const std::string raw = io::read_text_file(filepath);
        return _parser.parse_program(raw);
    }

    Program Loader::load_binary(const std::string& filepath)
    {
        const std::vector<uint32_t> raw = io::read_binary_file<uint32_t, io::Endianness::Big>(filepath);

        const uint32_t start_addr = 0;
        uint32_t addr = start_addr;
        common::unordered_map<uint32_t, Instruction> source;


        for (const auto word : raw)
        {
            source[addr] = decode_instruction(word);
            addr += 4;
        }

        return Program(std::move(source), start_addr);
    }

    Program Loader::load_auto(const std::string& filepath)
    {
        const std::filesystem::path path = filepath;
        const auto ext = path.extension();

        if (ext == ".s")   return load_assembly(filepath);
        if (ext == ".bin") return load_binary(filepath);
        if (ext == "")     return load_binary(filepath);
        throw std::runtime_error(strtools::catf("%s files are not supported", ext.generic_string().c_str()));
    }
}
