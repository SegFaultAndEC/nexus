cmake_minimum_required(VERSION 3.28)
project(nexus_test)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE TEST_FILES
        *.cpp
)

add_executable(test)

target_sources(test PRIVATE
        ${TEST_FILES}
)

target_link_libraries(test PRIVATE
        nexus_lib
)

