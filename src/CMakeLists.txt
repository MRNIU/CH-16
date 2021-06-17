
# This file is a part of MRNIU/CH-16 (https://github.com/MRNIU/CH-16).
#
# CMakeLists.txt for MRNIU/CH-16.

include_directories(${CH16_SOURCE_DIR}/include)

set(src
    ${CH16_SOURCE_DIR}/src/ch16.c)

add_library(${LIBNAME} SHARED ${src})
