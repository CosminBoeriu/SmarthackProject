cmake_minimum_required(VERSION 3.29)
project(SmarthackProject)

set(CMAKE_CXX_STANDARD 23)

add_executable(SmarthackProject main.cpp)

find_package(CURL REQUIRED)
target_link_libraries(SmarthackProject CURL::libcurl)
