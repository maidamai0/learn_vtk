set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

link_libraries(Qt5::Widgets Qt5::Core)

add_subdirectory(border_widget)