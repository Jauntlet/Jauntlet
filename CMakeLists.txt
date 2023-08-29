cmake_minimum_required(VERSION 3.10)

project(jauntlet)

# Set the directories for source code
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(JAUNTLET_DIR "${CMAKE_SOURCE_DIR}/Jauntlet")

# Get a list of all .cpp source files in the "src" directory
file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp")

# Get a list of all .cpp source files in the "Jauntlet" directory
file(GLOB_RECURSE JAUNTLET_FILES "${JAUNTLET_DIR}/*.cpp")

# Set the output directory for libraries
set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/dependencies/lib")

# Find and configure OpenGL with GLVND preference
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# Find and configure SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Find and configure GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

# Create a library from the "Jauntlet" source files
add_library(jauntlet ${JAUNTLET_FILES})

# Specify include directories for the "Jauntlet" library
target_include_directories(jauntlet PUBLIC "${CMAKE_SOURCE_DIR}/dependencies/include")

# Create an executable from the "src" source files
add_executable(game ${SRC_FILES})

# Link the "game" executable with SDL2, GLEW, and OpenGL
target_link_libraries(game PRIVATE jauntlet SDL2::SDL2 GLEW::GLEW OpenGL::OpenGL)

# Specify include directories for the "Jauntlet" directory (outside of it)
target_include_directories(game PRIVATE "${CMAKE_SOURCE_DIR}")

# Specify include directories for SDL2, GLEW, and GLM
target_include_directories(game PRIVATE "${CMAKE_SOURCE_DIR}/dependencies/include")
