#pragma once
#include <string>
#include <sstream>
#include <unordered_map>

class LabelGenerator
{
public:
	[[nodiscard]] std::string generate(const std::string &name);

private:
	std::unordered_map<std::string, size_t> _registered_names;
};

