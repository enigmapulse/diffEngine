#!/usr/bin/env bash

set -e # Exit immediately if a command fails

# Get absolute path to the project root
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$PROJECT_ROOT/build"

echo "⚙️  Building diffEngine..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure and compile
cmake ..
cmake --build . -j"$(nproc)"

echo
echo "🚀 Running diffEngine..."
echo

# Path to the compiled executable (based on CMAKE_RUNTIME_OUTPUT_DIRECTORY)
MAIN_EXEC="$BUILD_DIR/bin/diffEngine"

# Check if the executable exists before running
if [ -f "$MAIN_EXEC" ]; then
    # Run the engine, passing any arguments given to this bash script directly to it
    "$MAIN_EXEC" "$@"
else
    echo "❌ Executable not found at: $MAIN_EXEC"
    echo "Ensure you have an add_executable(diffEngine src/main.cpp) target in your root CMakeLists.txt"
    exit 1
fi