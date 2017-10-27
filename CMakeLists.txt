# Copyright (c) 2017 Matthias Noack <ma.noack.pr@gmail.com>
#
# See accompanying file LICENSE and README for further information.

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(libnoma_typa LANGUAGES CXX)

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

# Boost
find_package(Boost REQUIRED)

# header only library 
add_library(noma_typa STATIC src/noma/typa/basic_types.cpp src/noma/typa/braced_list.cpp src/noma/typa/pair src/noma/typa/util.cpp)

# NOTE: we want to use '#include "noma/typa/typa.hpp"', not '#include "typa.hpp"'
target_include_directories(noma_typa PUBLIC include ${Boost_INCLUDE_DIRS}) 

set_target_properties(noma_typa PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

add_subdirectory(src)
