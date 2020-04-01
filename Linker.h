#pragma once

#include <unordered_map>
#include <functional>

class Linker
{
public:
	void label(const std::string& name, uint32_t current);
	
	[[nodiscard]] int32_t resolve_begin(const std::string& name, uint32_t current) const;
	void resolve_end(const std::string& name, uint32_t current);
	
	void resolve(const std::string& name,
				 const std::function<uint32_t()> &get_current,
		         const std::function<void(int32_t offset)> &body);

	void terminate_local(uint8_t* block_addr);

	[[nodiscard]] const std::unordered_map<std::string, uint8_t*>& global_map() const;
	
private:
	std::unordered_map<int32_t, std::string> _unresolved_locals;
	std::unordered_map<std::string, int32_t> _local_map;
	std::unordered_map<std::string, uint8_t*> _global_map;
};

