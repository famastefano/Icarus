#pragma once

#include <test_framework/matchers/specializations/matcher_base.hpp>

namespace test_framework::matchers
{
    template <typename T>
    constexpr bool starts_with(T const &str)
    {
        return specialization::matcher<T>::current->starts_with(str);
    }

    template <typename T>
    constexpr bool ends_with(T const &str)
    {
        return specialization::matcher<T>::current->ends_with(str);
    }

    template <typename T>
    constexpr bool contains(T const &str)
    {
        return specialization::matcher<T>::current->contains(str);
    }
}