cmake_minimum_required(VERSION 3.28)
project(vectorjs VERSION 0.1.0 LANGUAGES CXX)

# Require C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

option(ENABLE_MSVC_WIN_MAIN "Hide the console window for MSVC builds" OFF)

# -----------------------------------------------------------------------------
# 1. Dependencies
# -----------------------------------------------------------------------------

# CLI11 (Header-only CLI parser)
FetchContent_Declare(
        cli11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG v2.7.2
)
set(CLI11_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CLI11_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# raylib (Windowing, input, and graphics library)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 6.0
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

# qjs
FetchContent_Declare(
        qjs
        GIT_REPOSITORY https://github.com/quickjs-ng/quickjs
        GIT_TAG v0.16.1
)

# Bring all dependencies into the build tree automatically
FetchContent_MakeAvailable(cli11 raylib qjs)

# -----------------------------------------------------------------------------
# 2. Executable Target
# -----------------------------------------------------------------------------

# Define the target executable
add_executable(vectorjs)

# Add source files to the target
target_sources(vectorjs PRIVATE
        "src/main.cpp"
        "src/qjs.cpp"
        "src/core.cpp"
        "src/api/api_application.cpp"
        "src/api/api_classes.cpp"
        "src/api/api_enums.cpp"
)

# Hide console on Windows, keep standard main() entry point
if(MSVC AND ENABLE_MSVC_WIN_MAIN)
    target_link_options(vectorjs PRIVATE
            "LINKER:/SUBSYSTEM:WINDOWS"
            "LINKER:/ENTRY:mainCRTStartup"
    )
endif()

# Include directories
target_include_directories(vectorjs PRIVATE src)

# Link dependencies
target_link_libraries(vectorjs PRIVATE
        CLI11::CLI11
        raylib
        qjs
)

# -----------------------------------------------------------------------------
# 3. Options
# -----------------------------------------------------------------------------

#function(add_feature_flag OPTION_NAME DESCRIPTION DEFAULT_VAL MACRO_NAME)
#    option(${OPTION_NAME} "${DESCRIPTION}" ${DEFAULT_VAL})
#
#    if(${OPTION_NAME})
#        target_compile_definitions(vectorjs PRIVATE ${MACRO_NAME}=1)
#    else()
#        target_compile_definitions(vectorjs PRIVATE ${MACRO_NAME}=0)
#    endif()
#endfunction()
#
#add_feature_flag(ENABLE_2D "Enable 2D graphics systems" ON ENABLE_FEATURE_2D)
#add_feature_flag(ENABLE_3D "Enable 3D graphics systems" ON ENABLE_FEATURE_3D)
#add_feature_flag(ENABLE_FONT "Enable fonts"             OFF ENABLE_FEATURE_FONT)
#add_feature_flag(ENABLE_AUDIO "Enable audio engine"     ON ENABLE_FEATURE_AUDIO)

