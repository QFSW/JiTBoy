#pragma once

#include <functional>

#include <common/common.hpp>

namespace x86
{
    class Linker
    {
    public:
        void label(const std::string& name, uint32_t current);
        void label_global(const std::string& name, void* addr);

        [[nodiscard]] int32_t resolve_begin(const std::string& name, uint32_t current) const;
        void resolve_end(const std::string& name, uint32_t current);

        void resolve(const std::string& name,
            const std::function<uint32_t()>& get_current,
            const std::function<void(int32_t offset)>& body);

        void terminate_local(uint8_t* block_addr);

        [[nodiscard]] const common::unordered_map<std::string, uint8_t*>& global_map() const;

    private:
        common::unordered_map<int32_t, std::string> _unresolved_locals;
        common::unordered_map<std::string, int32_t> _local_map;
        common::unordered_map<std::string, uint8_t*> _global_map;
    };
}

