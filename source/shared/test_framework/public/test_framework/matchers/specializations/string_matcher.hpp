#pragma once

#include <test_framework/matchers/specializations/matcher_base.hpp>

#include <string>
#include <string_view>
#include <concepts>

namespace test_framework::matchers::specialization
{
    template <typename T>
    concept string =
        std::same_as<T, std::string> || std::same_as<T, std::wstring> || std::same_as<T, std::string_view> || std::same_as<T, std::wstring_view>;

    template <string T>
    struct matcher<T> : public matcher_base<T>
    {
        T& s;

        using view = std::basic_string_view<typename T::CharT, typename T::traits_type>;

        explicit matcher(T& str) : s(str) {}

        constexpr bool equals_to(view str)
        {
            return s == str;
        }

        constexpr bool starts_with(view str)
        {
            return view(s).starts_with(str);
        }

        constexpr bool ends_with(view str)
        {
            return view(s).ends_with(str);
        }

        constexpr bool contains(view str)
        {
            return view(s).find(str) != view::npos;
        }
    };
}