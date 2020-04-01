#pragma once

#include <unordered_map>

class Linker
{
public:
	void label(const std::string& name, uint32_t current);
	[[nodiscard]] int32_t resolve_rel(const std::string& name, uint32_t current) const;

	void terminate_local(uint8_t* block_addr);

	[[nodiscard]] const std::unordered_map<std::string, uint8_t*>& global_map() const;
	
private:
	std::unordered_map<std::string, int32_t> _local_map;
	std::unordered_map<std::string, uint8_t*> _global_map;
};

