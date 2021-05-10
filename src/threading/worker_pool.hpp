#pragma once

#include <mutex>
#include <exception>

#include <common/common.hpp>
#include <threading/thread_pool.hpp>
#include <threading/worker_job.hpp>
#include <utils/utils.hpp>

namespace threading
{
    template <typename Worker>
    class WorkerPool
    {
    public:
        using Factory = std::function<std::unique_ptr<Worker>()>;

        explicit WorkerPool(ThreadPool* thread_pool);
        explicit WorkerPool(ThreadPool* thread_pool, Factory factory);
        ~WorkerPool();

        void schedule_job(WorkerJob<Worker>&& job);
        void handle_exceptions(const std::function<void(std::exception_ptr)>& handler);
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
        common::concurrent_queue<std::exception_ptr> _exceptions;
        common::concurrent_queue<std::unique_ptr<Worker>> _workers;

        std::unique_ptr<Worker> borrow_worker();
        void release_worker(std::unique_ptr<Worker>&& worker);
        void invoke_job(const WorkerJob<Worker>& job);
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
    std::unique_ptr<Worker> WorkerPool<Worker>::borrow_worker()
    {
        std::unique_ptr<Worker> worker;
        if (!_workers.try_dequeue(worker))
            worker = _factory();

        return worker;
    }

    template <typename Worker>
    void WorkerPool<Worker>::release_worker(std::unique_ptr<Worker>&& worker)
    {
        _workers.enqueue(std::move(worker));
    }

    template <typename Worker>
    void WorkerPool<Worker>::schedule_job(WorkerJob<Worker>&& job)
    {
        ++_pending_jobs;
        _thread_pool->schedule_job(Job([this, job=std::move(job)]
        {
            invoke_job(job);
            
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
    void WorkerPool<Worker>::invoke_job(const WorkerJob<Worker>& job)
    {
        if (_running)
        {
            std::unique_ptr<Worker> worker = borrow_worker();
            auto _ = utils::finally([&] { release_worker(std::move(worker)); });

            if (_running)
            {
                try
                {
                    job.execute(*worker);
                }
                catch (...)
                {
                    _exceptions.enqueue(std::current_exception());
                }
            }
        }
    }

    template <typename Worker>
    void WorkerPool<Worker>::handle_exceptions(const std::function<void(std::exception_ptr)>& handler)
    {
        std::exception_ptr eptr;
        while (_exceptions.try_dequeue(eptr))
        {
            handler(eptr);
        }
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