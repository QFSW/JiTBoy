#pragma once

#include <stdexcept>

namespace memory
{
    template <typename T, size_t Size>
    class FixedPool
    {
    public:
        FixedPool();
        ~FixedPool();

        template <typename...Args>
        [[nodiscard]] T* get(Args&&...args);
        void release(T* ptr);

        class Handle
        {
        public:
            Handle(T* obj, FixedPool* pool);
            Handle(const Handle&) = delete;
            Handle(Handle&& handle) noexcept;
            ~Handle();

        private:
            T* _obj;
            FixedPool* _pool;
        };

        template <typename...Args>
        [[nodiscard]] Handle borrow(Args&&...args);

    private:
        std::allocator<T> _allocator;
        T* _pool;
        T* _ptr_stack[Size];
        size_t _size;
    };

    template <typename T, size_t Size>
    FixedPool<T, Size>::FixedPool()
        : _pool(_allocator.allocate(Size))
        , _size(Size)
    {
        for (size_t i = 0; i < _size; i++)
            _ptr_stack[i] = &_pool[i];
    }

    template <typename T, size_t Size>
    FixedPool<T, Size>::~FixedPool()
    {
        _allocator.deallocate(_pool, Size);
    }

    template <typename T, size_t Size>
    template <typename...Args>
    T* FixedPool<T, Size>::get(Args&&...args)
    {
        if (_size == 0) [[unlikely]]
            throw std::runtime_error("Pool empty: cannot allocate");

        T* ptr = _ptr_stack[--_size];
        new (ptr) T(std::forward<Args>(args)...);
        return ptr;
    }

    template <typename T, size_t Size>
    void FixedPool<T, Size>::release(T* ptr)
    {
        if (_size == Size) [[unlikely]]
            throw std::runtime_error("Pool full: cannot de-allocate");

        ptr->~T();
        _ptr_stack[_size++] = ptr;
    }

    template <typename T, size_t Size>
    FixedPool<T, Size>::Handle::Handle(T* obj, FixedPool* pool)
        : _obj(obj)
        , _pool(pool)
    { }

    template <typename T, size_t Size>
    FixedPool<T, Size>::Handle::Handle(Handle&& handle) noexcept
        : _obj(handle._obj)
        , _pool(handle._pool)
    {
        handle._obj = nullptr;
        handle._pool = nullptr;
    }

    template <typename T, size_t Size>
    FixedPool<T, Size>::Handle::~Handle()
    {
        if (_obj && _pool)
        {
            _pool->release(_obj);
        }
    }

    template <typename T, size_t Size>
    template <typename...Args>
    typename FixedPool<T, Size>::Handle FixedPool<T, Size>::borrow(Args&&...args)
    {
        return Handle(get(std::forward<Args>(args)...), this);
    }
}