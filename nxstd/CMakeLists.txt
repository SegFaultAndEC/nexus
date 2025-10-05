cmake_minimum_required(VERSION 3.28)
project(nxstd)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCE_FILES
        *.cpp
)

add_library(nxstd SHARED)

target_sources(nxstd PRIVATE
        ${SOURCE_FILES}
)

target_link_libraries(nxstd PRIVATE
        nexus_lib
)
