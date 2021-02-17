#pragma once

#include <vector>
#include <thread>

#include <threading/job.hpp>
#include <threading/concurrent_queue.hpp>

namespace threading
{
    class ThreadPool
    {
    public:
        ThreadPool();
        explicit ThreadPool(size_t worker_count);
        ~ThreadPool();

        void schedule_job(Job&& job);
        void shutdown();

        [[nodiscard]] bool running() const noexcept { return _running; }
        [[nodiscard]] size_t pending_jobs() const noexcept { return _job_queue.size(); };

    private:
        std::vector<std::thread> _workers;
        concurrent_queue<Job> _job_queue;
        std::atomic<bool> _running;

        void flush_job_queue();
        void worker_routine();

        static size_t get_auto_thread_count();
    };
}