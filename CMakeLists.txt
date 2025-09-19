cmake_minimum_required(VERSION 3.31)
project(game)

set(CMAKE_CXX_STANDARD 20)
set(SFML_AUDIO_DISABLE_FLAC TRUE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
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
        "src/ScreenState.cpp"
        "src/screens/MainMenu.cpp"
        "src/screens/GameScreen.cpp"
        "src/screens/PauseScreen.cpp"
        "src/screens/Difficulty.cpp"
        "src/screens/HighScores.cpp"
        "src/screens/Settings.cpp"
        "src/screens/Exit.cpp"
        "src/utils/EventLogger.cpp"
        "src/utils/DebugUI.cpp"
        "src/utils/ResourceLoader.cpp"
        "src/utils/ResourceManager.cpp"
        "src/utils/GameGrid.cpp"
        "src/utils/ScalingUtils.cpp"
        "src/SnakeSprite.cpp"
        "src/Snake.cpp"
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::Audio)

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
