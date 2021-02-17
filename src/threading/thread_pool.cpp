#include "thread_pool.hpp"

namespace threading
{
    ThreadPool::ThreadPool()
        : ThreadPool(get_auto_thread_count())
    { }

    ThreadPool::ThreadPool(const size_t worker_count)
        : _running(true)
    {
        for (uint32_t i = 0; i < worker_count; i++)
        {
            _workers.emplace_back([&] {
                worker_routine();
            });
        }
    }

    ThreadPool::~ThreadPool()
    {
        shutdown();
    }

    void ThreadPool::schedule_job(Job&& job)
    {
        _job_queue.push(std::move(job));
    }

    void ThreadPool::shutdown()
    {
        _running = false;
        flush_job_queue();

        for (auto& worker : _workers)
        {
            worker.join();
        }

        _workers.clear();
    }

    void ThreadPool::flush_job_queue()
    {
        for (size_t i = 0; i < _workers.size(); i++)
        {
            schedule_job(Job([]{ }));
        }
    }

    void ThreadPool::worker_routine()
    {
        while (_running)
        {
            Job job = _job_queue.pop_wait();
            if (!_running) return;

            job.execute();
        }
    }

    size_t ThreadPool::get_auto_thread_count()
    {
        const uint32_t threads = std::thread::hardware_concurrency();
        return threads > 0
            ? threads
            : 8;
    }
}