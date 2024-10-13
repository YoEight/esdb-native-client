cmake_minimum_required(VERSION 3.29)
project(esdb_native_client)

set(CMAKE_CXX_STANDARD 20)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

add_library(esdb_native_client STATIC esdb.cpp)
target_link_libraries(esdb_native_client gRPC Protobuf)
