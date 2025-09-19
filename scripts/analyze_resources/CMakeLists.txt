cmake_minimum_required(VERSION 3.16)
project(ImageAnalyzer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(analyze_resources analyze_resources.cpp)

# Set output directory
set_target_properties(analyze_resources PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Platform-specific settings
if(WIN32)
    target_compile_definitions(analyze_resources PRIVATE _WIN32)
else()
    target_compile_definitions(analyze_resources PRIVATE UNIX)
endif()
