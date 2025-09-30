#pragma once

#include <test_framework/matchers/specializations/matcher_base.hpp>

#include <cstddef>
#include <cmath>
#include <concepts>
#include <limits>
#include <bit>

namespace test_framework::matchers::specialization
{
    template <std::floating_point T>
    struct matcher<T> : public matcher_base<T>
    {
        using limits = std::numeric_limits<T>;

        T v;

        explicit matcher(T value) : v(value) {}

        constexpr bool equals_to(T value)
        {
            return !std::isunordered(v, value) && v == value;
        }

        constexpr bool greater_than(T value)
        {
            return std::isgreater(v, value);
        }

        constexpr bool less_than(T value)
        {
            return std::isless(v, value);
        }

        constexpr bool is_signed()
        {
            return limits::is_signed;
        }

        constexpr bool is_between(T a, T b)
        {
            //                 a <= v                &&             v <= b
            return (equals_to(a) || greater_than(a)) && (equals_to(b) || less_than(b));
        }

        template <std::integral Dst>
        constexpr bool can_be_represented_as_impl()
        {
            using dst_limits = std::numeric_limits<Dst>;
            if (!dst_limits::is_signed() && std::signbit(v))
                return false;

            return has_exact_integer_representation() && is_between(dst_limits::min(), dst_limits::max());
        }

        template <std::floating_point Dst>
        constexpr bool can_be_represented_as_impl()
        {
            using Src = T;
            using dst_limits = std::numeric_limits<Dst>;
            if constexpr (sizeof(Src) <= sizeof(Dst))
                return true;
            else
                return is_between(dst_limits::min(), dst_limits::max());
        }

        constexpr bool is_nan()
        {
            return std::isnan(v);
        }

        constexpr bool is_inf()
        {
            return std::isinf(v) && !std::signbit(v);
        }

        constexpr bool is_negative_inf()
        {
            return std::isinf(v) && std::signbit(v);
        }

        constexpr bool has_exact_integer_representation()
        {
            if (!std::isfinite(v))
                return false;

            const auto truncated = std::trunc(v);
            return truncated == v;
        }

        constexpr bool equals_to_using_ULP(T value, unsigned int ULP)
        {
            using UInt = std::conditional_t<std::is_same_v<T, float>, uint32_t, uint64_t>;
            auto to_ordered = [](T x) constexpr
            {
                const UInt bits = std::bit_cast<UInt>(x);
                // If sign bit set, flip ordering so negatives come before positives
                if (bits & (UInt(1) << (sizeof(T) * CHAR_BIT - 1)))
                    return ~bits + 1;
                return bits | (UInt(1) << (sizeof(T) * CHAR_BIT - 1));
            };
            const UInt a = to_ordered(v);
            const UInt b = to_ordered(value);
            const UInt diff = (a > b ? a - b : b - a);
            return diff <= ULP;
        }

        constexpr bool equals_to_using_relative_error(T value, unsigned int error)
        {
            T diff = v > value ? v - value : value - v;
            // relative error = N * epsilon * max(|a|, |b|)
            T rel_error = error * limits::epsilon() *
                          (std::fabs(v) > std::fabs(value) ? std::fabs(v) : std::fabs(value));
            return diff <= error;
        }
    };
}