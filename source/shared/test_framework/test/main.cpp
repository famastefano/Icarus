#include <test_framework/test_framework.hpp>

#include <format>
#include <iostream>
#include <functional>

#include <cmath>

float test_sqrt(float in)
{
    TEST_THAT(in, equals_to(0) || greater_than(0));
    if (in < 0)
        return NAN;

    return std::sqrt(in);
}

int failed_count = 0;
int total_runs = 0;
struct run_results
{
    bool triggered;
    char const *suite_name;
    char const *test_expression;
    char const *function;
    char const *file_name;
    int line;
} last_run{};
void handler(char const *suite_name, char const *test_expression, char const *function, char const *file_name, int line)
{
    last_run.triggered = true;
    last_run.suite_name = suite_name;
    last_run.test_expression = test_expression;
    last_run.function = function;
    last_run.file_name = file_name;
    last_run.line = line;
}

template <typename... Args>
void do_test(bool should_trigger, Args &&...args)
{
    last_run = {};
    std::invoke(std::forward<Args>(args)...);
    ++total_runs;
    if (should_trigger ^ last_run.triggered)
    {
        ++failed_count;
        if (last_run.triggered)
        {
            std::cerr << std::format(
                "TEST {} FAILED. Expected to {}.\n"
                "\t{} {}\n"
                "\t{}:{}\n",
                total_runs,
                should_trigger ? "trigger" : "not trigger",
                last_run.function, last_run.test_expression,
                last_run.file_name, last_run.line);
        }
        else
        {
            std::cerr << std::format("TEST {} FAILED. Expected to {}.\n",
                                     total_runs,
                                     should_trigger ? "trigger" : "not trigger");
        }
    }
}

int main()
{
    test_framework::set_handler(handler, test_framework::handler_scope::global);
    do_test(false, test_sqrt, 0.f);
    do_test(false, test_sqrt, 2.f);
    do_test(false, test_sqrt, -0.f);
    do_test(true, test_sqrt, NAN);
    do_test(false, test_sqrt, INFINITY);
    do_test(true, test_sqrt, -INFINITY);
    if (failed_count > 0)
    {
        std::cout << std::format("Total {} Passed {} Failed {} ({:.2f}%)\n", total_runs, total_runs - failed_count, failed_count, float(failed_count) / total_runs * 100.f);
    }
    return failed_count;
}