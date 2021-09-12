cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(Raytracing LANGUAGES CXX C)

# some config
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # set executable directory as build/bin

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fcolor-diagnostics")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdiagnostics-color=always")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
endif()

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    message(STATUS "Found ccache found and enabled")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache) 
endif(CCACHE_FOUND)

# libraries
add_subdirectory(lib/glm)
add_subdirectory(lib/stb)
find_package(Threads)

# output executable
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/util.cpp
    src/scene.cpp
    src/material.cpp
    src/object.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:DEBUG>")
target_compile_definitions(${PROJECT_NAME} PRIVATE "$<$<CONFIG:RELEASE>:RELEASE>")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# link with libraries
target_link_libraries(${PROJECT_NAME} glm stb Threads::Threads)
