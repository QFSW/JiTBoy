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
}
