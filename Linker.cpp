#include "Linker.h"
#include "limits"

void Linker::label(const std::string& name, const uint32_t current)
{
	_local_map[name] = current;
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
	for (const auto& label : _local_map)
	{
		_global_map[label.first] = block_addr + label.second;
	}
	
	for (const auto& unresolved : _unresolved_locals)
	{
		auto* end_ptr = block_addr + unresolved.first;
		auto* offset_ptr = reinterpret_cast<int32_t*>(end_ptr - sizeof(int32_t));
		
		auto label_local = _local_map.find(unresolved.second);
		if (label_local != _local_map.end())
		{
			*offset_ptr = label_local->second - unresolved.first;
		}
		else
		{
			auto label_global = _global_map.find(unresolved.second);
			if (label_global == _global_map.end())
			{
				throw "Unresolved symbol";
			}

			*offset_ptr = label_global->second - end_ptr;
		}
	}

	_unresolved_locals.clear();
	_local_map.clear();
}
