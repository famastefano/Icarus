#include <toml/ast.hpp>

namespace toml::internal
{
    stl::expected<void, std::vector<std::string>> make_ast(std::string_view text)
    {
        return std::vector<std::string>{"Unimplemented."};
    }
}