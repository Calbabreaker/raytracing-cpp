cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(Raytracing LANGUAGES CXX C)

# use yacpm
if(NOT EXISTS "${CMAKE_BINARY_DIR}/yacpm.cmake")
    file(DOWNLOAD "https://github.com/Calbabreaker/yacpm/raw/v1/yacpm.cmake" "${CMAKE_BINARY_DIR}/yacpm.cmake")
endif()

include(${CMAKE_BINARY_DIR}/yacpm.cmake)
yacpm_use_extended()

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
