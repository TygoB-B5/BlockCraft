cmake_minimum_required (VERSION 3.8)
project ("Minecraft")

# Set Build Type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
endif()

add_subdirectory("Vendor/glfw")
add_subdirectory("Vendor/glad")

add_subdirectory("Engine/Core")
