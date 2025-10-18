#pragma once
#include "DifficultySettings.hpp"
#include "../SettingStorage.hpp"

/**
 * @brief Manager for difficulty settings based on game level
 * 
 * This class provides predefined difficulty settings for each game level
 * and handles the conversion from GameLevel enum to DifficultySettings.
 */
class DifficultyManager {
public:
    /**
     * @brief Get difficulty settings for the specified game level
     * 
     * @param level The game level
     * @return const DifficultySettings& Settings for the specified level
     */
    static const DifficultySettings& getDifficultySettings(GameLevel level);

    /**
     * @brief Get the display name for a difficulty level
     * 
     * @param level The game level
     * @return std::wstring Display name in Russian
     */
    static std::wstring getDifficultyDisplayName(GameLevel level);

private:
    /**
     * @brief Initialize all difficulty presets
     */
    static void initializeDifficultyPresets();

    // Static storage for difficulty settings
    static DifficultySettings easySettings;
    static DifficultySettings harderThanEasySettings;
    static DifficultySettings middleSettings;
    static DifficultySettings harderThanMiddleSettings;
    static DifficultySettings hardSettings;

    static bool initialized;
};