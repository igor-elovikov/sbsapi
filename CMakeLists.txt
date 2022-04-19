cmake_minimum_required(VERSION 3.21)

project(sbsar)

set(CMAKE_CXX_STANDARD 20)
set(SBS_SDK_PATH D:/dev/sbssdk)
#set(SAIL_COMBINE_CODECS ON)

find_package(Boost REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(utf8cpp CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)
find_package(OpenImageIO CONFIG REQUIRED)

#find_package(SailC++ CONFIG REQUIRED)

add_subdirectory(external/tinyxml)
add_subdirectory(external/pybind11) # ? vcpkg package is screwed

add_library(sbsarlib STATIC sbsar/Graph.cpp sbsar/Package.cpp sbsar/Parameter.cpp sbsar/Context.cpp sbsar/Context.h
        sbsar/Output.cpp sbsar/Output.h sbsar/types.h sbsar/Image.cpp sbsar/Image.h sbsar/Input.cpp sbsar/Input.h
        )

target_compile_options(sbsarlib PUBLIC "/Zc:__cplusplus")

target_link_libraries(sbsarlib
        "D:/dev/sbssdk/lib/win32-msvc2019-64/debug_md/substance_d3d10pc_blend.lib"
        "D:/dev/sbssdk/lib/win32-msvc2019-64/debug_md/substance_linker.lib"
        "D:/dev/sbssdk/lib/win32-msvc2019-64/debug_md/substance_framework.lib"
        tinyxml
        utf8cpp
        spdlog::spdlog_header_only
        OpenImageIO::OpenImageIO
        #SAIL::sail-c++
        )

# main library
target_include_directories(sbsarlib PUBLIC
        ./
        utils
        "${SBS_SDK_PATH}/include")

# sbsar C API
add_library(sbsapi SHARED capi/sbsar.cpp)
target_include_directories(sbsapi PUBLIC
        capi
        )
target_link_libraries(sbsapi PUBLIC sbsarlib)

# python bindings
pybind11_add_module(pysbsar py/bindings.cpp)
target_link_libraries(pysbsar PRIVATE sbsarlib)
set_target_properties(pysbsar
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY D:/work/python/pysbsar
        RUNTIME_OUTPUT_DIRECTORY D:/work/python/pysbsar
        )

# test executable
add_executable(main main.cpp)
target_link_libraries(main sbsarlib)