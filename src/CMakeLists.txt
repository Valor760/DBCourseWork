cmake_minimum_required(VERSION 3.16)

set(TARGET app)

project(${TARGET})

file(GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(${TARGET} ${SRC} ${HEADERS})

target_link_libraries(${TARGET} PUBLIC glfw3 imgui)
