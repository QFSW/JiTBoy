#pragma once

#include <functional>

namespace threading
{
    template <typename Worker>
    class WorkerJob
    {
    public:
        WorkerJob(std::function<void(Worker&)> f);

        void execute(Worker& worker) const;
        static WorkerJob empty();

    private:
        std::function<void(Worker&)> _f;
    };

    template <typename Worker>
    WorkerJob<Worker>::WorkerJob(std::function<void(Worker&)> f)
        : _f(f)
    { }

    template <typename Worker>
    void WorkerJob<Worker>::execute(Worker& worker) const
    {
        _f(worker);
    }

    template <typename Worker>
    WorkerJob<Worker> WorkerJob<Worker>::empty()
    {
        return WorkerJob([](Worker&) {});
    }
}