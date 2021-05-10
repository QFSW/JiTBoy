#pragma once

#include <common/common.hpp>

namespace threading
{
    template <typename T>
    class ResultQueue
    {
    public:
        [[nodiscard]] bool expecting_results() const noexcept { return _results_expected > 0; }

        bool enqueue(T&& item);
        bool enqueue(const T& item);
        bool try_dequeue(T& item);

    private:
        common::concurrent_queue<T> _results;
        std::atomic<size_t> _results_expected;
    };

    template <typename T>
    bool ResultQueue<T>::enqueue(T&& item)
    {
        ++_results_expected;
        return _results.enqueue(std::move(item));
    }

    template <typename T>
    bool ResultQueue<T>::enqueue(const T& item)
    {
        ++_results_expected;
        return _results.enqueue(item);
    }

    template <typename T>
    bool ResultQueue<T>::try_dequeue(T& item)
    {
        if (_results.try_dequeue(item))
        {
            --_results_expected;
            return true;
        }

        return false;
    }
}