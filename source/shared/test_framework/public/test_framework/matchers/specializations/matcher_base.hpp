#pragma once

#include <cstddef>

namespace test_framework::matchers::specialization
{
    template<typename>
    struct matcher;

    template<typename T>
    struct matcher_base
    {
        using type = T;
        inline static thread_local matcher<T>* current{};
        constexpr matcher_base()
        {
            current = static_cast<matcher<T>*>(this);
        }
    };

    template<typename>
    struct matcher
    {
        static_assert(false, "matcher specialization not found.");
    };
}
