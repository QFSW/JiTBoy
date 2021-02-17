#include "job.hpp"

namespace threading
{
    Job::Job(std::function<void()> f)
        : _f(f)
    { }

    void Job::execute()
    {
        _f();
    }
}