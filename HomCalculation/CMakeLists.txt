cmake_minimum_required(VERSION 2.8.4)
project(LD_SCIENCE)

set(CMAKE_C_FLAGS "-std=c99")

set(SOURCE_FILES
complex_struct.h
complex_struct.c
main.c
file_util.h
file_util.c
complex_storage.h
complex_storage.c
simplex.h
simplex.c
generate_complex.h
generate_complex.c
polytop_struct.h
polytop_struct.c
simplex_basic.h
simplex_basic.c
)

add_executable(LD_SCIENCE ${SOURCE_FILES})