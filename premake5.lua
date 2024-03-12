include "./vendor/premake/premake_customisation/solution_items.lua"
include "Dependencies.lua"

workspace "SFMLProject"
    architecture "x64"
    startproject "SFMLProject"

    configurations
    {
        "Debug",
        "Release"
    }

    solution_items
    {
        ".editorconfig"
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "vendor/premake"
group""

include "SFMLProject"