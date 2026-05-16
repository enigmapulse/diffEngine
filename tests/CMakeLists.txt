# /tests/CMakeLists.txt

# Automatically find all test files
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    *.cpp
)

# No tests yet → don't fail the build
if(NOT TEST_SOURCES)
    message(STATUS "No test files found")
    return()
endif()

# Create the test executable
add_executable(diffEngine_tests ${TEST_SOURCES})

target_include_directories(diffEngine_tests
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

# Link against the core library (if it exists) and GTest
if(TARGET diffEngine_lib)
    target_link_libraries(diffEngine_tests PRIVATE diffEngine_lib GTest::gtest_main)
else()
    target_link_libraries(diffEngine_tests PRIVATE GTest::gtest_main)
endif()

# Register with CTest
include(GoogleTest)
gtest_discover_tests(diffEngine_tests)