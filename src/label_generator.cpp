#include "label_generator.hpp"

#include <utils/strtools.hpp>

std::string LabelGenerator::generate(const std::string& name)
{
    const auto name_count = _registered_names[name]++;
    return strtools::catf("<%x>_%s", name_count, name.c_str());
}
