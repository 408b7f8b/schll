cmake_minimum_required(VERSION 2.4)
project(schll C)

set(CMAKE_C_STANDARD 90)

add_executable(schll test.c)
SET_TARGET_PROPERTIES(schll PROPERTIES LINKER_LANGUAGE C)