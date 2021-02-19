#pragma once

#include <vector>
#include <thread>

#include <common/common.hpp>
#include <threading/job.hpp>

namespace threading
{
    class ThreadPool
    {
    public:
        explicit ThreadPool(size_t worker_count = get_auto_thread_count());
        ThreadPool(const ThreadPool&) = delete;
        ThreadPool(ThreadPool&&) = delete;
        ~ThreadPool();

        void schedule_job(Job&& job);
        void shutdown();

        [[nodiscard]] bool running() const noexcept { return _running; }

    private:
        const size_t _max_workers;
        std::vector<std::thread> _workers;
        common::blocking_concurrent_queue<Job> _job_queue;
        std::atomic<bool> _running;
        std::atomic<size_t> _busy_workers;

        void flush_job_queue();
        void create_worker();
        void worker_routine();

        static size_t get_auto_thread_count();
    };
}