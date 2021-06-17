
# This file is a part of MRNIU/CH-16 (https://github.com/MRNIU/CH-16).
#
# CMakeLists.txt for MRNIU/CH-16.

include_directories(${CH16_SOURCE_DIR}/include)

aux_source_directory(${CH16_SOURCE_DIR}/test src)

set(test_src
    test.c)

link_libraries(${LIBNAME})

add_executable(test ${test_src})
