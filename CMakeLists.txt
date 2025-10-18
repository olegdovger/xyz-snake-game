cmake_minimum_required(VERSION 3.31)
project(game)

set(CMAKE_CXX_STANDARD 20)
set(SFML_AUDIO_DISABLE_FLAC TRUE)

# Additional compiler search directories
set(CMAKE_PREFIX_PATH
    "C:/msys64/mingw32/bin"
    "C:/msys64/mingw64/bin"
    "C:/msys64/clang32/bin"
    "C:/msys64/clang64/bin"
    "C:/msys64/clangarm64/bin"
    "C:/msys64/ucrt64/bin"
)

# Force output to bin/ directory without Debug/Release subdirectories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/bin)

# Optimize linker for Windows to prevent memory issues
if(WIN32 AND MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj")
    # Add linker flags to handle memory issues
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--disable-auto-image-base")
    # Reduce memory usage during linking
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g1")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
endif()


include(FetchContent)

# Fetch SFML
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.1
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(${PROJECT_NAME}
        "src/main.cpp"
        "src/Game.cpp"
        "src/Screen.cpp"
        "src/screens/MainMenu.cpp"
        "src/screens/GameScreen.cpp"
        "src/screens/PauseScreen.cpp"
        "src/screens/DifficultyScreen.cpp"
        "src/screens/HighScores.cpp"
        "src/screens/Settings.cpp"
        "src/screens/Exit.cpp"
        "src/config/AudioConstants.hpp"
        "src/utils/EventLogger.cpp"
        "src/utils/DebugUI.cpp"
        "src/utils/ResourceLoader.cpp"
        "src/utils/ResourceManager.cpp"
        "src/utils/GameGrid.cpp"
        "src/utils/ScalingUtils.cpp"
        "src/utils/SettingStorage.cpp"
        "src/utils/CountdownTimer.cpp"
        "src/utils/GameUI.cpp"
        "src/utils/Digits.cpp"
        "src/utils/TimerManager.cpp"
        "src/utils/PausableClock.cpp"
        "src/utils/WallManager.cpp"
        "src/utils/Wall.cpp"
        "src/utils/GameItemManager.cpp"
        "src/utils/GameItem.cpp"
        "src/utils/RedApple.cpp"
        "src/utils/GreenApple.cpp"
        "src/utils/WaterBubble.cpp"
        "src/utils/FantomApple.cpp"
        "src/utils/difficulty/DifficultySettings.cpp"
        "src/utils/difficulty/DifficultyManager.cpp"
        "src/SnakeSprite.cpp"
        "src/Snake.cpp"
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
# Additional linker optimizations for MinGW on Windows
if(WIN32 AND MINGW)
    target_link_options(${PROJECT_NAME} PRIVATE
        -Wl,--disable-auto-image-base
    )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::Audio)

# Copy resources folder to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR}/bin)

# Copy required MinGW DLLs for release
if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    find_program(MINGW_GCC gcc PATHS "C:/msys64/ucrt64/bin" "C:/msys64/mingw64/bin")
    if(MINGW_GCC)
        get_filename_component(MINGW_BIN_DIR ${MINGW_GCC} DIRECTORY)
        file(GLOB MINGW_DLLS
            "${MINGW_BIN_DIR}/libgcc_s_seh-1.dll"
            "${MINGW_BIN_DIR}/libstdc++-6.dll"
            "${MINGW_BIN_DIR}/libwinpthread-1.dll"
        )
        foreach(DLL ${MINGW_DLLS})
            if(EXISTS ${DLL})
                file(COPY ${DLL} DESTINATION ${CMAKE_BINARY_DIR}/bin)
            endif()
        endforeach()
    endif()
endif()

# CPack configuration
set(CPACK_PACKAGE_NAME "Snake Game")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A classic Snake game implementation")
set(CPACK_PACKAGE_VENDOR "Your Name")

# Set package generator (ZIP for cross-platform)
set(CPACK_GENERATOR "ZIP")

# Include the executable and resources in the package
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION bin)

# Install MinGW DLLs for release
if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    find_program(MINGW_GCC gcc PATHS "C:/msys64/ucrt64/bin" "C:/msys64/mingw64/bin")
    if(MINGW_GCC)
        get_filename_component(MINGW_BIN_DIR ${MINGW_GCC} DIRECTORY)
        file(GLOB MINGW_DLLS
            "${MINGW_BIN_DIR}/libgcc_s_seh-1.dll"
            "${MINGW_BIN_DIR}/libstdc++-6.dll"
            "${MINGW_BIN_DIR}/libwinpthread-1.dll"
        )
        foreach(DLL ${MINGW_DLLS})
            if(EXISTS ${DLL})
                install(FILES ${DLL} DESTINATION bin)
            endif()
        endforeach()
    endif()
endif()

# Include CPack module
include(CPack)

# Code formatting target
find_program(CLANG_FORMAT clang-format)
if (CLANG_FORMAT)
    add_custom_target(format
            COMMAND ${CLANG_FORMAT} -i
            ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
            ${CMAKE_CURRENT_SOURCE_DIR}/src/screens/*.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/screens/*.hpp
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Formatting source code with clang-format"
    )
    message(STATUS "Format target available: 'make format' or 'cmake --build . --target format'")
else ()
    message(WARNING "clang-format not found. Format target not available.")
endif ()
