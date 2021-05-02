#pragma once

#include <mutex>

#include <common/common.hpp>
#include <threading/thread_pool.hpp>
#include <threading/worker_job.hpp>

namespace threading
{
    template <typename Worker>
    class WorkerPool
    {
    public:
        using Factory = std::function<Worker()>;

        explicit WorkerPool(ThreadPool* thread_pool);
        explicit WorkerPool(ThreadPool* thread_pool, Factory factory);
        ~WorkerPool();

        void schedule_job(WorkerJob<Worker>&& job);
        void shutdown();

        [[nodiscard]] bool running() const noexcept { return _running; }
        [[nodiscard]] size_t pending_jobs() const noexcept { return _pending_jobs; }

    private:
        ThreadPool* _thread_pool;
        Factory _factory;
        std::atomic<bool> _running;
        std::atomic<bool> _shutdown_done;
        std::atomic<size_t> _pending_jobs;
        std::mutex _mutex;
        std::condition_variable _shutdown_cond;
    };

    template <typename Worker>
    WorkerPool<Worker>::WorkerPool(ThreadPool* thread_pool)
        : WorkerPool(thread_pool, [] { return Worker(); })
    { }

    template <typename Worker>
    WorkerPool<Worker>::WorkerPool(ThreadPool* thread_pool, Factory factory)
        : _thread_pool(thread_pool)
        , _factory(factory)
        , _running(true)
        , _shutdown_done(false)
        , _pending_jobs(0)
    { }

    template <typename Worker>
    WorkerPool<Worker>::~WorkerPool()
    {
        shutdown();
    }

    template <typename Worker>
    void WorkerPool<Worker>::schedule_job(WorkerJob<Worker>&& job)
    {
        ++_pending_jobs;
        _thread_pool->schedule_job(Job([this, job=std::move(job)]
        {
            if (_running)
            {
                Worker worker = _factory();
                if (_running) job.execute(worker);
            }
            
            if (--_pending_jobs == 0 && !_running)
            {
                {
                    std::lock_guard<std::mutex> lock(_mutex);
                    _shutdown_done = true;
                }
                _shutdown_cond.notify_one();
            }
        }));
    }

    template <typename Worker>
    void WorkerPool<Worker>::shutdown()
    {
        _running = false;

        while (pending_jobs())
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _shutdown_cond.wait(lock, [&] { return _shutdown_done.load(); });
        }
    }
}