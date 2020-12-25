#pragma once

#include <vector>
#include <map>
#include <memory>

#include <utils/strtools.hpp>
#include <memory/executable_allocator.hpp>

namespace memory
{
    template <size_t PartitionSize = win_page_size>
    class DynamicExecutableAllocator
    {
    public:
        uint8_t* alloc(size_t size);
        void commit(void* buffer, size_t size);

        [[nodiscard]] size_t get_used() const noexcept;
        [[nodiscard]] size_t get_free() const noexcept;
        [[nodiscard]] size_t get_total() const noexcept;

    private:
        using Allocator = ExecutableAllocator<PartitionSize>;
        std::vector<std::unique_ptr<Allocator>> _allocators;
        std::map<void*, Allocator*> _allocations;
    };

    template <size_t PartitionSize>
    uint8_t* DynamicExecutableAllocator<PartitionSize>::alloc(const size_t size)
    {
        if (size > PartitionSize)
            throw std::runtime_error(strtools::catf("Cannot make a single allocation of size %d, max is %d", size, PartitionSize));

        Allocator* viable = nullptr;
        for (auto& allocator : _allocators)
        {
            if (allocator->get_free() >= size)
            {
                viable = allocator.get();
                break;
            }
        }

        if (!viable)
        {
            auto allocator = std::make_unique<Allocator>();
            viable = allocator.get();
            _allocators.push_back(std::move(allocator));
        }

        uint8_t* allocation = viable->alloc(size);
        _allocations[static_cast<void*>(allocation)] = viable;

        return allocation;
    }

    template <size_t PartitionSize>
    void DynamicExecutableAllocator<PartitionSize>::commit(void* buffer, const size_t size)
    {
        _allocations[buffer]->commit(buffer, size);
    }

    template <size_t PartitionSize>
    size_t DynamicExecutableAllocator<PartitionSize>::get_used() const noexcept
    {
        size_t used = 0;
        for (const auto& allocator : _allocators)
            used += allocator->get_used();

        return used;
    }

    template <size_t PartitionSize>
    size_t DynamicExecutableAllocator<PartitionSize>::get_free() const noexcept
    {
        size_t free = 0;
        for (const auto& allocator : _allocators)
            free += allocator->get_free();

        return free;
    }

    template <size_t PartitionSize>
    size_t DynamicExecutableAllocator<PartitionSize>::get_total() const noexcept
    {
        return _allocators.size() * PartitionSize;
    }
}