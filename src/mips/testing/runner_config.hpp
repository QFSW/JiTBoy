#pragma once

namespace mips::testing
{
    struct RunnerConfig
    {
        struct Timing
        {
            size_t batch_size = 1;
            size_t threshold = 1;
            double precision = 1;

            static Timing none();
            static Timing fast();
            static Timing accurate();
            static Timing final();
        } timing;
    };
}