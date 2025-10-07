call "C:\Program Files\Microsoft Visual Studio\2022\Preview\VC\Auxiliary\Build\vcvars64.bat"

:: TOML Parser
cl @bootstrap.cl ^
    /Fo: "build/bootstrap/obj/toml/" ^
    /I source/shared/stl/public ^
    /I source/shared/toml/public ^
    /I source/shared/toml/private ^
    /c source/shared/toml/private/toml/ast.cpp ^
       source/shared/toml/private/toml/parser.cpp ^
       source/shared/toml/private/toml/toml.cpp
lib /OUT:"build/bootstrap/lib/toml/toml.lib" build/bootstrap/obj/toml/*.obj

:: :: Tests
::cl @bootstrap.cl /Fe: "build/bootstrap/exe/test_framework" ^
::    /I source/shared/test_framework/public ^
::    source/shared/test_framework/test/main.cpp ^
::    build/bootstrap/lib/test_framework.lib ^
::    /link /SUBSYSTEM:console /DEBUG /PDB:"build/bootstrap/exe/test_framework.pdb"

:: JSON
:: cl @bootstrap.cl /c /I source/shared/test_framework/public source/shared/json/test/json_test.cpp

::   Runner
:: cl @bootstrap.cl /Fe: "build/bootstrap/exe/json" /I source/shared/test_framework/public /I source/shared/test_framework/private source/shared/test_framework/private/main.cpp build/bootstrap/obj/test_framework.obj build/bootstrap/obj/json_test.obj /link /SUBSYSTEM:console /DEBUG /PDB:"build/bootstrap/exe/json.pdb"