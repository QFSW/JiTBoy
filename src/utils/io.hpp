#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <stdexcept>
#include <fstream>

#include <utils/utils.hpp>

namespace io
{
    enum class Endianness
    {
        Little,
        Big
    };

    std::string read_text_file(const std::string& filepath);
    std::vector<std::filesystem::path> get_files_recursive(const std::string& dir_path);

    void create_directories_for_file(const std::string& filepath);

    template <typename T = uint8_t, Endianness Endian = Endianness::Little>
    std::vector<T> read_binary_file(const std::string& filepath)
    {
        std::ifstream file;
        file.open(filepath, std::ios_base::binary | std::ios_base::in | std::ios_base::ate);
        if (!file.is_open())
            throw std::runtime_error("Could not open file " + filepath);

        const auto size = static_cast<size_t>(file.tellg());
        if (size % sizeof(T) > 0)
            throw std::runtime_error("File contained non integral number of words");

        file.seekg(0, std::ios::beg);

        const auto word_count = size / sizeof(T);
        std::vector<T> words(word_count);
        file.read(reinterpret_cast<char*>(words.data()), size);

        if constexpr (Endian == Endianness::Big)
        {
            for (auto& word : words)
                word = utils::bswap(word);
        }

        return words;
    }
}