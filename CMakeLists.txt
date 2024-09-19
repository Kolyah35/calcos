cmake_minimum_required(VERSION 3.5)

project(calcos-sim)

set(CMAKE_C_STANDARD_REQUIRED 99)

include(cmake/CPM.cmake)
CPMAddPackage("gh:raysan5/raylib#5.0")

include_directories(${PROJECT_NAME} 
    src/
    src/screens/
    ${raylib_SOURCE_DIR}/src
)

add_executable(${PROJECT_NAME} 
    src/main.c
    src/gfx.c
    src/screen.c
    src/font.c

    src/screens/home_screen.c
)

target_link_libraries(${PROJECT_NAME} raylib)