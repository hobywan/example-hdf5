
cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0048 NEW)

project(example VERSION 1.0.0 LANGUAGES C CXX)

add_executable(example src/main.cpp src/loader.cpp)

find_package(HDF5 REQUIRED)
find_package(MPI REQUIRED)

target_include_directories(example PRIVATE "${CMAKE_SOURCE_DIR}/include" "${HDF5_INCLUDE_DIRS}")

target_link_libraries(example PRIVATE ${HDF5_LIBRARIES})
target_link_libraries(example PRIVATE MPI::MPI_CXX)