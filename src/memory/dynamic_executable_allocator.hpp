#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>

#include <utils/strtools.hpp>
#include <memory/executable_allocator.hpp>

namespace memory
{
    template <size_t PartitionSize = win_page_size, bool StackAlloc = true>
    class DynamicExecutableAllocator
    {
    public:
        DynamicExecutableAllocator();

        uint8_t* alloc(size_t size);
        void commit(void* buffer, size_t size);

        [[nodiscard]] size_t get_used() const noexcept;
        [[nodiscard]] size_t get_free() const noexcept;
        [[nodiscard]] size_t get_total() const noexcept;

    private:
        using Allocator = ExecutableAllocator<PartitionSize>;
        using FixedAllocator = ExecutableAllocator<StackAlloc ? PartitionSize : 0>;

        FixedAllocator _fixed_allocator;
        std::unordered_set< std::unique_ptr<Allocator>> _dynamic_allocators;
        std::vector<Allocator*> _allocators;
        std::unordered_map<void*, Allocator*> _allocations;
    };

    template <size_t PartitionSize, bool StackAlloc>
    DynamicExecutableAllocator<PartitionSize, StackAlloc>::DynamicExecutableAllocator()
    {
        if constexpr (StackAlloc)
            _allocators.push_back(&_fixed_allocator);
    }

    template <size_t PartitionSize, bool StackAlloc>
    uint8_t* DynamicExecutableAllocator<PartitionSize, StackAlloc>::alloc(const size_t size)
    {
        if (size > PartitionSize)
            throw std::runtime_error(strtools::catf("Cannot make a single allocation of size %d, max is %d", size, PartitionSize));

        Allocator* viable = nullptr;
        for (auto& allocator : _allocators)
        {
            if (allocator->get_free() >= size)
            {
                viable = allocator;
                break;
            }
        }

        if (!viable)
        {
            auto allocator = std::make_unique<Allocator>();
            viable = allocator.get();
            _dynamic_allocators.emplace(std::move(allocator));
            _allocators.push_back(viable);
        }

        uint8_t* allocation = viable->alloc(size);
        _allocations[static_cast<void*>(allocation)] = viable;

        return allocation;
    }

    template <size_t PartitionSize, bool StackAlloc>
    void DynamicExecutableAllocator<PartitionSize, StackAlloc>::commit(void* buffer, const size_t size)
    {
        _allocations[buffer]->commit(buffer, size);
    }

    template <size_t PartitionSize, bool StackAlloc>
    size_t DynamicExecutableAllocator<PartitionSize, StackAlloc>::get_used() const noexcept
    {
        size_t used = 0;
        for (const auto& allocator : _allocators)
            used += allocator->get_used();

        return used;
    }

    template <size_t PartitionSize, bool StackAlloc>
    size_t DynamicExecutableAllocator<PartitionSize, StackAlloc>::get_free() const noexcept
    {
        size_t free = 0;
        for (const auto& allocator : _allocators)
            free += allocator->get_free();

        return free;
    }

    template <size_t PartitionSize, bool StackAlloc>
    size_t DynamicExecutableAllocator<PartitionSize, StackAlloc>::get_total() const noexcept
    {
        return _allocators.size() * PartitionSize;
    }
}