#pragma once

#undef REGISTER_TEST_SUITE
#undef TEST
#undef TEST_FALSE
#undef TEST_THAT

#define REGISTER_TEST_SUITE(Name) \
    inline static constexpr char const *_suite_name = #Name

#define TEST(Expr) \
    if (!(Expr))   \
    test_framework::trigger_test_handler(_suite_name, "(" #Expr ")")

#define TEST_FALSE(Expr) \
    if ((Expr))          \
    test_framework::trigger_test_handler(_suite_name, "!(" #Expr ")")

#define TEST_THAT(Expr, Matchers)                                                            \
    {                                                                                        \
        test_framework::matchers::matcher<decltype(Expr)> _m{(Expr)};                        \
        if (!(Matchers))                                                                     \
            test_framework::trigger_test_handler(_suite_name, "!(" #Expr " " #Matchers ")"); \
    }