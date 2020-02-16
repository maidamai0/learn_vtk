set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)
link_libraries(Qt5::Widgets Qt5::Core)


# every sub directory is a qt based project
file(GLOB subdirs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
foreach(subdir ${subdirs})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${subdir})
        file(GLOB src "${subdir}/*.cpp")
        file(GLOB hdr "${subdir}/*.h")
        file(GLOB ui "${subdir}/*.ui")
        add_executable(${subdir} ${ui} ${src} ${hdr})
    endif()
endforeach(subdir )