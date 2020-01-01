cmake_minimum_required(VERSION 3.15)

project(test)

set(CMAKE_XX_STANDARD 11)
set(CMAKE_C_FLAGS "-lgmp")

add_executable(poly_gmp poly_gmp.c)
