#pragma once

#include <stl/expected.hpp>

#include <string>
#include <string_view>

namespace toml
{
    stl::expected<void, std::vector<std::string>> parse(std::string_view text);
}

/*
Subset supported:
https://toml.io/en/v1.0.0#spec
https://toml.io/en/v1.0.0#comment
https://toml.io/en/v1.0.0#keyvalue-pair (partially)
    
https://toml.io/en/v1.0.0#keys 
- bare keys only in this format: [A-z][_A-z0-9]+
- dotted keys                  : <bare-key>.<bare-key>[.<bare-key>...]
- Defining a key multiple times is invalid.

https://toml.io/en/v1.0.0#string
- no UTF-8 escapes like \U+1234
- strings/multi-line strings/literal/multi-line literal strings, are supported

https://toml.io/en/v1.0.0#integer
https://toml.io/en/v1.0.0#boolean
https://toml.io/en/v1.0.0#array

https://toml.io/en/v1.0.0#table
- follow the same rules as #keys
- Defining a table multiple times is invalid. ([a.b] and [a] are treated as 2 tables)
- implicitly declares a root table until a [<bare-key>] table is found
- table name can't be [root]

https://toml.io/en/v1.0.0#array-of-tables
- follows the rules of #table
*/

/* build.toml SPEC

Smallest possible, a simple main.cpp executable that returns 0

    [project]
    type = "executable"

Windows/Linux project with custom configs

    [project]
    type = "executable" (or shared_library)

    [[config]]
    compiler_flags = ["-std=c++20"]

    [[config]]
    os = "Windows"
    defines = ["ICARUS_ON_WINDOWS"]

    [[config]]
    os = "Linux"
    defines = ["ICARUS_ON_LINUX"]

    [[config]]
    name = "debug"
    compiler_flags = ["/W4"]
    os = "Windows"

    [[config]]
    name = "debug"
    compiler_flags = ["-fno-omit-frame-pointer"]
    os = "Linux"

Complex project having subprojects with particular os abstractions

    /engine
        /networking
            build.toml
            /public
                /networking
                    my_header.hpp
            /private
                /networking
                    internal_header.hpp    -> always available
                /windows                   -> only for os="Windows"
                    header.hpp
                    source.cpp
                    build.toml
                /linux                     -> only for os="Linux"
                    header.hpp
                    source.cpp
                    build.toml

    # Networking
    [project]
    type = "shared_library"
    
    [[config]]
    compiler_flags = ["-std=c++20"]

    [[config]]
    os = "windows"
    defines = ["USE_WINDOWS"]

    [[config]]
    os = "linux"
    defines = ["USE_LINUX"]

        # Windows
        [subproject]
        type = "source_code"
        os   = "windows"

        # Linux
        [subproject]
        type = "source_code"
        os   = "linux"
    
    internal_header.hpp
        #if USE_WINDOWS
            #include <windows/header.hpp>
        #elif USE_LINUX
            #include <linux/header.hpp>
        #endif
*/