cmake_minimum_required(VERSION 3.21)

project(sbsar)

set(CMAKE_CXX_STANDARD 20)
set(SUBSTANCE_FRAMEWORK_ENGINE_VARIANT mtl_blend)

find_package(spdlog CONFIG REQUIRED)
find_package(substance CONFIG REQUIRED)

add_library(sbsarlib STATIC sbsar/graph.cpp sbsar/package.cpp sbsar/context.h
        sbsar/output.cpp sbsar/output.h sbsar/types.h sbsar/image.cpp sbsar/image.h sbsar/input.cpp sbsar/input.h
        sbsar/parameter.cpp utils/error.h sbsar/context.cpp utils/common.h)

target_link_libraries(sbsarlib
        spdlog::spdlog_header_only
        Substance::Framework
        Substance::Linker
)

# sbsar C API
add_library(sbsapi SHARED capi/sbsar.h capi/sbsar.cpp)
target_include_directories(sbsapi PUBLIC
        capi
        )
target_link_libraries(sbsapi PUBLIC sbsarlib)

# test executable
add_executable(main main.cpp)
target_link_libraries(main PUBLIC sbsarlib)