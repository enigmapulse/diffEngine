#!/usr/bin/env bash

set -e # Exit immediately if a command fails

# Get absolute path to the project root
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$PROJECT_ROOT/build"

echo "⚙️  Building diffEngine Tests..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure and compile
cmake ..
cmake --build . -j"$(nproc)"

echo
echo "🧪 Running Test Suite..."
echo

# Run the tests
ctest --output-on-failure