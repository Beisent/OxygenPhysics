cmake_minimum_required(VERSION 3.10)
project(OxygenRenderExample)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin) 

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/OxygenPhysics/include)
set(VENDOR_DIR ${CMAKE_SOURCE_DIR}/vendor)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)

# 包含头文件目录
include_directories(
    ${VENDOR_DIR}/OxygenRender
    ${VENDOR_DIR}/glm
    ${VENDOR_DIR}/glfw
    ${VENDOR_DIR}/GLAD
    ${VENDOR_DIR}

    ${TEST_DIR}
    ${INCLUDE_DIR}
    ${INCLUDE_DIR}/Components
    ${INCLUDE_DIR}/Common
    ${INCLUDE_DIR}/Factories


)
add_subdirectory(${VENDOR_DIR}/entt)

# 自动收集源文件
file(GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/OxygenPhysics/src/*.cpp
    ${CMAKE_SOURCE_DIR}/OxygenPhysics/utils/*.cpp
    ${CMAKE_SOURCE_DIR}/tests/*.cpp
)

add_executable(OxygenRenderExample ${SOURCES})

target_link_libraries(OxygenRenderExample
    ${CMAKE_SOURCE_DIR}/bin/libOxygenRender.dll.a
    EnTT::EnTT
    opengl32
)
