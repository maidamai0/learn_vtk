cmake_minimum_required(VERSION 3.10)

project(VTK_textbook_code)

find_package(VTK REQUIRED)

include(${VTK_USE_FILE})
link_libraries(${VTK_LIBRARIES})

file(GLOB sources "*.cpp")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_CLANG_TIDY clang-tidy --fix --fix-errors)

foreach(source ${sources})
    get_filename_component(target ${source} NAME_WLE)
    message("source is ${source}")
    message("target is ${target}")
    add_executable(${target} ${source})
endforeach()


# add_executable(example_3.10_render_a_cone example_3.10_render_a_cone.cpp)