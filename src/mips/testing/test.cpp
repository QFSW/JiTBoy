#include "test.hpp"

#include <utils/csv.hpp>

template<> void csv::write_header<mips::testing::Test>(std::ostream& o)
{
    o << "name, description, src";
}

template<> void csv::write_row<mips::testing::Test>(std::ostream& o, const mips::testing::Test& test)
{
    o
    << test.name << ", "
    << test.description << ", "
    << test.src << ", ";
}