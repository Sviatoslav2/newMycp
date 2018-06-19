cmake_minimum_required(VERSION 3.5.1)
project(mycp)

find_package(Boost 1.45.0 COMPONENTS filesystem system)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(mycp ${SOURCE_FILES})
target_link_libraries(mycp ${Boost_LIBRARIES})