cmake_minimum_required(VERSION 3.0)

project(demo VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

aux_source_directory(src SRC)

add_executable(demo ${SRC})

# include_directories(~/boost_1_75_0/)

target_link_libraries(demo dl)

