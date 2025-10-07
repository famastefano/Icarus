#pragma once

#include <stl/expected.hpp>

#include <vector>
#include <string>
#include <string_view>

namespace toml::internal
{
    stl::expected<void, std::vector<std::string>> build_ast(std::string_view text);
}