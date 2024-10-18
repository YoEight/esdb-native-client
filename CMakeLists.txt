cmake_minimum_required(VERSION 3.29)
project(esdb_native_client)

include(FetchContent)
set(CMAKE_CXX_STANDARD 20)

option(ESDB_BUILD_TESTS "Build test suite (requires GTest)" ON)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_program(PROTOC_EXE protoc REQUIRED)
find_program(GRPC_CPP_PLUGIN_EXE grpc_cpp_plugin REQUIRED)

set(PROTOS)
set(gen_proto_sources)
set(gen_proto_headers)
set(gen_grpc_sources)
set(gen_grpc_headers)
set(protos_dir "${CMAKE_SOURCE_DIR}/protos")

file(GLOB proto_files "${protos_dir}/*")
foreach (file ${proto_files})
    get_filename_component(name ${file} NAME_WE)
    list(APPEND gen_proto_sources "${CMAKE_CURRENT_BINARY_DIR}/${name}.pb.cc")
    list(APPEND gen_proto_headers "${CMAKE_CURRENT_BINARY_DIR}/${name}.pb.h")
    list(APPEND gen_grpc_sources "${CMAKE_CURRENT_BINARY_DIR}/${name}.grpc.pb.cc")
    list(APPEND gen_grpc_headers "${CMAKE_CURRENT_BINARY_DIR}/${name}.grpc.pb.h")
endforeach ()

set(gen_files)
list(APPEND gen_files ${gen_proto_sources})
list(APPEND gen_files ${gen_proto_headers})
list(APPEND gen_files ${gen_grpc_sources})
list(APPEND gen_files ${gen_grpc_headers})

message(STATUS "Binary Dir: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "Gen Files: ${gen_files}")
message(STATUS "Proto Files: ${proto_files}")

add_custom_command(
        OUTPUT ${gen_files}
        COMMAND ${PROTOC_EXE}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${protos_dir}"
        --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN_EXE}"
        ${proto_files}
        DEPENDS ${proto_files})

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
add_library(generated_files ${gen_files})

file(GLOB source_files "${CMAKE_CURRENT_SOURCE_DIR}/*")

target_link_libraries(generated_files
#        absl::absl_log
#        ${_REFLECTION}
        protobuf::libprotobuf
        gRPC::grpc++
)

file(GLOB_RECURSE source_files "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

foreach(src_file ${source_files})
    message(STATUS ${src_file})
endforeach ()

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

find_package(uriparser 0.9.8 CONFIG REQUIRED char wchar_t)

add_library(esdb_native_client
        STATIC
        ${source_files}
        ${generated_files}
)

target_include_directories(esdb_native_client PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_link_libraries(esdb_native_client
        gRPC::grpc++
        protobuf::libprotobuf
        uriparser::uriparser
        nlohmann_json::nlohmann_json
)

if (ESDB_BUILD_TESTS)
    enable_testing()

    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/valid_connection_strings.json" "${CMAKE_CURRENT_BINARY_DIR}/valid_connection_strings.json" COPYONLY)

    find_package(GTest 1.8.0 REQUIRED)

    add_executable(testrunner
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/connection_string_tests.cpp
    )

    target_link_libraries(testrunner
            esdb_native_client
            GTest::gtest_main
            nlohmann_json::nlohmann_json
    )

    include(GoogleTest)
    gtest_discover_tests(testrunner)
endif ()
