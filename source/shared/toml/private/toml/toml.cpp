#include <toml/toml.hpp>
#include <toml/parser.hpp>

namespace toml
{
    stl::expected<void, std::vector<std::string>> parse(std::string_view text)
    {
        return internal::build_ast(text).and_then(internal::build);
    }
}