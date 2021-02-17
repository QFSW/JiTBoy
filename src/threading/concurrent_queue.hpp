#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace threading
{
    template <typename T>
    class concurrent_queue
    {
    public:
        [[nodiscard]] size_t size() const noexcept;
        [[nodiscard]] bool empty() const noexcept;

        void push(const T& item);
        void push(T&& item);
        T pop_wait();

    private:
        std::atomic<size_t> _size = 0;
        std::queue<T> _queue;
        std::mutex _mutex;
        std::condition_variable _cond;
    };

    template <typename T>
    size_t concurrent_queue<T>::size() const noexcept
    {
        return _size;
    }

    template <typename T>
    bool concurrent_queue<T>::empty() const noexcept
    {
        return size() == 0;
    }

    template <typename T>
    void concurrent_queue<T>::push(const T& item)
    {
        push(T(item));
    }

    template <typename T>
    void concurrent_queue<T>::push(T&& item)
    {
        {
            std::lock_guard lock(_mutex);
            _queue.push(std::move(item));
            _size++;
        }

        _cond.notify_one();
    }

    template <typename T>
    T concurrent_queue<T>::pop_wait()
    {
        std::unique_lock lock(_mutex);
        _cond.wait(lock, [&] { return !empty(); });

        T item = _queue.front();
        _queue.pop();
        _size--;

        return item;
    }
}