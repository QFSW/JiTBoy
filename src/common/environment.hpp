#pragma once

namespace common
{
    class Environment
    {
    public:
        static Environment& get();

    private:
        Environment() = default;
    };
}
