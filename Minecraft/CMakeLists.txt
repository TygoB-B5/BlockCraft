cmake_minimum_required (VERSION 3.8)
project ("Minecraft")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

find_package("OpenGL" REQUIRED)

add_executable (Minecraft "Minecraft.cpp" "Minecraft.h")
