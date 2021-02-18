#pragma once

namespace memory
{
    template <typename T, typename Target>
    class PoolHandle
    {
    public:
        PoolHandle(T* obj, Target* target);
        PoolHandle(const PoolHandle&) = delete;
        PoolHandle(PoolHandle&& handle) noexcept;
        ~PoolHandle();

    private:
        T* _obj;
        Target* _target;
    };

    template <typename T, typename Target>
    PoolHandle<T, Target>::PoolHandle(T* obj, Target* target)
        : _obj(obj)
        , _target(target)
    { }

    template <typename T, typename Target>
    PoolHandle<T, Target>::PoolHandle(PoolHandle&& handle) noexcept
        : _obj(handle._obj)
        , _target(handle._target)
    {
        handle._obj = nullptr;
        handle._target = nullptr;
    }

    template <typename T, typename Target>
    PoolHandle<T, Target>::PoolHandle::~PoolHandle()
    {
        if (_obj && _target)
        {
            _target->release(_obj);
        }
    }
}
