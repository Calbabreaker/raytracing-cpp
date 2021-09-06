cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(Raytracing LANGUAGES CXX)

# some config
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # set executable directory as build/bin

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    message(STATUS "Found ccache found and enabled")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache) 
endif(CCACHE_FOUND)

# libraries
add_subdirectory(lib/glm)

# output executable
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/util.cpp
    src/scene.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:DEBUG>")
target_compile_definitions(${PROJECT_NAME} PRIVATE "$<$<CONFIG:RELEASE>:RELEASE>")

# link with libraries
target_link_libraries(${PROJECT_NAME} glm)
