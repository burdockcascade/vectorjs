set_project("vectorjs")
set_description("VectorJS")
set_version("0.0.1") -- x-release-please-version
set_languages("cxx23")

add_rules("mode.debug", "mode.release")

add_requires("cli11 2.6.2")
add_requires("raylib 5.5")
add_requires("quickjs-ng 0.13.0")

task("compile-runtime")
    on_run(function ()
        os.execv("npx.cmd", {"esbuild", "src/runtime/runtime.js", "--bundle", "--target=es2023", "--format=esm", "--outfile=src/runtime/dist.js"})
        os.execv("qjsc", {"-s", "-s", "-n", "runtime.js", "-N", "qjsc_runtime", "-C", "-m", "-o", "src/host/runtime.hpp", "src/runtime/dist.js"})
        print("JavaScript runtime compiled to src/app/runtime.h")
    end)
    set_menu {
        usage = "xmake compile-runtime",
        description = "Compile runtime.js to a C header using qjsc"
    }

target("vectorjs")
    set_kind("binary")
    add_files("src/host/*.cpp")
    add_packages("cli11", "raylib", "quickjs-ng")
