#!/bin/bash

# Format all C++ files in the project
echo "Formatting C++ code..."

# Find all C++ files and format them
find src -name "*.cpp" -o -name "*.hpp" -o -name "*.h" | while read file; do
    echo "Formatting $file"
    clang-format -i "$file"
done

echo "Code formatting complete!"
