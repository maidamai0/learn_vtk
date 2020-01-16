cmake_minimum_required(VERSION 3.10)

project(learn_vtk)

find_package(VTK REQUIRED)

include(${VTK_USE_FILE})
link_libraries(${VTK_LIBRARIES})

file(GLOB sources "*.cpp")

# msvc 2019 enable clang-tidy
if(MSVC_TOOLSET_VERSION GREATER_EQUAL  142) 
    set(CMAKE_CXX_CLANG_TIDY clang-tidy --fix --fix-errors)
endif()

foreach(source ${sources})
    get_filename_component(target ${source} NAME_WLE)
    message("source is ${source}")
    message("target is ${target}")
    add_executable(${target} ${source})
endforeach()