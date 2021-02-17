#include "thread_pool.hpp"

namespace threading
{
    ThreadPool::ThreadPool()
        : ThreadPool(get_auto_thread_count())
    { }

    ThreadPool::ThreadPool(size_t worker_count)
        : _worker_count(worker_count)
        , _working(false)
    {
        spawn_workers();
    }

    void ThreadPool::schedule_job(Job&& job)
    {
        _job_queue.push(std::move(job));
    }

    void ThreadPool::spawn_workers()
    {
        _working = true;
        for (uint32_t i = 0; i < _worker_count; i++)
        {
            _workers.push_back(std::thread([&] {
                worker_routine();
            }));
        }
    }

    void ThreadPool::kill_workers()
    {
        _working = false;
        flush_job_queue();

        for (size_t i = 0; i < _workers.size(); i++)
        {
            _workers[i].join();
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
        while (_working)
        {
            Job job = _job_queue.pop_wait();
            if (!_working) return;

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