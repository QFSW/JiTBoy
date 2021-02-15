#include "runner_config.hpp"

namespace mips::testing
{
    RunnerConfig::Timing RunnerConfig::Timing::none()
    {
        return Timing{
            .batch_size = 1,
            .threshold = 0,
            .precision = 1,
        };
    }

    RunnerConfig::Timing RunnerConfig::Timing::fast()
    {
        return Timing{
            .batch_size = 1,
            .threshold = 2,
            .precision = 0.1,
        };
    }

    RunnerConfig::Timing RunnerConfig::Timing::accurate()
    {
        return Timing{
            .batch_size = 10,
            .threshold = 10,
            .precision = 0.01,
        };
    }

    RunnerConfig::Timing RunnerConfig::Timing::final()
    {
        return Timing{
            .batch_size = 100,
            .threshold = 10,
            .precision = 0.0001,
        };
    }
}