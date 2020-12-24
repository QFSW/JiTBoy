#pragma once

#include <chrono>

namespace benchmark
{
    template <typename F>
    std::chrono::duration<double> measure(F&& f, const size_t count = 1)
    {
        const auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < count; i++)
        {
            f();
        }
        const auto finish = std::chrono::high_resolution_clock::now();

        return (finish - start) / count;
    }
}