cmake_minimum_required(VERSION 3.16)
project(diffEngine VERSION 0.1.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# FetchContent for dependencies
include(FetchContent)

# Fetch Catch2 for testing
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.7.1
)
FetchContent_MakeAvailable(catch2)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Library target
add_library(diff_engine INTERFACE)
target_include_directories(diff_engine INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Example executable (optional)
add_executable(example src/example.cpp)
target_link_libraries(example diff_engine)

# Test executable
enable_testing()
add_executable(test_diff_engine tests/test_main.cpp)
target_link_libraries(test_diff_engine PRIVATE Catch2::Catch2WithMain diff_engine)

# Register tests with CTest
include(CTest)
add_test(NAME diff_engine_tests COMMAND test_diff_engine)

# Installation rules
install(TARGETS diff_engine example
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include)
