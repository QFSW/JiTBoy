#include "LabelGenerator.h"

std::string LabelGenerator::generate(const std::string &name)
{
    const auto name_count = _registered_names[name]++;

    std::stringstream ss;
    ss << "<" << std::hex << name_count << std::dec << ">_" << name;
    return ss.str();
}
