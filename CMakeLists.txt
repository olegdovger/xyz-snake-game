cmake_minimum_required(VERSION 3.31)
project(game)

set(CMAKE_CXX_STANDARD 20)
set(SFML_AUDIO_DISABLE_FLAC TRUE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

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
        "src/utils/WallManager.cpp"
        "src/utils/Wall.cpp"
        "src/utils/GameItemManager.cpp"
        "src/utils/GameItem.cpp"
        "src/utils/RedApple.cpp"
        "src/utils/GreenApple.cpp"
        "src/utils/WaterBubble.cpp"
        "src/utils/FantomApple.cpp"
        "src/SnakeSprite.cpp"
        "src/Snake.cpp"
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::Audio)

# Copy resources folder to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR}/bin)

# CPack configuration
set(CPACK_PACKAGE_NAME "Snake Game")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A classic Snake game implementation")
set(CPACK_PACKAGE_VENDOR "Your Name")

# Set package generator (ZIP for cross-platform)
set(CPACK_GENERATOR "ZIP")

# Include the executable and resources in the package
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION bin/resources)

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
