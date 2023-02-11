cmake_minimum_required(VERSION 3.21)

project(sbsar)

message(STATUS "VC Version: $ENV{VCToolsVersion}")

set(CMAKE_CXX_STANDARD 20)

#set(CUSTOM_PYTHON_DIR C:/python/python39) # need to reset cache...

if (DEFINED CUSTOM_PYTHON_DIR)
    set(Python_EXECUTABLE ${CUSTOM_PYTHON_DIR}/python.exe)
    set(Python_INCLUDE_DIR ${CUSTOM_PYTHON_DIR}/include)
endif (DEFINED CUSTOM_PYTHON_DIR)

find_package(Python COMPONENTS Interpreter Development REQUIRED)

find_package(Boost REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(utf8cpp CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)
find_package(OpenImageIO CONFIG REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(PNG REQUIRED)
find_package(tinyxml CONFIG REQUIRED)

add_library(sbsarlib STATIC sbsar/graph.cpp sbsar/package.cpp sbsar/context.h
        sbsar/output.cpp sbsar/output.h sbsar/types.h sbsar/image.cpp sbsar/image.h sbsar/input.cpp sbsar/input.h
        )

if (WIN32)
    target_compile_options(sbsarlib PUBLIC "/Zc:__cplusplus")

    target_link_directories(sbsarlib PUBLIC
            $<$<CONFIG:Debug>:$ENV{SBS_SDK_PATH}/lib/win32-msvc2019-64/debug_md>
            $<$<CONFIG:Release>:$ENV{SBS_SDK_PATH}/lib/win32-msvc2019-64/release_md>
            )

    set(SBS_ENGINE_LIB substance_d3d11pc_blend)
endif (WIN32)

if (UNIX)
    target_link_directories(sbsarlib PUBLIC
            $ENV{SBS_SDK_PATH}/lib/macx-clang-64/release
            $ENV{SBS_SDK_PATH}/bin/macx-clang-64/release
            )
    set(SBS_ENGINE_LIB substance_ogl3_blend)
endif (UNIX)

target_link_libraries(sbsarlib
        ${SBS_ENGINE_LIB}
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
add_library(sbsapi SHARED capi/sbsar.h capi/sbsar.cpp)
target_include_directories(sbsapi PUBLIC
        capi
        )
target_link_libraries(sbsapi PUBLIC sbsarlib)

# python bindings
set(PYSBSAR_DIR ${CMAKE_SOURCE_DIR}/pysbsar)
file(MAKE_DIRECTORY ${PYSBSAR_DIR})

pybind11_add_module(pysbsar py/bindings.cpp)

target_link_libraries(pysbsar PUBLIC sbsarlib)
if (WIN32 AND MSVC)
    target_link_libraries(pysbsar PUBLIC ${Python_LIBRARY_RELEASE})
endif (WIN32 AND MSVC)


set_target_properties(pysbsar
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${PYSBSAR_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${PYSBSAR_DIR}
        )


# test executable
add_executable(main main.cpp)
target_link_libraries(main sbsarlib)