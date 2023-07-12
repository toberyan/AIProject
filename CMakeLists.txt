cmake_minimum_required(VERSION 3.5)

project(DemoInteface)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Qt5 COMPONENTS
    Widgets
    DBus
    REQUIRED)

execute_process(COMMAND ${PYTHON_EXECUTABLE} "-c" "print('Hello, world!')")

include_directories("/usr/include/python3.9")

pkg_search_module(gRPC REQUIRED grpc++ IMPORTED_TARGET)
pkg_search_module(PROTOBUF REQUIRED protobuf IMPORTED_TARGET)

FILE(GLOB_RECURSE SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

FILE(GLOB PROTOC_DEF
    "${CMAKE_CURRENT_SOURCE_DIR}/proto/*.proto"
)

# need to install protobuf-compiler and protobuf-compiler-grpc
find_program(PROTOC_EXE protoc)
find_program(GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)

execute_process(COMMAND ${PROTOC_EXE} --grpc_out ${CMAKE_CURRENT_BINARY_DIR} --cpp_out ${CMAKE_CURRENT_BINARY_DIR} -I ${CMAKE_CURRENT_SOURCE_DIR}/proto --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN_EXECUTABLE} ${PROTOC_DEF}
                RESULT_VARIABLE result_var
                OUTPUT_VARIABLE output_var
                ERROR_VARIABLE error_var
)

message(STATUS "gen procto file result: ${result_var}" " output: ${output_var}" " error: ${error_var}")

FILE(GLOB PROTOCL_SRC
  "${CMAKE_CURRENT_BINARY_DIR}/*.h"
  "${CMAKE_CURRENT_BINARY_DIR}/*.cc"
)

message("import protocl files:  ${PROTOCL_SRC}")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${PROTOCL_SRC})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
        ${Qt5DBus_INCLUDE_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME}
    Qt5::Widgets
    Qt5::DBus
    ${gRPC_LIBRARIES}
    ${PROTOBUF_LIBRARIES}
)

target_link_libraries(
    DemoInteface -lpython3.9
)


