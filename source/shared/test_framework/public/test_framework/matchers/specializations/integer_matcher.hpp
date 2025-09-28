#pragma once

#include <test_framework/matchers/specializations/matcher_base.hpp>

#include <cstddef>
#include <concepts>
#include <cmath>
#include <bit>
#include <limits>

namespace test_framework::matchers::specialization
{
    template <std::integral T>
    struct matcher<T> : public matcher_base<T>
    {
        using limits = std::numeric_limits<T>;

        T v;

        explicit matcher(T value) : v(value) {}

        constexpr bool equals_to(T value)
        {
            return v == value;
        }

        constexpr bool greater_than(T value)
        {
            return v > value;
        }

        constexpr bool less_than(T value)
        {
            return v < value;
        }

        constexpr bool is_between(T a, T b)
        {
            return a <= v && v <= b;
        }

        constexpr bool is_signed()
        {
            return limits::is_signed;
        }

        template <std::integral Dst>
        constexpr bool can_be_represented_as_impl()
        {
            using Src = T;
            using src_limits = limits;
            using dst_limits = std::numeric_limits<Dst>;

            if constexpr (src_limits::is_signed() == dst_limits::is_signed())
            {
                return v >= static_cast<Src>(dst_limits::min()) &&
                       v <= static_cast<Src>(dst_limits::max());
            }
            else if constexpr (src_limits::is_signed() && !dst_limits::is_signed())
            {
                if (v < 0)
                    return false;
                using U = std::make_unsigned_t<Src>;
                return static_cast<U>(v) <= dst_limits::max();
            }
            else
            {
                using U = std::make_unsigned_t<Dst>;
                return v <= static_cast<U>(dst_limits::max());
            }
        }

        template <std::floating_point Dst>
        bool can_be_represented_as_impl()
        {
            if constexpr (!std::same_as<long double, Dst>)
            {
                constexpr auto p = std::same_as<double, Dst> ? 53 : 24;
                constexpr auto max_representable = 1ull << p;
                const std::make_unsigned_t<T> abs_value = std::abs(v);
                if (abs_value <= max_representable)
                    return true;

                // ULP = 2^e-(p-1)
                auto e = std::bit_width(abs_value) - 1;
                auto ULP = 1ull << (e - (p - 1));
                return abs_value % ULP == 0;
            }
            else
            {
                static_assert(sizeof(T) <= 8, "function doesn't support integers larger than 64 bits.");
                return true;
            }
        }

        /* overflow/underflow are checked on the same function */

        constexpr bool overflows_adding(T b)
        {
            if constexpr (is_signed())
            {
                if (b > 0 && v > limits::max() - b)
                    return true;
                if (b < 0 && v < limits::min() - b)
                    return true;
                return false;
            }
            else // unsigned
            {
                return v > limits::max() - b;
            }
        }

        constexpr bool overflows_subtracting(T b)
        {
            if constexpr (is_signed())
            {
                if (b > 0 && v < limits::min() + b)
                    return true;
                if (b < 0 && v > limits::max() + b)
                    return true;
                return false;
            }
            else // unsigned
            {
                return v < b;
            }
        }

        constexpr bool overflows_multiplying_by(T b)
        {
            if (v == 0 || b == 0)
                return false;

            if constexpr (is_signed())
            {
                if (v == -1 && b == limits::min())
                    return true;
                if (b == -1 && a == limits::min())
                    return true;

                if (v > 0)
                {
                    if (b > 0)
                        return v > limits::max() / b;
                    if (b < 0)
                        return b < limits::min() / v;
                }
                else // v < 0
                {
                    if (b > 0)
                        return v < limits::min() / b;
                    if (b < 0)
                        return v < limits::max() / b;
                }
                return false;
            }
            else // unsigned
            {
                return v > limits::max() / b;
            }
        }

        constexpr bool overflows_dividing_by(T b)
        {
            if constexpr (is_signed())
            {
                return (v == limits::min() && b == -1);
            }
            else // unsigned
            {
                return false;
            }
        }
    };
}