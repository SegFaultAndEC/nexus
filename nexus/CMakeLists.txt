cmake_minimum_required(VERSION 3.28)
project(nexus_lib)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCE_FILES
        *.cpp
)

add_library(nexus_lib STATIC)

target_sources(nexus_lib PRIVATE
        ${SOURCE_FILES}
)

target_include_directories(nexus_lib PUBLIC
        ./
)
