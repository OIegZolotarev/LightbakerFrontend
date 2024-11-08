cmake_minimum_required(VERSION 3.29)
project(LightbakerFrontend)

set(CMAKE_CXX_STANDARD 20)

file(GLOB sources
        "src/*.h"
        "src/*.cpp"
)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/imgui)
include_directories(${CMAKE_SOURCE_DIR}/include/json/include)
include_directories(${CMAKE_SOURCE_DIR}/include/glm)
include_directories(${CMAKE_SOURCE_DIR}/lib)


add_executable(LightbakerFrontend

        ${sources})
