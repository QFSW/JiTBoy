#pragma once

#include <functional>

namespace threading
{
    class Job
    {
    public:
        Job(std::function<void()> f);

        void execute();

    private:
        std::function<void()> _f;
    };
}