file(GLOB sources "*.cpp" "*.cxx")

add_compile_definitions(BASE_IMG_DIR="${CMAKE_CURRENT_SOURCE_DIR}/base_img/")
add_compile_definitions(TEMP_IMG_DIR="${CMAKE_CURRENT_SOURCE_DIR}/temp_img/")

# add each cpp/cxx file to a executable
# add to test for cxx
foreach(source ${sources})
    get_filename_component(target ${source} NAME_WLE)
    # message("source is ${source}")
    # message("target is ${target}")
    add_executable(${target} ${source})

    if (VTK_VERSION VERSION_LESS "8.90.0")
        # old system
        include(${VTK_USE_FILE})
        target_link_libraries(${target} PRIVATE ${VTK_LIBRARIES})
    else ()
        # include all components
        target_link_libraries(${target} PRIVATE ${VTK_LIBRARIES})
        # vtk_module_autoinit is needed
        vtk_module_autoinit(
            TARGETS ${target}
            MODULES ${VTK_LIBRARIES}
            )
    endif () 

    get_filename_component(test ${source} EXT)
    add_test("${target}_test" ${target})
endforeach()