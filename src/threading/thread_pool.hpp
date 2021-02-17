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
        ThreadPool(size_t worker_count);

        void schedule_job(Job&& job);

        [[nodiscard]] size_t pending_jobs() const noexcept { return _job_queue.size(); };

    private:
        size_t _worker_count;
        std::vector<std::thread> _workers;
        concurrent_queue<Job> _job_queue;
        std::atomic<bool> _working;

        void spawn_workers();
        void kill_workers();
        void flush_job_queue();
        void worker_routine();

        static size_t get_auto_thread_count();
    };
}