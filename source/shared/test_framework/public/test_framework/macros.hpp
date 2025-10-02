#pragma once

#undef REGISTER_TEST_SUITE
#undef TEST
#undef TEST_FALSE
#undef TEST_THAT
#undef TEST_SUITE
#undef TEST_SUITE_FALSE
#undef TEST_SUITE_THAT

#include <test_framework/handler.hpp>

#define REGISTER_TEST_SUITE(Name) \
    inline static constexpr char const *_suite_name = #Name

#define ICARUS_TEST_IMPL(Suite, Expr) \
    if (!(Expr))                      \
    ::test_framework::trigger_test_handler(Suite, "(" #Expr ")")

#define ICARUS_TEST_FALSE_IMPL(Suite, Expr) \
    if ((Expr))                             \
    ::test_framework::trigger_test_handler(Suite, "!(" #Expr ")")

#define ICARUS_TEST_THAT_IMPL(Suite, Expr, Matchers) \
    if (![&] {\
    using matcher_class = ::test_framework::matchers::matcher<std::remove_reference_t<decltype(Expr)>>; \
class impl : protected matcher_class \
{\
public:\
    using matcher_class::matcher_class;\
    bool operator()() { return (Matchers); }\
} _{(Expr)}; return _(); }())                                  \
    ::test_framework::trigger_test_handler(Suite, "!(" #Expr " " #Matchers ")")

#define TEST(Expr) ICARUS_TEST_IMPL("", (Expr))
#define TEST_FALSE(Expr) ICARUS_TEST_FALSE_IMPL("", (Expr))
#define TEST_THAT(Expr, Matchers) ICARUS_TEST_THAT_IMPL("", (Expr), (Matchers))

#define TEST_SUITE(Expr) ICARUS_TEST_IMPL(_suite_name, (Expr))
#define TEST_SUITE_FALSE(Expr) ICARUS_TEST_FALSE_IMPL(_suite_name, (Expr))
#define TEST_SUITE_THAT(Expr, Matchers) ICARUS_TEST_THAT_IMPL(_suite_name, (Expr), (Matchers))
