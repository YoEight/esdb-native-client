cmake_minimum_required(VERSION 3.29)
project(esdb_native_client)

include(FetchContent)
FetchContent_Declare(
        gRPC
        GIT_REPOSITORY  https://github.com/grpc/grpc
        GIT_TAG         v1.67.0
)

set(CMAKE_CXX_STANDARD 20)
set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(gRPC)

add_library(esdb_native_client STATIC esdb.cpp)
target_link_libraries(grpc++)
