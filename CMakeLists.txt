cmake_minimum_required(VERSION 3.10)
project(OxygenRenderExample)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 包含头文件目录
include_directories(
    ${CMAKE_SOURCE_DIR}/vendor/OxygenRender
    ${CMAKE_SOURCE_DIR}/vendor/glm
    ${CMAKE_SOURCE_DIR}/vendor/glfw
    ${CMAKE_SOURCE_DIR}/vendor/GLAD
    ${CMAKE_SOURCE_DIR}/vendor

    ${CMAKE_SOURCE_DIR}/tests
)

# 自动收集源文件
file(GLOB SOURCES 
    ${CMAKE_SOURCE_DIR}/OxygenPhysics/src/*.cpp
    ${CMAKE_SOURCE_DIR}/tests/*.cpp
)

add_executable(OxygenRenderExample ${SOURCES})

target_link_libraries(OxygenRenderExample 
    ${CMAKE_SOURCE_DIR}/bin/libOxygenRender.dll.a
    opengl32
)