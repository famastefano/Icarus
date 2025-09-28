#pragma once

#include <test_framework/matchers/specializations/matcher_base.hpp>

namespace test_framework::matchers
{
    template <typename T>
    constexpr bool is_signed()
    {
        return specialization::matcher<T>::current->is_signed();
    }

#define can_be_represented_as(type_to_check) can_be_represented_as_impl<decltype(_m)::type, type_to_check>()

    template <typename T, std::integral U>
    constexpr bool can_be_represented_as_impl()
    {
        return specialization::matcher<T>::current->can_be_represented_as_impl<U>();
    }

    template <typename T, std::floating_point F>
    bool can_be_represented_as_impl()
    {
        return specialization::matcher<T>::current->can_be_represented_as_impl<F>();
    }

    /* overflow/underflow are checked on the same function */

    template <typename T>
    constexpr bool overflows_adding(T value)
    {
        return specialization::matcher<T>::current->overflows_adding(std::forward<T>(value));
    }

    template <typename T>
    constexpr bool overflows_subtracting(T value)
    {
        return specialization::matcher<T>::current->overflows_subtracting(std::forward<T>(value));
    }

    template <typename T>
    constexpr bool overflows_multiplying_by(T value)
    {
        return specialization::matcher<T>::current->overflows_multiplying_by(std::forward<T>(value));
    }

    template <typename T>
    constexpr bool overflows_dividing_by(T value)
    {
        return specialization::matcher<T>::current->overflows_dividing_by(std::forward<T>(value));
    }
}