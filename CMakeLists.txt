# /CMakeLists.txt
cmake_minimum_required(VERSION 3.14)

project(diffEngine
    VERSION 0.1
    LANGUAGES C CXX
)

# ===============================
# Compiler Settings
# ===============================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are forbidden. Use build/ directory.")
endif()

# ===============================
# Output Directories
# ===============================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ===============================
# Auto Reconfigure on File Change
# ===============================
set(CMAKE_CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/tests
)

# ===============================
# Collect Sources
# ===============================
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.c
)

# Exclude main.cpp from the library if you ever add a standalone executable
list(FILTER ENGINE_SOURCES EXCLUDE REGEX ".*/main\\.cpp$")

# ===============================
# Core Library
# ===============================
if(ENGINE_SOURCES)
    add_library(diffEngine_lib STATIC ${ENGINE_SOURCES})

    target_include_directories(diffEngine_lib
        PUBLIC ${PROJECT_SOURCE_DIR}/include
    )
endif()

# ===============================
# Testing
# ===============================
include(CTest)
enable_testing()

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

if(EXISTS ${PROJECT_SOURCE_DIR}/tests/CMakeLists.txt)
    add_subdirectory(tests)
endif()