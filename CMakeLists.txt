cmake_minimum_required(VERSION 3.13)
project(library C)

set(CMAKE_C_STANDARD 99)
include_directories(/usr/local/libevent/include /usr/include/mysql)
link_directories(/usr/local/libevent/lib)

aux_source_directory(. SOURCE_DIRS)

add_executable(library ${SOURCE_DIRS})

target_link_libraries(library event mysqlclient)

