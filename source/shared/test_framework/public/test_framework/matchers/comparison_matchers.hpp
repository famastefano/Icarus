#pragma once

#include <test_framework/matchers/specializations/matcher_base.hpp>

namespace test_framework::matchers
{
    template <typename T>
    constexpr bool equals_to(T&& value)
    {
        return specialization::matcher<T>::current->equals_to(std::forward<T>(value));
    }

    template <typename T>
    constexpr bool greater_than(T&& value)
    {
        return specialization::matcher<T>::current->greater_than(std::forward<T>(value));
    }

    template <typename T>
    constexpr bool less_than(T&& value)
    {
        return specialization::matcher<T>::current->less_than(std::forward<T>(value));
    }

    template <typename T>
    constexpr bool is_between(T&& a, T&& b)
    {
        return specialization::matcher<T>::current->is_between(std::forward<T>(a), std::forward<T>(b));
    }
}