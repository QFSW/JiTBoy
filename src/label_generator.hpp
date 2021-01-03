#pragma once

#include <string>

#include <common/common.hpp>

class LabelGenerator
{
public:
    [[nodiscard]] std::string generate(const std::string& name);

private:
    common::unordered_map<std::string, size_t> _registered_names;
};

