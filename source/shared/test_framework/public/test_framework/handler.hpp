#pragma once

#include <source_location>

namespace test_framework
{
    using test_handler_t = void(*)(char const* suite_name, char const* test_expression, char const* function, char const* file_name, int line);
    enum class handler_scope
    {
        thread,
        global,
    };
    test_handler_t get_default_handler();
    test_handler_t get_handler(handler_scope scope);
    test_handler_t set_handler(test_handler_t new_handler, handler_scope scope);

    void trigger_test_handler(char const* suite_name, char const* test_expression, std::source_location loc = std::source_location::current());
}
