project(BorderWidget)

file(GLOB src "*.cpp")
file(GLOB hdr "*.h")
file(GLOB ui "*.ui")

add_executable(border_widget ${ui} ${src} ${hdr})