set_runtimes("MT")
add_rules("mode.debug", "mode.release")

includes("@builtin/xpack")

local SDK_PATH = "sdk"

target("cvar-unhide-s2-citadel")
    set_kind("shared")

    add_files("main.cpp", "cvars.cpp")

    add_files(
        SDK_PATH .. "/interfaces/interfaces.cpp",
        SDK_PATH .. "/tier1/convar.cpp",
        SDK_PATH .. "/tier1/generichash.cpp"
    )

    add_links(
        SDK_PATH .. "/lib/public/win64/2015/libprotobuf.lib",
        SDK_PATH .. "/lib/public/win64/tier0.lib",
        SDK_PATH .. "/lib/public/win64/tier1.lib",
        SDK_PATH .. "/lib/public/win64/interfaces.lib",
        SDK_PATH .. "/lib/public/win64/mathlib.lib"
    )

    add_includedirs(
        SDK_PATH,
        SDK_PATH .. "/thirdparty/protobuf-3.21.8/src",
        SDK_PATH .. "/common",
        SDK_PATH .. "/game/shared",
        SDK_PATH .. "/game/server",
        SDK_PATH .. "/public",
        SDK_PATH .. "/public/engine",
        SDK_PATH .. "/public/mathlib",
        SDK_PATH .. "/public/tier0",
        SDK_PATH .. "/public/tier1",
        SDK_PATH .. "/public/entity2",
        SDK_PATH .. "/public/game/server"
    )

    add_defines(
        "CreateInterface=_CreateInterface",
        "COMPILER_MSVC",
        "COMPILER_MSVC64",
        "PLATFORM_64BITS",
        "X64BITS",
        "WIN32",
        "WINDOWS",
        "CRT_SECURE_NO_WARNINGS",
        "CRT_SECURE_NO_DEPRECATE",
        "CRT_NONSTDC_NO_DEPRECATE",
        "_MBCS"
   )

   set_languages("cxx20")

xpack("cvar-unhide-s2-citadel")
    set_formats("zip")
    add_installfiles(
    	"build/windows/x64/release/cvar-unhide-s2-citadel.dll",
    	{ filename = "server.dll", prefixdir = "cvar_unlocker/bin/win64" }
    )
    add_installfiles("README.md")
    add_installfiles("gameinfo.gi")
