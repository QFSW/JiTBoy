#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <stdexcept>

namespace io
{
    std::string read_text_file(const std::string& filepath);
    std::vector<std::filesystem::path> get_files_recursive(const std::string& dir_path);

    template <typename T = uint8_t>
    std::vector<T> read_binary_file(const std::string& filepath)
    {
        throw std::logic_error("io::read_binary_file not implemented");
    }
}