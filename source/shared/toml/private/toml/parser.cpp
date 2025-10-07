#pragma once

#include <toml/parser.hpp>

namespace toml::internal
{
    stl::expected<void, std::vector<std::string>> build()
    {
        return std::vector<std::string>{"Unimplemented."};
    }
}