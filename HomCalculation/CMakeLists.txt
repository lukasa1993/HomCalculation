cmake_minimum_required(VERSION 2.8.4)
project(LD_SCIENCE)

set(CMAKE_C_FLAGS "-std=c99")

IF(true)
message(STATUS "Checking OpenMP")
find_package(OpenMP)
IF(OPENMP_FOUND)
    message("Found OpenMP! ^_^")
    # add flags for OpenMP
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_SHARED_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
ELSE()
    message("Missed OpenMP! x_x")
ENDIF()
ENDIF()

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