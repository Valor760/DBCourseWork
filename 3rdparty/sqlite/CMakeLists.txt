cmake_minimum_required(VERSION 3.16)

set(TARGET sqlite)

project(${TARGET})

add_library(${TARGET} "sqlite3.c" "sqlite3.h" "shell.c")

