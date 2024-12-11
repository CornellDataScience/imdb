cmake_minimum_required(VERSION 3.10)
project(IMDb)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CLIENT_SOURCES
    src/network/tcp_client.cpp
    src/redis/parse.cpp
)

set(REDIS_SOURCES
    src/redis/parse.cpp 
    src/redis/redis.cpp
    src/redis/store.cpp
    src/redis/server.cpp
)

add_executable(client ${CLIENT_SOURCES})
add_executable(redis ${REDIS_SOURCES})  

target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(redis PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
