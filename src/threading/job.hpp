#pragma once

#include <functional>

namespace threading
{
    class Job
    {
    public:
        explicit Job(std::function<void()> f);

        void execute() const;

    private:
        std::function<void()> _f;
    };
}