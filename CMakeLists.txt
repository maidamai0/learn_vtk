cmake_minimum_required(VERSION 3.10)

project(learn_vtk)

set(CMAKE_CXX_STANDARD  17)

set(VTK_LIBS
    vtkRenderingCore
    vtkRenderingLOD
    vtkRenderingLabel
    vtkCommonCore
    vtkInteractionStyle
    vtkIOParallel
    vtkIOMovie
    vtkInteractionWidgets
    vtkFiltersProgrammable
    vtkFiltersTexture
    vtkFiltersFlowPaths
    vtkGUISupportQt
)

find_package(VTK COMPONENTS ${VTK_LIBS} REQUIRED)

message(STATUS "VTK_VERSION: ${VTK_VERSION}")

include(${VTK_USE_FILE})
link_libraries(${VTK_LIBRARIES})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# msvc 2019 enable clang-tidy
if(MSVC_TOOLSET_VERSION GREATER_EQUAL  142) 
    # set(CMAKE_CXX_CLANG_TIDY clang-tidy --fix --fix-errors)
endif()

# copy data to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB sources "*.cpp")

# add etch cpp file to a executable
foreach(source ${sources})
    get_filename_component(target ${source} NAME_WLE)
    # message("source is ${source}")
    # message("target is ${target}")
    add_executable(${target} ${source})
endforeach()

add_subdirectory(qt)