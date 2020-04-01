#include "Linker.h"

void Linker::label(const std::string& name, const uint32_t current)
{
	_local_map[name] = current;
}

int32_t Linker::resolve_rel(const std::string& name, const uint32_t current) const
{
	return _local_map.at(name) - current;
}

void Linker::terminate_local(uint8_t* block_addr)
{
	for (const auto& label : _local_map)
	{
		_global_map[label.first] = block_addr + label.second;
	}

	_local_map.clear();
}
