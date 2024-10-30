cmake_minimum_required(VERSION 3.10)
project(IMDb)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SERVER_SOURCES
    src/network/server.cpp
)

set(REDIS_SOURCES
    src/redis/parse.cpp 
    src/redis/redis.cpp
    src/redis/store.cpp
)

add_executable(server ${SERVER_SOURCES})
add_executable(redis ${REDIS_SOURCES})  

target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

