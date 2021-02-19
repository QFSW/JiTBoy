#pragma once

#include <threading/thread_pool.hpp>

namespace common
{
    class Environment
    {
    public:
        threading::ThreadPool& thread_pool() { return _thread_pool; }

        static Environment& get();

    private:
        Environment() = default;

        threading::ThreadPool _thread_pool;
    };
}
