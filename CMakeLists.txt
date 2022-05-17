cmake_minimum_required(VERSION 3.21)

project(sbsar)

set(CMAKE_CXX_STANDARD 20)
set(PYBIND11_PYTHON_VERSION 3.7)

find_package(Boost REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(utf8cpp CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)
find_package(OpenImageIO CONFIG REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(PNG REQUIRED)

add_subdirectory(external/tinyxml)
#add_subdirectory(external/pybind11) # ? vcpkg package is screwed

add_library(sbsarlib STATIC sbsar/graph.cpp sbsar/package.cpp sbsar/context.h
        sbsar/output.cpp sbsar/output.h sbsar/types.h sbsar/image.cpp sbsar/image.h sbsar/input.cpp sbsar/input.h
        )

if (WIN32)
target_compile_options(sbsarlib PUBLIC "/Zc:__cplusplus")
endif(WIN32)

if (WIN32)
set (SBS_ENGINE $ENV{SBS_SDK_PATH}/lib/win32-msvc2019-64/debug_md/substance_d3d10pc_blend.lib)
set (SBS_LINKER $ENV{SBS_SDK_PATH}/lib/win32-msvc2019-64/debug_md/substance_linker.lib)
set (SBS_FRAMEWORK $ENV{SBS_SDK_PATH}/lib/win32-msvc2019-64/debug_md/substance_framework.lib)
endif(WIN32)

target_link_directories(sbsarlib PUBLIC
        $ENV{SBS_SDK_PATH}/lib/macx-clang-64/release/
        $ENV{SBS_SDK_PATH}/bin/macx-clang-64/release
        )

target_link_libraries(sbsarlib
        substance_ogl3_blend
        substance_linker
        substance_framework
        tinyxml
        utf8cpp
        spdlog::spdlog_header_only
        OpenImageIO::OpenImageIO
        )

# main library
target_include_directories(sbsarlib PUBLIC
        ./
        utils
        $ENV{SBS_SDK_PATH}/include)

# sbsar C API
add_library(sbsapi SHARED capi/sbsar.cpp)
target_include_directories(sbsapi PUBLIC
        capi
        )
target_link_libraries(sbsapi PUBLIC sbsarlib)

# python bindings
pybind11_add_module(pysbsar py/bindings.cpp)
target_link_libraries(pysbsar PUBLIC sbsarlib)
set_target_properties(pysbsar
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY /Users/elovikov/work/python/pysbsar
        RUNTIME_OUTPUT_DIRECTORY /Users/elovikov/work/python/pysbsar
        )

# test executable
add_executable(main main.cpp)
target_link_libraries(main sbsarlib)