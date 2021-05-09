#pragma once

#include <threading/thread_pool.hpp>

namespace common
{
    class Environment
    {
    public:
        threading::ThreadPool& thread_pool() { return _thread_pool; }

        Environment(const Environment&)    = delete;
        Environment(Environment&&)         = delete;
        void operator=(const Environment&) = delete;
        void operator=(Environment&&)      = delete;

        static Environment& get();

    private:
        Environment()  = default;
        ~Environment() = default;

        threading::ThreadPool _thread_pool;
    };
}
