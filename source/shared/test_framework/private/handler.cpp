#include <test_framework/handler.hpp>
#include <iostream>
#include <format>
#include <utility>

namespace test_framework
{
    test_handler_t global_handler = get_default_handler();
    static thread_local test_handler_t thread_handler = nullptr;

    void default_handler(char const* suite_name, char const* test_expression, char const* function, char const* file_name, int line)
    {
        std::cerr << std::format(
            "{} - TEST FAILED\n"
            "     {}\n"
            "     {} {}:{}\n",
        suite_name, test_expression, function, file_name, line);
    }

    test_handler_t get_default_handler()
    {
        return default_handler;
    }
    
    test_handler_t get_handler(handler_scope scope)
    {
        if(handler_scope::thread == scope)
            return thread_handler;
        return global_handler;
    }

    test_handler_t set_handler(test_handler_t new_handler, handler_scope scope)
    {
        if(handler_scope::thread == scope)
            return std::exchange(thread_handler, new_handler);
        return std::exchange(global_handler, new_handler);
    }

    void trigger_test_handler(char const* suite_name, char const* test_expression, std::source_location loc)
    {
        if(auto handler = get_handler(handler_scope::thread))
            return handler(suite_name, test_expression, loc.function_name(), loc.file_name(), loc.line());

        if(auto handler = get_handler(handler_scope::global))
            return handler(suite_name, test_expression, loc.function_name(), loc.file_name(), loc.line());
    }
}
