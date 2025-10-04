cmake_minimum_required(VERSION 3.10)
project(OxygenPhysicsExample)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin) 

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/OxygenPhysics/include)
set(VENDOR_DIR ${CMAKE_SOURCE_DIR}/vendor)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)

# 包含头文件目录
include_directories(
    ${VENDOR_DIR}/SFML/include
    ${VENDOR_DIR}

    ${TEST_DIR}
    ${INCLUDE_DIR}
    ${INCLUDE_DIR}/Components
    ${INCLUDE_DIR}/Common
    ${INCLUDE_DIR}/Factories
    ${INCLUDE_DIR}/Systems
)
add_subdirectory(${VENDOR_DIR}/entt)

# 自动收集源文件
file(GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/OxygenPhysics/src/*.cpp
    ${CMAKE_SOURCE_DIR}/OxygenPhysics/utils/*.cpp
    ${CMAKE_SOURCE_DIR}/OxygenPhysics/src/Systems/*.cpp
    ${CMAKE_SOURCE_DIR}/OxygenPhysics/src/Common/*.cpp
    ${CMAKE_SOURCE_DIR}/tests/*.cpp
)

add_executable(OxygenPhysicsExample ${SOURCES})

# 链接SFML库和其他依赖项
if(WIN32)
    # Windows平台
    target_link_libraries(OxygenPhysicsExample
        ${VENDOR_DIR}/SFML/lib/libsfml-graphics-s.a
        ${VENDOR_DIR}/SFML/lib/libsfml-window-s.a
        ${VENDOR_DIR}/SFML/lib/libsfml-system-s.a
        opengl32
        ${VENDOR_DIR}/SFML/lib/libfreetype.a
        winmm
        gdi32
        pthread
        EnTT::EnTT
    )
    # 定义静态链接宏
    target_compile_definitions(OxygenPhysicsExample PRIVATE SFML_STATIC)
else()
    # Linux/macOS平台
    target_link_libraries(OxygenPhysicsExample
        sfml-graphics
        sfml-window
        sfml-system
        EnTT::EnTT
    )
endif()
