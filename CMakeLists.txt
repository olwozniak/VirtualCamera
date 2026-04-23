cmake_minimum_required(VERSION 4.1)
project(ProjektGrafikaKomputerowa)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        master
)

FetchContent_Declare(
        tinyobjloader
        GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
        GIT_TAG        release
)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG        master
)


set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw tinyobjloader glm)

add_executable(ProjektGrafikaKomputerowa src/main.cpp
        src/Object.cpp
        src/glad.c
        include/Object.h
        src/Window.cpp
        include/Window.h
        src/Shader.cpp
        include/Shader.h
        src/DepthManagement.cpp
        include/DepthManagement.h
        include/stb_image.h)

target_include_directories(ProjektGrafikaKomputerowa PRIVATE include ${tinyobjloader_SOURCE_DIR})

target_link_libraries(ProjektGrafikaKomputerowa PRIVATE glfw glm)
