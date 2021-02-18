#pragma once

#include <vector>
#include <thread>

#include <common/common.hpp>
#include <threading/job.hpp>

namespace threading
{
    template <typename Worker>
    class ThreadPool
    {
    public:
        using Factory = std::function<Worker()>;

        explicit ThreadPool(size_t worker_count = get_auto_thread_count());
        explicit ThreadPool(Factory factory, size_t worker_count = get_auto_thread_count() - 1);
        ~ThreadPool();

        void schedule_job(Job<Worker>&& job);
        void shutdown();

        [[nodiscard]] bool running() const noexcept { return _running; }
        [[nodiscard]] size_t pending_jobs() const noexcept { return _job_queue.size(); };

    private:
        const size_t _max_workers;
        Factory _factory;
        std::vector<std::thread> _workers;
        common::blocking_concurrent_queue<Job<Worker>> _job_queue;
        std::atomic<bool> _running;
        std::atomic<size_t> _busy_workers;

        void flush_job_queue();
        void create_worker();
        void worker_routine();

        static size_t get_auto_thread_count();
    };

    template <typename Worker>
    ThreadPool<Worker>::ThreadPool(const size_t worker_count)
        : ThreadPool([] { return Worker(); }, worker_count)
    { }

    template <typename Worker>
    ThreadPool<Worker>::ThreadPool(Factory factory, const size_t worker_count)
        : _max_workers(worker_count)
        , _factory(factory)
        , _running(true)
        , _busy_workers(0)
    {
        _workers.reserve(_max_workers);
    }

    template <typename Worker>
    ThreadPool<Worker>::~ThreadPool()
    {
        shutdown();
    }

    template <typename Worker>
    void ThreadPool<Worker>::schedule_job(Job<Worker>&& job)
    {
        _job_queue.enqueue(std::move(job));

        if (_running
        && _busy_workers == _workers.size()
        && _workers.size() < _max_workers)
            create_worker();
    }

    template <typename Worker>
    void ThreadPool<Worker>::shutdown()
    {
        _running = false;
        flush_job_queue();

        for (auto& worker : _workers)
        {
            worker.join();
        }

        _workers.clear();
    }

    template <typename Worker>
    void ThreadPool<Worker>::flush_job_queue()
    {
        for (size_t i = 0; i < _workers.size(); i++)
        {
            schedule_job(Job<Worker>::empty());
        }
    }

    template <typename Worker>
    void ThreadPool<Worker>::create_worker()
    {
        _workers.emplace_back([&] {
            worker_routine();
        });
    }

    template <typename Worker>
    void ThreadPool<Worker>::worker_routine()
    {
        Worker worker = _factory();
        Job<Worker> job = Job<Worker>::empty();

        while (_running)
        {
            _job_queue.wait_dequeue(job);
            if (!_running) return;

            ++_busy_workers;
            job.execute(worker);
            --_busy_workers;
        }
    }

    template <typename Worker>
    size_t ThreadPool<Worker>::get_auto_thread_count()
    {
        const uint32_t threads = std::thread::hardware_concurrency();
        return threads > 0
            ? threads
            : 8;
    }
}