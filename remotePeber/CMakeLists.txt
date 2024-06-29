cmake_minimum_required (VERSION 3.8)

project("remotePeber")

add_executable (remotePeber "remotePeber.cpp" "remotePeber.h")

target_link_libraries(${PROJECT_NAME}
    sub::lib2
)