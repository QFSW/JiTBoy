#include <utils/io.hpp>

#include <fstream>
#include <sstream>

namespace io
{
    std::string read_text_file(const std::string& filepath)
    {
        const std::ifstream file(filepath);
        if (!file.is_open())
            throw std::runtime_error("Could not open file " + filepath);

        std::stringstream buf;
        buf << file.rdbuf();

        return buf.str();
    }

    std::vector<std::filesystem::path> get_files_recursive(const std::string& dir_path)
    {
        std::vector<std::filesystem::path> files;
        for (const auto& entry : std::filesystem::recursive_directory_iterator(dir_path))
        {
            if (entry.is_regular_file())
            {
                files.push_back(entry.path());
            }
        }

        return files;
    }
}
