#pragma once
#include "../SettingStorage.hpp"
#include "DifficultySettings.hpp"

class DifficultyManager {
public:
  static const DifficultySettings& getDifficultySettings(GameDifficultyLevel difficultyLevel);
  static std::wstring getDifficultyDisplayName(GameDifficultyLevel difficultyLevel);

private:
  static void initializeDifficultyPresets();

  static DifficultySettings easySettings;
  static DifficultySettings harderThanEasySettings;
  static DifficultySettings middleSettings;
  static DifficultySettings harderThanMiddleSettings;
  static DifficultySettings hardSettings;

  static bool initialized;
};