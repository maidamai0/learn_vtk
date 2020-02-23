cmake_minimum_required(VERSION 3.10)

project(learn_vtk)

set(CMAKE_CXX_STANDARD  17)

set(VTK_LIBS
    vtkRenderingCore
    vtkRenderingLOD
    vtkRenderingLabel
    vtkRenderingContext2D
    vtkRenderingContextOpenGL2
    vtkRenderingFreeType
    vtkRenderingGL2PSOpenGL2
    vtkRenderingOpenGL2
    vtkCommonCore
    vtkCommonDataModel
    vtkInteractionStyle
    vtkIOParallel
    vtkIOMovie
    vtkInteractionWidgets
    vtkFiltersProgrammable
    vtkFiltersTexture
    vtkFiltersFlowPaths
    vtkGUISupportQt
    vtkRenderingQt
    vtkViewsContext2D
    vtkChartsCore
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

# enable test
enable_testing()

# add include path of 3rdparty
set(3RDPARTY_INCLUDE_PATH
doctest
fmtlib/include
)
foreach(dep ${3RDPARTY_INCLUDE_PATH})
    include_directories(${CMAKE_SOURCE_DIR}/3rdparty/${dep})
endforeach()

# compile fmtlib
add_subdirectory(3rdparty/fmtlib)
link_libraries(fmt::fmt)

# for doctest
add_compile_definitions(DOCTEST_CONFIG_IMPLEMENT DOCTEST_CONFIG_SUPER_FAST_ASSERTS)

# disable test code in release version
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "disable doctest in release version")
    add_compile_definitions(DOCTEST_CONFIG_DISABLE)
endif()


# copy data to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB sources "*.cpp" "*.cxx")

# add each cpp/cxx file to a executable
# add to test for cxx
foreach(source ${sources})
    get_filename_component(target ${source} NAME_WLE)
    # message("source is ${source}")
    # message("target is ${target}")
    add_executable(${target} ${source})

    get_filename_component(test ${source} EXT)
    if(${test} STREQUAL ".cxx")
        add_test("${target}_test" ${target})
    endif()
endforeach()

add_test(distance_between_points_test distance_between_points)

add_subdirectory(qt)