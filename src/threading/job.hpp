#pragma once

#include <functional>

namespace threading
{
    template <typename Worker>
    class Job
    {
    public:
        Job(std::function<void(Worker&)> f);

        void execute(Worker& worker) const;

    private:
        std::function<void(Worker&)> _f;
    };

    template <typename Worker>
    Job<Worker>::Job(std::function<void(Worker&)> f)
        : _f(f)
    { }

    template <typename Worker>
    void Job<Worker>::execute(Worker& worker) const
    {
        _f(worker);
    }
}