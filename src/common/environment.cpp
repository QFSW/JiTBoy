#include "environment.hpp"

namespace common
{
    Environment& Environment::get()
    {
        static Environment env;
        return env;
    }
}
