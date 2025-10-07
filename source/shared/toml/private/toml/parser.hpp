#pragma once

#include <toml/ast.hpp>

namespace toml::internal
{
    stl::expected<void, std::vector<std::string>> build();
}