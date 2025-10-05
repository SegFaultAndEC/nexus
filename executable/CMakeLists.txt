cmake_minimum_required(VERSION 3.28)
project(nexus)
set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCE_FILES
        *.cpp
)

add_executable(nexus)

target_sources(nexus PRIVATE
        ${SOURCE_FILES}
)

target_link_libraries(nexus PRIVATE
        nexus_lib
)