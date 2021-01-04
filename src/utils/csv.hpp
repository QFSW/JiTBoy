#pragma once

#include <fstream>
#include <vector>

#include <utils/io.hpp>

namespace csv
{
    template <typename T>
    void write_header(std::ostream& o);

    template <typename T>
    void write_row(std::ostream& o, const T& t);

    template <typename T>
    void write_table(std::ostream& o, const std::vector<T>& rows)
    {
        write_header<T>(o);
        for (const auto& row : rows)
        {
            o << "\n";
            write_row(o, row);
        }
    }

    template <typename T>
    void write_file(const std::string& filepath, const std::vector<T>& rows)
    {
        std::ofstream file;

        io::create_directories_for_file(filepath);

        file.open(filepath);
        if (!file.is_open())
            throw std::runtime_error("Could not open file " + filepath);

        write_table(file, rows);
        file.close();
    }
}