cmake_minimum_required(VERSION 3.23)

project(onnxruntimeConan VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options("-g")
else()
    add_compile_options("-O3")
endif()

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
find_package(onnxruntime REQUIRED)

message(STATUS ${onnxruntime_INSTALL_PREFIX})
message(STATUS ${onnxruntime_INCLUDE_DIRS})
message(STATUS ${onnxruntime_LIBRARIES})

set(EXE_NAME styletransfer)

file(COPY model DESTINATION ./src/)
file(COPY images DESTINATION ./src/)

add_subdirectory(src)


