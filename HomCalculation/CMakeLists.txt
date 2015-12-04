cmake_minimum_required(VERSION 2.8.4)
project(LD_SCIENCE)

set(CMAKE_C_FLAGS "-std=c99")

IF (false)
    message(STATUS "Checking OpenMP")
    find_package(OpenMP)
    IF (OPENMP_FOUND)
        message("Found OpenMP! ^_^")
        # add flags for OpenMP
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_SHARED_LINKER_FLAGS}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    ELSE ()
        message("Missed OpenMP! x_x")
    ENDIF ()
ENDIF ()

#include_directories(lis/include)
#add_subdirectory(lis)

set(SOURCE_FILES
        #lis_bridge.h
        #lis_bridge.c
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
        simplex_basic.h
        simplex_basic.c
        ipsolve_bridge.c ipsolve_bridge.h)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/build/)


add_executable(LD_SCIENCE ${SOURCE_FILES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ip_solve/osx/liblpsolve55.dylib DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lib/osx64)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/A.txt DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/B.txt DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/ip_solve/osx)
#target_link_libraries(LD_SCIENCE lis)
target_link_libraries(LD_SCIENCE ${CMAKE_CURRENT_SOURCE_DIR}/ip_solve/osx/liblpsolve55.dylib)
