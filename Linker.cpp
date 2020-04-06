#include "Linker.h"
#include <limits>
#include <stdexcept>

void Linker::label(const std::string& name, const uint32_t current)
{
	_local_map[name] = current;
}

void Linker::label_global(const std::string& name, void* addr)
{
	_global_map[name] = reinterpret_cast<uint8_t*>(addr);
}

int32_t Linker::resolve_begin(const std::string& name, const uint32_t current) const
{
	if (_local_map.find(name) == _local_map.end())
	{
		return std::numeric_limits<int32_t>::max();
	}
	
	return _local_map.at(name) - current;
}

void Linker::resolve_end(const std::string& name, const uint32_t current)
{
	if (_local_map.find(name) == _local_map.end())
	{
		_unresolved_locals[current] = name;
	}
}

void Linker::resolve(const std::string& name,
					 const std::function<uint32_t()> &get_current,
                     const std::function<void(int32_t offset)> &body)
{
	const auto offset = resolve_begin(name, get_current());
	body(offset);
	resolve_end(name, get_current());
}

const std::unordered_map<std::string, uint8_t*>& Linker::global_map() const
{
	return _global_map;
}

void Linker::terminate_local(uint8_t* block_addr)
{
	for (const auto& [label, offset] : _local_map)
	{
		_global_map[label] = block_addr + offset;
	}
	
	for (const auto& [offset_end, label] : _unresolved_locals)
	{
		auto* end_ptr = block_addr + offset_end;
		auto* offset_ptr = reinterpret_cast<int32_t*>(end_ptr - sizeof(int32_t));
		
		auto global_label = _global_map.find(label);
		if (global_label == _global_map.end())
		{
			throw std::runtime_error("Unable to resolve symbol " + label);
		}

		*offset_ptr = global_label->second - end_ptr;
	}

	_unresolved_locals.clear();
	_local_map.clear();
}
