cmake_minimum_required(VERSION 3.16)

project(DB-CourseWork)

set(BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(3RDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)

# set(CMAKE_CXX_STANDARD 20)

# Set compile flags
if (WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20 /Zc:__cplusplus")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_CRT_SECURE_NO_WARNINGS")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -Wno-unknown-pragmas -Wno-unused-function")
endif()

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/src/*.h)

# INCLUDES
# include_directories("3rdparty/glfw/include")
# include_directories("3rdparty/glm")
# include_directories("3rdparty/")

# LIBS
# link_directories("3rdparty/libs")

# add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

# add_library(GLAD "3rdparty/glad/glad.c")
# set(LIBS glfw3 GLAD)

# target_link_libraries(${PROJECT_NAME} ${LIBS})

# Link libs
link_directories("3rdparty/GLFW/lib")


# Add include directories
include_directories("3rdparty/imgui")
include_directories("3rdparty/")


# Add sub projects
add_subdirectory(src)
add_subdirectory(3rdparty/imgui)
add_subdirectory(3rdparty/sqlite)
