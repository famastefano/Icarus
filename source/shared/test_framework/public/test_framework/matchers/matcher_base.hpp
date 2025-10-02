#pragma once

#include <cstddef>

namespace test_framework::matchers
{
    template<typename>
    struct matcher
    {
        static_assert(false, "matcher specialization not found.");
    };
}
